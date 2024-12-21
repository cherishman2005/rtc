#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint32_t sps_parser_offset;

uint8_t sps_parser_base64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

size_t sps_parser_base64_decode(char *buffer) {

	uint8_t dtable[256], block[4], tmp, pad = 0;
	size_t i, count = 0, pos = 0, len = strlen(buffer);

	memset(dtable, 0x80, 256);
	for (i = 0; i < sizeof(sps_parser_base64_table) - 1; i++) {
		dtable[sps_parser_base64_table[i]] = (unsigned char) i;
	}

	dtable['='] = 0;

	for (i = 0; i < len; i++) {
		if (dtable[buffer[i]] != 0x80) {
			count++;
		}
	}

	if (count == 0 || count % 4) return 0;


	count = 0;
	for (i = 0; i < len; i++) {
		tmp = dtable[buffer[i]];
		if (tmp == 0x80) continue;

		if (buffer[i] == '=') pad++;
		block[count] = tmp;
		count++;
		if (count == 4) {

			buffer[pos++] = (block[0] << 2) | (block[1] >> 4);
			buffer[pos++] = (block[1] << 4) | (block[2] >> 2);
			buffer[pos++] = (block[2] << 6) | block[3];

			count = 0;
			if (pad) {

				if (pad == 1) pos--;
				else if (pad == 2) pos -= 2;
				else return 0;

				break;
			}
		}
	}

	return pos;
}

uint32_t sps_parser_read_bits(char *buffer, uint32_t count) {
	uint32_t result = 0;
	uint8_t index = (sps_parser_offset / 8);
	uint8_t bitNumber = (sps_parser_offset - (index * 8));
	uint8_t outBitNumber = count - 1;
	for (uint8_t c = 0; c < count; c++) {
		if (buffer[index] << bitNumber & 0x80) {
			result |= (1 << outBitNumber);
		}
		if (++bitNumber > 7) { bitNumber = 0; index++; }
		outBitNumber--;
	}
	sps_parser_offset += count;
	return result;
}

uint32_t sps_parser_read_ueg(char* buffer) {
    uint32_t bitcount = 0;

    for (;;) {
    	if (sps_parser_read_bits(buffer, 1) == 0) {
	        bitcount++;
    	} else {
    		// bitOffset--;
    		break;
    	}
    }

    	// bitOffset --;
    uint32_t result = 0;
    if (bitcount) {
    	uint32_t val = sps_parser_read_bits(buffer, bitcount);
        result = (uint32_t) ((1 << bitcount) - 1 + val);
    }

    return result;
}

int32_t sps_parser_read_eg(char* buffer) {
	uint32_t value = sps_parser_read_ueg(buffer);
	if (value & 0x01) {
		return (value + 1) / 2;
	} else {
		return -(value / 2);
	}
}

void sps_parser_skipScalingList(char* buffer, uint8_t count) {
	uint32_t deltaScale, lastScale = 8, nextScale = 8;
	for (uint8_t j = 0; j < count; j++) {
		if (nextScale != 0) {
			deltaScale = sps_parser_read_eg(buffer);
			nextScale = (lastScale + deltaScale + 256) % 256;
		}
		lastScale = (nextScale == 0 ? lastScale : nextScale);
	}
}

uint32_t sps_parser(char *buffer) {

	uint8_t profileIdc = 0;
	uint32_t pict_order_cnt_type = 0;
	uint32_t picWidthInMbsMinus1 = 0;
	uint32_t picHeightInMapUnitsMinus1 = 0;
	uint8_t frameMbsOnlyFlag = 0;
	uint32_t frameCropLeftOffset = 0;
	uint32_t frameCropRightOffset = 0;
	uint32_t frameCropTopOffset = 0;
	uint32_t frameCropBottomOffset = 0;


	sps_parser_offset = 0;
	sps_parser_base64_decode(buffer);
	sps_parser_read_bits(buffer, 8);
	profileIdc = sps_parser_read_bits(buffer, 8);
	sps_parser_read_bits(buffer, 16);
	sps_parser_read_ueg(buffer);

	if (profileIdc == 100 || profileIdc == 110 || profileIdc == 122 ||
		profileIdc == 244 || profileIdc == 44 || profileIdc == 83 ||
		profileIdc == 86 || profileIdc == 118 || profileIdc == 128) {
		uint32_t chromaFormatIdc = sps_parser_read_ueg(buffer);
		if (chromaFormatIdc == 3) sps_parser_read_bits(buffer, 1);
		sps_parser_read_ueg(buffer);
		sps_parser_read_ueg(buffer);
		sps_parser_read_bits(buffer, 1);
		if (sps_parser_read_bits(buffer, 1)) {
			for (uint8_t i = 0; i < (chromaFormatIdc != 3) ? 8 : 12; i++) {
				if (sps_parser_read_bits(buffer, 1)) {
					if (i < 6) {
						sps_parser_skipScalingList(buffer, 16);
					} else {
						sps_parser_skipScalingList(buffer, 64);
					}
				}
			}
		}
	}

	sps_parser_read_ueg(buffer);
	pict_order_cnt_type = sps_parser_read_ueg(buffer);

	if (pict_order_cnt_type == 0) {
		sps_parser_read_ueg(buffer);
	} else if (pict_order_cnt_type == 1) {
		sps_parser_read_bits(buffer, 1);
		sps_parser_read_eg(buffer);
		sps_parser_read_eg(buffer);
		for (uint32_t i = 0; i < sps_parser_read_ueg(buffer); i++) {
			sps_parser_read_eg(buffer);
		}
	}

	sps_parser_read_ueg(buffer);
	sps_parser_read_bits(buffer, 1);
	picWidthInMbsMinus1 = sps_parser_read_ueg(buffer);
	picHeightInMapUnitsMinus1 = sps_parser_read_ueg(buffer);
	frameMbsOnlyFlag = sps_parser_read_bits(buffer, 1);
	if (!frameMbsOnlyFlag) sps_parser_read_bits(buffer, 1);
	sps_parser_read_bits(buffer, 1);
	if (sps_parser_read_bits(buffer, 1)) {
		frameCropLeftOffset = sps_parser_read_ueg(buffer);
		frameCropRightOffset = sps_parser_read_ueg(buffer);
		frameCropTopOffset = sps_parser_read_ueg(buffer);
		frameCropBottomOffset = sps_parser_read_ueg(buffer);
	}

	return (
		(((picWidthInMbsMinus1 + 1) * 16) - frameCropLeftOffset * 2 - frameCropRightOffset * 2) << 16 |
		((2 - frameMbsOnlyFlag) * (picHeightInMapUnitsMinus1 + 1) * 16) - ((frameMbsOnlyFlag ? 2 : 4) * (frameCropTopOffset + frameCropBottomOffset))
	);
}

int main(int argc , char *argv[]) {

	uint8_t buffer[] = "J00AH41qBQBboQAAAwABAAADADKE";
	uint32_t dimensions = sps_parser(buffer);
	// 1280x720
	printf("width = %d\nheight = %d\n", dimensions >> 16, dimensions & 0xFFFF);

	strcpy(buffer, "J00AH41qCwEmhAAAAwAEAAADAMoQ");
	dimensions = sps_parser(buffer);
	// 704x576
	printf("width = %d\nheight = %d\n", dimensions >> 16, dimensions & 0xFFFF);

	strcpy(buffer, "J00AH41qCwPaEAAAAwAQAAADAyhA");
	dimensions = sps_parser(buffer);
	// 704x480
	printf("width = %d\nheight = %d\n", dimensions >> 16, dimensions & 0xFFFF);

}
