# .kkapture
![screenshot](screenshot/screenshot.png)

.kkapture is a small tool that produces video+audio captures of fullscreen apps (usually demos). Unlike FRaPS, it does not run in realtime; instead, it makes the demos run at a given, fixed framerate you can specify beforehand. In other words, .kkapture can make 60fps video captures of any demo your computer can run, even if each frame takes several seconds to render.

## Why would someone want this?
For Windows demos, it is usually not much work to add a video writer to your program. However, that has to be done by the authors; the nice part about .kkapture is that it is general enough to work with a wide variety of demos, automatically. If you write demos yourself and .kkapture is able to handle your demo without problems, well, you've just saved yourself the work of coding a video writer yourself.

The main application area is when you want to make several quality video captures of different demos in a short period of time, without having to contact each of the coders. Production of demo DVDs or pre-cut demoshows is a prime example.

## How did it come to be?
Well, rather simple story; I was rather annoyed by the generally so-so video quality of the 2004 scene.org awards, mostly caused by repeated frame-rate changes (first FRAPS which captures at whatever rate the demo runs, then video encoding, then playback on a computer with a different refresh rate). The fixed framerate capturing should be able to get rid of most of these problems, by simply avoiding the necessity of framerate conversions altogether.

## How does it work?
If you want the gory details, just look at the provided source code. Here's a short executive summary: .kkapture works by hooking certain graphics api functions to capture each frame just as it is presented on screen. All popular ways of getting time into the program are wrapped aswell - timeGetTime, QueryPerformanceCounter, you name it. This is necessary so .kkapture can make the program think it runs at a fixed framerate (whatever you specified). The most tricky part is audio, because it doubles as both capture signal and sync source. .kkapture provides a custom (and very dumb) DirectSound implementation that emulates an actual soundcard again updating with your exact specified framerate. WaveOut support is presen too, and in recent versions, .kkapture also intercepts some popular sound systems directly to make sure the correct time is reported back to the application.

## How do I use it?
Well, download it, start kkapture.exe, specify demo/video file name and framerate, hit "kkapture!", select your codec of choice and that should be it :). [Rarefluid](http://wurstcaptures.untergrund.net/capture_kkapture.html) has written an article that explains some of the finer points if you want to create high-quality video captures.

There's also some checkboxes that turn on certain workarounds. The defaults usually work best, but if you're dealing with a picky demo, try playing around with the flags a bit, it might help.

## Where can I get it?
Latest release: https://github.com/DemoJameson/kkapture/releases

Older versions: http://www.farbrausch.de/~fg/kkapture