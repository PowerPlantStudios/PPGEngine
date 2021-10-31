# GLAD as part of OpenGL dependency
add_subdirectory(glad)
set_target_properties(glad PROPERTIES FOLDER "Dependencies")

# GLFW dependency
# Set some options before adding GLFW in to your project
# to prevent cluttered projects that are not needed.
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
set(GLFW_VULKAN_STATIC OFF)
add_subdirectory(glfw)
set_target_properties(glfw PROPERTIES FOLDER "Dependencies")

# ImGui GUI rendering dependecy
add_subdirectory(imgui)
set_target_properties(imgui PROPERTIES FOLDER "Dependencies")

# Fast and efficient vectorized math dependency
add_subdirectory(math)
set_target_properties(simplemath PROPERTIES FOLDER "Dependencies")
