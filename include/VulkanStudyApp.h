#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <map>
#include <optional>

typedef struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    bool isComplete()
    {
        return graphicsFamily.has_value();
    }
} QueueFamilyIndices;

class VulkanStudyApp
{
public:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    void run();

private:
    GLFWwindow *window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice logicalDevice;
    VkQueue graphicsQueue;

    void initWindow();
    void initVulkan();
    void mainLoop();
    void createInstance();
    bool checkValidationLayerSupport();
    void pickPhysicalDevice();
    int rateDeviceSuitability(VkPhysicalDevice device);
    bool isDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    void createLogicalDevice();
    void cleanup();
};
