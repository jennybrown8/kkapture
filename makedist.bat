mkdir ..\kkapture-bin
copy *.txt ..\kkapture-bin
move x64\Release\kkapture64.exe ..\kkapture-bin
move x64\Release\kkapturedll64.dll ..\kkapture-bin
move Release\kkapture.exe ..\kkapture-bin
move Release\kkapturedll.dll ..\kkapture-bin

rmdir /s /q ipch
rmdir /s /q Debug
rmdir /s /q Release
rmdir /s /q x64
rmdir /s /q kkapture\Debug
rmdir /s /q kkapture\Release
rmdir /s /q kkapture\x64
rmdir /s /q kkapturedll\Debug
rmdir /s /q kkapturedll\Release
rmdir /s /q kkapturedll\x64
rmdir /s /q .vs
del /f /ah *.suo
del *.sdf

del ..\kkapture-src.zip
del ..\kkapture-bin.zip
zip -rv9 ..\kkapture-src.zip .
cd ..\kkapture-bin
zip -rv9 ..\kkapture-bin.zip .

@pause