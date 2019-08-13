cmake_minimum_required (VERSION 2.6)
project (5G_QoS C)

ENABLE_TESTING()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unknown-pragmas ")

#option(USE_ARM "build application with ARM" ON) # OFF is the defaut

#	add_definitions(-DFAKE_CAMERA)

include_directories(${SOURCE_DIR})

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

if(FIRST_9th_f)
  add_definitions(-DFIRST_9th=1)
endif(FIRST_9th_f)

if(FIRST_10th_f)
  add_definitions(-DFIRST_10th=1)
endif(FIRST_10th_f)

if(FIRST_11th_f)
  add_definitions(-DFIRST_11th=1)
endif(FIRST_11th_f)


if(FIRST_12th_f)
  add_definitions(-DFIRST_12th=1)
endif(FIRST_12th_f)


if(FIRST_13th_f)
  add_definitions(-DFIRST_13th=1)
endif(FIRST_13th_f)


if(FIRST_14th_f)
  add_definitions(-DFIRST_14th=1)
endif(FIRST_14th_f)

if(FIRST_15th_f)
  add_definitions(-DFIRST_15th=1)
endif(FIRST_15th_f)

if(FIRST_16th_f)
  add_definitions(-DFIRST_16th=1)
endif(FIRST_16th_f)

if(FIRST_17th_f)
  add_definitions(-DFIRST_17th=1)
endif(FIRST_17th_f)



###### Second Scenario

if(SECOND_1st_f)
	add_definitions(-DSECOND_1st=1)
endif(SECOND_1st_f)

if(SECOND_2nd_10_f)
	add_definitions(-DSECOND_2nd_10=1)
endif(SECOND_2nd_10_f)

if(SECOND_2nd_20_f)
	add_definitions(-DSECOND_2nd_20=1)
endif(SECOND_2nd_20_f)

if(SECOND_2nd_30_f)
	add_definitions(-DSECOND_2nd_30=1)
endif(SECOND_2nd_30_f)

if(SECOND_3rd_f)
	add_definitions(-DSECOND_3rd=1)
endif(SECOND_3rd_f)

if(SECOND_4th_10_f)
	add_definitions(-DSECOND_4th_10=1)
endif(SECOND_4th_10_f)

if(SECOND_4th_20_f)
	add_definitions(-DSECOND_4th_20=1)
endif(SECOND_4th_20_f)

if(SECOND_4th_30_f)
	add_definitions(-DSECOND_4th_30=1)
endif(SECOND_4th_30_f)

if(SECOND_5th_f)
	add_definitions(-DSECOND_5th=1)
endif(SECOND_5th_f)

if(SECOND_6th_f)
	add_definitions(-DSECOND_6th=1)
endif(SECOND_6th_f)

if(SECOND_7th_f)
	add_definitions(-DSECOND_7th=1)
endif(SECOND_7th_f)

if(SECOND_8th_f)
	add_definitions(-DSECOND_8th=1)
endif(SECOND_8th_f)

if(SECOND_9th_f)
	add_definitions(-DSECOND_9th=1)
endif(SECOND_9th_f)

if(SECOND_10th_f)
	add_definitions(-DSECOND_10th=1)
endif(SECOND_10th_f)

if(SECOND_11th_f)
	add_definitions(-DSECOND_11th=1)
endif(SECOND_11th_f)

if(SECOND_12th_f)
	add_definitions(-DSECOND_12th=1)
endif(SECOND_12th_f)


#SET(CMAKE_SHARED_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")
#SET(CMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")

add_executable(5G_QoS ${SRC})

find_library(LFDS_LIB lfds711 HINTS  liblfds7.1.1/liblfds711/bin/)

TARGET_LINK_LIBRARIES(5G_QoS PUBLIC ${LFDS_LIB} pthread mnl netfilter_queue m)

