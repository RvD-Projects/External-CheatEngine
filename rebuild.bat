rmdir /S /Q Engine\GameDumper\Dumps

cd Engine\GameDumper
call dump.bat

cd ..
cd ..
rmdir /S /Q Build

call msbuild Application.sln /p:Configuration=Release /t:Rebuild
call msbuild Application.sln /p:Configuration=Release
call Build\Release\Application.exe