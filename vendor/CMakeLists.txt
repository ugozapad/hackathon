#add_subdirectory(EASTL)

add_subdirectory(bullet)

set_target_properties(bullet PROPERTIES
    MSVC_RUNTIME_LIBRARY 
        "MultiThreaded$<$<OR:$<CONFIG:Debug>>:Debug>"
)

set(ozz_build_msvc_rt_dll OFF CACHE BOOL "" FORCE)
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