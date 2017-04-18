cmake_minimum_required(VERSION 3.6)
project(EasyJNI)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fpermissive")

include_directories(/usr/lib/jvm/java-8-oracle/include/ jni/)
include_directories(/usr/lib/jvm/java-8-oracle/include/linux/ jni/)

set(SOURCE_FILES main.cpp include/Converter.h
		include/class/JavaClassInfo.cpp
		include/class/JavaField.cpp
		include/class/JavaMethode.cpp
		include/Utils.cpp
		)

set(INCLUDE_FILES include/utils/StringUtil.h

		include/class/JavaClassInfo.h
		include/class/JavaField.h
		include/class/JavaFieldImpl.h
		include/class/JavaMethode.h
		include/class/JavaMethodeImpl.h

		include/Utils.h)

set(TEST_FILES
		test/TestJavaClasses.h
		test/JNIImpl.cpp
		)

add_executable(EasyJNIExec ${SOURCE_FILES} ${INCLUDE_FILES})
add_library(EasyJNI SHARED ${SOURCE_FILES} ${INCLUDE_FILES})

add_library(EasyJNITest SHARED ${TEST_FILES})
target_link_libraries(EasyJNITest EasyJNI)