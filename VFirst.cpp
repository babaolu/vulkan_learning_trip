//#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

const std::vector<const char *> validationLayers =
  {
    "VK_LAYER_KHRONOS_validation"
  };

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication
{
	public:
		void run()
		{
			initWindow();
			initVulkan();
			mainLoop();
			cleanup();
		}

	private:
		GLFWwindow *window;
		VkInstance instance;

		void initWindow()
		{
			glfwInit();
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
		}

		void initVulkan()
		{
			createInstance();
		}

		void createInstance()
		{
		  if (enableValidationLayers && !checkValidationLayerSupport())
		    throw std::runtime_error("Validation layers requested, but not available!");

		  VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Hello Triangle";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "No Engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			if (enableValidationLayers)
			  {
			    std::cout << "Validation Layer enabled!" << std::endl;

			    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
			    createInfo.ppEnabledLayerNames = validationLayers.data();
			  }
			else
			  {
			    createInfo.enabledLayerCount = 0;
			  }

			uint32_t glfwExtensionCount = 0;
			const char **glfwExtensions;

			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			// To know the extensions requested by glfw
			std::vector<const char *> glfwSExt(glfwExtensions, glfwExtensions + glfwExtensionCount);
			
			std::cout << "GLFW requested extension:\n";

			for (const auto& gextention : glfwSExt)
			  std::cout << '\t' << gextention << '\n';
			
			createInfo.enabledExtensionCount = glfwExtensionCount;
			createInfo.ppEnabledExtensionNames = glfwExtensions;
			createInfo.enabledLayerCount = 0;

			uint32_t extensionCount = 0;

			// To know how many extends are available
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> extensions(extensionCount);

			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

			std::cout << "Available extensions:\n";

			for (const auto& extension : extensions)
			  {
			    std::cout << '\t' << extension.extensionName << '\n';
			  }

			//VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
			if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
			  throw std::runtime_error("failed to create instance!");
		}

  bool checkValidationLayerSupport()
  {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char *layerName : validationLayers)
      {
	bool layerFound = false;

	for (const auto& layerProperties : availableLayers)
	  if (strcmp(layerName, layerProperties.layerName) == 0)
	    {
	      layerFound = true;
	      break;
	    }
	
	if (!layerFound)
	  return false;
      }

    return true;
  }
		void mainLoop()
		{
			while (!glfwWindowShouldClose(window))
				glfwPollEvents();
		}

		void cleanup()
		{
		  vkDestroyInstance(instance, nullptr);
			glfwDestroyWindow(window);

			glfwTerminate();
		}
};

int main()
{
	HelloTriangleApplication app;

	try
	{
		app.run();
	} catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}