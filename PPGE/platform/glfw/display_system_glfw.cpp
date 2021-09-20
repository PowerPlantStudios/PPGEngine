#include "PPGEpch.h"

#include "display_system_windows.h"

#include <glad/glad.h>

namespace PPGE
{
static uint8_t s_SystemInstanceCount = 0;

static void GLFWErrorCallback(int error, const char *description)
{
    PPGE_ERROR("GLFW Error ({0}): {1}", error, description);
}

void *DisplaySystemWindows::GetNativeDisplayPtr() const
{
    return m_window_ptr;
}

WindowProps::AttributeValue DisplaySystemWindows::GetWindowAttribute(WindowProps::AttributeTag attribute) const
{
    return m_props.window_attributes[static_cast<size_t>(attribute)];
}

void DisplaySystemWindows::SetWindowAttribute(WindowProps::AttributeTag attribute, WindowProps::AttributeValue value)
{
    switch (attribute)
    {
    case WindowProps::AttributeTag::ALWAYS_ON_TOP: {
        glfwSetWindowAttrib(m_window_ptr, GLFW_FLOATING, static_cast<int>(value));
        break;
    }
    case WindowProps::AttributeTag::DECORATED: {
        glfwSetWindowAttrib(m_window_ptr, GLFW_DECORATED, static_cast<int>(value));
        break;
    }
    case WindowProps::AttributeTag::RESIZABLE: {
        glfwSetWindowAttrib(m_window_ptr, GLFW_RESIZABLE, static_cast<int>(value));
        break;
    }
    case WindowProps::AttributeTag::FOCUSED: {
        if (static_cast<bool>(value))
            glfwFocusWindow(m_window_ptr);
        break;
    }
    default: {
        PPGE_ASSERT(true, "Unknown window attribute.");
    }
    }

    m_props.window_attributes[static_cast<size_t>(attribute)] = value;
}

WindowProps::WindowMode DisplaySystemWindows::GetWindowMode() const
{
    return m_props.window_mode;
}

void DisplaySystemWindows::SetWindowMode(WindowProps::WindowMode mode)
{
    switch (mode)
    {
    case WindowProps::WindowMode::MAXIMIZED: {
        glfwMaximizeWindow(m_window_ptr);
        break;
    }
    case WindowProps::WindowMode::FULLSCREEN: {
        if (GetWindowMode() != WindowProps::WindowMode::FULLSCREEN)
        {
            GLFWmonitor *monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode *mode = glfwGetVideoMode(monitor);
            glfwSetWindowMonitor(m_window_ptr, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
        }
        break;
    }
    case WindowProps::WindowMode::WINDOWED: {
        if (GetWindowMode() == WindowProps::WindowMode::FULLSCREEN)
        {
            glfwSetWindowMonitor(m_window_ptr, nullptr, m_props.posX, m_props.posY, m_props.width, m_props.height,
                                 GLFW_DONT_CARE);
        }
        break;
    }
    case WindowProps::WindowMode::MINIMIZED: {
        glfwIconifyWindow(m_window_ptr);
        break;
    }
    default: {
        PPGE_ASSERT(true, "Unknown window mode.");
    }
    }
    m_props.window_mode = mode;
}

bool DisplaySystemWindows::IsVsyncEnabled() const
{
    return m_props.vSync;
}

void DisplaySystemWindows::EnableVsync()
{
    if (!m_props.vSync)
    {
        m_props.vSync = true;
        glfwSwapInterval(1);
    }
}

void DisplaySystemWindows::DisableVsync()
{
    if (m_props.vSync)
    {
        m_props.vSync = false;
        glfwSwapInterval(0);
    }
}

bool DisplaySystemWindows::IsMinimized() const
{
    return m_props.window_mode == WindowProps::WindowMode::MINIMIZED;
}

bool DisplaySystemWindows::IsMaximized() const
{
    return m_props.window_mode == WindowProps::WindowMode::MAXIMIZED;
}

WindowProps::InputEventCallback DisplaySystemWindows::GetInputEventCallback() const
{
    return m_props.input_event_callback;
}

void DisplaySystemWindows::SetInputEventCallback(WindowProps::InputEventCallback callback)
{
    m_props.input_event_callback = callback;
}

WindowProps::ApplicationEventCallback DisplaySystemWindows::GetApplicationEventCallback() const
{
    return m_props.application_event_callback;
}

void DisplaySystemWindows::SetApplicationEventCallback(WindowProps::ApplicationEventCallback callback)
{
    m_props.application_event_callback = callback;
}

std::string_view DisplaySystemWindows::GetTitle() const
{
    return m_props.title;
}

void DisplaySystemWindows::SetTitle(const std::string &title)
{
    m_props.title = title;
    glfwSetWindowTitle(m_window_ptr, title.c_str());
}

uint32_t DisplaySystemWindows::GetHeight() const
{
    return m_props.height;
}

void DisplaySystemWindows::SetHeight(uint32_t height)
{
    if (!glfwGetWindowAttrib(m_window_ptr, GLFW_MAXIMIZED))
    {
        m_props.height = height;
        glfwSetWindowSize(m_window_ptr, m_props.width, m_props.height);
    }
}

uint32_t DisplaySystemWindows::GetWidth() const
{
    return m_props.width;
}

void DisplaySystemWindows::SetWidth(uint32_t width)
{
    if (!glfwGetWindowAttrib(m_window_ptr, GLFW_MAXIMIZED))
    {
        m_props.width = width;
        glfwSetWindowSize(m_window_ptr, m_props.width, m_props.height);
    }
}

void DisplaySystemWindows::StartUp(const DisplaySystemProps &props)
{
    PPGE_ASSERT(!DisplaySystem::s_instance, "Application already exists!");
    DisplaySystem::s_instance = this;

    if (s_SystemInstanceCount == 0)
    {
        int success = glfwInit();
        PPGE_ASSERT(success, "Failed to initialize GLFW.");
        glfwSetErrorCallback(GLFWErrorCallback);
#if defined(PPGE_DEBUG)
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        // TODO: Before debug context is set, it should be check which renderer API is active.
        //       When renderer interface is implemented add condition to make sure current renderer API is OpenGL
#endif
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        PPGE_ASSERT(status, "Failed to initialize GLAD!");
    }

    m_props = props;
    m_window_ptr = nullptr;

    PPGE_INFO("Creating Display '{0}' ({1} x {1}) | Platform : Windows", m_props.title, m_props.width, m_props.height);
    {
        m_window_ptr =
            glfwCreateWindow((int)m_props.width, (int)m_props.height, m_props.title.c_str(), nullptr, nullptr);
        ++s_SystemInstanceCount;
        PPGE_ASSERT(m_window_ptr, "Failed to create a new GLFW window.");
        glfwGetWindowPos(m_window_ptr, (int *)&m_props.posX, (int *)&m_props.posY);
    }

    glfwMakeContextCurrent(m_window_ptr);

    glfwSetWindowUserPointer(m_window_ptr, &m_props);

    m_props.vSync = true;
    EnableVsync();

    glfwSetWindowSizeCallback(m_window_ptr, [](GLFWwindow *window, int width, int height) {
        DisplaySystemProps &window_props = *(DisplaySystemProps *)glfwGetWindowUserPointer(window);
        window_props.width = width;
        window_props.height = height;

        WindowResizeEvent event(width, height);
        window_props.application_event_callback(event);
    });

    glfwSetWindowPosCallback(m_window_ptr, [](GLFWwindow *window, int xPos, int yPos) {
        DisplaySystemProps &window_props = *(DisplaySystemProps *)glfwGetWindowUserPointer(window);
        window_props.posX = xPos;
        window_props.posY = yPos;

        WindowMoveEvent event(xPos, yPos);
        window_props.application_event_callback(event);
    });

    glfwSetWindowCloseCallback(m_window_ptr, [](GLFWwindow *window) {
        DisplaySystemProps &window_props = *(DisplaySystemProps *)glfwGetWindowUserPointer(window);

        WindowCloseEvent event;
        window_props.application_event_callback(event);
    });

    glfwSetWindowFocusCallback(m_window_ptr, [](GLFWwindow *window, int focused) {
        DisplaySystemProps &window_props = *(DisplaySystemProps *)glfwGetWindowUserPointer(window);

        switch (focused)
        {
        case GLFW_TRUE: {
            window_props.window_attributes[static_cast<size_t>(WindowProps::AttributeTag::FOCUSED)] =
                static_cast<WindowProps::AttributeValue>(1);
            break;
        }
        case GLFW_FALSE: {
            window_props.window_attributes[static_cast<size_t>(WindowProps::AttributeTag::FOCUSED)] =
                static_cast<WindowProps::AttributeValue>(0);
            break;
        }
        }
    });

    glfwSetWindowIconifyCallback(m_window_ptr, [](GLFWwindow *window, int iconified) {
        DisplaySystemProps &window_props = *(DisplaySystemProps *)glfwGetWindowUserPointer(window);

        switch (iconified)
        {
        case GLFW_TRUE: {
            window_props.window_mode = WindowProps::WindowMode::MINIMIZED;
            WindowMinimizedEvent event;
            window_props.application_event_callback(event);
            break;
        }
        case GLFW_FALSE: {
            // Here we don't know the previous window mode. Save it before minimized and restore it.
            window_props.window_mode = WindowProps::WindowMode::WINDOWED;
            WindowRestoredEvent event;
            window_props.application_event_callback(event);
            break;
        }
        }
    });

    glfwSetWindowMaximizeCallback(m_window_ptr, [](GLFWwindow *window, int maximized) {
        DisplaySystemProps &window_props = *(DisplaySystemProps *)glfwGetWindowUserPointer(window);

        switch (maximized)
        {
        case GLFW_TRUE: {
            window_props.window_mode = WindowProps::WindowMode::MAXIMIZED;
            WindowMaximizedEvent event;
            window_props.application_event_callback(event);
            break;
        }
        case GLFW_FALSE: {
            // Here we don't know the previous window mode. Save it before minimized and restore it.
            window_props.window_mode = WindowProps::WindowMode::WINDOWED;
            WindowRestoredEvent event;
            window_props.application_event_callback(event);
            break;
        }
        }
    });

    glfwSetKeyCallback(m_window_ptr, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        DisplaySystemProps &window_props = *(DisplaySystemProps *)glfwGetWindowUserPointer(window);

        switch (action)
        {
        case GLFW_PRESS: {
            KeyPressedEvent event(static_cast<KeyCode>(key), 0);
            window_props.input_event_callback(event);
            break;
        }
        case GLFW_RELEASE: {
            KeyReleasedEvent event(static_cast<KeyCode>(key));
            window_props.input_event_callback(event);
            break;
        }
        case GLFW_REPEAT: {
            KeyPressedEvent event(static_cast<KeyCode>(key), 1);
            window_props.input_event_callback(event);
            break;
        }
        }
    });

    glfwSetCharCallback(m_window_ptr, [](GLFWwindow *window, unsigned int keycode) {
        DisplaySystemProps &window_props = *(DisplaySystemProps *)glfwGetWindowUserPointer(window);

        KeyTypedEvent event(static_cast<KeyCode>(keycode));
        window_props.input_event_callback(event);
    });

    glfwSetMouseButtonCallback(m_window_ptr, [](GLFWwindow *window, int button, int action, int mods) {
        DisplaySystemProps &window_props = *(DisplaySystemProps *)glfwGetWindowUserPointer(window);

        switch (action)
        {
        case GLFW_PRESS: {
            MouseButtonPressedEvent event(static_cast<MouseCode>(button));
            window_props.input_event_callback(event);
            break;
        }
        case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
            window_props.input_event_callback(event);
            break;
        }
        }
    });

    glfwSetScrollCallback(m_window_ptr, [](GLFWwindow *window, double xOffset, double yOffset) {
        DisplaySystemProps &window_props = *(DisplaySystemProps *)glfwGetWindowUserPointer(window);

        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        window_props.input_event_callback(event);
    });

    glfwSetCursorPosCallback(m_window_ptr, [](GLFWwindow *window, double xPos, double yPos) {
        DisplaySystemProps &window_props = *(DisplaySystemProps *)glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float)xPos, (float)yPos);
        window_props.input_event_callback(event);
    });
}

void DisplaySystemWindows::Update()
{
    glfwPollEvents();
    glfwSwapBuffers(m_window_ptr);
}

void DisplaySystemWindows::ShutDown()
{
    glfwDestroyWindow(m_window_ptr);
    --s_SystemInstanceCount;

    if (s_SystemInstanceCount == 0)
    {
        glfwTerminate();
    }
}
} // namespace PPGE