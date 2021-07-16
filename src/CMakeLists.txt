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

file(GLOB FILE_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/file/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/file/*.cpp"
)
source_group("src\\file" FILES ${FILE_SRC})

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

add_executable(engine
	${ROOTDIR_SRC}
	${COMMON_SRC}
	${FILE_SRC}
	${GRAPHICS_SRC}
	${GRAPHICS_GL_SRC}
)

set_target_name_ex(engine Hackathon)
set_target_out_dir(engine "${CMAKE_SOURCE_DIR}/bin")

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

# PCH Support
if (MSVC)
   set_target_properties(engine PROPERTIES COMPILE_FLAGS "/Yupch.h")
   set_source_files_properties(pch.cpp PROPERTIES COMPILE_FLAGS "/Ycpch.h")

   set_source_files_properties(graphics/gl/glad.c PROPERTIES COMPILE_FLAGS /Y-)
else()
	target_precompile_headers(engine PRIVATE "pch.h")
endif()



target_include_directories(engine PUBLIC "${CMAKE_SOURCE_DIR}/vendor/spdlog/include")

if(CMAKE_SIZEOF_VOID_P EQUAL 8 AND WIN32)
	target_include_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/vendor/glfw-3.3.4.bin.WIN64/include")
	target_link_directories(engine PRIVATE "${CMAKE_SOURCE_DIR}/vendor/glfw-3.3.4.bin.WIN64/lib-vc2019")
	target_link_libraries(engine glfw3)
endif()

target_link_libraries(engine EASTL)