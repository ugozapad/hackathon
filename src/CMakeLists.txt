file(GLOB ROOTDIR_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
source_group("src" FILES ${ROOTDIR_SRC})

file(GLOB COMMON_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/common/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp"
)
source_group("src\\common" FILES ${COMMON_SRC})

file(GLOB CORE_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/core/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp"
)
source_group("src\\core" FILES ${CORE_SRC})

file(GLOB FILE_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/file/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/file/*.cpp"
)
source_group("src\\file" FILES ${FILE_SRC})

file(GLOB ENGINE_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/engine/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/engine/*.cpp"
)
source_group("src\\engine" FILES ${ENGINE_SRC})

file(GLOB ENGINE_CONTENT_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/engine/content/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/engine/content/*.cpp"
)
source_group("src\\engine\\content" FILES ${ENGINE_CONTENT_SRC})

file(GLOB ENGINE_COMPONENTS_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/engine/components/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/engine/components/*.cpp"
)
source_group("src\\engine\\components" FILES ${ENGINE_COMPONENTS_SRC})

file(GLOB GRAPHICS_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/graphics/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/graphics/*.cpp"
)
source_group("src\\graphics" FILES ${GRAPHICS_SRC})

file(GLOB GRAPHICS_GL_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/graphics/gl/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/graphics/gl/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graphics/gl/*.c"
)
source_group("src\\graphics\\gl" FILES ${GRAPHICS_GL_SRC})

file(GLOB GRAPHICS_IM3D_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/graphics/im3d/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/graphics/im3d/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/graphics/im3d/*.c"
)
source_group("src\\graphics\\im3d" FILES ${GRAPHICS_IM3D_SRC})

file(GLOB AUDIO_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/audio/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/audio/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/audio/*.c"
)
source_group("src\\audio" FILES ${AUDIO_SRC})

file(GLOB PHYSICS_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/physics/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/physics/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/physics/*.c"
)
source_group("src\\physics" FILES ${PHYSICS_SRC})

file(GLOB GAME_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/game/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/game/*.cpp"
)
source_group("src\\game" FILES ${GAME_SRC})

add_executable(engine
	${ROOTDIR_SRC}
	${COMMON_SRC}
	${CORE_SRC}
	${FILE_SRC}
	${ENGINE_SRC}
	${ENGINE_CONTENT_SRC}
	${ENGINE_COMPONENTS_SRC}
	${GRAPHICS_SRC}
	${GRAPHICS_GL_SRC}
	${GRAPHICS_IM3D_SRC}
	${AUDIO_SRC}
	${PHYSICS_SRC}
	${GAME_SRC}
)

set_target_name(engine)
set_target_out_dir(engine "${CMAKE_SOURCE_DIR}/bin")

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# PCH Support
if (MSVC)
   set_target_properties(engine PROPERTIES COMPILE_FLAGS "/Yupch.h")
   set_source_files_properties(pch.cpp PROPERTIES COMPILE_FLAGS "/Ycpch.h")

   set_source_files_properties(graphics/gl/glad.c PROPERTIES COMPILE_FLAGS /Y-)
   set_source_files_properties(audio/miniaudio.c PROPERTIES COMPILE_FLAGS /Y-)
   set_source_files_properties(audio/stb_vorbis.c PROPERTIES COMPILE_FLAGS /Y-)
else()
	target_precompile_headers(engine PRIVATE "pch.h")
endif()

target_include_directories(engine PUBLIC "${CMAKE_SOURCE_DIR}/vendor/spdlog/include")
target_include_directories(engine PUBLIC "${CMAKE_SOURCE_DIR}/vendor/glm")
target_include_directories(engine PUBLIC "${CMAKE_SOURCE_DIR}/vendor/stb")

if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND WIN32)
	target_include_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/vendor/glfw-3.3.4.bin.WIN64/include")
	target_link_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/vendor/glfw-3.3.4.bin.WIN64/lib-vc2019")
	target_link_libraries(engine glfw3_mt)
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 4)
	target_link_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/vendor/libs/x32")
else()
	target_link_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/vendor/libs/x64")
endif()

target_include_directories(engine PUBLIC "${CMAKE_SOURCE_DIR}/vendor/include")
target_include_directories(engine PUBLIC "${CMAKE_SOURCE_DIR}/vendor/include/fmod")

if (MSVC)
#	target_link_libraries(engine debug "assimp-vc142-mtd.lib")
#	target_link_libraries(engine optimized "assimp-vc142-mt.lib")
else()
	target_link_libraries(engine "libassimp.dll.a")
endif()


if (MSVC AND WIN32)
	target_link_libraries(engine "fmod_vc.lib")
endif()

target_link_libraries(engine bullet)

set_target_properties(engine PROPERTIES
    MSVC_RUNTIME_LIBRARY 
        "MultiThreaded$<$<OR:$<CONFIG:Debug>>:Debug>"
)