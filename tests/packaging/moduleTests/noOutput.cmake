include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/moduleHelper.cmake)

# No output
tolc_translate_file(MODULE_NAME myLib LANGUAGE python FILE include/simple.hpp)
