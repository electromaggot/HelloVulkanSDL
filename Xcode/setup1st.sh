#!/bin/bash

#
# Run this script once after you first clone the project to create
#  directories and symbolic links that the build process expects.
#
# IMPORTANT!: Change EXTERNAL_LIBRARY_PATH below to match where your VulkanSDK and
#  SDL directories reside, relative to the 'External' directory in project root.
#

EXTERNAL_LIBRARY_PATH=../../../../Modules/3rdParty

if [ ! -d "$EXTERNAL_LIBRARY_PATH" ]; then
  echo "Warning: directory '$EXTERNAL_LIBRARY_PATH' not found."
  echo "You must set EXTERNAL_LIBRARY_PATH, by editing it inside this script,"
  echo " to the location of your VulkanSDK and SDL directories; assuming"
  echo " you installed them into a common parent directory."
  exit
fi

echo "Create 'External' directory with LINKS to Frameworks..."

if [ ! -d ../External ]; then
  mkdir ../External
fi

cd ../External

ln -sfv "$EXTERNAL_LIBRARY_PATH/vulkansdk-macos-1.1.126.0" VulkanSDK

ln -sfv "$EXTERNAL_LIBRARY_PATH/SDL2-2.0.10" SDL

ln -sfv "$EXTERNAL_LIBRARY_PATH/SDL2_image-2.0.5" SDL_image

echo "Done."

