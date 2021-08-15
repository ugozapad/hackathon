add_executable(packer "packer.cpp" "FS.h" "FS.cpp" "StdAfx.cpp" "StdAfx.h")
set_target_name(packer)
set_target_out_dir(packer "${CMAKE_SOURCE_DIR}/bin")

target_include_directories(packer PUBLIC "${CMAKE_SOURCE_DIR}/vendor/spdlog/include")
target_include_directories(packer PUBLIC "${CMAKE_SOURCE_DIR}/src")