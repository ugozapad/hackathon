#add_subdirectory(EASTL)

add_subdirectory(bullet)

set_target_properties(bullet PROPERTIES
    MSVC_RUNTIME_LIBRARY 
        "MultiThreaded$<$<OR:$<CONFIG:Debug>>:Debug>"
)