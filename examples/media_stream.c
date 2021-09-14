// This example demonstrates the usage of `implPrimary`.

#include <interface99.h>

#include <stdio.h>

#define MediaStream_INTERFACE(FN, CTX) FN(CTX, void, play, void *self)
interface(MediaStream);

typedef struct {
    char dummy;
} VideoStream;

void VideoStream_play(void *self) {
    (void)self;
    puts("VideoStream_play");
    // Some meaningful computation here...
}

// The primary interface for `VideoStream` is `MediaStream`, therefore, it is appropriate to define
// `VideoStream_play` instead of `VideoStream_MediaStream_play`.
implPrimary(MediaStream, VideoStream);

typedef struct {
    char dummy;
} AudioStream;

void AudioStream_play(void *self) {
    (void)self;
    puts("AudioStream_play");
    // Some meaningful computation here...
}

implPrimary(MediaStream, AudioStream);

/*
 * Output:
 * VideoStream_play
 * AudioStream_play
 */
int main(void) {
    MediaStream video = DYN(VideoStream, MediaStream, &(VideoStream){0});
    video.vptr->play(video.self);

    MediaStream audio = DYN(AudioStream, MediaStream, &(AudioStream){0});
    audio.vptr->play(audio.self);
}
