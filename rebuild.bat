rmdir /S /Q Build
call msbuild Application.sln /p:Configuration=Release /t:Rebuild
call Build\Release\Application.exe
