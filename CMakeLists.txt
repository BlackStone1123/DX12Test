cmake_minimum_required(VERSION 3.10)

project(Dx12Sample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")

add_definitions("-DUNICODE -D_UNICODE")

file(GLOB_RECURSE DrawableHeaders Drawable/*.h)
file(GLOB_RECURSE DrawableSource Drawable/*.cpp)
file(GLOB_RECURSE BindableHeaders Bindable/*.h)
file(GLOB_RECURSE BindableSource Bindable/*.cpp)
file(GLOB_RECURSE GraphicsHeaders Graphics/*.h)
file(GLOB_RECURSE GraphicsSource Graphics/*.cpp)

set( SHADER_FILES
    shaders/VertexShader.hlsl
    shaders/PixelShader.hlsl
)

source_group("Header Files\\Drawable" FILES ${DrawableHeaders})
source_group("Source Files\\Drawable" FILES ${DrawableSource})
source_group("Header Files\\Bindable" FILES ${BindableHeaders})
source_group("Source Files\\Bindable" FILES ${BindableSource})
source_group("Header Files\\Graphics" FILES ${GraphicsHeaders})
source_group("Source Files\\Graphics" FILES ${GraphicsSource})
source_group("Resources\\Shaders" FILES ${SHADER_FILES} )

set_source_files_properties( ${SHADER_FILES} 
    PROPERTIES
        VS_SHADER_MODEL 6.0
        VS_SHADER_DISABLE_OPTIMIZATIONS $<$<CONFIG:Debug>:ON>
        VS_SHADER_ENABLE_DEBUG $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:ON>
        VS_SHADER_FLAGS $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-Qembed_debug>
        VS_SHADER_OBJECT_FILE_NAME "$(ProjectDir)%(FileName).h"
)


set_source_files_properties( shaders/VertexShader.hlsl 
PROPERTIES
    VS_SHADER_TYPE Vertex
)

set_source_files_properties( shaders/PixelShader.hlsl 
PROPERTIES
    VS_SHADER_TYPE Pixel
)

add_executable(${PROJECT_NAME}
    main.cpp
    App.h
    App.cpp
    Window.h
    Window.cpp
    ${DrawableHeaders}
    ${DrawableSource}
    ${BindableHeaders}
    ${BindableSource}
    ${SHADER_FILES}
    ${GraphicsHeaders}
    ${GraphicsSource}
)
target_include_directories(${PROJECT_NAME} PUBLIC Drawable Bindable Graphics)
target_link_libraries(${PROJECT_NAME} PUBLIC dxgi.lib d3d12.lib d3dcompiler.lib dxguid.lib)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND editbin /subsystem:console $(OutDir)$(TargetName).exe)