#include "PPGEpch.h"

#include "display_system_windows.h"

namespace PPGE
{
	static uint8_t s_SystemInstanceCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		PPGE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	DisplaySystemWindows::DisplaySystemWindows(const DisplaySystemProps& props)
	{
		Init(props);
	}


	DisplaySystemWindows::~DisplaySystemWindows()
	{
		Shutdown();
	}

	void DisplaySystemWindows::OnUpdate()
	{
		glfwPollEvents();
	}

	void* DisplaySystemWindows::GetNativeDisplayPtr() const
	{
		return m_window;
	}

	bool DisplaySystemWindows::GetWindowFlag(WindowFlags flag) const
	{	
		bool b_value = false;

		switch (flag)
		{
			case WindowFlags::ALWAYS_ON_TOP :
			{
				b_value = glfwGetWindowAttrib(m_window, GLFW_FLOATING);
			} break;
			case WindowFlags::DECORATED :
			{
				b_value = glfwGetWindowAttrib(m_window, GLFW_DECORATED);
			} break;
			case WindowFlags::RESIZABLE :
			{
				b_value = glfwGetWindowAttrib(m_window, GLFW_RESIZABLE);
			} break;
			case WindowFlags::FOCUSED :
			{
				b_value = glfwGetWindowAttrib(m_window, GLFW_FOCUSED);
			} break;
		}

		return b_value;
	}

	void DisplaySystemWindows::SetWindowFlag(WindowFlags flag, bool b_value)
	{
		switch (flag)
		{
			case WindowFlags::ALWAYS_ON_TOP:
			{
				glfwSetWindowAttrib(m_window, GLFW_FLOATING, b_value);
			} break;
			case WindowFlags::DECORATED:
			{
				glfwSetWindowAttrib(m_window, GLFW_DECORATED, b_value);
			} break;
			case WindowFlags::RESIZABLE:
			{
				glfwSetWindowAttrib(m_window, GLFW_RESIZABLE, b_value);
			} break;
			case WindowFlags::FOCUSED:
			{
				if (b_value)
					glfwFocusWindow(m_window);
			} break;
		}
	}

	void DisplaySystemWindows::SetWindowMode(WindowModes mode)
	{
		switch (mode)
		{
			case WindowModes::MAXIMIZED :
			{
				glfwMaximizeWindow(m_window);
			} break;
			case WindowModes::FULLSCREEN :
			{
				if (!m_props.fullscreen)
				{
					m_props.fullscreen = true;
					GLFWmonitor* monitor = glfwGetPrimaryMonitor();
					const GLFWvidmode* mode = glfwGetVideoMode(monitor);

					glfwSetWindowMonitor(m_window, monitor, 0, 0, mode->width, mode->height, GLFW_DONT_CARE);
				}
			} break;
			case WindowModes::WINDOWED :
			{
				if (m_props.fullscreen)
				{
					m_props.fullscreen = false;

					glfwSetWindowMonitor(m_window, nullptr, m_props.posX, m_props.posY, m_props.width, m_props.height, GLFW_DONT_CARE);
				}
			} break;
			case WindowModes::MINIMIZED :
			{
				glfwIconifyWindow(m_window);
			} break;
		}
	}

	bool DisplaySystemWindows::IsVsyncEnabled() const
	{
		return m_props.vSync;
	}

	void DisplaySystemWindows::SetVsync(bool b_value)
	{
		if (m_props.vSync ^ b_value)
		{
			m_props.vSync = b_value;
			glfwSwapInterval((int)m_props.vSync);
		}
	}

	DisplaySystemWindows::InputEventCallback DisplaySystemWindows::InputEventCallbackFunctionPtr() const
	{
		return m_props.InputEventCallback;
	}

	void DisplaySystemWindows::SetInputEventCallbackFunctionPtr(InputEventCallback callback)
	{
		m_props.InputEventCallback = callback;
	}

	DisplaySystemWindows::ApplicationEventCallback DisplaySystemWindows::ApplicationEventCallbackFunctionPtr() const
	{
		return m_props.ApplicationEventCallback;
	}

	void DisplaySystemWindows::SetApplicationEventCallbackFunctionPtr(ApplicationEventCallback callback)
	{
		m_props.ApplicationEventCallback = callback;
	}

	std::string DisplaySystemWindows::Title() const
	{
		return m_props.title;
	}

	void DisplaySystemWindows::Title(const std::string& title)
	{
		glfwSetWindowTitle(m_window, title.c_str());
		m_props.title = title;
	}

	uint32_t DisplaySystemWindows::Height() const
	{
		return m_props.height;
	}

	void DisplaySystemWindows::SetHeight(uint32_t height)
	{
		if (!glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED))
		{
			m_props.height = height;
			glfwSetWindowSize(m_window, m_props.width, m_props.height);
		}
	}

	uint32_t DisplaySystemWindows::Width() const
	{
		return m_props.width;
	}

	void DisplaySystemWindows::SetWidth(uint32_t width)
	{
		if (!glfwGetWindowAttrib(m_window, GLFW_MAXIMIZED))
		{
			m_props.width = width;
			glfwSetWindowSize(m_window, m_props.width, m_props.height);
		}
	}

	void DisplaySystemWindows::Init(const DisplaySystemProps& props)
	{
		m_props.title  = props.title;
		m_props.height = props.height;
		m_props.width  = props.width;
		m_props.posX  = 0;
		m_props.posY  = 0;
		m_props.vSync = false;
		m_props.fullscreen = false;
		m_props.InputEventCallback = nullptr;
		m_props.ApplicationEventCallback = nullptr;

		PPGE_INFO("Creating Display '{0}' ({1} x {1}) | Platform : Windows", m_props.title, m_props.width, m_props.height);

		if (s_SystemInstanceCount == 0)
		{
			int success = glfwInit();
			// TODO: Add assertion macro to check GLFW is initialized sucessfully.
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			#if defined(PPGE_DEBUG)
			// TODO: Before debug context is set, it should be check which renderer API is active.
			//       When renderer interface is implemented add condition to make sure current renderer API is OpenGL
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			#endif
			m_window = glfwCreateWindow((int)m_props.width, (int)m_props.height, m_props.title.c_str(), nullptr, nullptr);
			++s_SystemInstanceCount;
			// TODO: Add assertion macro to check window is initialized sucessfully.
		}

		{
			glfwGetWindowPos(m_window, (int*)&m_props.posX, (int*)&m_props.posY);
		}

		glfwSetWindowUserPointer(m_window, &m_props);
		SetVsync(true);

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
		{
			DisplaySystemWindowsProps& win_props = *(DisplaySystemWindowsProps*)glfwGetWindowUserPointer(window);
			win_props.width = width;
			win_props.height = height;

			WindowResizeEvent event(width, height);
			win_props.ApplicationEventCallback(event);
		});

		glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int xPos, int yPos)
		{
			DisplaySystemWindowsProps& win_props = *(DisplaySystemWindowsProps*)glfwGetWindowUserPointer(window);
			win_props.posX = xPos;
			win_props.posY = yPos;

			WindowMoveEvent event(xPos, yPos);
			win_props.ApplicationEventCallback(event);
		});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
		{
			DisplaySystemWindowsProps& win_props = *(DisplaySystemWindowsProps*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			win_props.ApplicationEventCallback(event);
		});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			DisplaySystemWindowsProps& win_props = *(DisplaySystemWindowsProps*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(static_cast<KeyCode>(key), 0);
				win_props.InputEventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(static_cast<KeyCode>(key));
				win_props.InputEventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(static_cast<KeyCode>(key), 1);
				win_props.InputEventCallback(event);
				break;
			}
			}
		});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
		{
			DisplaySystemWindowsProps& win_props = *(DisplaySystemWindowsProps*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(static_cast<KeyCode>(keycode));
			win_props.InputEventCallback(event);
		});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
		{
			DisplaySystemWindowsProps& win_props = *(DisplaySystemWindowsProps*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(static_cast<MouseCode>(button));
				win_props.InputEventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(static_cast<MouseCode>(button));
				win_props.InputEventCallback(event);
				break;
			}
			}
		});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			DisplaySystemWindowsProps& win_props = *(DisplaySystemWindowsProps*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			win_props.InputEventCallback(event);
		});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
		{
			DisplaySystemWindowsProps& win_props = *(DisplaySystemWindowsProps*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			win_props.InputEventCallback(event);
		});
	}

	void DisplaySystemWindows::Shutdown()
	{
		glfwDestroyWindow(m_window);
		--s_SystemInstanceCount;

		if (s_SystemInstanceCount == 0)
		{
			glfwTerminate();
		}
	}
}