/*
     This file is part of libextractor.
     Copyright Copyright (C) 2008, 2012 Heikki Lindholm and Christian Grothoff

     libextractor is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published
     by the Free Software Foundation; either version 3, or (at your
     option) any later version.

     libextractor is distributed in the hope that it will be useful, but
     WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with libextractor; see the file COPYING.  If not, write to the
     Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
     Boston, MA 02110-1301, USA.
 */
/**
 * @file thumbnailffmpeg_extractor.c
 * @author Heikki Lindholm
 * @author Christian Grothoff
 * @brief this extractor produces a binary encoded
 * thumbnail of images and videos using the ffmpeg libs.
 *
 * This is a thumbnail extractor using the ffmpeg libraries that will eventually
 * support extracting thumbnails from both image and video files.
 *
 * Note that ffmpeg has a few issues:
 * (1) there are no recent official releases of the ffmpeg libs
 * (2) ffmpeg has a history of having security issues (parser is not robust)
 *
 *  So this plugin cannot be recommended for system with high security
 *requirements.
 */
#include "platform.h"
#include "extractor.h"
#include <magic.h>

#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

#if USE_JPEG
#ifdef PIX_FMT_YUVJ420P
#define PIX_OUTPUT_FORMAT PIX_FMT_YUVJ420P
#else
#define PIX_OUTPUT_FORMAT AV_PIX_FMT_YUVJ420P
#endif
#else
#ifdef PIX_FMT_RGB24
#define PIX_OUTPUT_FORMAT PIX_FMT_RGB24
#else
#define PIX_OUTPUT_FORMAT AV_PIX_FMT_RGB24
#endif
#endif

/**
 * Set to 1 to use JPEG, PNG otherwise
 */
#define USE_JPEG 1

/**
 * Set to 1 to enable a output file for testing.
 */
#define OUTPUT_FILE 0


/**
 * Set to 1 to use jpeg.
 */
#define DEBUG 0

/**
 * max dimension in pixels for the thumbnail.
 */
#define MAX_THUMB_DIMENSION 128

/**
 * Maximum size in bytes for the thumbnail.
 */
#define MAX_THUMB_BYTES (100 * 1024)

/**
 * Number of bytes to feed to libav in one go.
 */
#define BUFFER_SIZE (32 * 1024)

/**
 * Number of bytes to feed to libav in one go, with padding (padding is zeroed).
 */
#ifdef AV_INPUT_BUFFER_PADDING_SIZE
#define PADDED_BUFFER_SIZE (BUFFER_SIZE + AV_INPUT_BUFFER_PADDING_SIZE)
#else
/* legacy */
#define PADDED_BUFFER_SIZE (BUFFER_SIZE + FF_INPUT_BUFFER_PADDING_SIZE)
#endif

#ifndef AV_CODEC_FLAG_QSCALE
#define AV_CODEC_FLAG_QSCALE CODEC_FLAG_QSCALE
#endif


/**
 * Global handle to MAGIC data.
 */
static magic_t magic;


/**
 * Read callback.
 *
 * @param opaque the 'struct EXTRACTOR_ExtractContext'
 * @param buf where to write data
 * @param buf_size how many bytes to read
 * @return -1 on error (or for unknown file size)
 */
static int
read_cb (void *opaque,
         uint8_t *buf,
         int buf_size)
{
  struct EXTRACTOR_ExtractContext *ec = opaque;
  void *data;
  ssize_t ret;

  ret = ec->read (ec->cls, &data, buf_size);
  if (ret <= 0)
    return ret;
  memcpy (buf, data, ret);
  return ret;
}


/**
 * Seek callback.
 *
 * @param opaque the 'struct EXTRACTOR_ExtractContext'
 * @param offset where to seek
 * @param whence how to seek; AVSEEK_SIZE to return file size without seeking
 * @return -1 on error (or for unknown file size)
 */
static int64_t
seek_cb (void *opaque,
         int64_t offset,
         int whence)
{
  struct EXTRACTOR_ExtractContext *ec = opaque;

  if (AVSEEK_SIZE == whence)
    return ec->get_size (ec->cls);
  return ec->seek (ec->cls, offset, whence);
}


/**
 * Rescale and encode a PNG thumbnail.
 *
 * @param src_width source image width
 * @param src_height source image height
 * @param src_stride
 * @param src_pixfmt
 * @param src_data source data
 * @param dst_width desired thumbnail width
 * @param dst_height desired thumbnail height
 * @param output_data where to store the resulting PNG data
 * @param output_max_size maximum size of result that is allowed
 * @return the number of bytes used, 0 on error
 */
static size_t
create_thumbnail (AVCodecContext *pCodecCtx, int src_width, int src_height,
                  int src_stride[],
                  enum AVPixelFormat src_pixfmt,
                  const uint8_t *const src_data[],
                  int dst_width, int dst_height,
                  uint8_t **output_data,
                  size_t output_max_size)
{
  AVCodecContext *encoder_codec_ctx;
  AVDictionary *opts;
  AVCodec *encoder_codec;
  struct SwsContext *scaler_ctx;
  AVFrame *dst_frame;
  uint8_t *dst_buffer;
  uint8_t *encoder_output_buffer;
  size_t encoder_output_buffer_size;
  int err;

  AVPacket pkt;
  av_init_packet (&pkt);
  pkt.data = NULL;
  pkt.size = 0;
  int gotPacket;
#if USE_JPEG
  if (NULL == (encoder_codec = avcodec_find_encoder (AV_CODEC_ID_MJPEG)))
#else
  if (NULL == (encoder_codec = avcodec_find_encoder_by_name ("png")))
#endif
  {
#if DEBUG
    fprintf (stderr,
             "Couldn't find a encoder\n");
#endif
    return 0;
  }

  /* NOTE: the scaler will be used even if the src and dst image dimensions
   * match, because the scaler will also perform colour space conversion */
  if (NULL ==
      (scaler_ctx =
         sws_getContext (src_width, src_height, src_pixfmt,
                         dst_width, dst_height,
                         PIX_OUTPUT_FORMAT,
                         SWS_BILINEAR, NULL, NULL, NULL)))
  {
#if DEBUG
    fprintf (stderr,
             "Failed to get a scaler context\n");
#endif
    return 0;
  }

  dst_frame = av_frame_alloc ();
  if (NULL == dst_frame)
  {
#if DEBUG
    fprintf (stderr,
             "Failed to allocate the destination image frame\n");
#endif
    sws_freeContext (scaler_ctx);
    return 0;
  }
  if (NULL == (dst_buffer =
                 av_malloc (av_image_get_buffer_size (PIX_OUTPUT_FORMAT,
                                                      dst_width,
                                                      dst_height,
                                                      1))))
  {
#if DEBUG
    fprintf (stderr,
             "Failed to allocate the destination image buffer\n");
#endif
    av_frame_free (&dst_frame);
    sws_freeContext (scaler_ctx);
    return 0;
  }
  av_image_fill_arrays (dst_frame->data,
                        dst_frame->linesize,
                        dst_buffer,
                        PIX_OUTPUT_FORMAT,
                        dst_width,
                        dst_height,
                        1);
  sws_scale (scaler_ctx,
             src_data,
             src_stride,
             0, src_height,
             dst_frame->data,
             dst_frame->linesize);

  encoder_output_buffer_size = output_max_size;
  if (NULL == (encoder_output_buffer = av_malloc (encoder_output_buffer_size)))
  {
#if DEBUG
    fprintf (stderr,
             "Failed to allocate the encoder output buffer\n");
#endif
    av_free (dst_buffer);
    av_frame_free (&dst_frame);
    sws_freeContext (scaler_ctx);
    return 0;
  }

  if (NULL == (encoder_codec_ctx = avcodec_alloc_context3 (encoder_codec)))
  {
#if DEBUG
    fprintf (stderr,
             "Failed to allocate the encoder codec context\n");
#endif
    av_free (encoder_output_buffer);
    av_free (dst_buffer);
    av_frame_free (&dst_frame);
    sws_freeContext (scaler_ctx);
    return 0;
  }
  encoder_codec_ctx->width = dst_width;
  encoder_codec_ctx->height = dst_height;
#if USE_JPEG
  encoder_codec_ctx->bit_rate      = pCodecCtx->bit_rate;
  encoder_codec_ctx->codec_id      = AV_CODEC_ID_MJPEG;
  encoder_codec_ctx->codec_type    = AVMEDIA_TYPE_VIDEO;
  encoder_codec_ctx->time_base.num = pCodecCtx->time_base.num;
  encoder_codec_ctx->time_base.den = pCodecCtx->time_base.den;
  encoder_codec_ctx->pix_fmt = PIX_OUTPUT_FORMAT;
#else
  encoder_codec_ctx->pix_fmt = PIX_OUTPUT_FORMAT;
#endif

  opts = NULL;
  if (avcodec_open2 (encoder_codec_ctx, encoder_codec, &opts) < 0)
  {
#if DEBUG
    fprintf (stderr,
             "Failed to open the encoder\n");
#endif
    avcodec_free_context (&encoder_codec_ctx);
    av_free (encoder_output_buffer);
    av_free (dst_buffer);
    av_frame_free (&dst_frame);
    sws_freeContext  (scaler_ctx);
    return 0;
  }


#ifdef USE_JPEG
#if FF_API_MPV_OPT
  encoder_codec_ctx->mb_lmin        = encoder_codec_ctx->lmin =
    encoder_codec_ctx->qmin * FF_QP2LAMBDA;
  encoder_codec_ctx->mb_lmax        = encoder_codec_ctx->lmax =
    encoder_codec_ctx->qmax * FF_QP2LAMBDA;
#else
  encoder_codec_ctx->mb_lmin = encoder_codec_ctx->qmin * FF_QP2LAMBDA;
  encoder_codec_ctx->mb_lmax = encoder_codec_ctx->qmax * FF_QP2LAMBDA;
#endif
  encoder_codec_ctx->flags          = AV_CODEC_FLAG_QSCALE;
  encoder_codec_ctx->global_quality = encoder_codec_ctx->qmin * FF_QP2LAMBDA;

  dst_frame->pts     = 1;
  dst_frame->quality = encoder_codec_ctx->global_quality;
#endif

  err = avcodec_encode_video2 (encoder_codec_ctx,
                               &pkt,
                               dst_frame, &gotPacket);

  if (err < 0)
    goto cleanup;
  err = pkt.size;
  memcpy (encoder_output_buffer,pkt.data, pkt.size);

  av_packet_unref (&pkt);
cleanup:
  av_dict_free (&opts);
  avcodec_close (encoder_codec_ctx);
  avcodec_free_context (&encoder_codec_ctx);
  av_free (dst_buffer);
  av_frame_free (&dst_frame);
  sws_freeContext (scaler_ctx);
  *output_data = encoder_output_buffer;

  return err < 0 ? 0 : err;
}


/**
 * calculate the thumbnail dimensions, taking pixel aspect into account
 *
 * @param src_width source image width
 * @param src_height source image height
 * @param src_sar_num
 * @param src_sar_den
 * @param dst_width desired thumbnail width (set)
 * @param dst_height desired thumbnail height (set)
  */
static void
calculate_thumbnail_dimensions (int src_width,
                                int src_height,
                                int src_sar_num,
                                int src_sar_den,
                                int *dst_width,
                                int *dst_height)
{
  if ( (src_sar_num <= 0) || (src_sar_den <= 0) )
  {
    src_sar_num = 1;
    src_sar_den = 1;
  }
  if ((src_width * src_sar_num) / src_sar_den > src_height)
  {
    *dst_width = MAX_THUMB_DIMENSION;
    *dst_height = (*dst_width * src_height)
                  / ((src_width * src_sar_num) / src_sar_den);
  }
  else
  {
    *dst_height = MAX_THUMB_DIMENSION;
    *dst_width = (*dst_height
                  * ((src_width * src_sar_num) / src_sar_den))
                 / src_height;
  }
  if (*dst_width < 8)
    *dst_width = 8;
  if (*dst_height < 1)
    *dst_height = 1;
#if DEBUG
  fprintf (stderr,
           "Thumbnail dimensions: %d %d\n",
           *dst_width, *dst_height);
#endif
}


#define ENUM_CODEC_ID enum AVCodecID

/**
 * Perform thumbnailing when the input is an image.
 *
 * @param image_codec_id ffmpeg codec for the image format
 * @param ec extraction context to use
 */
static void
extract_image (ENUM_CODEC_ID image_codec_id,
               struct EXTRACTOR_ExtractContext *ec)
{
  AVDictionary *opts;
  AVCodecContext *codec_ctx;
  AVCodec *codec;
  AVPacket avpkt;
  AVFrame *frame;
  uint8_t *encoded_thumbnail;
  int thumb_width;
  int thumb_height;
  int err;
  int frame_finished;
  ssize_t iret;
  void *data;
  unsigned char padded_data[PADDED_BUFFER_SIZE];

  if (NULL == (codec = avcodec_find_decoder (image_codec_id)))
  {
#if DEBUG
    fprintf (stderr,
             "No suitable codec found\n");
#endif
    return;
  }
  if (NULL == (codec_ctx = avcodec_alloc_context3 (codec)))
  {
#if DEBUG
    fprintf (stderr,
             "Failed to allocate codec context\n");
#endif
    return;
  }
  opts = NULL;
  if (0 != avcodec_open2 (codec_ctx, codec, &opts))
  {
#if DEBUG
    fprintf (stderr,
             "Failed to open image codec\n");
#endif
    avcodec_free_context (&codec_ctx);
    return;
  }
  av_dict_free (&opts);
  frame = av_frame_alloc ();
  if (NULL == frame)
  {
#if DEBUG
    fprintf (stderr,
             "Failed to allocate frame\n");
#endif
    avcodec_close (codec_ctx);
    avcodec_free_context (&codec_ctx);
    return;
  }

  frame_finished = 0;
  while (! frame_finished)
  {
    if (0 >= (iret = ec->read (ec->cls,
                               &data,
                               BUFFER_SIZE)))
      break;
    memcpy (padded_data, data, iret);
    memset (&padded_data[iret], 0, PADDED_BUFFER_SIZE - iret);
    av_init_packet (&avpkt);
    avpkt.data = padded_data;
    avpkt.size = iret;
    avcodec_decode_video2 (codec_ctx, frame, &frame_finished, &avpkt);
  }
  if (! frame_finished)
  {
#if DEBUG
    fprintf (stderr,
             "Failed to decode a complete frame\n");
#endif
    av_frame_free (&frame);
    avcodec_close (codec_ctx);
    avcodec_free_context (&codec_ctx);
    return;
  }
  calculate_thumbnail_dimensions (codec_ctx->width, codec_ctx->height,
                                  codec_ctx->sample_aspect_ratio.num,
                                  codec_ctx->sample_aspect_ratio.den,
                                  &thumb_width, &thumb_height);

  err = create_thumbnail (codec_ctx, codec_ctx->width, codec_ctx->height,
                          frame->linesize, codec_ctx->pix_fmt,
                          (const uint8_t *const*) frame->data,
                          thumb_width, thumb_height,
                          &encoded_thumbnail, MAX_THUMB_BYTES);
  if (err > 0)
  {
    ec->proc (ec->cls,
              "thumbnailffmpeg",
              EXTRACTOR_METATYPE_THUMBNAIL,
              EXTRACTOR_METAFORMAT_BINARY,
              "image/png",
              (const char*) encoded_thumbnail,
              err);

    #if OUTPUT_FILE
    FILE *f;
      #ifdef USE_JPEG
    f = fopen ("thumb.jpg", "wb");
      #else
    f = fopen ("thumb.png", "wb");
      #endif
    if (! f)
    {
      fprintf (stderr, "Could not open %s\n", "file");
      exit (1);
    }

    fwrite (encoded_thumbnail, 1, err, f);
    fclose (f);

        #endif


    av_free (encoded_thumbnail);
  }
  av_frame_free (&frame);
  avcodec_close (codec_ctx);
  avcodec_free_context (&codec_ctx);
}


/**
 * Perform thumbnailing when the input is a video
 *
 * @param ec extraction context to use
 */
static void
extract_video (struct EXTRACTOR_ExtractContext *ec)
{
  AVPacket packet;
  AVIOContext *io_ctx;
  struct AVFormatContext *format_ctx;
  AVCodecContext *codec_ctx;
  AVCodecParameters *codecpar;
  AVCodec *codec;
  AVDictionary *options;
  AVFrame *frame;
  uint8_t *encoded_thumbnail;
  int video_stream_index;
  int thumb_width;
  int thumb_height;
  int i;
  int err;
  int frame_finished;
  unsigned char *iob;
  int duration;

  if (NULL == (iob = av_malloc (16 * 1024)))
    return;
  if (NULL == (io_ctx = avio_alloc_context (iob,
                                            16 * 1024,
                                            0, ec,
                                            &read_cb,
                                            NULL /* no writing */,
                                            &seek_cb)))
  {
    av_free (iob);
    return;
  }
  if (NULL == (format_ctx = avformat_alloc_context ()))
  {
    av_free (io_ctx);
    return;
  }
  format_ctx->pb = io_ctx;
  options = NULL;
  if (0 != avformat_open_input (&format_ctx, "<no file>", NULL, &options))
  {
    av_free (io_ctx);
    return;
  }
  av_dict_free (&options);
  if (0 > avformat_find_stream_info (format_ctx, NULL))
  {
#if DEBUG
    fprintf (stderr,
             "Failed to read stream info\n");
#endif
    avformat_close_input (&format_ctx);
    av_free (io_ctx);
    return;
  }
  codec = NULL;
  codec_ctx = NULL;
  video_stream_index = -1;
  for (i = 0; i<format_ctx->nb_streams; i++)
  {
    codecpar = format_ctx->streams[i]->codecpar;
    codec_ctx = format_ctx->streams[i]->codec;
    if (AVMEDIA_TYPE_VIDEO != codec_ctx->codec_type)
      continue;
    if (NULL == (codec = avcodec_find_decoder (codecpar->codec_id)))
      continue;
    options = NULL;
    if (0 != (err = avcodec_open2 (codec_ctx, codec, &options)))
    {
      codec = NULL;
      continue;
    }
    av_dict_free (&options);
    video_stream_index = i;
    break;
  }
  if ( (-1 == video_stream_index) ||
       (0 == codec_ctx->width) ||
       (0 == codec_ctx->height) )
  {
#if DEBUG
    fprintf (stderr,
             "No video streams or no suitable codec found\n");
#endif
    if (NULL != codec)
      avcodec_close (codec_ctx);
    avformat_close_input (&format_ctx);
    av_free (io_ctx);
    return;
  }

  frame = av_frame_alloc ();
  if (NULL == frame)
  {
#if DEBUG
    fprintf (stderr,
             "Failed to allocate frame\n");
#endif
    avcodec_close (codec_ctx);
    avformat_close_input (&format_ctx);
    av_free (io_ctx);
    return;
  }

  if (format_ctx->duration == AV_NOPTS_VALUE)
  {
    duration = -1;
#if DEBUG
    fprintf (stderr,
             "Duration unknown\n");
#endif
  }
  else
  {
    duration = format_ctx->duration;
  }

  /* if duration is known, seek to first tried,
   * else use 10 sec into stream */

  if (-1 != duration)
    err = av_seek_frame (format_ctx, -1, (duration / 3), 0);
  else
    err = av_seek_frame (format_ctx, -1, 10 * AV_TIME_BASE, 0);

  if (err >= 0)
    avcodec_flush_buffers (codec_ctx);
  frame_finished = 0;

  while (1)
  {
    err = av_read_frame (format_ctx, &packet);
    if (err < 0)
      break;
    if (packet.stream_index == video_stream_index)
    {
      avcodec_decode_video2 (codec_ctx,
                             frame,
                             &frame_finished,
                             &packet);
      if (frame_finished && frame->key_frame)
      {
        av_packet_unref (&packet);
        break;
      }
    }
    av_packet_unref (&packet);
  }

  if (! frame_finished)
  {
#if DEBUG
    fprintf (stderr,
             "Failed to decode a complete frame\n");
#endif
    av_frame_free (&frame);
    avcodec_close (codec_ctx);
    avformat_close_input (&format_ctx);
    av_free (io_ctx);
    return;
  }
  calculate_thumbnail_dimensions (codec_ctx->width, codec_ctx->height,
                                  codec_ctx->sample_aspect_ratio.num,
                                  codec_ctx->sample_aspect_ratio.den,
                                  &thumb_width, &thumb_height);

  err = create_thumbnail (codec_ctx, codec_ctx->width, codec_ctx->height,
                          frame->linesize, codec_ctx->pix_fmt,
                          (const uint8_t*const *) frame->data,
                          thumb_width, thumb_height,
                          &encoded_thumbnail, MAX_THUMB_BYTES);
  if (err > 0)
  {
    ec->proc (ec->cls,
              "thumbnailffmpeg",
              EXTRACTOR_METATYPE_THUMBNAIL,
              EXTRACTOR_METAFORMAT_BINARY,
              "image/png",
              (const char*) encoded_thumbnail,
              err);
#if OUTPUT_FILE
    FILE *f;
#ifdef USE_JPEG
    f = fopen ("thumb.jpg", "wb");
#else
    f = fopen ("thumb.png", "wb");
#endif
    if (! f)
    {
      fprintf (stderr, "Could not open %s\n", "file");
      exit (1);
    }

    fwrite (encoded_thumbnail, 1, err, f);
    fclose (f);
#endif
    av_free (encoded_thumbnail);
  }
  av_frame_free (&frame);
  avcodec_close (codec_ctx);
  avformat_close_input (&format_ctx);
  av_free (io_ctx);
}


/**
 * Pair of mime type and ffmpeg decoder ID.
 */
struct MIMEToDecoderMapping
{
  /**
   * String for a mime type.
   */
  const char *mime_type;

  /**
   * Corresponding ffmpeg decoder ID.
   */
  ENUM_CODEC_ID codec_id;
};


/**
 * map MIME image types to an ffmpeg decoder
 */
static const struct MIMEToDecoderMapping m2d_map[] = {

#if LIBAVCODEC_BUILD >= AV_VERSION_INT (54,25,0)
  { "image/x-bmp", AV_CODEC_ID_BMP },
  { "image/gif", AV_CODEC_ID_GIF },
  { "image/jpeg", AV_CODEC_ID_MJPEG },
  { "image/png", AV_CODEC_ID_PNG },
  { "image/x-png", AV_CODEC_ID_PNG },
  { "image/x-portable-pixmap", AV_CODEC_ID_PPM },
  { NULL, AV_CODEC_ID_NONE }
#else
  { "image/x-bmp", CODEC_ID_BMP },
  { "image/gif", CODEC_ID_GIF },
  { "image/jpeg", CODEC_ID_MJPEG },
  { "image/png", CODEC_ID_PNG },
  { "image/x-png", CODEC_ID_PNG },
  { "image/x-portable-pixmap", CODEC_ID_PPM },
  { NULL, CODEC_ID_NONE }
#endif

};


/**
 * Main method for the ffmpeg-thumbnailer plugin.
 *
 * @param ec extraction context
 */
void
EXTRACTOR_thumbnailffmpeg_extract_method (struct EXTRACTOR_ExtractContext *ec)
{
  unsigned int i;
  ssize_t iret;
  void *data;
  const char *mime;

  if (-1 == (iret = ec->read (ec->cls,
                              &data,
                              16 * 1024)))
    return;
  if (NULL == (mime = magic_buffer (magic, data, iret)))
    return;
  if (0 != ec->seek (ec->cls, 0, SEEK_SET))
    return;
  for (i = 0; NULL != m2d_map[i].mime_type; i++)
    if (0 == strcmp (m2d_map[i].mime_type, mime))
    {
      extract_image (m2d_map[i].codec_id, ec);
      return;
    }
  extract_video (ec);
}


/**
 * This plugin sometimes is installed under the alias 'thumbnail'.
 * So we need to provide a second entry method.
 *
 * @param ec extraction context
 */
void
EXTRACTOR_thumbnail_extract_method (struct EXTRACTOR_ExtractContext *ec)
{
  EXTRACTOR_thumbnailffmpeg_extract_method (ec);
}


/**
 * Log callback.  Does nothing.
 *
 * @param ptr NULL
 * @param level log level
 * @param format format string
 * @param ap arguments for format
 */
static void
thumbnailffmpeg_av_log_callback (void*ptr,
                                 int level,
                                 const char *format,
                                 va_list ap)
{
#if DEBUG
  vfprintf (stderr, format, ap);
#endif
}


/**
 * Initialize av-libs and load magic file.
 */
void __attribute__ ((constructor))
thumbnailffmpeg_lib_init (void)
{
  av_log_set_callback (&thumbnailffmpeg_av_log_callback);
  magic = magic_open (MAGIC_MIME_TYPE);
  if (0 != magic_load (magic, NULL))
  {
    /* FIXME: how to deal with errors? */
  }
}


/**
 * Destructor for the library, cleans up.
 */
void __attribute__ ((destructor))
thumbnailffmpeg_ltdl_fini ()
{
  if (NULL != magic)
  {
    magic_close (magic);
    magic = NULL;
  }
}


/* end of thumbnailffmpeg_extractor.c */