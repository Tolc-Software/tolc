include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/moduleHelper.cmake)

# No language
tolc_translate_file(MODULE_NAME myLib FILE include/myLib.hpp FILE headers/simple.hpp OUTPUT out)
