include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/moduleHelper.cmake)

# No module
tolc_translate_file(LANGUAGE python FILE include/myLib.hpp FILE headers/simple.hpp OUTPUT_DIR out)
