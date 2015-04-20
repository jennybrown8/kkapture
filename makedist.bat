rmdir /s /q ipch
mkdir ..\bin
copy *.txt ..\bin
move x64\Release\kkapture64.exe ..\bin
move x64\Release\kkapturedll64.dll ..\bin
move Release\kkapture.exe ..\bin
move Release\kkapturedll.dll ..\bin
rmdir /s /q Debug
rmdir /s /q Release
rmdir /s /q x64
rmdir /s /q kkapture\Debug
rmdir /s /q kkapture\Release
rmdir /s /q kkapture\x64
rmdir /s /q kkapturedll\Debug
rmdir /s /q kkapturedll\Release
rmdir /s /q kkapturedll\x64
del /f /ah *.suo
del *.sdf
del ..\kkapture-src.zip
zip -rv9 ..\kkapture-src.zip .
cd ..\bin
del ..\kkapture-bin.zip
zip -rv9 ..\kkapture-bin.zip .
@pause