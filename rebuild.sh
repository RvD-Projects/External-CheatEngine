rm -rf Engine/GameDumper/Dumps

cd Engine/GameDumper
./dump.bat

cd ../../
rm -rf Build

msbuild Application.sln /p:Configuration=Release /t:Rebuild
./Build/Release/Application.exe