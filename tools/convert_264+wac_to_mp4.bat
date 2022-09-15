
%~dp0ffmpeg -y -i "%~1" -i "%~dpn1.wav" -c:v copy -c:a aac -b:a 320k "%~dpn1.mp4"
@pause