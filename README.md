# Video-reverse-playing
A pretty easy demo of using FFmpeg to play video in reverse

## Environment
- Qt 5.14 MinGW 64-bit 
- ffmpeg n4.4.2-20220415

## How it works?
- I use the producer-consumer design pattern. The video decoder will decode video frames into a blocking queue. Once the queue is non-empty, the frame render will be notified and then retrieve frames from the buffer and render them on the screen.  

## Important classes:
- `ReverseDisplay` in `/reverse_v3/reversedisplay.h`,  rendering thread.
- `ReverseDecode` in `/reverse_v3/reversedecode.h`, video-decoding thread.
- `Controller` in `/reverse_v3/utils.h`, a structure that contains a blocking queue to control the rendering and decoding thread.



![demo](https://user-images.githubusercontent.com/78400045/172114723-67d8f044-c21b-4da7-9f41-3c2ed10d79d3.jpg)

