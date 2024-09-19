rmdir /s /q .\build\deploy\
mkdir .\build\deploy\

copy "build\release\Labplot.exe" "build\deploy"
copy "C:\Qwt-6.3.0-dev\lib\qwt.dll" "build\deploy"

windeployqt "build\deploy\Labplot.exe"