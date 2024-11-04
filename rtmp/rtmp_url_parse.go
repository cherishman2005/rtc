package main

import (
	"fmt"
	"net/url"
	"strings"
)

func main() {
	rtmpURL := "rtmp://example.com:1935/live/stream"

	parsedURL, err := url.Parse(rtmpURL)
	if err != nil {
		fmt.Println("Error parsing URL:", err)
		return
	}

	fmt.Println("Scheme:", parsedURL.Scheme)
	fmt.Println("Host:", parsedURL.Host)
	fmt.Println("Path:", parsedURL.Path)
	fmt.Println("User:", parsedURL.User)

	// 获取路径的第一部分作为应用程序名称
	pathParts := strings.Split(parsedURL.Path, "/")
	if len(pathParts) > 1 {
		application := pathParts[1]
		fmt.Println("Application:", application)
	}

	// 获取路径的最后一部分作为流名称
	stream := pathParts[len(pathParts)-1]
	fmt.Println("Stream:", stream)
}

/*
Scheme: rtmp
Host: example.com:1935
Path: /live/stream
User:
Application: live
Stream: stream
 */
