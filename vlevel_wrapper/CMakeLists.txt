# CMAKE version of Ubuntu 16.04 is 3.5.
cmake_minimum_required(VERSION 3.5)
project(vlevel)

set(SRC)
set(INCL)
set(LINKLIBS)
set(COMPILE_OPTION)

include(${CMAKE_CURRENT_LIST_DIR}/../lib/WAV/WAV.cmake)

list(APPEND SRC
	test.cpp
	agc.cpp
	agc.h
	${CMAKE_CURRENT_LIST_DIR}/../vlevel/volumeleveler/volumeleveler.cpp
	${CMAKE_CURRENT_LIST_DIR}/../vlevel/volumeleveler/volumeleveler.h
  )
list(APPEND INCL

  ${CMAKE_CURRENT_LIST_DIR}/.
  ${CMAKE_CURRENT_LIST_DIR}/../vlevel/volumeleveler
  
  )
list(APPEND LINKLIBS
  )
if(MSVC)
  list(APPEND COMPILE_OPTION
    )
else()
  list(APPEND COMPILE_OPTION
    )
endif()

add_executable(${PROJECT_NAME} ${SRC})
target_include_directories(${PROJECT_NAME}    PRIVATE    ${INCL}  )
target_link_libraries(${PROJECT_NAME} ${LINKLIBS})
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_OPTION} )
set_target_properties(${PROJECT_NAME} PROPERTIES  CXX_STANDARD 17 )