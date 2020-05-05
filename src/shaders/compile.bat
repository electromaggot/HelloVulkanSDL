@echo off
setLocal enableExtensions enableDelayedExpansion

REM
REM  Recompile any shader having its source file more recently modified than its target executable file.
REM
REM  Assumes all source files are in this script's same directory, while the destination directory
REM	is different (included with the "build" products) as specified below.
REM

set COMPILER=..\..\External\VulkanSDK\Bin\glslangValidator.exe

set DSTDIR=..\..\VisualStudio\build\compiledShaders

if not exist "%DSTDIR%" (
  mkdir "%DSTDIR%"
)

for %%F in (*.vert *.frag) do (

  set SRC=%%F

  set DST=%DSTDIR%\!SRC:.vert=-vert.spv!

  set DST=!DST:.frag=-frag.spv!

  REM Don't use something like "set /a" despite these dates being all-numeric, because an example
  REM  date of 20190924175607 far exceeds CMD's signed 32-bit integer size of 2,147,483,647.
  REM  However, things are still kept simple via lexical comparison of the values as strings.

  call :getModifiedDate "!SRC!"
  set SRC_DATE=!DATE_MODIFIED!

  call :getModifiedDate "!DST!"
  set DST_DATE=!DATE_MODIFIED!

  REM Note that this date determination is slightly more complicated than, for instance,
  REM	simply sorting using:  for /F %%X in ('dir /B /O:D "%SRC%" "%DST%"') do set NEWEST=%%X
  REM  however, that '/O:D' option fails to sort files in different directories.  Hence this method.

  if "%1" == "debug" (
    echo SRC: !SRC! modified !SRC_DATE!
    echo DST: !DST! modified !DST_DATE!
  )

  if "!SRC_DATE!" gtr "!DST_DATE!" (

    echo COMPILING !DST! from:

    %COMPILER% -V "!SRC!" -o "!DST!"

  ) else (
    echo (unchanged !SRC!^)
  )
)

endlocal
goto :EOF


:getModifiedDate
  set FILE=%~f1
  set FILE=%FILE:\=\\%
  for /F "skip=1 delims=. usebackq" %%D in (`wmic datafile where name^="%FILE%" get LastModified`) do (
    set DATE_MODIFIED=%%D
    goto done
  )
  :done
exit /B


REM e.g.
REM ..\..\External\VulkanSDK\1.1.126.0\Bin\glslangValidator.exe -V FullScreenTriangle.vert -o ..\..\VisualStudio\x64\Debug\compiledShaders\FullScreenTriangle-vert.spv
REM ..\..\External\VulkanSDK\1.1.126.0\Bin\glslangValidator.exe -V tjVolcanic.frag -o ..\..\VisualStudio\x64\Debug\compiledShaders\tjVolcanic-frag.spv

