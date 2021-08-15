#add_subdirectory(EASTL)

add_subdirectory(bullet)

set_target_properties(bullet PROPERTIES
    MSVC_RUNTIME_LIBRARY 
        "MultiThreaded$<$<OR:$<CONFIG:Debug>>:Debug>"
)

set(ozz_build_msvc_rt_dll OFF CACHE BOOL "" FORCE)
set(ozz_build_samples OFF CACHE BOOL "" FORCE)
set(ozz_build_howtos OFF CACHE BOOL "" FORCE)
set(ozz_build_tests OFF CACHE BOOL "" FORCE)
set(ozz_build_tools OFF CACHE BOOL "" FORCE)
set(ozz_build_fbx OFF CACHE BOOL "" FORCE)
#set(ozz_build_msvc_rt_dll OFF)

add_subdirectory(ozz-animation)

set_target_properties(ozz_base PROPERTIES
    MSVC_RUNTIME_LIBRARY 
        "MultiThreaded$<$<OR:$<CONFIG:Debug>>:Debug>"
)

set_target_properties(ozz_animation PROPERTIES
    MSVC_RUNTIME_LIBRARY 
        "MultiThreaded$<$<OR:$<CONFIG:Debug>>:Debug>"
)

set_target_properties(ozz_animation_offline PROPERTIES
    MSVC_RUNTIME_LIBRARY 
        "MultiThreaded$<$<OR:$<CONFIG:Debug>>:Debug>"
)

set_target_properties(ozz_geometry PROPERTIES
    MSVC_RUNTIME_LIBRARY 
        "MultiThreaded$<$<OR:$<CONFIG:Debug>>:Debug>"
)

set_target_properties(ozz_options PROPERTIES
    MSVC_RUNTIME_LIBRARY 
        "MultiThreaded$<$<OR:$<CONFIG:Debug>>:Debug>"
)

set(IMGUI_SRC "${CMAKE_CURRENT_SOURCE_DIR}/imgui")

add_library(imgui STATIC 
	"${IMGUI_SRC}/imconfig.h"
	"${IMGUI_SRC}/imgui.cpp"
	"${IMGUI_SRC}/imgui.h"
	"${IMGUI_SRC}/imgui_demo.cpp"
	"${IMGUI_SRC}/imgui_draw.cpp"
	"${IMGUI_SRC}/imgui_internal.h"
	"${IMGUI_SRC}/imgui_tables.cpp"
	"${IMGUI_SRC}/imgui_widgets.cpp"
	"${IMGUI_SRC}/imstb_rectpack.h"
	"${IMGUI_SRC}/imstb_textedit.h"
	"${IMGUI_SRC}/imstb_truetype.h"
)

target_include_directories(imgui PUBLIC "${IMGUI_SRC}")
set_target_properties(imgui PROPERTIES MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<OR:$<CONFIG:Debug>>:Debug>")