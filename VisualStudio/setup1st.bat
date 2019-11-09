@echo off
enableExtensions enableDelayedExpansion

REM
REM  Run this script once after you first clone the project to create
REM   directories and symbolic links that the build process expects.
REM
REM  IMPORTANT!: Change EXTERNAL_LIBRARY_PATH below to match where your VulkanSDK and
REM   SDL directories reside, relative to the 'External' directory in project root.
REM

set EXTERNAL_LIBRARY_PATH=..\..\..\..\Modules\3rdParty

if not exist %EXTERNAL_LIBRARY_PATH% (
  echo Warning: directory '%EXTERNAL_LIBRARY_PATH%' not found.
  echo You must set EXTERNAL_LIBRARY_PATH, by editing it inside this script,
  echo to the location of your VulkanSDK and SDL directories; assuming
  echo you installed them into a common parent directory.
  goto :EOF 
)
setlocal

echo Create 'External' directory with LINKS to DLLs...

if not exist ..\External (
  mkdir ..\External
)
cd ..\External
if exist SDL2 (
  del SDL2
)
mklink /J SDL2 %EXTERNAL_LIBRARY_PATH%\SDL2-2.0.10
if exist SDL2_image (
  del SDL2_image
)
mklink /J SDL2_image %EXTERNAL_LIBRARY_PATH%\SDL2_image-2.0.5
if exist VulkanSDK (
  del VulkanSDK
)
mklink /J VulkanSDK %EXTERNAL_LIBRARY_PATH%\VulkanSDK

cd ..\VisualStudio

if not exist build (
  mkdir build
)
cd build

echo Create one common 'build/compiledShaders' directory...

if not exist compiledShaders (
  mkdir compiledShaders
)

echo Create LINKS from Build Products directories to 'textures' directory...

call :createPlatformDir x64
call :createPlatformDir x86

echo DONE.

endlocal
goto :EOF


:createPlatformDir
  set DIRNAME=%~f1

  if not exist %DIRNAME% (
    mkdir %DIRNAME%
  )
  cd %DIRNAME%

  call :createConfigurationDir Debug

  call :createConfigurationDir Release

  cd ..
exit /B


:createConfigurationDir
  set SUBDIRNAME=%~f1

  if not exist %SUBDIRNAME% (
    mkdir %SUBDIRNAME%
  )
  cd %SUBDIRNAME%

  if exist textures (
    del textures
  )
  mklink /J textures ..\..\..\..\assets\textures
  if exist compiledShaders (
    del compiledShaders
  )
  mklink /J compiledShaders ..\..\compiledShaders
  cd ..
exit /B

