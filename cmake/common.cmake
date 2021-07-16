if (MSVC)
	if (MSVC_TOOLSET_VERSION EQUAL 142)
		set(PLATFORM_NAME "2019")
	elseif (MSVC_TOOLSET_VERSION EQUAL 141)
		set(PLATFORM_NAME "2017")
	elseif (MSVC_TOOLSET_VERSION EQUAL 140)
		set(PLATFORM_NAME "2015")
	endif()
endif()

function(set_target_name_ex TARGET NAME)
	if (CMAKE_SIZEOF_VOID_P EQUAL 4 AND WIN32)
		set(PLATFORM_STR "win32")
	elseif (WIN32)
		set(PLATFORM_STR "win64")
	endif()
	
	set_target_properties(${TARGET} PROPERTIES
		#OUTPUT_NAME "${NAME}_${PLATFORM_STR}_${PLATFORM_NAME}"
		OUTPUT_NAME "${NAME}_${PLATFORM_STR}"
	
		DEBUG_POSTFIX "_d"
		RELEASE_POSTFIX "_r"
		RELWITHDEBINFO_POSTFIX "_rwdi"
		MINSIZEREL_POSTFIX "_msr"
	)
endfunction()

function(set_target_name TARGET)
	if (CMAKE_SIZEOF_VOID_P EQUAL 4 AND WIN32)
		set(PLATFORM_STR "win32")
	elseif (WIN32)
		set(PLATFORM_STR "win64")
	endif()
	
	set_target_properties(${TARGET} PROPERTIES
		OUTPUT_NAME "${TARGET}_${PLATFORM_STR}"
		
		DEBUG_POSTFIX "_d"
		RELEASE_POSTFIX "_r"
		RELWITHDEBINFO_POSTFIX "_rwdi"
		MINSIZEREL_POSTFIX "_msr"
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