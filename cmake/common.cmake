function(set_target_name TARGET)
	if (CMAKE_SIZEOF_VOID_P EQUAL 4 AND WIN32)
		set(PLATFORM_STR "Win32")
	elseif (WIN32)
		set(PLATFORM_STR "Win64")
	endif()
	
	set_target_properties(${TARGET} PROPERTIES
		OUTPUT_NAME "${TARGET}_${PLATFORM_STR}"
	
		DEBUG_POSTFIX "_Debug"
		RELEASE_POSTFIX "_Release"
		RELWITHDEBINFO_POSTFIX "_RelWithDebInfo"
		MINSIZEREL_POSTFIX "_MinSizeRel"
	)
endfunction()

function(set_target_out_dir TARGET PATH)
	set_target_properties(${TARGET} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PATH}"
		RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PATH}"
		RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PATH}"
		RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PATH}"
	)
endfunction()