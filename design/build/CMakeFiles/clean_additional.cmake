# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\kanban_cli_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\kanban_cli_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\kanban_common_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\kanban_common_autogen.dir\\ParseCache.txt"
  "CMakeFiles\\kanban_gui_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\kanban_gui_autogen.dir\\ParseCache.txt"
  "kanban_cli_autogen"
  "kanban_common_autogen"
  "kanban_gui_autogen"
  )
endif()
