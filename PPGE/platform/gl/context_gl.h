#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "renderer/context.h"
#include "system/logger_system.h"

class GLFWwindow;

namespace PPGE
{
class PPGE_API ContextGL : public Context
{
  public:
    ContextGL(GLFWwindow *window);

  private:
    GLFWwindow *m_window_ptr;
};
} // namespace PPGE