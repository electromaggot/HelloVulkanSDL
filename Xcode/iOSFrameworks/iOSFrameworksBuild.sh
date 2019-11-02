 # iOS FRAMEWORK BUILD and
# Merge Script
 # Originally from: http://code.hootsuite.com/an-introduction-to-creating-and-distributing-embedded-frameworks-in-ios/
 #	but modified heavily by Tadd 14 July 2016
 #(Tadd's added comments are these single-space-indented ones.)
 # (Tip: Put "env" atop "Run Script" before calling this to see Environment Variable values available to you.)
 # $CONFIGURATION is one such Environment Variable, providing "Debug" vs. "Release"
 
# 1
# Set bash script to exit immediately if any commands fail.
set -e
 
# 2
# Setup some constants for use later on.
 # Framework Name is passed in from command-line, so
 #	this one script can build any number of frameworks.
PROJECT_NAME="$1"
 # and this constant locates our Product Destination:
OUTPUT_DIRECTORY=${SRCROOT}
 # (...which should be the directory: <projBase>/Modules/3rdParty/SDL/AppleFrameworks)
 # FYI WAS, for test:
#OUTPUT_DIRECTORY="${HOME}/Desktop"
 
 # 2.5
 # Apple frowns upon dashes in the Framework Name, so it's too bad we've been using them to append
 #	"-iOS" vs. "-macOS" yet differentiate from, say, "SDL" vs. "SDL_image" for these projects.
 #	Therefore, while each project still carries the dash in its name, the resultant framework replaces
 #	that with underscore.  This difference doesn't seem to hinder Xcodebuild from finding "_" targets,
 #	but only when there's a single .pbxproj for it to find by default.  With our 2 projects, we must
 #	provide Xcodebuild -project to tell it which one.  Anyway, "automate" the substitution here:
FRAMEWORK_NAME="$(echo "$1" | sed "s/-/_/g")"
 #	Again, if you only have one project/.pbxproj in the directory, all you have to specify is: -target
 #		(funny note: Product Bundle Identifier, on the other hand, can contain
 #		 hyphens, but not underscores.  So good luck keeping that all straight.)

# 3
# If remnants from a previous build exist, delete them.
if [ -d "${SRCROOT}/build" ]; then
  rm -rf "${SRCROOT}/build"
fi
 
# 4
# Build the framework for device and for simulator (using
# all needed architectures).
xcodebuild -project "${PROJECT_NAME}.xcodeproj" -scheme "${PROJECT_NAME}" -configuration ${CONFIGURATION} -arch arm64 -arch armv7 -arch armv7s only_active_arch=no defines_module=yes -sdk "iphoneos"
xcodebuild -project "${PROJECT_NAME}.xcodeproj" -scheme "${PROJECT_NAME}" -configuration ${CONFIGURATION} -arch x86_64 -arch i386 only_active_arch=no defines_module=yes -sdk "iphonesimulator"
 
# 5
# Remove .framework file if exists in Destination from previous run.
if [ -d "${OUTPUT_DIRECTORY}/${FRAMEWORK_NAME}.framework" ]; then
  rm -rf "${OUTPUT_DIRECTORY}/${FRAMEWORK_NAME}.framework"
fi
 
# 6
# Copy the device version of framework to Destination.
cp -r "${SRCROOT}/build/${CONFIGURATION}-iphoneos/${FRAMEWORK_NAME}.framework" "${OUTPUT_DIRECTORY}/${FRAMEWORK_NAME}.framework"
 
# 7
# Replace the framework executable within the framework with
# a new version created by merging the device and simulator
# frameworks' executables with lipo.
lipo -create -output "${OUTPUT_DIRECTORY}/${FRAMEWORK_NAME}.framework/${FRAMEWORK_NAME}" "${SRCROOT}/build/${CONFIGURATION}-iphoneos/${FRAMEWORK_NAME}.framework/${FRAMEWORK_NAME}" "${SRCROOT}/build/${CONFIGURATION}-iphonesimulator/${FRAMEWORK_NAME}.framework/${FRAMEWORK_NAME}"
 
 # The below has been excluded.  We don't have any Swift, so no .swiftmodule I guess.
 #	Nor do I want to delete the build directory, which seems like a strange idea... (so we "want"
 #	to recompile every file every time? we "never" want to look at build logs or see non-stdout
 #	build progress, like which .o/.obj files were generated vs. which ones weren't? etc. etc.)

# 8
# Copy the Swift module mappings for the simulator into the 
# framework.  The device mappings already exist from step 6.
#cp -r "${SRCROOT}/build/${CONFIGURATION}-iphonesimulator/${FRAMEWORK_NAME}.framework/Modules/${FRAMEWORK_NAME}.swiftmodule/" "${OUTPUT_DIRECTORY}/${FRAMEWORK_NAME}.framework/Modules/${FRAMEWORK_NAME}.swiftmodule"
 
# 9
# Delete the most recent build.
#if [ -d "${SRCROOT}/build" ]; then
#  rm -rf "${SRCROOT}/build"
#fi

