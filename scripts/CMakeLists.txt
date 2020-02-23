
# Find Python in venv instead of the normal location
find_program(PYTHON_EXECUTABLE python PATHS "${CMAKE_SOURCE_DIR}/venv/bin" NO_DEFAULT_PATH)

# Make the background header
add_custom_target(filament-scale-background 
                  ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/make-background.py ${CMAKE_CURRENT_SOURCE_DIR}/background.png ${CMAKE_CURRENT_BINARY_DIR}/background.h
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  COMMENT "Making background image..."
                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/background.png
                          ${CMAKE_CURRENT_SOURCE_DIR}/make-background.py)
