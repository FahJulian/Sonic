#include <gl/glew.h>
#include <gl/wglew.h>
#include "_WIN32Include.h"
#include "Sonic/App.h"
#include "Sonic/Debug/Log/Log.h"
#include "Sonic/Event/EventDispatcher.h"
#include "Sonic/Event/Events/WindowEvents.h"
#include "Sonic/Event/Events/MouseEvents.h"
#include "Sonic/Event/Events/KeyEvents.h"
#include "Sonic/Util/StringUtils.h"
#include "WindowInfoLoader.h"
#include "Window.h"

using namespace Sonic;


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

    return true;
}

bool Window::createWin32Window()
{
    const wchar_t* className = L"Sample Window Class";
    HMODULE win32ModuleHandle = GetModuleHandle(0);

    WNDCLASSEX win32WindowClass = { };
    win32WindowClass.cbSize = sizeof(WNDCLASSEX);
    win32WindowClass.lpfnWndProc = Window::WindowProc;
    win32WindowClass.hInstance = win32ModuleHandle;
    win32WindowClass.lpszClassName = className;
    win32WindowClass.style = CS_OWNDC;

    RegisterClassEx(&win32WindowClass);

    s_Win32Handle = CreateWindowEx(0, className, Util::toWideString(s_Title).c_str(), 0,
        0, 0, 0, 0, NULL, NULL, win32ModuleHandle, NULL
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
