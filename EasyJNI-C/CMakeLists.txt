cmake_minimum_required(VERSION 3.6)
project(EasyJNI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive")

include_directories(/usr/lib/jvm/java-8-oracle/include/ jni/)
include_directories(/usr/lib/jvm/java-8-oracle/include/linux/ jni/)

set(SOURCE_FILES main.cpp
		src/JavaClassInfo.cpp
		src/JavaField.cpp
		src/JavaMethode.cpp
		src/Utils.cpp
		)

set(INCLUDE_FILES
		include/EasyJNI.h

		include/utils/StringUtil.h

		include/class/JavaClassInfo.h
		include/class/JavaField.h
		include/class/JavaFieldImpl.h
		include/class/JavaMethode.h
		include/class/JavaMethodeImpl.h
		include/class/DefaultClassDefinitions.h
		include/TypeConverter.h

		include/JavaClassDefine.h
		include/Utils.h)

set(TEST_FILES
		test/JNIImpl.cpp
		)

add_definitions(-DEasyJNI_ENABLE_DEBUG)

add_executable(EasyJNIExec ${SOURCE_FILES} ${INCLUDE_FILES})
target_link_libraries(EasyJNIExec pthread)

add_library(EasyJNI SHARED ${SOURCE_FILES} ${INCLUDE_FILES})

add_library(EasyJNITest SHARED ${TEST_FILES})
target_link_libraries(EasyJNITest EasyJNI)