#
# Run this script (enter command: ./Run*.sh) to establish ../VulkanModule as a submodule outside
#	of this .git repo.  That is, ../VulkanModule in the same directory as ./HelloVulkanSDL,
#	so the single VulkanModule source set can be shared equally with other projects too.
#

echo gitdir: ../../../VulkanModule/.git > VulkanModule/.git
