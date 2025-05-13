cmake_minimum_required(VERSION 3.25)

project(
  TestStage
  VERSION 1.0
  LANGUAGES CXX)

# Export compile flags to compile_commands.json database. Useful for linters and
# autocompletion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Get the variable 'target_name' NOTE: This will be generated for each test
include(cmake/TargetName.cmake)

# Get the variable 'sources' NOTE: This will be generated for each test
include(cmake/Sources.cmake)

find_package(
  Java
  COMPONENTS Development
  REQUIRED)
find_package(JNI REQUIRED)
include(UseJava)

# Note: Creates the target passed by GENERATE_NATIVE_HEADERS so in this case
# MyModule-native

# NOTE: The jar has to be of the same name as the class that has the main
# function
add_jar(MyModule SOURCES com/tolc/mymodule/MyModule.java
                         GENERATE_NATIVE_HEADERS MyModule-native)
add_library(${target_name} SHARED ${sources})
target_include_directories(${target_name} PRIVATE ${JNI_INCLUDE_DIRS})
target_link_libraries(${target_name} PRIVATE ${JNI_LIBRARIES} MyModule-native)

get_target_property(_MYMODULE_JAR_FILE MyModule JAR_FILE)

enable_testing()
add_test(
  NAME test_${target}
  COMMAND
    ${Java_JAVA_EXECUTABLE}
    -Djava.library.path=$<TARGET_FILE_DIR:${target_name}> --class-path
    ${_MYMODULE_JAR_FILE} com.tolc.mymodule.MyModule)

# Use C++17 since the tests are using std::optional etc.
set_target_properties(
  ${target_name}
  PROPERTIES CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS OFF
             CXX_STANDARD 17)
