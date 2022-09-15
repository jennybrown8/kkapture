
%~dp0ffmpeg -y -i "%~nx1" -i "%~n1.wav" -c:v copy -c:a aac -b:a 320k "%~n1.mp4"
pause