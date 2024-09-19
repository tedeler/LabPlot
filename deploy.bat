rmdir /s /q .\build\deploy\
mkdir .\build\deploy\

copy "build\release\Labplot.exe" "build\deploy"
copy "C:\Qwt-6.3.0-dev\lib\qwt.dll" "build\deploy"
copy "C:\Windows\System32\LabJackWUSB.dll" "build\deploy"
copy "C:\Windows\System32\LabJackUD.dll" "build\deploy"
copy "C:\Qt\5.15.2\mingw81_64\bin\Qt5OpenGL.dll" "build\deploy"

windeployqt "build\deploy\Labplot.exe"