#include <windows.h>

// This definition determines that vulkan.h needs to include vulkan_win32.h.
// Must be defined before including the vulkan headers.
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>

// TODO: Add surface KHR extension to initialization struct
// TODO: Obtain device for vulkan
int loadVulkan() {
	// Load vulkan library and obtain function pointer to vkGetInstanceProcAddr.
	// vkGetInstanceProcAddr can be used to obtain function pointers for the other
	// vulkan functions.
	HMODULE vkLib = LoadLibraryExW(L"vulkan-1.dll", NULL, 0);
	PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;

	if (vkLib) {
		vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vkLib, "vkGetInstanceProcAddr");
	}

	if (vkLib == NULL || vkGetInstanceProcAddr == NULL) {
		// Failed to load vulkan-1.dll. Free library if it opened and end program.
		if (vkLib) FreeLibrary(vkLib);
		return 1;
	} else {
		PFN_vkCreateInstance vkCreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(NULL, "vkCreateInstance");

		VkApplicationInfo applicationInfo = {
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pNext = NULL,
			.pApplicationName = "test",
			.applicationVersion = 1,
			.pEngineName = "gfg",
			.engineVersion = 1,
			.apiVersion = VK_VERSION_1_2
		};

		VkInstanceCreateInfo instanceCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.pApplicationInfo = &applicationInfo,
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = NULL,
			.enabledExtensionCount = 0,
			.ppEnabledExtensionNames = NULL
		};

		VkInstance vkInstance;
		if (vkCreateInstance(&instanceCreateInfo, NULL, &vkInstance) != VK_SUCCESS) {
			return 2;
		}

		return 0;
	}


}
