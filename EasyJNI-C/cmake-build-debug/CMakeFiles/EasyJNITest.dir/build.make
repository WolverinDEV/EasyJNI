# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/wolverindev/clion-2016.3.3/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/wolverindev/clion-2016.3.3/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wolverindev/git/EasyJNI/EasyJNI-C

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wolverindev/git/EasyJNI/EasyJNI-C/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/EasyJNITest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/EasyJNITest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/EasyJNITest.dir/flags.make

CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o: CMakeFiles/EasyJNITest.dir/flags.make
CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o: ../test/JNIImpl.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wolverindev/git/EasyJNI/EasyJNI-C/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o -c /home/wolverindev/git/EasyJNI/EasyJNI-C/test/JNIImpl.cpp

CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wolverindev/git/EasyJNI/EasyJNI-C/test/JNIImpl.cpp > CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.i

CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wolverindev/git/EasyJNI/EasyJNI-C/test/JNIImpl.cpp -o CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.s

CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o.requires:

.PHONY : CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o.requires

CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o.provides: CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o.requires
	$(MAKE) -f CMakeFiles/EasyJNITest.dir/build.make CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o.provides.build
.PHONY : CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o.provides

CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o.provides.build: CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o


# Object files for target EasyJNITest
EasyJNITest_OBJECTS = \
"CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o"

# External object files for target EasyJNITest
EasyJNITest_EXTERNAL_OBJECTS =

libEasyJNITest.so: CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o
libEasyJNITest.so: CMakeFiles/EasyJNITest.dir/build.make
libEasyJNITest.so: libEasyJNI.so
libEasyJNITest.so: CMakeFiles/EasyJNITest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wolverindev/git/EasyJNI/EasyJNI-C/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libEasyJNITest.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EasyJNITest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/EasyJNITest.dir/build: libEasyJNITest.so

.PHONY : CMakeFiles/EasyJNITest.dir/build

CMakeFiles/EasyJNITest.dir/requires: CMakeFiles/EasyJNITest.dir/test/JNIImpl.cpp.o.requires

.PHONY : CMakeFiles/EasyJNITest.dir/requires

CMakeFiles/EasyJNITest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/EasyJNITest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/EasyJNITest.dir/clean

CMakeFiles/EasyJNITest.dir/depend:
	cd /home/wolverindev/git/EasyJNI/EasyJNI-C/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wolverindev/git/EasyJNI/EasyJNI-C /home/wolverindev/git/EasyJNI/EasyJNI-C /home/wolverindev/git/EasyJNI/EasyJNI-C/cmake-build-debug /home/wolverindev/git/EasyJNI/EasyJNI-C/cmake-build-debug /home/wolverindev/git/EasyJNI/EasyJNI-C/cmake-build-debug/CMakeFiles/EasyJNITest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/EasyJNITest.dir/depend

