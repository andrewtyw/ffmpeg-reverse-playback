# ffmpeg-reverse-playback
A pretty easy demo of using ffmpeg to playback a video (reverse playing).  一个简单的demo: 使用ffmpeg实现流畅的视频倒放、跳转。

## Environment
- Qt 5.14 MinGW 64-bit 
- ffmpeg n4.4.2-20220415

## How it works? （原理）
- 生产者消费者模型：有2个线程，解码线程和渲染线程。解码线程负责解码若干段帧放到Controller对象的队列Q中，渲染线程从Q中取帧，显示在画面上
- english version:  Just like the producer-consumer problem, we have 2 threads to decode and render frames. The decode thread produces frames and puts it to the queue in `Controller`; the rendering thread gets frame from the queue and show it.

## Important classes:
- `ReverseDisplay` in `/reverse_v3/reversedisplay.h`, this is the rendering thread.
- `ReverseDecode` in `/reverse_v3/reversedecode.h`, this is the decode thread.
- `Controller` in `/reverse_v3/utils.h`, the structure that controls the decode and rendering thread.



![demo](https://user-images.githubusercontent.com/78400045/172114723-67d8f044-c21b-4da7-9f41-3c2ed10d79d3.jpg)

