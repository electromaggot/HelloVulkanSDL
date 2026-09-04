#!/bin/sh
# Create the link to VulkanModule, which this project consumes as a PEER library:
#	one checkout of the module, shared by every project that uses it, rather than a
#	private copy per project.  Run once after cloning.  Safe to re-run.
#
#	  GitHub/
#	    HelloVulkanSDL/   <- you are here
#	    VulkanModule/     <- the peer
#
# The link itself is committed, so a fresh clone already has it; this script exists to
#	report clearly when its target is missing, and to clone the module if you ask it to.

cd "$(dirname "$0")"

MODULE_URL="https://github.com/electromaggot/VulkanModule"

if [ ! -d ../VulkanModule ]; then
	echo "VulkanModule not found at ../VulkanModule"
	printf "  Clone it there now? [y/N] "
	read answer
	case "$answer" in
		[Yy]*)	git clone "$MODULE_URL" ../VulkanModule || exit 1 ;;
		*)		echo "  Skipped.  Clone it yourself with:"
				echo "      git clone $MODULE_URL ../VulkanModule"
				exit 1 ;;
	esac
fi

if [ -L src/VulkanModule ]; then
	echo "  src/VulkanModule -> $(readlink src/VulkanModule) (already exists)"
elif [ -e src/VulkanModule ]; then
	echo "  src/VulkanModule exists but is not a symlink -- leaving it alone"
else
	ln -s ../../VulkanModule src/VulkanModule
	echo "  src/VulkanModule -> ../../VulkanModule (created)"
fi

echo "Done.  Now:  mkdir -p build && cd build && cmake .. && cmake --build . -j"
