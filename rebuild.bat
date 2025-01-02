cd Engine\GameDumper
call dump.bat

cd ..
cd ..

call msbuild Application.sln /p:Configuration=Release /t:Rebuild
call Build\Release\Application.exe