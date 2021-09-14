# GLAD dependency
add_subdirectory(glad)
set_target_properties(glad PROPERTIES FOLDER "Dependencies")

# GLFW dependency
# Set some options before adding GLFW in to your project
# to prevent cluttered projects that are not needed.
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(glfw)
set_target_properties(glfw PROPERTIES FOLDER "Dependencies")
