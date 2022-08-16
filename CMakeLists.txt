cmake_minimum_required(VERSION 3.10)

project(Dx12Sample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # Output directory for static lib (.LIB)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for shared lib (.DLL)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin) # Output directory for executables (.EXE)

add_subdirectory( extern/DirectXTex )
add_definitions("-DUNICODE -D_UNICODE")

file(GLOB_RECURSE DrawableHeaders Drawable/*.h)
file(GLOB_RECURSE DrawableSource Drawable/*.cpp)
file(GLOB_RECURSE BindableHeaders Bindable/*.h)
file(GLOB_RECURSE BindableSource Bindable/*.cpp)
file(GLOB_RECURSE GraphicsHeaders Graphics/*.h)
file(GLOB_RECURSE GraphicsSource Graphics/*.cpp)
file(GLOB_RECURSE ImGuiHeaders ImGui/*.h)
file(GLOB_RECURSE ImGuiSource ImGui/*.cpp)

set( SHADER_FILES
    shaders/VertexShader.hlsl
    shaders/PixelShader.hlsl
    shaders/TextureVS.hlsl
    shaders/TexturePS.hlsl
)

source_group("Drawable" FILES ${DrawableHeaders} ${DrawableSource})
source_group("Bindable" FILES ${BindableHeaders} ${BindableSource})
source_group("Graphics" FILES ${GraphicsHeaders} ${GraphicsSource})
source_group("ImGui" FILES ${ImGuiHeaders} ${ImGuiSource})
source_group("Resources\\Shaders" FILES ${SHADER_FILES} )

set_source_files_properties( ${SHADER_FILES} 
    PROPERTIES
        VS_SHADER_MODEL 6.0
        VS_SHADER_DISABLE_OPTIMIZATIONS $<$<CONFIG:Debug>:ON>
        VS_SHADER_ENABLE_DEBUG $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:ON>
        VS_SHADER_FLAGS $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-Qembed_debug>
        VS_SHADER_OBJECT_FILE_NAME "$(OutDir)%(FileName).cso"
)


set_source_files_properties( shaders/VertexShader.hlsl shaders/TextureVS.hlsl
PROPERTIES
    VS_SHADER_TYPE Vertex
)

set_source_files_properties( shaders/PixelShader.hlsl shaders/TexturePS.hlsl
PROPERTIES
    VS_SHADER_TYPE Pixel
)

add_executable(${PROJECT_NAME}
    main.cpp
    App.h
    App.cpp
    Window.h
    Window.cpp
    ImguiManager.h
    ImguiManager.cpp
    Timer.h
    Timer.cpp
    ${DrawableHeaders}
    ${DrawableSource}
    ${BindableHeaders}
    ${BindableSource}
    ${SHADER_FILES}
    ${GraphicsHeaders}
    ${GraphicsSource}
    ${ImGuiHeaders}
    ${ImGuiSource}
)
set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$(OutDir)")

target_include_directories(${PROJECT_NAME} PUBLIC Drawable Bindable Graphics ImGui)
target_link_libraries(${PROJECT_NAME}
    PUBLIC DirectXTex
    PUBLIC dxgi.lib d3d12.lib d3dcompiler.lib dxguid.lib
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND editbin /subsystem:console $(OutDir)$(TargetName).exe)