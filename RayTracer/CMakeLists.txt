print_header("COMPILING RAYTRACER")
add_executable(RayTracer
    src/main.cpp
    src/MainWindow.cpp
    src/App.cpp
    src/Image.cpp
    src/Renderer.cpp
    src/Camera.cpp
)

target_include_directories(RayTracer
    PUBLIC ${CMAKE_SOURCE_DIR}/external/ImGui/include
    PUBLIC include
)
target_link_libraries(RayTracer
    PUBLIC fmt::fmt
    PUBLIC ImGui
)

set_property(TARGET RayTracer PROPERTY CXX_STANDARD 23)

print_header("RAYTRACER COMPILED")