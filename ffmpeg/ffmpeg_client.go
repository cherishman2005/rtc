package main

import (
    //"bytes"
    "fmt"
    //"io/ioutil"
    //"log"
     "time"

    fluentffmpeg "github.com/modfy/fluent-ffmpeg"
)

func main() {
    done := make(chan error, 1)
    cmd := fluentffmpeg.NewCommand("/usr/bin/ffmpeg").
            InputPath("./vtest.avi").
            OutputFormat("mp4").
            OutputPath("./vtest.mp4").
            Overwrite(true).
            Build()
    cmd.Start()

    go func() {
        done <- cmd.Wait()
    }()

    select {
    case <-time.After(time.Second * 5):
        fmt.Println("Timed out")
        cmd.Process.Kill()
    case <-done:
        fmt.Println("ffmpeg done")
    }
    
    
}
