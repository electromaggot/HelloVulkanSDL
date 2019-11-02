//
// VulkanSetup.cpp
//	Vulkan Module
//
// See matched header file for definitive main comment.
//
// Note that ValidationLayers and DebugReport are initialized as early as possible
//	to enable and begin, especially initialization-related, debug reporting.
//
// 1/31/19 Tadd Jensen
//	© 0000 (uncopyrighted; use at will)
//
#include "VulkanSetup.h"


VulkanSetup::VulkanSetup(iPlatform& platform)
	:	validation(),							// Initializer list: instantiate components
		vulkan(validation, platform),			//	in ascending order of explicit dependencies.
		debugReport(vulkan),
		windowSurface(vulkan, platform),
		device(windowSurface, vulkan, validation),
		renderPass(device),
		swapchain(device, windowSurface),
		framebuffers(swapchain, renderPass, device),
		syncObjects(device),
		commandPool(device)
{
	pShaderModules	= nullptr;
	pUniformBuffer	= nullptr;	//TJ_CONSIDER: for MVP, could this be made part of above init list?
	pPipeline		= nullptr;
	pCommandObjects	= nullptr;
}

// On the other hand, these later child objects will NOT leave scope and self-destruct,
//	so:
VulkanSetup::~VulkanSetup()
{
	delete pCommandObjects;
	delete pPipeline;
	for (auto& pTextureImage : pTextureImages)
		delete pTextureImage;
	delete pUniformBuffer;
	delete pShaderModules;
}

// pipeline and commandObjects are not in the list above, because they need to be part
//	of a more flexible Rendering component, which initializes later (than "load time"
//	or "allocation time") and passes in runtime aspects like then-loaded shaders.
//	Thus shader loading/unloading is dynamic.
//
void VulkanSetup::PostConstruction_SetUpForRendering(Shaders& shaders, VertexBasedObject& vtxObj,
													 vector<Described> describedAddOns)
{
	pShaderModules = new ShaderModules(shaders, device);

	pDescriptors = new Descriptors(describedAddOns, swapchain, device);

	pPipeline = new GraphicsPipeline(*pShaderModules, renderPass, swapchain, device,
									 &vtxObj.vertexType, pDescriptors);

	pCommandObjects = new CommandObjects(*pPipeline,
										framebuffers, renderPass, swapchain, device,
										vtxObj, pDescriptors);

	pVertexObject = &vtxObj; //TJ_TODO: What this points to better not be freed or changed randomly!
							 //  We're lucky that in this early going, it points to a combination of
							 //  statics/globals, but if this reference ever arrives to a dynamically
							 //  allocated object or one that leaves scope, "not such a good idea."
							 //  Saving this is hokey anyway, only persisting a VertexType for a
							 //	 Recreate that keeps the same Vertex(&Index)Buffer.
							 // This code, AddOns, and 'Recreate's are all in flux as it is, so I don't
							 //  want to prematurely optimize.  It'll probably change then get fixed.
}

// Assemble a collection of Descriptors to be "added on."  Ordering is critical:
//	make sure each INDEX matches its "layout(binding = <INDEX>)" in your Shader.
//																					// e.g. :
vector<Described> VulkanSetup::AddOn(UBO* pUBO, TextureSpec textureSpecs[],
									 iPlatform& platform)
{
	vector<Described> describedItems;

	// Uniform Buffer Objects first (explicitly: the MVP UBO)
	if (pUBO) {
		pUniformBuffer = new UniformBuffer(pUBO->byteSize, swapchain, device);
		shaderStageForUBO = pUBO->getShaderStageFlags();
		describedItems.emplace_back(pUniformBuffer->getDescriptorBufferInfo(),		// layout(binding = 0)
									shaderStageForUBO);
	}

	// Textures next (may be more than one)... order is important here too == binding index
	for (TextureSpec* pTextureSpec = textureSpecs; pTextureSpec && pTextureSpec->fileName; ++pTextureSpec) {
		TextureImage* pTexture = new TextureImage(*pTextureSpec, commandPool.vkInstance, device, platform);
		if (pTexture) {
			pTextureImages.emplace_back(pTexture);
			describedItems.emplace_back(pTexture->getDescriptorImageInfo(),			// layout(binding = 1) ... 2) ... 3)...
										VK_SHADER_STAGE_FRAGMENT_BIT);
										// ^^^^^^ TODO: ^^^^^^^^ We don't have a mechanism (YET!) allowing an image to
		}								//		be specified for the VERTEX STAGE, which could be helpful for something
	}									//		like offseting vertices based on a depth map.
	return describedItems;
}

	//TJ_LATER_DEV_NOTE:
	// Having extracted the Framebuffer reference from Swapchain*,
	//	where it didn't really belong (and perhaps I reached to avoid...),
	//	it now adds yet another dependency to CommandObjects construction.
	// That's SIX dependencies, nearly all of our member Vulkan objects!
	//	(later edit: now FIVE, perhaps making this paragraph more moot.)
	// It seems ugly/smelly.  If I'm going to have this many dependencies
	//	passed, I may as well just pass the VulkanSetup object itself as
	//	a single dependency of CommandObjects!  (If it's a child of this
	//	class, that would be a circular dependency, so it's not as easy.)
	// So I'd like to reconsider this design, although it builds & works
	//	for now; I'm checking it in as a progress ledge with this comment.

	// * - I had originally thought that Framebuffer should be a child
	//	object of Swapchain.  I'm not sure what the cleanest way to do it
	//	is... however, my current setup seems monolithic and it would be
	//	nice to have a bit more depth to what should be more hierarchical.
	// The number of Framebuffers depends on (matches) the number of
	//	Swapchain imageViews, which is determined by a runtime call to
	//	vkGetSwapchainImagesKHR().  Thus ImageViews is indeed a
	//	dependency of Framebuffers... which also requires Swapchain's Extent.
	// Initializing the ImageViews could be done earlier in
	//	an initializer list chain.  For instance, Swapchain could construct...
	//	Actually, not really possible in a clean RAII fashion (avoiding
	//	news/deletes for instance)... and Swapchain, by recognizable name,
	//	must be a top-level object.  Also Framebuffers need RenderPass while
	//	Swapchain doesn't, so this would be a pure, polluting pass-through.

	// createSwapchain depends on: (device/surface, queueFamilyIndices behind scenes)
	//		does come up with its own nImages (has logic (method) for this,
	//				based on what vkGetPhysicalDeviceSurfaceCapabilities returns)
	//		calculates extent (if not specified by Capabilities (above), gleans
	//							from surface, clamped to limits)
	//  - so basically, not dependent on much, must construct first
	// createImageViews depends on: (device)
	//	- Swapchain (used in call to vkGetSwapchainImages)
	//		get nImages from this call, loops to init its array
	// createFramebuffers depends on: (besides device)
	//	- ImageViews (child of Swapchain)	\
	//	- Swapchain.extent					 > So: Swapchain, RenderPass
	//	- RenderPass						/
	//
	// VERDICT: I guess it seems most clean to keep ImageViews a child of
	//	Swapchain, and keep Framebuffers separate, as it is right now.
	//	This means the only remaining ugliness would be all the parameters
	//	passed to CommandObjects, which I'd like to rework anyway, especially
	//	to get rid of the "prepareForMainLoop" pointers in HelloTriangle.cpp,
	//	which still seem quite klunky.
	//	GOAL: Replace all the pointers assigned in "prepareForMainLoop" with
	//	more-clean, initializer-list-assigned references, replacing those at
	//	the top of application::drawFrame().
	//		SUBGOAL: replace application::Init().initialShaders instead with
	//		a constructor, that initializes same, which could be supplanted
	//		with another object to subsequently init follow-on shaders.

	//TJ_ANOTHER_NOTE
	// Here are some "general rules" I'd like to establish explicitly and
	//	try to follow, for this entire project:
	//		1. VulkanModule object constructors get passed other alread-
	//			constructed VulkanModule objects that they depend on.
	//		2. Each constructor then calls a "create..." method.
	//		3. ::create...() methods should only be allowed to receive
	//			Vulkan's native "VkObjects" (not VulkanModule objects),
	//			so that their exact dependencies are EXPLICIT.
	// As a general rule, ALL objects depend on VkDevice, also for
	//	destruction, so its reference is saved and handled differently,
	//	not necessarily being passed to ::create...().
	//


void VulkanSetup::RecreateRenderingRudiments()
{
	const bool reloadMesh = false;

	vkDeviceWaitIdle(device.getLogical());

	VertexType* pVertexType = pVertexObject ? const_cast<VertexType*>(&pVertexObject->vertexType) : nullptr;

	swapchain.Recreate();
	framebuffers.Recreate(swapchain, renderPass);
	if (pUniformBuffer)
		pUniformBuffer->Recreate(-1, swapchain);
	vector<Described> describedAddOns = reDescribe();
	if (pDescriptors)
		pDescriptors->Recreate(describedAddOns, swapchain);
	pPipeline->Recreate(*pShaderModules, renderPass, swapchain, pVertexType, pDescriptors);
	pCommandObjects->Recreate(*pPipeline, framebuffers, renderPass, swapchain,
							  *pVertexObject, reloadMesh, pDescriptors);
}

// Note that TextureImages are specifically not reloaded or regenerated.
//	This may be necessary later, should the images change or animate, although
//	loading entirely new images (and discarding old ones) is separate matter.
//
vector<Described> VulkanSetup::reDescribe()
{
	vector<Described> redescribedAddOns;
	if (pUniformBuffer)
		redescribedAddOns.emplace_back(pUniformBuffer->getDescriptorBufferInfo(),
									   shaderStageForUBO);
	for (auto& pTextureImage : pTextureImages)
		redescribedAddOns.emplace_back(pTextureImage->getDescriptorImageInfo(),
									   VK_SHADER_STAGE_FRAGMENT_BIT);
	return redescribedAddOns;
}


/*TJ: (delete later)
	This was 15_hello_triangle's (Vulkan-Tutorial's) order of initialization, FYI:

	void HelloTriangleApplication::run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

	void HelloTriangleApplication::initVulkan() {
		createInstance();
		setupDebugMessenger();
		createSurface();
		pickPhysicalDevice();
		createLogicalDevice();
		createSwapChain();
		createImageViews();
		createRenderPass();
		createGraphicsPipeline();
		createFramebuffers();
		createCommandPool();
		createCommandBuffers();
		//i.e.: void createCommandBuffers(std::vector<VkFramebuffer> swapChainFramebuffers, VkExtent2D swapChainExtent);
		createSyncObjects();
	}

 	Using C calls to flat C methods, despite looking straight-forward
 	above, it quickly devolved into a mega morass of monolithic C.
*/
