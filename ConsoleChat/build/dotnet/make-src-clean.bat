@echo off

set PROJECT_NAME3="consolechatclient"
set PROJECT_NAME4="consolechatserver-dotnet"

if exist "..\..\src\%PROJECT_NAME3%\obj" (
	echo "remove ..\..\src\%PROJECT_NAME3%\obj" directory"
	rmdir /s /q "..\..\src\%PROJECT_NAME3%\obj"
)

if exist "..\..\src\%PROJECT_NAME4%\obj" (
	echo "remove ..\..\src\%PROJECT_NAME4%\obj" directory"
	rmdir /s /q "..\..\src\%PROJECT_NAME4%\obj"
)
