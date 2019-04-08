cmake_minimum_required (VERSION 2.6)
project (5G_QoS C)

ENABLE_TESTING()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unknown-pragmas")

#option(USE_ARM "build application with ARM" ON) # OFF is the defaut

#	add_definitions(-DFAKE_CAMERA)

include_directories(${SOURCE_DIR})
#include_directories(${Node_SOURCE_DIR}/include)
#include_directories(${Node_SOURCE_DIR}/include/jpeg)

file(GLOB SRC
    "*.h"
    "*.c"
)


  #add_definitions(-DFIRST_1st=1 -DFIRST_2nd_10=0 -DFIRST_2nd_20=0 -DFIRST_2nd_30=0 -DFIRST_3rd=0 -DFIRST_4th_10=0 -DFIRST_4th_20=0 -DFIRST_4th_30=0 -DFIRST_5th=0 -DFIRST_6th=0 -DFIRST_7th=0 -DFIRST_8th=0)

if(FIRST_1st_f)
  add_definitions(-DFIRST_1st=1)
endif(FIRST_1st_f)

if(FIRST_2nd_10_f)
  add_definitions(-DFIRST_2nd_10=1)
endif(FIRST_2nd_10_f)

if(FIRST_2nd_20_f)
  add_definitions(-DFIRST_2nd_20=1)
endif(FIRST_2nd_20_f)

if(FIRST_2nd_30_f)
  add_definitions(-DFIRST_2nd_30=1)
endif(FIRST_2nd_30_f)

if(FIRST_3rd_f)
  add_definitions(-DFIRST_3rd=1)
endif(FIRST_3rd_f)

if(FIRST_4th_10_f)
  add_definitions(-DFIRST_4th_10=1)
endif(FIRST_4th_10_f)

if(FIRST_4th_20_f)
  add_definitions(-DFIRST_4th_20=1)
endif(FIRST_4th_20_f)

if(FIRST_4th_30_f)
  add_definitions(-DFIRST_4th_30=1)
endif(FIRST_4th_30_f)

if(FIRST_5th_f)
  add_definitions(-DFIRST_5th=1)
endif(FIRST_5th_f)

if(FIRST_6th_f)
  add_definitions(-DFIRST_6th=1)
endif(FIRST_6th_f)

if(FIRST_7th_f)
  add_definitions(-DFIRST_7th=1)
endif(FIRST_7th_f)

if(FIRST_8th_f)
  add_definitions(-DFIRST_8th=1)
endif(FIRST_8th_f)

#SET(CMAKE_SHARED_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")
#SET(CMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")

add_executable(5G_QoS ${SRC})

find_library(LFDS_LIB lfds711 HINTS  liblfds7.1.1/liblfds711/bin/)

TARGET_LINK_LIBRARIES(5G_QoS PUBLIC ${LFDS_LIB} pthread mnl netfilter_queue m)

