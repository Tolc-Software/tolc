include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/moduleHelper.cmake)

tolc_translate_file(
  MODULE_NAME
  myLib
  LANGUAGE
  python
  FILE
  ${CMAKE_CURRENT_LIST_DIR}/headers/simple.hpp
  OUTPUT_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/out
  INCLUDES
  doesNotMatter)
# tolc_translate_file(MODULE_NAME myLib LANGUAGE python FILE include/myLib.hpp)
# tolc_translate_file(LANGUAGE python FILE include/myLib.hpp)
# tolc_translate_file(MODULE_NAME myLib FILE include/myLib.hpp)
# tolc_translate_file(MODULE_NAME myLib LANGUAGE python)
