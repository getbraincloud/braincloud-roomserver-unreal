@ echo off
:: usage:
::      autobuild\makebuild.bat Win64

::eg. Jenkins (no quotes, no spaces)
:: 		> set UE_INSTALL_PATH=D:\ProgramFiles\UE_5.0

set TARGET=%~1
if "%TARGET%" == "" set TARGET=Win64

::set SERVERPLATFORM=Linux
set SERVERPLATFORM=Linux

set PROJECTNAME=DedicatedDemo
if "%PROJECTNAME%" == "" goto Proj_Error

::Package Project
::-serverplatform=%SERVERPLATFORM% -nocompile -nocompileeditor
call "%UE_INSTALL_PATH%\Engine\Build\BatchFiles\RunUAT.bat" BuildCookRun -project="%WORKSPACE%\%PROJECTNAME%.uproject" -noP4 -utf8output -platform=Linux -serverconfig=Development -build -cook -pak -stage -package -server -archive -archivedirectory="%WORKSPACE%\ServerBuild"
:: return code for tests
exit /B %errorlevel%

:Proj_Error
echo "Pass project name."
exit /B 1

:Path_Error
echo Set UE_INSTALL_PATH
exit /B 1
