#include <set>
#include <optional>
#include <gl/glew.h>
#include <gl/wglew.h>
#include <vulkan/vulkan.hpp>
#include "_WIN32Include.h"
#include "Sonic/App.h"
#include "Sonic/Debug/Log/Log.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events/WindowEvents.h"
#include "Sonic/Event/Events/MouseEvents.h"
#include "Sonic/Event/Events/KeyEvents.h"
#include "Sonic/Util/StringUtils.h"
#include "Sonic/Util/Math/Math.h"
#include "WindowInfoLoader.h"
#include "Window.h"

using namespace Sonic;


#ifdef SONIC_DEBUG
const uint32_t VULKAN_INSTANCE_EXTENSION_COUNT = 3;
#else
const uint32_t VULKAN_INSTANCE_EXTENSION_COUNT = 2;
#endif

const char* const VULKAN_INSTANCE_EXTENSION_NAMES[VULKAN_INSTANCE_EXTENSION_COUNT] = {
    "VK_KHR_surface",
    "VK_KHR_win32_surface",
#ifdef SONIC_DEBUG
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
};

const uint32_t VULKAN_DEVICE_EXTENSION_COUNT = 1;
const char* const VULKAN_DEVICE_EXTENSION_NAMES[VULKAN_DEVICE_EXTENSION_COUNT] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

const uint32_t VULKAN_VALIDATION_LAYER_COUNT = 1;
const char* const VULKAN_VALIDATION_LAYER_NAMES[VULKAN_VALIDATION_LAYER_COUNT] = {
    "VK_LAYER_KHRONOS_validation"
};

const uint32_t MAX_FRAMES_IN_FLIGHT = 3;

const wchar_t* const WIN32_WINDOW_CLASS_NAME = L"Sonic";


struct VulkanQueueFamilieIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool IsComplete()
    {
        return graphicsFamily.has_value() &&
            presentFamily.has_value();
    }
};

struct VulkanSwapChainDetails 
{
    VkSurfaceCapabilitiesKHR surfaceCapabilities;
    std::vector<VkSurfaceFormatKHR> availableSurfaceFormats;
    std::vector<VkPresentModeKHR> availableSurfacePresentationModes;

    bool IsSuitable()
    {
        return !availableSurfaceFormats.empty() && !availableSurfacePresentationModes.empty();
    }
};


static WindowInfo s_Info;

static int s_CurrentWidth;
static int s_CurrentHeight;
static int s_UnmaximizedWidth;
static int s_UnmaximizedHeight;
static String s_Title;
static WindowMode s_Mode;
static Color s_ClearColor;
static bool s_CloseOnAltF4 = true;

static HWND s_Win32Handle;
static HINSTANCE s_Win32Module;

static uint64_t s_TimerOffset;
static uint64_t s_TimerFrequency;

static DWORD s_WindowedStyle;
static DWORD s_WindowedExStyle;

static int s_MonitorWidth;
static int s_MonitorHeight;

static bool s_Maximized;
static bool s_Minimized;

static bool s_IgnoreSizeMessage = false;

static HDC s_DeviceContext;
static HGLRC s_OpenglContext;

static HICON smallIconHandle;
static HICON largeIconHandle;

static HCURSOR s_CurrentCursor;

static std::unordered_map<String, HCURSOR> s_Cursors;

static VkInstance s_VulkanInstance;
static VkDebugUtilsMessengerEXT s_VulkanDebugMessenger;
static PFN_vkDestroyDebugUtilsMessengerEXT s_VulkanDebugMessengerDestroyFunc;
static VkSurfaceKHR s_VulkanSurface;
static VkPhysicalDevice s_VulkanPhysicalDevice;
static VkDevice s_VulkanLogicalDevice;
static VkQueue s_VulkanGraphicsQueue;
static VkQueue s_VulkanPresentQueue;
static VkSwapchainKHR s_VulkanSwapChain;
static std::vector<VkImage> s_VulkanSwapChainImages;
static VkFormat s_VulkanSwapChainImageFormat;
static VkExtent2D s_VulkanSwapChainExtent;
static std::vector<VkImageView> s_VulkanSwapChainImageViews;
static VkRenderPass s_VulkanRenderPass;
static VkPipelineLayout s_VulkanPipelineLayout;
static VkPipeline s_VulkanGraphicsPipeline;
static std::vector<VkFramebuffer> s_VulkanSwapChainFramebuffers;
static VkCommandPool s_VulkanCommandPool;
static std::vector<VkCommandBuffer> s_VulkanCommandBuffers;
static std::vector<VkSemaphore> s_VulkanSwapChainImageAvailableSemaphores;
static std::vector<VkSemaphore> s_VulkanRenderingFinishedSemaphores;
static std::vector<VkFence> s_VulkanInFlightFences;
static std::vector<VkFence> s_VulkanImagesInFlight;
static size_t s_VulkanCurrentFrame;


static void initVulkan();
static void createVulkanInstance();
static bool checkRequiredVulkanExtensions();
static bool checkRequiredVulkanValidationLayers();
static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

static void initVulkanDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* messengerInfo);
static void initVulkanDebugMessenger();
static void initVulkanSurface();
static bool checkVulkanPhysicalDeviceExtensionSupport(VkPhysicalDevice* physicalDevice);

static VulkanSwapChainDetails getVulkanSwapChainDetails(VkPhysicalDevice* physicalDevice);
static bool isVulkanPhysicalDeviceSuitable(VkPhysicalDevice* physicalDevice);
static VulkanQueueFamilieIndices findVulkanQueueFamilies(VkPhysicalDevice* physicalDevice);
static void initVulkanPhysicalDevice();
static void initVulkanLogicalDevice();

static VkSurfaceFormatKHR chooseVulkanSurfaceFormat(VulkanSwapChainDetails* swapChainDetails);
static VkPresentModeKHR chooseVulkanPresentationMode(VulkanSwapChainDetails* swapChainDetails);
static VkExtent2D chooseSwapExtent(VulkanSwapChainDetails* swapChainDetails);
static void initVulkanSwapChain();
static void initVulkanSwapChainImageViews();

static void initVulkanRenderPass();
static std::vector<uint8_t> readBinaryVulkanShader(const String& filePath);
static VkShaderModule createVulkanShader(const std::vector<uint8_t>& binaryCode);
static void initVulkanGraphicsPipeline();

static void initVulkanFramebuffers();
static void initVulkanCommandPool();
static void initVulkanCommandBuffers();
static void initVulkanSyncing();


static void initTimer();
static void initCursors();
static void initIcons();
static void initStyle();
static HICON createNativeIcon(int width, int height, uint8_t* bitmap, int isCursor = FALSE, int hotspotX = 0, int hotspotY = 0);
static BITMAPV5HEADER createBitmapHeader(int width, int height);

static void setDecorated(bool decorated);
static void adjustSize(int width, int height);
static void setFullscreen(bool fullscreen);
static void updateWindowSize();

static Key getKey(LPARAM lParam, WPARAM wParam);


bool Window::init(const String& infoFilePath, bool overrideBinary)
{
    return init(Util::loadWindowInfo(infoFilePath, overrideBinary));
}

bool Window::init(const WindowInfo& info)
{
    s_Info = info;
    s_UnmaximizedWidth = s_Info.width;
    s_UnmaximizedHeight = s_Info.height;
    s_Mode = s_Info.mode;
    s_Title = info.title;

    if (!createWin32Window())
        return false;

    initTimer();

    initStyle();

    s_MonitorWidth = GetSystemMetrics(SM_CXSCREEN);
    s_MonitorHeight = GetSystemMetrics(SM_CYSCREEN);

    setFullscreen(s_Mode == WindowMode::Fullscreen);
    setDecorated(s_Mode == WindowMode::Windowed);

    s_Maximized = info.maximized;
    s_Minimized = !s_Maximized && info.minimized;

    updateWindowSize();

    initCursors();
    initIcons();

    if (!createContext())
        return false;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    setClearColor(s_ClearColor = s_Info.clearColor);

    initVulkan();

    return true;
}

bool Window::createWin32Window()
{
    s_Win32Module = GetModuleHandle(NULL);

    WNDCLASSEX win32WindowClass = { };
    win32WindowClass.cbSize = sizeof(WNDCLASSEX);
    win32WindowClass.lpfnWndProc = Window::WindowProc;
    win32WindowClass.hInstance = s_Win32Module;
    win32WindowClass.lpszClassName = WIN32_WINDOW_CLASS_NAME;
    win32WindowClass.style = CS_OWNDC;

    RegisterClassEx(&win32WindowClass);

    s_Win32Handle = CreateWindowEx(0, WIN32_WINDOW_CLASS_NAME, Util::toWideString(s_Title).c_str(), 0,
        0, 0, 0, 0, NULL, NULL, s_Win32Module, NULL
    );

    if (s_Win32Handle == NULL)
    {
        DWORD error = GetLastError();
        SONIC_LOG_ERROR("Error creating win32 Window (Win32 Error code: ", error, ")");
        return false;
    }

    return true;
}


bool Window::createContext()
{
    s_DeviceContext = GetDC(s_Win32Handle);
    if (s_DeviceContext == 0)
    {
        SONIC_LOG_ERROR("Error getting device context");
        return false;
    }

    PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
        PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
        32,                   // Colordepth of the framebuffer.
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                   // Number of bits for the depthbuffer
        8,                    // Number of bits for the stencilbuffer
        0,                    // Number of Aux buffers in the framebuffer.
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int pixelFormat = ChoosePixelFormat(s_DeviceContext, &pfd);
    if (pixelFormat == 0)
    {
        SONIC_LOG_ERROR("Error creating pixel format");
        return false;
    }

    SetPixelFormat(s_DeviceContext, pixelFormat, &pfd);

    HGLRC dummyContext = wglCreateContext(s_DeviceContext);
    wglMakeCurrent(s_DeviceContext, dummyContext);

    const int attribList[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        0, // End
    };

    if (glewInit() != GLEW_OK)
    {
        SONIC_LOG_ERROR("Error initializing glew");
        return false;
    }

    UINT numFormats;
    if (!wglChoosePixelFormatARB(s_DeviceContext, attribList, NULL, 1, &pixelFormat, &numFormats))
    {
        SONIC_LOG_ERROR("Error creating opengl context");
        return false;
    }

    SetPixelFormat(s_DeviceContext, pixelFormat, &pfd);

    s_OpenglContext = wglCreateContext(s_DeviceContext);
    wglMakeCurrent(s_DeviceContext, s_OpenglContext);

    return true;
}

void Window::pollEvents()
{
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void Window::onResized(int width, int height)
{
    if (s_Mode == WindowMode::Windowed && !s_Minimized && !s_Maximized)
    {
        s_UnmaximizedWidth = width;
        s_UnmaximizedHeight = height;
    }

    if (s_CurrentWidth != width || s_CurrentHeight != height)
    {
        s_CurrentWidth = width;
        s_CurrentHeight = height;

        WindowResizedEvent e = { static_cast<float>(width), static_cast<float>(height) };
        EventDispatcher::dispatch(e);
        App::onWindowResized(e);
    }
}

void Window::onMouseButtonPressed(MouseButton button)
{
    Mouse::s_Buttons[button] = true;
    EventDispatcher::dispatch(MouseButtonPressedEvent(button, Mouse::s_X, Mouse::s_Y));
    SetCapture(s_Win32Handle);
}

void Window::onMouseButtonReleased(MouseButton button)
{
    Mouse::s_Buttons[button] = false;
    EventDispatcher::dispatch(MouseButtonReleasedEvent(button, Mouse::s_X, Mouse::s_Y));
    ReleaseCapture();
}

void Window::setTitle(String title)
{
    s_Title = title;
    SetWindowText(s_Win32Handle, Util::toWideString(title).c_str());
}

String Window::getTitle()
{
    return s_Title;
}

void Window::setWidth(float width)
{
    adjustSize((int)(width + 0.5f), s_CurrentHeight);
}

void Window::setHeight(float height)
{
    adjustSize(s_CurrentWidth, (int)(height + 0.5f));
}

void Window::setSize(float width, float height)
{
    adjustSize((int)(width + 0.5f), int(height + 0.5f));
}

void Window::setMaximized(bool b)
{
    s_Maximized = b;

    if (s_Mode == WindowMode::Windowed)
        ShowWindow(s_Win32Handle, s_Maximized ? SW_MAXIMIZE : SW_NORMAL);
}

void Window::setMinimized(bool b)
{
    s_Minimized = b;

    ShowWindow(s_Win32Handle, s_Minimized ? SW_MINIMIZE : (s_Maximized && s_Mode == WindowMode::Windowed) ? SW_MAXIMIZE : SW_NORMAL);
}

bool Window::isMaximized()
{
    return s_Maximized || s_Mode == WindowMode::WindowedFullscreen || s_Mode == WindowMode::Fullscreen ||
        (s_Mode == WindowMode::WindowedBorderless && s_CurrentWidth == s_MonitorWidth && s_CurrentWidth == s_MonitorHeight);
}

bool Window::isMinimized()
{
    return s_Minimized;
}

float Window::getWidth()
{
    return static_cast<float>(s_CurrentWidth);
}

float Window::getHeight()
{
    return static_cast<float>(s_CurrentHeight);
}

float Window::getMonitorWidth()
{
    return (float)s_MonitorWidth;
}

float Window::getMonitorHeight()
{
    return (float)s_MonitorHeight;
}

static WindowMode getWindowMode()
{
    return s_Mode;
}

void Window::setWindowMode(WindowMode mode)
{
    if (mode == s_Mode)
        return;

    s_IgnoreSizeMessage = true;

    if (mode == WindowMode::Fullscreen)
        setFullscreen(true);
    else if (s_Mode == WindowMode::Fullscreen)
        setFullscreen(false);

    if (mode == WindowMode::Windowed)
        setDecorated(true);
    else if (s_Mode == WindowMode::Windowed)
        setDecorated(false);

    s_Mode = mode;

    updateWindowSize();
}

void Window::setClearColor(const Color& color)
{
    s_ClearColor = color;
    glClearColor(color.r, color.g, color.b, color.a);
}

void Window::setCursor(const String& name)
{
    RECT clientArea;
    POINT cursorPos;

    if (!GetCursorPos(&cursorPos))
        return;

    if (WindowFromPoint(cursorPos) != s_Win32Handle)
        return;

    GetClientRect(s_Win32Handle, &clientArea);
    ClientToScreen(s_Win32Handle, (POINT*)&clientArea.left);
    ClientToScreen(s_Win32Handle, (POINT*)&clientArea.right);

    if (!PtInRect(&clientArea, cursorPos))
        return;

    if (auto it = s_Cursors.find(name);
        it != s_Cursors.end())
    {
        SetCursor(s_CurrentCursor = it->second);
    }
}

void Window::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Window::swapBuffers()
{
    SwapBuffers(s_DeviceContext);
}

double Window::getTime()
{
    uint64_t timer;
    QueryPerformanceCounter((LARGE_INTEGER*)&timer);
    return (double)(timer - s_TimerOffset) / s_TimerFrequency;
}

void Window::saveInfo()
{
    if (s_Info.saveSize)
    {
        s_Info.width = s_UnmaximizedWidth;
        s_Info.height = s_UnmaximizedHeight;
    }

    if (s_Info.saveTitle)
        s_Info.title = s_Title;
    if (s_Info.saveMode)
        s_Info.mode = s_Mode;
    if (s_Info.saveClearColor)
        s_Info.clearColor = s_ClearColor;
    if (s_Info.saveMaximized)
        s_Info.maximized = s_Maximized;
    if (s_Info.saveMinimized)
        s_Info.minimized = s_Minimized;

    Util::saveWindowInfo(s_Info);
}

void Window::destroy()
{
    wglMakeCurrent(s_DeviceContext, NULL);
    wglDeleteContext(s_OpenglContext);

    vkDeviceWaitIdle(s_VulkanLogicalDevice);

#ifdef SONIC_DEBUG
    s_VulkanDebugMessengerDestroyFunc(s_VulkanInstance, s_VulkanDebugMessenger, nullptr);
#endif

    for (auto& semaphore : s_VulkanSwapChainImageAvailableSemaphores)
        vkDestroySemaphore(s_VulkanLogicalDevice, semaphore, nullptr);
    for (auto& semaphore : s_VulkanRenderingFinishedSemaphores)
        vkDestroySemaphore(s_VulkanLogicalDevice, semaphore, nullptr);
    for (auto& fence : s_VulkanInFlightFences)
        vkDestroyFence(s_VulkanLogicalDevice, fence, nullptr);

    vkDestroyCommandPool(s_VulkanLogicalDevice, s_VulkanCommandPool, nullptr);

    for (auto& framebuffer : s_VulkanSwapChainFramebuffers)
        vkDestroyFramebuffer(s_VulkanLogicalDevice, framebuffer, nullptr);

    vkDestroyPipeline(s_VulkanLogicalDevice, s_VulkanGraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(s_VulkanLogicalDevice, s_VulkanPipelineLayout, nullptr);
    vkDestroyRenderPass(s_VulkanLogicalDevice, s_VulkanRenderPass, nullptr);

    for (auto& imageView : s_VulkanSwapChainImageViews)
        vkDestroyImageView(s_VulkanLogicalDevice, imageView, nullptr);

    vkDestroySwapchainKHR(s_VulkanLogicalDevice, s_VulkanSwapChain, nullptr);
    vkDestroySurfaceKHR(s_VulkanInstance, s_VulkanSurface, nullptr);
    vkDestroyDevice(s_VulkanLogicalDevice, nullptr);
    vkDestroyInstance(s_VulkanInstance, nullptr);

    if (s_Mode == WindowMode::Fullscreen)
        setFullscreen(false);

    if (smallIconHandle)
        DestroyIcon(smallIconHandle);
    if (largeIconHandle)
        DestroyIcon(largeIconHandle);

    for (auto& [_, cursor] : s_Cursors)
        DestroyCursor(cursor);

    DestroyWindow(s_Win32Handle);
}

LRESULT CALLBACK Window::WindowProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE: [[fallthrough]];
    case WM_QUIT:
    {
        WindowClosedEvent e;
        EventDispatcher::dispatch(e);
        App::onWindowClosed(e);

        return 0;
    }

    case WM_LBUTTONDOWN:
        onMouseButtonPressed(MouseButtons::Left);
        return 0;

    case WM_LBUTTONUP:
        onMouseButtonReleased(MouseButtons::Left);
        return 0;

    case WM_RBUTTONDOWN:
        onMouseButtonPressed(MouseButtons::Right);
        return 0;

    case WM_RBUTTONUP:
        onMouseButtonReleased(MouseButtons::Right);
        return 0;

    case WM_MBUTTONDOWN:
        onMouseButtonPressed(MouseButtons::Middle);
        return 0;

    case WM_MBUTTONUP:
        onMouseButtonReleased(MouseButtons::Middle);
        return 0;

    case WM_XBUTTONDOWN:
        onMouseButtonPressed(GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? MouseButtons::MB_4 : MouseButtons::MB_5);
        return 0;

    case WM_XBUTTONUP:
        onMouseButtonReleased(GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? MouseButtons::MB_4 : MouseButtons::MB_5);
        return 0;

    case WM_MOUSEMOVE:
    {
        float beforeX = Mouse::s_X;
        float beforeY = Mouse::s_Y;

        Mouse::s_X = (float)GET_X_LPARAM(lParam);
        Mouse::s_Y = s_CurrentHeight - (float)GET_Y_LPARAM(lParam) - 1;

        float deltaX = Mouse::s_X - beforeX;
        float deltaY = Mouse::s_Y - beforeY;

        for (MouseButton button = 0; button < Mouse::s_Buttons.size(); button++)
            if (Mouse::s_Buttons[button])
                EventDispatcher::dispatch(MouseDraggedEvent(button, Mouse::s_X, Mouse::s_Y, beforeX, beforeY, deltaX, deltaY));

        EventDispatcher::dispatch(MouseMovedEvent(Mouse::s_X, Mouse::s_Y, beforeX, beforeY, deltaX, deltaY));

        return 0;
    }

    case WM_MOUSEWHEEL:
        EventDispatcher::dispatch(MouseScrolledEvent(Mouse::s_X, Mouse::s_Y, 0.0f, (float)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA));
        return 0;

    case WM_SYSKEYDOWN: [[fallthrough]];
    case WM_KEYDOWN:
    {
        Key key = getKey(lParam, wParam);

        wchar_t keyChar = 0;
        if (MSG charMessage;
            PeekMessage(&charMessage, NULL, WM_CHAR, WM_CHAR, PM_REMOVE))
        {
            keyChar = *((wchar_t*)&charMessage.wParam);
        }

        Keyboard::s_Keys[key] = true;
        EventDispatcher::dispatch(KeyPressedEvent(key, keyChar));

        if (s_Info.closeOnAltF4 && key == Keys::F4 && Keyboard::isKeyPressed(Keys::Alt))
            EventDispatcher::dispatch(WindowClosedEvent());

        return 0;
    }

    case WM_SYSKEYUP: [[fallthrough]];
    case WM_KEYUP:
    {
        Key key = getKey(lParam, wParam);

        Keyboard::s_Keys[key] = false;
        EventDispatcher::dispatch(KeyReleasedEvent(key));

        return 0;
    }

    case WM_SIZE:
        if (!s_IgnoreSizeMessage)
        {
            if (wParam == SIZE_MINIMIZED)
                s_Minimized = true;
            else if (s_Mode == WindowMode::Windowed && wParam == SIZE_MAXIMIZED)
                s_Maximized = true;

            onResized((int)LOWORD(lParam), (int)HIWORD(lParam));
        }
        return 0;

    case WM_KILLFOCUS:
        EventDispatcher::dispatch(WindowLostFocusEvent());

        if (s_Mode == WindowMode::Fullscreen)
            ShowWindow(s_Win32Handle, SW_MINIMIZE);

        return 0;

    case WM_SETFOCUS:
        Mouse::s_Buttons = { false };
        Keyboard::s_Keys = { false };

        EventDispatcher::dispatch(WindowGainedFocusEvent());
        return 0;

    case WM_SETCURSOR:
        if (LOWORD(lParam) == HTCLIENT)
        {
            SetCursor(s_CurrentCursor);
            return TRUE;
        }
        else
        {
            break;
        }
    }

    return DefWindowProc(handle, msg, wParam, lParam);
}


static void initVulkan()
{
    createVulkanInstance();

#ifdef SONIC_DEBUG
    initVulkanDebugMessenger();
#endif

    initVulkanSurface();
    initVulkanPhysicalDevice();
    initVulkanLogicalDevice();
    initVulkanSwapChain();
    initVulkanSwapChainImageViews();
    initVulkanRenderPass();
    initVulkanGraphicsPipeline();
    initVulkanFramebuffers();
    initVulkanCommandPool();
    initVulkanCommandBuffers();
    initVulkanSyncing();
}

static void createVulkanInstance()
{
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = s_Title.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Sonic Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceInfo = {};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.enabledExtensionCount = VULKAN_INSTANCE_EXTENSION_COUNT;
    instanceInfo.ppEnabledExtensionNames = VULKAN_INSTANCE_EXTENSION_NAMES;
    instanceInfo.enabledLayerCount = 0;

#ifdef SONIC_DEBUG
    instanceInfo.enabledLayerCount = VULKAN_VALIDATION_LAYER_COUNT;
    instanceInfo.ppEnabledLayerNames = VULKAN_VALIDATION_LAYER_NAMES;

    VkDebugUtilsMessengerCreateInfoEXT messengerInfo = { };
    initVulkanDebugMessengerCreateInfo(&messengerInfo);
    instanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&messengerInfo;
#else
    instanceInfo.enabledLayerCount = 0;
    instanceInfo.ppEnabledLayerNames = nullptr;
#endif

    SONIC_ASSERT(checkRequiredVulkanExtensions(), "Failed to initialize Vulkan: One or more required extensions could not be found.");
    SONIC_DEBUG_ASSERT(checkRequiredVulkanValidationLayers(), "Failed to initialize Vulkan: One or more required validation layers could not be found");

    VkResult err = vkCreateInstance(&instanceInfo, nullptr, &s_VulkanInstance);
    SONIC_ASSERT(err == VK_SUCCESS, "Failed to create Vulkan instance (Vulkan error code: ", err, ")");
}

static bool checkRequiredVulkanExtensions()
{
    uint32_t availableExtensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions = std::vector<VkExtensionProperties>(availableExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

    for (int i = 0; i < VULKAN_INSTANCE_EXTENSION_COUNT; i++)
    {
        bool found = false;
        for (auto& extension : availableExtensions)
        {
            if (strcmp(extension.extensionName, VULKAN_INSTANCE_EXTENSION_NAMES[i]))
            {
                found = true;
                break;
            }
        }

        if (!found)
            return false;
    }

    return true;
}

static bool checkRequiredVulkanValidationLayers()
{
    uint32_t availableValidationLayerCount;
    vkEnumerateInstanceLayerProperties(&availableValidationLayerCount, nullptr);

    std::vector<VkLayerProperties> availableValidationLayers = std::vector<VkLayerProperties>(availableValidationLayerCount);
    vkEnumerateInstanceLayerProperties(&availableValidationLayerCount, availableValidationLayers.data());

    for (int i = 0; i < VULKAN_VALIDATION_LAYER_COUNT; i++)
    {
        bool found = false;
        for (auto& validationLayer : availableValidationLayers)
        {
            if (strcmp(validationLayer.layerName, VULKAN_VALIDATION_LAYER_NAMES[i]))
            {
                found = true;
                break;
            }
        }

        if (!found)
            return false;
    }

    return true;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL vulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {

    const char* messageTypeName;
    if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
        messageTypeName = "[PERFORMANCE] ";
    else if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
        messageTypeName = "[VALIDATION] ";
    else
        messageTypeName = "[GENERAL] ";

    switch (messageSeverity)
    {
    //case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT: [[fallthrough]];
    //case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        //SONIC_LOG_DEBUG("Vulkan validation layer: ", messageTypeName, pCallbackData->pMessage);
        //break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        SONIC_LOG_WARN("Vulkan validation layer: ", messageTypeName, pCallbackData->pMessage);
        break;
    case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        SONIC_LOG_ERROR("Vulkan validation layer: ", messageTypeName, pCallbackData->pMessage);
        break;
    }

    return VK_FALSE;
}

static void initVulkanDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* messengerInfo)
{
    messengerInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    messengerInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    messengerInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    messengerInfo->pfnUserCallback = vulkanDebugCallback;
}
static void initVulkanDebugMessenger()
{
    VkDebugUtilsMessengerCreateInfoEXT messengerInfo = { };
    initVulkanDebugMessengerCreateInfo(&messengerInfo);

    auto vulkanDebugMessengerCreateFunc = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(s_VulkanInstance, "vkCreateDebugUtilsMessengerEXT");
    s_VulkanDebugMessengerDestroyFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(s_VulkanInstance, "vkDestroyDebugUtilsMessengerEXT");
    SONIC_DEBUG_ASSERT(vulkanDebugMessengerCreateFunc != nullptr, "Failed to initialize Vulkan debug messenger: Extension is not present.");
    SONIC_DEBUG_ASSERT(s_VulkanDebugMessengerDestroyFunc != nullptr, "Failed to initialize Vulkan debug messenger: Extension is not present.");

    vulkanDebugMessengerCreateFunc(s_VulkanInstance, &messengerInfo, nullptr, &s_VulkanDebugMessenger);
}

static void initVulkanSurface()
{
    VkWin32SurfaceCreateInfoKHR surfaceInfo = { };
    surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    surfaceInfo.hwnd = s_Win32Handle;
    surfaceInfo.hinstance = s_Win32Module;

    VkResult err = vkCreateWin32SurfaceKHR(s_VulkanInstance, &surfaceInfo, nullptr, &s_VulkanSurface);
    SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize vulkan: Could not initiaize win32 surface");
}

static bool checkVulkanPhysicalDeviceExtensionSupport(VkPhysicalDevice* physicalDevice)
{
    uint32_t availableExtensionCount;
    vkEnumerateDeviceExtensionProperties(*physicalDevice, nullptr, &availableExtensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions = std::vector<VkExtensionProperties>(availableExtensionCount);
    vkEnumerateDeviceExtensionProperties(*physicalDevice, nullptr, &availableExtensionCount, availableExtensions.data());

    for (int i = 0; i < VULKAN_DEVICE_EXTENSION_COUNT; i++)
    {
        bool found = false;
        for (auto& extension : availableExtensions)
        {
            if (strcmp(extension.extensionName, VULKAN_DEVICE_EXTENSION_NAMES[i]))
            {
                found = true;
                break;
            }
        }

        if (!found)
            return false;
    }

    return true;
}

static VulkanSwapChainDetails getVulkanSwapChainDetails(VkPhysicalDevice* physicalDevice)
{
    VulkanSwapChainDetails swapChainDetails;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*physicalDevice, s_VulkanSurface, &swapChainDetails.surfaceCapabilities);

    uint32_t availableSurfaceFormatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(*physicalDevice, s_VulkanSurface, &availableSurfaceFormatCount, nullptr);

    swapChainDetails.availableSurfaceFormats = std::vector<VkSurfaceFormatKHR>(availableSurfaceFormatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(*physicalDevice, s_VulkanSurface, &availableSurfaceFormatCount, 
        swapChainDetails.availableSurfaceFormats.data());

    uint32_t availableSurfacePresentationModesCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(*physicalDevice, s_VulkanSurface, &availableSurfacePresentationModesCount, nullptr);

    swapChainDetails.availableSurfacePresentationModes = std::vector<VkPresentModeKHR>(availableSurfaceFormatCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(*physicalDevice, s_VulkanSurface, &availableSurfacePresentationModesCount, 
        swapChainDetails.availableSurfacePresentationModes.data());

    return swapChainDetails;
}

static bool isVulkanPhysicalDeviceSuitable(VkPhysicalDevice* physicalDevice)
{
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(*physicalDevice, &properties);

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(*physicalDevice, &features);

    VulkanQueueFamilieIndices queueFamilies = findVulkanQueueFamilies(physicalDevice);
    VulkanSwapChainDetails swapChainDetails = getVulkanSwapChainDetails(physicalDevice);

    return checkVulkanPhysicalDeviceExtensionSupport(physicalDevice) && queueFamilies.IsComplete() && swapChainDetails.IsSuitable();
}

static VulkanQueueFamilieIndices findVulkanQueueFamilies(VkPhysicalDevice* physicalDevice)
{
    VulkanQueueFamilieIndices queueFamilies;

    uint32_t availableQueueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(*physicalDevice, &availableQueueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> availableQueueFamilies = std::vector<VkQueueFamilyProperties>(availableQueueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(*physicalDevice, &availableQueueFamilyCount, availableQueueFamilies.data());

    for (uint32_t i = 0, size = (uint32_t)availableQueueFamilies.size(); i < size; i++)
    {
        auto& queueFamily = availableQueueFamilies.at(i);

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            queueFamilies.graphicsFamily = i;

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(*physicalDevice, i, s_VulkanSurface, &presentSupport);

        if (presentSupport)
            queueFamilies.presentFamily = i;

        if (queueFamilies.IsComplete())
            break;
    }

    return queueFamilies;
}

static void initVulkanPhysicalDevice()
{
    uint32_t availablePhysicalDeviceCount;
    vkEnumeratePhysicalDevices(s_VulkanInstance, &availablePhysicalDeviceCount, nullptr);

    std::vector<VkPhysicalDevice> availablePhysicalDevices = std::vector<VkPhysicalDevice>(availablePhysicalDeviceCount);
    vkEnumeratePhysicalDevices(s_VulkanInstance, &availablePhysicalDeviceCount, availablePhysicalDevices.data());

    s_VulkanPhysicalDevice = VK_NULL_HANDLE;
    for (auto& physicalDevice : availablePhysicalDevices)
    {
        if (isVulkanPhysicalDeviceSuitable(&physicalDevice))
        {
            s_VulkanPhysicalDevice = physicalDevice;
            break;
        }
    }

    SONIC_ASSERT(s_VulkanPhysicalDevice != VK_NULL_HANDLE, "Failed to initialize Vulkan: No compatible GPU found");
}

static void initVulkanLogicalDevice()
{
    VulkanQueueFamilieIndices queueFamilies = findVulkanQueueFamilies(&s_VulkanPhysicalDevice);

    std::set<uint32_t> uniqueQueueFamilies = {
        queueFamilies.graphicsFamily.value(),
        queueFamilies.presentFamily.value()
    };

    float queuePriority = 1.0f;
    std::vector<VkDeviceQueueCreateInfo> queueInfos;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueInfo = { };
        queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo.queueFamilyIndex = queueFamily;
        queueInfo.queueCount = 1;
        queueInfo.pQueuePriorities = &queuePriority;

        queueInfos.push_back(queueInfo);
    }

    VkPhysicalDeviceFeatures features = { };

    VkDeviceCreateInfo deviceInfo = { };
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = (uint32_t)queueInfos.size();
    deviceInfo.pQueueCreateInfos = queueInfos.data();
    deviceInfo.pEnabledFeatures = &features;
    deviceInfo.enabledExtensionCount = VULKAN_DEVICE_EXTENSION_COUNT;
    deviceInfo.ppEnabledExtensionNames = VULKAN_DEVICE_EXTENSION_NAMES;

#ifdef SONIC_DEBUG
    deviceInfo.enabledLayerCount = VULKAN_VALIDATION_LAYER_COUNT;
    deviceInfo.ppEnabledLayerNames = VULKAN_VALIDATION_LAYER_NAMES;
#else
    deviceInfo.enabledLayerCount = 0;
    deviceInfo.ppEnabledLayerNames = 0;
#endif

    VkResult err = vkCreateDevice(s_VulkanPhysicalDevice, &deviceInfo, nullptr, &s_VulkanLogicalDevice);
    SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialized Vulkan logical device");

    vkGetDeviceQueue(s_VulkanLogicalDevice, queueFamilies.graphicsFamily.value(), 0, &s_VulkanGraphicsQueue);
    vkGetDeviceQueue(s_VulkanLogicalDevice, queueFamilies.presentFamily.value(), 0, &s_VulkanPresentQueue);
}

static VkSurfaceFormatKHR chooseVulkanSurfaceFormat(VulkanSwapChainDetails* swapChainDetails)
{
    for (auto& surfaceFormat : swapChainDetails->availableSurfaceFormats)
    {
        if (surfaceFormat.format == VK_FORMAT_R8G8B8A8_SRGB && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            return surfaceFormat;
    }

    return swapChainDetails->availableSurfaceFormats.at(0);
}

static VkPresentModeKHR chooseVulkanPresentationMode(VulkanSwapChainDetails* swapChainDetails)
{
    for (auto& presentationMode : swapChainDetails->availableSurfacePresentationModes)
    {
        if (presentationMode == VK_PRESENT_MODE_MAILBOX_KHR)
            return presentationMode;
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D chooseSwapExtent(VulkanSwapChainDetails* swapChainDetails)
{
    if (swapChainDetails->surfaceCapabilities.currentExtent.width != UINT32_MAX)
    {
        return swapChainDetails->surfaceCapabilities.currentExtent;
    }
    else
    {
        VkExtent2D extent;
        VkExtent2D* minExtent = &swapChainDetails->surfaceCapabilities.minImageExtent;
        VkExtent2D* maxExtent = &swapChainDetails->surfaceCapabilities.minImageExtent;

        extent.width = Math::clamp((uint32_t)s_CurrentWidth, minExtent->width, maxExtent->width);
        extent.width = Math::clamp((uint32_t)s_CurrentHeight, minExtent->height, maxExtent->height);

        return extent;
    }
}

static void initVulkanSwapChain()
{
    VulkanSwapChainDetails swapChainDetails = getVulkanSwapChainDetails(&s_VulkanPhysicalDevice);

    VkSurfaceFormatKHR surfaceFormat = chooseVulkanSurfaceFormat(&swapChainDetails);
    VkPresentModeKHR presentationMode = chooseVulkanPresentationMode(&swapChainDetails);
    VkExtent2D swapExtent = chooseSwapExtent(&swapChainDetails);

    uint32_t imageCount = swapChainDetails.surfaceCapabilities.minImageCount + 1;
    if (imageCount > swapChainDetails.surfaceCapabilities.maxImageCount && swapChainDetails.surfaceCapabilities.maxImageCount != 0)
        imageCount = swapChainDetails.surfaceCapabilities.maxImageCount;

    VkSwapchainCreateInfoKHR swapChainInfo = { };
    swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainInfo.surface = s_VulkanSurface;
    swapChainInfo.minImageCount = imageCount;
    swapChainInfo.imageFormat = surfaceFormat.format;
    swapChainInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapChainInfo.imageExtent = swapExtent;
    swapChainInfo.imageArrayLayers = 1;
    swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    VulkanQueueFamilieIndices queueFamilies = findVulkanQueueFamilies(&s_VulkanPhysicalDevice);
    uint32_t queueFamilyIndices[] = { queueFamilies.graphicsFamily.value(), queueFamilies.presentFamily.value() };

    if (queueFamilies.graphicsFamily != queueFamilies.presentFamily) {
        swapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainInfo.queueFamilyIndexCount = 2;
        swapChainInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapChainInfo.queueFamilyIndexCount = 0; // Optional
        swapChainInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    swapChainInfo.preTransform = swapChainDetails.surfaceCapabilities.currentTransform;
    swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainInfo.presentMode = presentationMode;
    swapChainInfo.clipped = VK_TRUE;
    swapChainInfo.oldSwapchain = VK_NULL_HANDLE;

    VkResult err = vkCreateSwapchainKHR(s_VulkanLogicalDevice, &swapChainInfo, nullptr, &s_VulkanSwapChain);
    SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize Vulkan: Swap chain initialization failed.");

    uint32_t swapChainImagesAmount;
    vkGetSwapchainImagesKHR(s_VulkanLogicalDevice, s_VulkanSwapChain, &swapChainImagesAmount, nullptr);

    s_VulkanSwapChainImages.resize(swapChainImagesAmount);
    vkGetSwapchainImagesKHR(s_VulkanLogicalDevice, s_VulkanSwapChain, &swapChainImagesAmount, s_VulkanSwapChainImages.data());

    s_VulkanSwapChainImageFormat = surfaceFormat.format;
    s_VulkanSwapChainExtent = swapExtent;
}

static void initVulkanSwapChainImageViews()
{
    s_VulkanSwapChainImageViews.resize(s_VulkanSwapChainImages.size());

    for (size_t i = 0, size = s_VulkanSwapChainImages.size(); i < size; i++)
    {
        VkImageViewCreateInfo imageViewInfo = { };
        imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewInfo.image = s_VulkanSwapChainImages.at(i);
        imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewInfo.format = s_VulkanSwapChainImageFormat;
        imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewInfo.subresourceRange.baseMipLevel = 0;
        imageViewInfo.subresourceRange.levelCount = 1;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount = 1;

        VkResult err = vkCreateImageView(s_VulkanLogicalDevice, &imageViewInfo, nullptr, &s_VulkanSwapChainImageViews.at(i));
        SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize Vulkan: Could not create swap chain image view");
    }
}

static void initVulkanRenderPass()
{
    VkAttachmentDescription colorAttachment = { };
    colorAttachment.format = s_VulkanSwapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = { };
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = { };
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkSubpassDependency dependency = { };
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = { };
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    VkResult err = vkCreateRenderPass(s_VulkanLogicalDevice, &renderPassInfo, nullptr, &s_VulkanRenderPass);
    SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize Vulkan: Could not create render pass");
}

static std::vector<uint8_t> readBinaryVulkanShader(const String& filePath)
{
    std::ifstream file = std::ifstream(coreResourceDir() + filePath, std::ios::ate | std::ios::binary);

    size_t fileSize = file.tellg();
    std::vector<uint8_t> data = std::vector<uint8_t>(fileSize);

    file.seekg(0);
    file.read((char*)data.data(), fileSize);

    file.close();

    return data;
}

static VkShaderModule createVulkanShader(const std::vector<uint8_t>& binaryCode)
{
    VkShaderModule shader;

    VkShaderModuleCreateInfo shaderInfo = { };
    shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    shaderInfo.codeSize = binaryCode.size();
    shaderInfo.pCode = (uint32_t*)binaryCode.data();

    VkResult err = vkCreateShaderModule(s_VulkanLogicalDevice, &shaderInfo, nullptr, &shader);
    SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize vulkan: Could not create shader");

    return shader;
}

static void initVulkanGraphicsPipeline()
{
    auto vertexShaderCode = readBinaryVulkanShader("shaders/vert.spv");
    auto fragmentShaderCode = readBinaryVulkanShader("shaders/frag.spv");

    VkShaderModule vertexShader = createVulkanShader(vertexShaderCode);
    VkShaderModule fragmentShader = createVulkanShader(fragmentShaderCode);

    VkPipelineShaderStageCreateInfo vertexShaderStageInfo{};
    vertexShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertexShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertexShaderStageInfo.module = vertexShader;
    vertexShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragmentShaderStageInfo{};
    fragmentShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragmentShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragmentShaderStageInfo.module = fragmentShader;
    fragmentShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStageInfos[2] = { vertexShaderStageInfo, fragmentShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = { };
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = { };
    inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)s_VulkanSwapChainExtent.width;
    viewport.height = (float)s_VulkanSwapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = { };
    scissor.offset = { 0, 0 };
    scissor.extent = s_VulkanSwapChainExtent;

    VkPipelineViewportStateCreateInfo viewportInfo = {  };
    viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportInfo.viewportCount = 1;
    viewportInfo.pViewports = &viewport;
    viewportInfo.scissorCount = 1;
    viewportInfo.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizerInfo = { };
    rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizerInfo.depthClampEnable = VK_FALSE;
    rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
    rasterizerInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizerInfo.lineWidth = 1.0f;
    rasterizerInfo.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
    rasterizerInfo.depthBiasEnable = VK_FALSE;
    rasterizerInfo.depthBiasConstantFactor = 0.0f; // Optional
    rasterizerInfo.depthBiasClamp = 0.0f; // Optional
    rasterizerInfo.depthBiasSlopeFactor = 0.0f; // Optional

    VkPipelineMultisampleStateCreateInfo multisamplingInfo = { };
    multisamplingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisamplingInfo.sampleShadingEnable = VK_FALSE;
    multisamplingInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisamplingInfo.minSampleShading = 1.0f; // Optional
    multisamplingInfo.pSampleMask = nullptr; // Optional
    multisamplingInfo.alphaToCoverageEnable = VK_FALSE; // Optional
    multisamplingInfo.alphaToOneEnable = VK_FALSE; // Optional

    VkPipelineColorBlendAttachmentState colorBlendAttachmentInfo{};
    colorBlendAttachmentInfo.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | 
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachmentInfo.blendEnable = VK_FALSE;
    colorBlendAttachmentInfo.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachmentInfo.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachmentInfo.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachmentInfo.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachmentInfo.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    colorBlendAttachmentInfo.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

    VkPipelineColorBlendStateCreateInfo colorBlendingInfo = { };
    colorBlendingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendingInfo.logicOpEnable = VK_FALSE;
    colorBlendingInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlendingInfo.attachmentCount = 1;
    colorBlendingInfo.pAttachments = &colorBlendAttachmentInfo;
    colorBlendingInfo.blendConstants[0] = 0.0f; // Optional
    colorBlendingInfo.blendConstants[1] = 0.0f; // Optional
    colorBlendingInfo.blendConstants[2] = 0.0f; // Optional
    colorBlendingInfo.blendConstants[3] = 0.0f; // Optional

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0; // Optional
    pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
    pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
    pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

    VkResult err = vkCreatePipelineLayout(s_VulkanLogicalDevice, &pipelineLayoutInfo, nullptr, &s_VulkanPipelineLayout);
    SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize Vulkan: Could not create pipeline layout");

    VkGraphicsPipelineCreateInfo pipelineInfo = { };
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStageInfos;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
    pipelineInfo.pViewportState = &viewportInfo;
    pipelineInfo.pRasterizationState = &rasterizerInfo;
    pipelineInfo.pMultisampleState = &multisamplingInfo;
    pipelineInfo.pDepthStencilState = nullptr; // Optional
    pipelineInfo.pColorBlendState = &colorBlendingInfo;
    pipelineInfo.pDynamicState = nullptr; // Optional
    pipelineInfo.layout = s_VulkanPipelineLayout;
    pipelineInfo.renderPass = s_VulkanRenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1; // Optional

    err = vkCreateGraphicsPipelines(s_VulkanLogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &s_VulkanGraphicsPipeline);
    SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize Vulkan: Could not create graphics pipeline");

    vkDestroyShaderModule(s_VulkanLogicalDevice, vertexShader, nullptr);
    vkDestroyShaderModule(s_VulkanLogicalDevice, fragmentShader, nullptr);
}

static void initVulkanFramebuffers()
{
    s_VulkanSwapChainFramebuffers.resize(s_VulkanSwapChainImageViews.size());

    for (size_t i = 0; i < s_VulkanSwapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            s_VulkanSwapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = s_VulkanRenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = s_VulkanSwapChainExtent.width;
        framebufferInfo.height = s_VulkanSwapChainExtent.height;
        framebufferInfo.layers = 1;

        VkResult err = vkCreateFramebuffer(s_VulkanLogicalDevice, &framebufferInfo, nullptr, &s_VulkanSwapChainFramebuffers[i]);
        SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize Vulkan: Could not create swap chain framebuffer");
    }
}

static void initVulkanCommandPool()
{
    VulkanQueueFamilieIndices queueFamilyIndices = findVulkanQueueFamilies(&s_VulkanPhysicalDevice);

    VkCommandPoolCreateInfo commandPoolInfo = { };
    commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    commandPoolInfo.flags = 0; // Optional

    VkResult err = vkCreateCommandPool(s_VulkanLogicalDevice, &commandPoolInfo, nullptr, &s_VulkanCommandPool);
}

static void initVulkanCommandBuffers()
{
    s_VulkanCommandBuffers.resize(s_VulkanSwapChainFramebuffers.size());

    VkCommandBufferAllocateInfo commandBufferAllocationInfo = { };
    commandBufferAllocationInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    commandBufferAllocationInfo.commandPool = s_VulkanCommandPool;
    commandBufferAllocationInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    commandBufferAllocationInfo.commandBufferCount = (uint32_t)s_VulkanCommandBuffers.size();

    VkResult err = vkAllocateCommandBuffers(s_VulkanLogicalDevice, &commandBufferAllocationInfo, s_VulkanCommandBuffers.data());
    SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize Vulkan: Could not allocate command buffers");

    for (size_t i = 0; i < s_VulkanCommandBuffers.size(); i++)
    {
        VkCommandBufferBeginInfo beginInfo = { };
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        err = vkBeginCommandBuffer(s_VulkanCommandBuffers.at(i), &beginInfo);
        SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize Vulkan: Could not begin recording command buffer");

        VkRenderPassBeginInfo renderBeginPassInfo { };
        renderBeginPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderBeginPassInfo.renderPass = s_VulkanRenderPass;
        renderBeginPassInfo.framebuffer = s_VulkanSwapChainFramebuffers.at(i);
        renderBeginPassInfo.renderArea.offset = { 0, 0 };
        renderBeginPassInfo.renderArea.extent = s_VulkanSwapChainExtent;
        renderBeginPassInfo.clearValueCount = 1;
        renderBeginPassInfo.pClearValues = (VkClearValue*)&s_ClearColor;

        vkCmdBeginRenderPass(s_VulkanCommandBuffers.at(i), &renderBeginPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(s_VulkanCommandBuffers.at(i), VK_PIPELINE_BIND_POINT_GRAPHICS, s_VulkanGraphicsPipeline);
        vkCmdDraw(s_VulkanCommandBuffers.at(i), 3, 1, 0, 0);
        vkCmdEndRenderPass(s_VulkanCommandBuffers.at(i));

        err = vkEndCommandBuffer(s_VulkanCommandBuffers.at(i));
        SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize Vulkan: Could not end recording command buffer");
    }
}

static void initVulkanSyncing()
{
    s_VulkanSwapChainImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    s_VulkanRenderingFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    s_VulkanInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    s_VulkanImagesInFlight.resize(s_VulkanSwapChainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo = { };
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = { };
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkResult err = vkCreateSemaphore(s_VulkanLogicalDevice, &semaphoreInfo, nullptr, &s_VulkanSwapChainImageAvailableSemaphores.at(i));
        SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize Vulkan: Could not create semaphores");
        err = vkCreateSemaphore(s_VulkanLogicalDevice, &semaphoreInfo, nullptr, &s_VulkanRenderingFinishedSemaphores.at(i));
        SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize Vulkan: Could not create semaphores");

        err = vkCreateFence(s_VulkanLogicalDevice, &fenceInfo, nullptr, &s_VulkanInFlightFences.at(i));
        SONIC_ASSERT(err == VK_SUCCESS, "Failed to initialize Vulkan: Could not create semaphores");
    }
}

void Window::testVulkanDrawing()
{
    vkWaitForFences(s_VulkanLogicalDevice, 1, &s_VulkanInFlightFences[s_VulkanCurrentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    vkAcquireNextImageKHR(s_VulkanLogicalDevice, s_VulkanSwapChain, UINT64_MAX, 
        s_VulkanSwapChainImageAvailableSemaphores.at(s_VulkanCurrentFrame), VK_NULL_HANDLE, &imageIndex);

    if (s_VulkanImagesInFlight[imageIndex] != VK_NULL_HANDLE) 
        vkWaitForFences(s_VulkanLogicalDevice, 1, &s_VulkanImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    
    // Mark the image as now being in use by this frame
    s_VulkanImagesInFlight[imageIndex] = s_VulkanInFlightFences[s_VulkanCurrentFrame];

    VkSubmitInfo submitInfo = { };
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { s_VulkanSwapChainImageAvailableSemaphores.at(s_VulkanCurrentFrame) };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &s_VulkanCommandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = { s_VulkanRenderingFinishedSemaphores.at(s_VulkanCurrentFrame) };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(s_VulkanLogicalDevice, 1, &s_VulkanInFlightFences[s_VulkanCurrentFrame]);

    VkResult err = vkQueueSubmit(s_VulkanGraphicsQueue, 1, &submitInfo, s_VulkanInFlightFences[s_VulkanCurrentFrame]);
    SONIC_ASSERT(err == VK_SUCCESS, "Failed to draw: Could not submit command to queue");

    VkPresentInfoKHR presentInfo = { };
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { s_VulkanSwapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optionals

    vkQueuePresentKHR(s_VulkanPresentQueue, &presentInfo);

    (++s_VulkanCurrentFrame) %= MAX_FRAMES_IN_FLIGHT;
}

static void initTimer()
{
    QueryPerformanceFrequency((LARGE_INTEGER*)&s_TimerFrequency);
    QueryPerformanceCounter((LARGE_INTEGER*)&s_TimerOffset);
}

static void initStyle()
{
    s_WindowedStyle = (WS_OVERLAPPED | WS_CAPTION);
    s_WindowedExStyle = 0;

    if (s_Info.closeButton)
        s_WindowedStyle |= WS_SYSMENU;

    if (s_Info.resizable)
        s_WindowedStyle |= (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
}

static void initCursors()
{
    for (auto& [name, cursor] : s_Info.cursors)
    {
        if (HCURSOR handle = (HCURSOR)createNativeIcon(cursor.width, cursor.height, cursor.bitmap, TRUE, cursor.hotspotX, cursor.hotspotY);
            handle)
            s_Cursors.emplace(name, handle);
    }

    s_CurrentCursor = s_Cursors[Cursors::Arrow];
}

static void initIcons()
{
    int smallIconWidth = GetSystemMetrics(SM_CXSMICON);
    int smallIconHeight = GetSystemMetrics(SM_CYSMICON);
    int largeIconWidth = GetSystemMetrics(SM_CXICON);
    int largeIconHeight = GetSystemMetrics(SM_CYICON);

    IconInfo smallIcon;
    double smallestSizeDifference = 1.0e35;
    for (auto& icon : s_Info.icons)
    {
        int xoff = smallIconWidth - icon.width;
        int yoff = smallIconHeight - icon.height;

        double offset = sqrt(xoff * xoff + yoff * yoff);
        if (offset < smallestSizeDifference)
        {
            smallestSizeDifference = offset;
            smallIcon = icon;
        }
    }

    IconInfo largeIcon;
    smallestSizeDifference = 1.0e35;
    for (auto& icon : s_Info.icons)
    {
        int xoff = largeIconWidth - icon.width;
        int yoff = largeIconHeight - icon.height;

        double offset = sqrt(xoff * xoff + yoff * yoff);
        if (offset < smallestSizeDifference)
        {
            smallestSizeDifference = offset;
            largeIcon = icon;
        }
    }

    smallIconHandle = createNativeIcon(smallIcon.width, smallIcon.height, smallIcon.bitmap);
    largeIconHandle = createNativeIcon(largeIcon.width, largeIcon.height, largeIcon.bitmap);

    if (smallIconHandle)
        SendMessage(s_Win32Handle, WM_SETICON, ICON_SMALL, (LPARAM)smallIconHandle);
    if (largeIconHandle)
        SendMessage(s_Win32Handle, WM_SETICON, ICON_BIG, (LPARAM)largeIconHandle);
}

static HICON createNativeIcon(int width, int height, uint8_t* bitmap, int isCursor, int hotspotX, int hotspotY)
{
    if (!bitmap)
        return NULL;

    HICON handle;
    HBITMAP color, mask;
    BITMAPV5HEADER bi = createBitmapHeader(width, height);
    ICONINFO info = {};
    uint8_t* nativeBitmap;

    HDC dc = GetDC(NULL);
    color = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS,
        (void**)&nativeBitmap, NULL, (DWORD)0);

    mask = CreateBitmap(width, height, 1, 1, NULL);
    ReleaseDC(NULL, dc);

    if (!color || !mask)
    {
        SONIC_LOG_ERROR("Error: Could not create win32 bitmaps");
        return NULL;
    }

    std::copy(bitmap, bitmap + 4 * width * height, nativeBitmap);

    info.fIcon = !isCursor;
    info.xHotspot = hotspotX;
    info.yHotspot = hotspotY;
    info.hbmMask = mask;
    info.hbmColor = color;

    handle = CreateIconIndirect(&info);

    DeleteObject(color);
    DeleteObject(mask);

    if (!handle)
    {
        SONIC_LOG_ERROR("Error: Could not create win32 ", isCursor ? "cursor" : "icon");
        return NULL;
    }

    return handle;
}

static BITMAPV5HEADER createBitmapHeader(int width, int height)
{
    BITMAPV5HEADER bi = {};

    bi.bV5Size = sizeof(BITMAPV5HEADER);
    bi.bV5Width = width;
    bi.bV5Height = height;
    bi.bV5Planes = 1;
    bi.bV5BitCount = 32;
    bi.bV5Compression = BI_BITFIELDS;
    bi.bV5RedMask = 0x00ff0000;
    bi.bV5GreenMask = 0x0000ff00;
    bi.bV5BlueMask = 0x000000ff;
    bi.bV5AlphaMask = 0xff000000;

    return bi;
}

static void setDecorated(bool decorated)
{
    s_IgnoreSizeMessage = true;
    SetWindowLong(s_Win32Handle, GWL_STYLE, decorated ? s_WindowedStyle : 0);
    SetWindowLong(s_Win32Handle, GWL_EXSTYLE, decorated ? s_WindowedExStyle : 0);
    s_IgnoreSizeMessage = false;
}

static void adjustSize(int width, int height)
{
    s_UnmaximizedWidth = (int)width;
    s_UnmaximizedHeight = height;

    updateWindowSize();
}

static void setFullscreen(bool fullscreen)
{
    DISPLAY_DEVICE displayDevice = {};
    displayDevice.cb = sizeof(DISPLAY_DEVICE);
    EnumDisplayDevices(NULL, 0, &displayDevice, 0);

    if (fullscreen)
    {
        DEVMODE deviceMode = {};
        deviceMode.dmSize = sizeof(DEVMODE);
        BOOL s = EnumDisplaySettingsEx(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &deviceMode, 0);

        deviceMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL |
            DM_DISPLAYFREQUENCY;

        LONG success = ChangeDisplaySettingsEx(displayDevice.DeviceName, &deviceMode, NULL, CDS_FULLSCREEN, NULL);
        if (success != DISP_CHANGE_SUCCESSFUL)
            SONIC_LOG_ERROR("Error changing display settings");
    }
    else
    {
        LONG success = ChangeDisplaySettingsEx(displayDevice.DeviceName, NULL, NULL, CDS_FULLSCREEN, NULL);
        if (success != DISP_CHANGE_SUCCESSFUL)
            SONIC_LOG_ERROR("Error changing display settings");
    }
}

static void updateWindowSize()
{
    if (s_Mode == WindowMode::Windowed)
    {
        RECT newWindow = { (s_MonitorWidth - s_UnmaximizedWidth) / 2, (s_MonitorHeight - s_UnmaximizedHeight) / 2,
            (s_MonitorWidth + s_UnmaximizedWidth) / 2, (s_MonitorHeight + s_UnmaximizedHeight) / 2 };
        AdjustWindowRectEx(&newWindow, s_WindowedStyle, FALSE, s_WindowedExStyle);

        SetWindowPos(s_Win32Handle, HWND_TOP, newWindow.left, newWindow.top,
            newWindow.right - newWindow.left, newWindow.bottom - newWindow.top, SWP_NOZORDER);

        ShowWindow(s_Win32Handle, s_Maximized ? SW_MAXIMIZE : s_Minimized ? SW_MINIMIZE : SW_NORMAL);
    }
    else if (s_Mode == WindowMode::WindowedBorderless)
    {
        SetWindowPos(s_Win32Handle, HWND_TOP, (s_MonitorWidth - s_UnmaximizedWidth) / 2, (s_MonitorHeight - s_UnmaximizedHeight) / 2,
            s_UnmaximizedWidth, s_UnmaximizedHeight, SWP_NOZORDER);

        ShowWindow(s_Win32Handle, s_Minimized ? SW_MINIMIZE : SW_SHOW);
    }
    else
    {
        SetWindowPos(s_Win32Handle, HWND_TOP, 0, 0, s_MonitorWidth, s_MonitorHeight, SWP_NOZORDER);
        ShowWindow(s_Win32Handle, s_Minimized ? SW_MINIMIZE : SW_SHOW);
    }
}

static Key getKey(LPARAM lParam, WPARAM wParam)
{
    Key key = *((Key*)&wParam);

    if (key == Keys::Control) // Win32 does weird things
    {
        if (lParam & 1 << 24) // Extended key -> Right Control
        {
            key = Keys::RightControl;
        }
        else if (MSG next;  // When right alt is pressed, two messages are sent, the first one has to be ignored
            PeekMessage(&next, NULL, WM_KEYFIRST, WM_KEYLAST, PM_NOREMOVE))
        {
            if (Key nextKey = *((Key*)&next.wParam);
                nextKey == Keys::Alt && next.lParam & 1 << 24)
            {
                return 0;
            }
        }
    }
    else if (lParam & 1 << 24)
    {
        if (key == Keys::Alt)
            key = Keys::RightAlt;
        else if (key == Keys::Enter)
            key = Keys::NumpadEnter;
    }

    return key;
}
