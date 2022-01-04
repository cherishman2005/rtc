package main

import (
	"bytes"
	"fmt"
	"io/ioutil"
	"log"

	fluentffmpeg "github.com/modfy/fluent-ffmpeg"
)

func main() {
	buf := &bytes.Buffer{}
	err := fluentffmpeg.NewCommand("/usr/bin/ffmpeg").
		InputPath("./vtest.avi").
		OutputFormat("mp4").
		OutputPath("./vtest.mp4").
		Overwrite(true).
		OutputLogs(buf).
		Run()

	if err != nil {
		log.Fatal(err)
	}

	out, _ := ioutil.ReadAll(buf)
	fmt.Println(string(out))
}
