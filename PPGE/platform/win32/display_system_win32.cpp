#include "display_system_win32.h"

#include <backends/imgui_impl_win32.h>
#include <windowsx.h>
#if defined(IsMinimized)
#undef IsMinimized
#endif
#if defined(IsMaximized)
#undef IsMaximized
#endif

#include "platform/win32/input_codes_win32.h"
#include "system/renderer_system.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace PPGE
{
static uint8_t s_SystemInstanceCount = 0;
static LPCTSTR s_WndClassName = "PPGEWindow";

void *DisplaySystemWin32::GetNativeDisplayPtr() const
{
    return static_cast<void *>(m_hwnd);
}

std::tuple<float, float> DisplaySystemWin32::GetMousePosition()
{
    POINT mouse_pos{0};
    GetCursorPos(&mouse_pos);
    ScreenToClient(static_cast<HWND>(DisplaySystem::Get().GetNativeDisplayPtr()), &mouse_pos);
    return {static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y)};
}

bool DisplaySystemWin32::IsKeyPressed(const KeyCode code)
{
    return GetKeyState(static_cast<int>(Win32_Key_Codes[static_cast<size_t>(code)])) & 0x8000;
}

bool DisplaySystemWin32::IsMouseButtonPressed(const MouseCode code)
{
    return GetKeyState(static_cast<int>(Win32_Mouse_Codes[static_cast<size_t>(code)])) & 0x8000;
}

float DisplaySystemWin32::GetMouseX()
{
    auto [x, y] = GetMousePosition();
    return x;
}

float DisplaySystemWin32::GetMouseY()
{
    auto [x, y] = GetMousePosition();
    return y;
}

WindowProps::AttributeValue DisplaySystemWin32::GetWindowAttribute(WindowProps::AttributeTag attribute) const
{
    return 0;
}

void DisplaySystemWin32::SetWindowAttribute(WindowProps::AttributeTag attribute, WindowProps::AttributeValue value)
{
}

WindowProps::WindowMode DisplaySystemWin32::GetWindowMode() const
{
    return m_props.window_mode;
}

void DisplaySystemWin32::SetWindowMode(WindowProps::WindowMode mode)
{
}

bool DisplaySystemWin32::IsVsyncEnabled() const
{
    return true;
}

void DisplaySystemWin32::EnableVsync()
{
}

void DisplaySystemWin32::DisableVsync()
{
}

bool DisplaySystemWin32::IsMinimized() const
{
    return IsIconic(m_hwnd);
}

bool DisplaySystemWin32::IsMaximized() const
{
    return IsZoomed(m_hwnd);
}

void DisplaySystemWin32::SetInputEventCallback(WindowProps::InputEventCallback callback)
{
    m_props.input_event_callback = callback;
}

void DisplaySystemWin32::SetApplicationEventCallback(WindowProps::ApplicationEventCallback callback)
{
    m_props.application_event_callback = callback;
}

std::string_view DisplaySystemWin32::GetTitle() const
{
    return m_props.title;
}

uint32_t DisplaySystemWin32::GetHeight() const
{
    return m_props.height;
}

uint32_t DisplaySystemWin32::GetWidth() const
{
    return m_props.width;
}

void DisplaySystemWin32::StartUp(const DisplaySystemProps &props)
{
    m_props = props;
    // Start creating the window//
    m_hmodule = GetModuleHandle(NULL);
    WNDCLASSEX wc = {0}; // Create a new extended windows class

    wc.cbSize = sizeof(WNDCLASSEX);               // Size of our windows class
    wc.style = CS_HREDRAW | CS_VREDRAW;           // class styles
    wc.lpfnWndProc = DisplaySystemWin32::WinProc; // Default windows procedure function
    wc.cbClsExtra = NULL;                         // Extra bytes after our wc structure
    wc.cbWndExtra = NULL;                         // Extra bytes after our windows instance
    wc.hInstance = m_hmodule;                     // Instance to current application
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);       // Title bar Icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);     // Default mouse Icon
    wc.hbrBackground = NULL;                      // Window bg color
    wc.lpszMenuName = NULL;                       // Name of the menu attached to our window
    wc.lpszClassName = s_WndClassName;            // Name of our windows class
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);     // Icon in your taskbar

    if (!RegisterClassEx(&wc)) // Register our windows class
    {
        // if registration failed, display error
        PPGE_ERROR("Failed to register window class.");
        MessageBox(NULL, "Error registering class", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PPGE_INFO("Creating Display '{0}' ({1} x {1}) | Platform : Windows", m_props.title, m_props.width, m_props.height);

    m_hwnd = CreateWindowEx(        // Create our Extended Window
        NULL,                       // Extended style
        s_WndClassName,             // Name of our windows class
        props.title.c_str(),        // Name in the title bar of our window
        WS_OVERLAPPEDWINDOW,        // style of our window
        m_props.posX, m_props.posY, // Top left corner of window
        m_props.width,              // Width of our window
        m_props.height,             // Height of our window
        NULL,                       // Handle to parent window
        NULL,                       // Handle to a Menu
        m_hmodule,                  // Specifies instance of current program
        this                        // used for an MDI client window
    );

    if (!m_hwnd) // Make sure our window has been created
    {
        // If not, display error
        PPGE_ERROR("Failed to create window.");
        MessageBox(NULL, "Error creating window", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    ShowWindow(m_hwnd, SW_SHOWDEFAULT); // Shows our window
    UpdateWindow(m_hwnd);               // Its good to update our window

    return; // if there were no errors, return true
}

void DisplaySystemWin32::Update()
{
    if (PeekMessage(&m_msg, 0, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&m_msg);
        DispatchMessage(&m_msg);
    }
}

void DisplaySystemWin32::ShutDown()
{
    DestroyWindow(m_hwnd);
}

LRESULT DisplaySystemWin32::WinProc(HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, w_param, l_param))
        return true;

    DisplaySystemWin32 *window = nullptr;

    if (msg == WM_CREATE)
    {
        CREATESTRUCT *p_create = reinterpret_cast<CREATESTRUCT *>(l_param);
        window = reinterpret_cast<DisplaySystemWin32 *>(p_create->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
        window->m_hwnd = hwnd;
    }
    else
    {
        window = reinterpret_cast<DisplaySystemWin32 *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    if (window)
        return window->HandleMessage(msg, w_param, l_param);
    return DefWindowProc(hwnd, msg, w_param, l_param);
}

LRESULT DisplaySystemWin32::HandleMessage(UINT msg, WPARAM w_param, LPARAM l_param)
{
    switch (msg)
    {
    case WM_CLOSE: {
        WindowCloseEvent event;
        m_props.application_event_callback(event);
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    case WM_SIZE: {
        m_props.width = LOWORD(l_param);
        m_props.height = HIWORD(l_param);
        WindowResizeEvent event(m_props.width, m_props.height);
        m_props.application_event_callback(event);

        if (w_param == SIZE_MAXIMIZED)
        {
            WindowMaximizedEvent event;
            m_props.application_event_callback(event);
        }
        else if (w_param == SIZE_MINIMIZED)
        {
            WindowMinimizedEvent event;
            m_props.application_event_callback(event);
        }
        else if (w_param == SIZE_RESTORED)
        {
            WindowRestoredEvent event;
            m_props.application_event_callback(event);
        }
        RendererSystem::Get().OnResize();
        break;
    }
    case WM_MOVE: {
        m_props.posX = static_cast<size_t>(LOWORD(l_param));
        m_props.posY = static_cast<size_t>(HIWORD(l_param));
        WindowMoveEvent event(m_props.posX, m_props.posY);
        m_props.application_event_callback(event);
        break;
    }
    case WM_SETFOCUS: {
        m_props.window_attributes[static_cast<size_t>(WindowProps::AttributeTag::FOCUSED)] =
            static_cast<WindowProps::AttributeValue>(1);
        break;
    }
    case WM_KILLFOCUS: {
        m_props.window_attributes[static_cast<size_t>(WindowProps::AttributeTag::FOCUSED)] =
            static_cast<WindowProps::AttributeValue>(0);
        break;
    }
    case WM_KEYDOWN: {
        int repeat_count = static_cast<int>(l_param & 0xFFFF); // First 16 bits are repeat count
        KeyPressedEvent event(static_cast<KeyCode>(w_param), repeat_count);
        m_props.input_event_callback(event);
        break;
    }
    case WM_KEYUP: {
        KeyReleasedEvent event(static_cast<KeyCode>(w_param));
        m_props.input_event_callback(event);
        break;
    }
    case WM_CHAR: {
        KeyTypedEvent event(static_cast<KeyCode>(w_param));
        m_props.input_event_callback(event);
        break;
    }
    case WM_MOUSEMOVE: {
        MouseMovedEvent event(static_cast<float>(GET_X_LPARAM(l_param)), static_cast<float>(GET_Y_LPARAM(l_param)));
        m_props.input_event_callback(event);
        break;
    }
    case WM_MOUSEWHEEL: {
        MouseScrolledEvent event(0.0f, static_cast<float>(GET_WHEEL_DELTA_WPARAM(w_param)));
        m_props.input_event_callback(event);
        break;
    }
    case WM_MOUSEHWHEEL: {
        MouseScrolledEvent event(static_cast<float>(GET_WHEEL_DELTA_WPARAM(w_param)), 0.0f);
        m_props.input_event_callback(event);
        break;
    }
    case WM_LBUTTONDOWN: {
        MouseButtonPressedEvent event(PPGE_MOUSE_BUTTON_0);
        m_props.input_event_callback(event);
        break;
    }
    case WM_LBUTTONUP: {
        MouseButtonReleasedEvent event(PPGE_MOUSE_BUTTON_0);
        m_props.input_event_callback(event);
        break;
    }
    case WM_RBUTTONDOWN: {
        MouseButtonPressedEvent event(PPGE_MOUSE_BUTTON_1);
        m_props.input_event_callback(event);
        break;
    }
    case WM_RBUTTONUP: {
        MouseButtonReleasedEvent event(PPGE_MOUSE_BUTTON_1);
        m_props.input_event_callback(event);
        break;
    }
    case WM_MBUTTONDOWN: {
        MouseButtonPressedEvent event(PPGE_MOUSE_BUTTON_2);
        m_props.input_event_callback(event);
        break;
    }
    case WM_MBUTTONUP: {
        MouseButtonReleasedEvent event(PPGE_MOUSE_BUTTON_2);
        m_props.input_event_callback(event);
        break;
    }
    case WM_XBUTTONDOWN: {
        MouseCode code;
        if (GET_XBUTTON_WPARAM(w_param) == XBUTTON1)
            code = PPGE_MOUSE_BUTTON_3;
        else if (GET_XBUTTON_WPARAM(w_param) == XBUTTON2)
            code = PPGE_MOUSE_BUTTON_4;
        MouseButtonPressedEvent event(code);
        m_props.input_event_callback(event);
        break;
    }
    case WM_XBUTTONUP: {
        MouseCode code;
        if (GET_XBUTTON_WPARAM(w_param) == XBUTTON1)
            code = PPGE_MOUSE_BUTTON_3;
        else if (GET_XBUTTON_WPARAM(w_param) == XBUTTON2)
            code = PPGE_MOUSE_BUTTON_4;
        MouseButtonPressedEvent event(code);
        m_props.input_event_callback(event);
        break;
    }
    default:
        return DefWindowProc(m_hwnd, msg, w_param, l_param);
    }
    return TRUE;
}
} // namespace PPGE