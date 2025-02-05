#rmdir /S /Q Engine\GameDumper\Dumps
#call Engine\GameDumper\dump.bat

rmdir /S /Q Build
call msbuild Application.sln /p:Configuration=Release /t:Rebuild
call Build\Release\Application.exe
