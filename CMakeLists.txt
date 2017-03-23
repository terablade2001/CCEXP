# cmake -G "CodeLite - MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
# cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
# cmake -DCMAKE_BUILD_TYPE=Release ..
# set(CMAKE_C_COMPILER "c:/TDM-GCC-64/bin/x86_64-w64-mingw32-gcc-5.1.0.exe")
# set(CMAKE_CXX_COMPILER "c:/TDM-GCC-64/bin/x86_64-w64-mingw32-g++.exe")
cmake_minimum_required(VERSION 2.8.11)

project(CCEXP)

if (MINGW)
	set(CXX_FLAGS "-std=c++11 -Wall")
endif(MINGW)

set(LIB__MVECTOR ON)


set(CMAKE_CONFIGURATION_TYPES "Release")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CXX_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${CXX_FLAGS}")
set(CMAKE_LDFLAGS "${CMAKE_LDFLAGS} -L. ")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-undefined")

set (CXX_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/C++/src/include/CCEXP.hpp    
    ${CMAKE_CURRENT_LIST_DIR}/C++/src/CCEXP.cpp
    ${CMAKE_CURRENT_LIST_DIR}/C++/test/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/C++/test/t00_Basics.cpp
)

if(LIB__MVECTOR)
	message("")
	message("####################################################################")
	message("#############     Using also MVECTOR library     ###################")
	message("####################################################################")
	message("  -> Download MVECTOR from https://github.com/terablade2001/MVECTOR")
	message("  and place MVECTOR.hpp and MVECTOR.cpp files at folders:")
	message("     * MVECTOR.hpp: ${CMAKE_CURRENT_LIST_DIR}/C++/MVECTOR/include/MVECTOR.hpp")
	message("     * MVECTOR.cpp: ${CMAKE_CURRENT_LIST_DIR}/C++/MVECTOR/MVECTOR.cpp")
	message("####################################################################")
	message("####################################################################")
	message("####################################################################")
	message("")
	set (CXX_SRCS ${CXX_SRCS}
		${CMAKE_CURRENT_LIST_DIR}/C++/MVECTOR/include/MVECTOR.hpp
		${CMAKE_CURRENT_LIST_DIR}/C++/MVECTOR/MVECTOR.cpp
	)
endif(LIB__MVECTOR)
		

add_executable(CCEXP ${CXX_SRCS})
target_link_libraries(CCEXP ${LINK_OPTIONS})
