#include "VulkanStudyApp.h"

void VulkanStudyApp::run()
{
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void VulkanStudyApp::initWindow()
{
    glfwInit();
    // to not create an OpenGL context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // to not resize the window
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Study App", nullptr, nullptr);
}

void VulkanStudyApp::initVulkan()
{
    createInstance();
    pickPhysicalDevice();
    createLogicalDevice();
}

void VulkanStudyApp::mainLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}

void VulkanStudyApp::createInstance()
{
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
        throw std::runtime_error("validation layers requested, but not available!");
    }
    // This struct is optional, but it may provide some useful information to the driver in order to optimize our specific application.
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Vulkan Study App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // This struct tells the Vulkan driver which global extensions and validation layers we want to use.
    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    // Vulkan is a platform agnostic API, which means that you need an extension to interface with the window system
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }

    uint32_t extensionCount = 0;
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    std::cout << "available extensions:\n";

    for (const auto &extension : extensions)
    {
        std::cout << '\t' << extension.extensionName << '\n';
    }
}

bool VulkanStudyApp::checkValidationLayerSupport()
{
    // get the number of available layers
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    // get the available layers
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    // check if all the layers in validationLayers are in availableLayers
    for (const char *layerName : validationLayers)
    {
        bool layerFound = false;
        for (const auto &layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }
        // if one of the layers is not found, return false
        if (!layerFound)
        {
            return false;
        }
    }
    return true;
}

void VulkanStudyApp::pickPhysicalDevice()
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    // if there are no devices with Vulkan support
    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }
    printf("deviceCount: %d\n", deviceCount);

    // use an ordered map to automatically sort candidates by increasing score
    std::multimap<int, VkPhysicalDevice> candidates;
    for (auto device : devices)
    {
        printf("device: %p\n", device);
        int score = rateDeviceSuitability(device);
        candidates.insert(std::make_pair(score, device));
    }
    // check if the best candidate is suitable at all
    if (candidates.rbegin()->first > 0)
    {
        physicalDevice = candidates.rbegin()->second;
    }
    else
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

int VulkanStudyApp::rateDeviceSuitability(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
    printf("deviceProperties.deviceName: %s\n", deviceProperties.deviceName);
    int score = 0;
    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
    }
    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;
    // Application can't function without geometry shaders
    if (!deviceFeatures.geometryShader)
    {
        printf("score: %d\n [NO GEOMETRY SHADER]", score);
        return 0;
    }
    printf("score: %d\n", score);
    return score;
}

bool isDeviceSuitable(VkPhysicalDevice device)
{
    if (device == VK_NULL_HANDLE)
    {
        printf("device is VK_NULL_HANDLE\n");
        return false;
    }
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    printf("deviceProperties.deviceName: %s\n", deviceProperties.deviceName);
    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
           deviceFeatures.geometryShader;
}

QueueFamilyIndices VulkanStudyApp::findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;
    // Logic to find queue family indices to populate struct with
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);                      // vector to hold all the queue families
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data()); // populate the vector

    int i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        // check if the queue family has at least 1 queue in order to support the operations we need
        if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            // if the queue family has the capability of presenting to our window surface
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
            }
        }
        if (indices.isComplete())
        {
            break;
        }
        i++;
    }
    return indices;
}

void VulkanStudyApp::createLogicalDevice()
{
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    // create a queue from the queue family
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;

    // specify the priority of the queue, required even if there is only one queue
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    // specify the set of device features that we'll be using
    VkPhysicalDeviceFeatures deviceFeatures{};

    // create the logical device
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    // specify the queues to create
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    // specify the device features to use
    createInfo.pEnabledFeatures = &deviceFeatures;

    // specify the extensions to enable
    createInfo.enabledExtensionCount = 0;

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
}

void VulkanStudyApp::cleanup()
{
    vkDestroyDevice(logicalDevice, nullptr);
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(VulkanStudyApp::window);
    glfwTerminate();
}
