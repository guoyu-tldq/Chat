# CMakeList.txt: Chat 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("Chat")

# --------------------------------------------------------------------------
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-D__STDC_CONSTANT_MACROS")

IF(MSVC)
    SET(CMAKE_CXX_FLAGS "/EHsc")  # deal with MSVC Warning C4530
ENDIF(MSVC)


######################################
#  WS2 lib (For windows)
#  Threads lib (For windows)
######################################
if(WIN32) # Check if we are on Windows
  message("compile on windows.")
  find_library(WS2_LIBRARY ws2_32)
elseif(UNIX)
	message("compile on UNIX")
	find_package(Threads REQUIRED) 
else()
  message(SEND_ERROR "You are on an unsupported platform! (Not Win32 or Unix)")
endif()




######################################
#  information
######################################

message("info: CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR}")
message("info: PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")
message("info: WS2_LIBRARY=${WS2_LIBRARY}")
message("info: CMAKE_THREAD_LIBS_INIT=${CMAKE_THREAD_LIBS_INIT}")


############################################
# Add source to this project's executable.
############################################
add_executable (${PROJECT_NAME} 
	src/Client.cpp
	src/control.cpp
	src/Server.cpp
)


target_include_directories( ${PROJECT_NAME}  
	PRIVATE 
		${PROJECT_SOURCE_DIR}/include
)


if(WIN32) # Check if we are on Windows
	target_link_libraries( ${PROJECT_NAME}   
		PRIVATE 
			${WS2_LIBRARY} 
	)
elseif(UNIX)
	target_link_libraries( ${PROJECT_NAME}   
		PRIVATE 
			${CMAKE_THREAD_LIBS_INIT}
	)
endif()




