#include "platform/gl/swap_chain_gl.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace PPGE
{
void SwapChainGL::Swap()
{
    glfwSwapBuffers(m_window_ptr);
}
} // namespace PPGE