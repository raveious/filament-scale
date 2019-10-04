find_program( AVR_CC avr-gcc )
find_program( AVR_CXX avr-g++ )
find_program( AVR_OBJCOPY avr-objcopy )
find_program( AVR_SIZE_TOOL avr-size )
find_program( AVR_OBJDUMP avr-objdump )
find_program( AWK awk )


##########################################################################
# toolchain starts with defining mandatory variables
##########################################################################
set( CMAKE_SYSTEM_NAME Generic )
set( CMAKE_SYSTEM_PROCESSOR avr )
set( CMAKE_C_COMPILER ${AVR_CC} )
set( CMAKE_CXX_COMPILER ${AVR_CXX} )


###########################################################################
# some cmake cross-compile necessities
##########################################################################
if( DEFINED ENV{AVR_FIND_ROOT_PATH} )
    set( CMAKE_FIND_ROOT_PATH $ENV{AVR_FIND_ROOT_PATH} )
else( DEFINED ENV{AVR_FIND_ROOT_PATH} )
    if( EXISTS "/opt/local/avr" )
      set( CMAKE_FIND_ROOT_PATH "/opt/local/avr" )
    elseif( EXISTS "/usr/local/avr" )
      set( CMAKE_FIND_ROOT_PATH "/usr/local/avr" )
    elseif( EXISTS "/usr/avr" )
      set( CMAKE_FIND_ROOT_PATH "/usr/avr" )
    elseif( EXISTS "/usr/local/avr" )
      set( CMAKE_FIND_ROOT_PATH "/usr/local/avr" )
    elseif( EXISTS "/usr/lib/avr" )
      set( CMAKE_FIND_ROOT_PATH "/usr/lib/avr" )
    else( EXISTS "/opt/local/avr" )
      message( FATAL_ERROR "Please set AVR_FIND_ROOT_PATH in your environment." )
    endif( EXISTS "/opt/local/avr" )
endif( DEFINED ENV{AVR_FIND_ROOT_PATH} )
set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )
# not added automatically, since CMAKE_SYSTEM_NAME is "generic"
set( CMAKE_SYSTEM_INCLUDE_PATH "${CMAKE_FIND_ROOT_PATH}/include" )
set( CMAKE_SYSTEM_LIBRARY_PATH "${CMAKE_FIND_ROOT_PATH}/lib" )


##########################################################################
# status messages for generating
##########################################################################
message( STATUS "Set CMAKE_FIND_ROOT_PATH to ${CMAKE_FIND_ROOT_PATH}" )
message( STATUS "Set CMAKE_SYSTEM_INCLUDE_PATH to ${CMAKE_SYSTEM_INCLUDE_PATH}" )
message( STATUS "Set CMAKE_SYSTEM_LIBRARY_PATH to ${CMAKE_SYSTEM_LIBRARY_PATH}" )


#########################################################################
# some necessary tools and variables for AVR builds, which may not
# defined yet
# - AVR_UPLOADTOOL
# - AVR_UPLOADTOOL_PORT
# - AVR_PROGRAMMER
# - AVR_MCU
# - AVR_SIZE_ARGS
##########################################################################

# default upload tool
if( NOT AVR_UPLOADTOOL )
   set(
      AVR_UPLOADTOOL avrdude
      CACHE STRING "Set default upload tool: avrdude"
   )
   find_program( AVR_UPLOADTOOL avrdude )
endif( NOT AVR_UPLOADTOOL )

# default upload tool port
if( NOT AVR_UPLOADTOOL_PORT )
   set(
      AVR_UPLOADTOOL_PORT usb
      CACHE STRING "Set default upload tool port: usb"
   )
endif( NOT AVR_UPLOADTOOL_PORT )

# default programmer (hardware)
if( NOT AVR_PROGRAMMER )
   set(
      AVR_PROGRAMMER wiring
      CACHE STRING "Set default programmer hardware model: wiring"
   )
endif( NOT AVR_PROGRAMMER )

# default programmer upload speed
if( NOT AVR_UPLOAD_SPEED )
   set(
      AVR_UPLOAD_SPEED 115200
      CACHE STRING "Set default AVR_UPLOAD_SPEED: 115200 baud"
   )
endif( NOT AVR_UPLOAD_SPEED )

# default MCU (chip)
if( NOT AVR_MCU )
   set(
      AVR_MCU atmega328p
      CACHE STRING "Set default MCU: atmega328p (see 'avr-gcc --target-help' for valid values)"
   )
endif( NOT AVR_MCU )

# default MCU speed
if( NOT AVR_MCU_SPEED )
    set(
       AVR_MCU_SPEED 16000000UL
       CACHE STRING "Set default MCU SPEED: 16000000UL"
    )
endif( NOT AVR_MCU_SPEED )

# Prep avrdude special options
if( AVR_UPLOADTOOL MATCHES avrdude )
    set( AVR_UPLOADTOOL_OPTIONS -b ${AVR_UPLOAD_SPEED})
endif( AVR_UPLOADTOOL MATCHES avrdude )


##########################################################################
# set default compiler options:
##########################################################################
set( CMAKE_C_FLAGS "-std=c99 -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=${AVR_MCU}"  CACHE STRING "Default C flags for all builds" FORCE )
set( CMAKE_CXX_FLAGS "-fno-exceptions -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=${AVR_MCU}"  CACHE STRING "Default C++ flags for all builds" FORCE )


set( CMAKE_C_FLAGS_RELEASE "-O3 -Wall" CACHE STRING "Default C flags for release" FORCE )
set( CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall" CACHE STRING "Default C++ flags for release" FORCE )

set( CMAKE_C_FLAGS_MINSIZEREL "-Os -mcall-prologues -Wall" CACHE STRING "Default C flags for minimum size release" FORCE )
set( CMAKE_CXX_FLAGS_MINSIZEREL "-Os -mcall-prologues -Wall" CACHE STRING "Default C++ flags for minimum size release" FORCE )

set( CMAKE_C_FLAGS_DEBUG "-g -Wall" CACHE STRING "Default C flags for debug" FORCE )
set( CMAKE_CXX_FLAGS_DEBUG "-g -Wall" CACHE STRING "Default C++ flags for debug" FORCE )

set( CMAKE_C_FLAGS_RELWITHDEBINFO "-O3 -g -Wall" CACHE STRING "Default C flags for release with debug info" FORCE )
set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -Wall" CACHE STRING "Default C++ flags for release with debug info" FORCE )

add_definitions(-DF_CPU=${AVR_MCU_SPEED})


##########################################################################
# status messages
##########################################################################
message( STATUS "Current uploadtool is: ${AVR_UPLOADTOOL}" )
message( STATUS "Current programmer is: ${AVR_PROGRAMMER}" )
message( STATUS "Current upload port is: ${AVR_UPLOADTOOL_PORT}" )
message( STATUS "Current uploadtool options are: ${AVR_UPLOADTOOL_OPTIONS}" )
message( STATUS "Current AVR MCU is set to: ${AVR_MCU}" )
message( STATUS "Current AVR MCU speed is set to: ${AVR_MCU_SPEED}" )


##########################################################################
# add_avr_executable
# - IN_VAR: EXECUTABLE_NAME
#
# Creates targets and dependencies for AVR toolchain, building an
# executable. Calls add_executable with ELF file as target name, so
# any link dependencies need to be using that target, e.g. for
# target_link_libraries(<EXECUTABLE_NAME>-${AVR_MCU}.elf ...).
##########################################################################

function( add_avr_executable EXECUTABLE_NAME )

   if( NOT ARGN )
      message( FATAL_ERROR "No source files given for ${EXECUTABLE_NAME}." )
   endif( NOT ARGN )

   # set file names
   set( elf_file ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}.elf )
   set( hex_file ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}.hex )
   set( map_file ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}.map )
   set( eeprom_image ${EXECUTABLE_NAME}${MCU_TYPE_FOR_FILENAME}-eeprom.hex )

   # elf file
   add_executable( ${elf_file} EXCLUDE_FROM_ALL ${ARGN} )

  add_custom_command(
     OUTPUT ${hex_file}
     COMMAND
        ${AVR_OBJCOPY} -j .text -j .data -O ihex ${elf_file} ${hex_file}
     DEPENDS ${elf_file}
  )

  add_custom_target(
     ${EXECUTABLE_NAME}
     ALL
     DEPENDS ${hex_file}
  )

  set_target_properties(
     ${EXECUTABLE_NAME}
     PROPERTIES
        OUTPUT_NAME "${elf_file}"
  )

  # clean
  get_directory_property( clean_files ADDITIONAL_MAKE_CLEAN_FILES )
  set_directory_properties(
     PROPERTIES
        ADDITIONAL_MAKE_CLEAN_FILES "${map_file}"
  )

  # upload - with avrdude
  add_custom_target(
     upload_${EXECUTABLE_NAME}
     ${AVR_UPLOADTOOL} -p ${AVR_MCU} -c ${AVR_PROGRAMMER} ${AVR_UPLOADTOOL_OPTIONS}
        -U flash:w:${hex_file}
        -U lfuse:w:${L_FUSE}:m
        -U hfuse:w:${H_FUSE}:m
        -U efuse:w:${E_FUSE}:m
        -P ${AVR_UPLOADTOOL_PORT}
     DEPENDS ${hex_file}
     COMMENT "Uploading ${hex_file} to ${AVR_MCU} using ${AVR_PROGRAMMER}"
  )

endfunction( add_avr_executable )



##########################################################################
# add_avr_library
# - IN_VAR: LIBRARY_NAME
#
# Calls add_library with an optionally concatenated name
# <LIBRARY_NAME>${MCU_TYPE_FOR_FILENAME}.
# This needs to be used for linking against the library, e.g. calling
# target_link_libraries(...).
##########################################################################

function( add_avr_library LIBRARY_NAME )

   if( NOT ARGN )
      message( FATAL_ERROR "No source files given for ${LIBRARY_NAME}." )
   endif( NOT ARGN )

   set( lib_file ${LIBRARY_NAME}${MCU_TYPE_FOR_FILENAME} )

   add_library( ${lib_file} STATIC ${ARGN} )

   set_target_properties(
      ${lib_file}
      PROPERTIES
         OUTPUT_NAME "${lib_file}"
   )

   if( NOT TARGET ${LIBRARY_NAME} )
      add_custom_target(
         ${LIBRARY_NAME}
         ALL
         DEPENDS ${lib_file}
      )

      set_target_properties(
         ${LIBRARY_NAME}
         PROPERTIES
            OUTPUT_NAME "${lib_file}"
      )
   endif( NOT TARGET ${LIBRARY_NAME} )

endfunction( add_avr_library )



##########################################################################
# avr_target_link_libraries
# - IN_VAR: EXECUTABLE_TARGET
# - ARGN  : targets and files to link to
#
# Calls target_link_libraries with AVR target names (concatenation,
# extensions and so on.
##########################################################################

function( avr_target_link_libraries EXECUTABLE_TARGET )

   if( NOT ARGN )
      message( FATAL_ERROR "Nothing to link to ${EXECUTABLE_TARGET}." )
   endif( NOT ARGN )

   get_target_property( TARGET_LIST ${EXECUTABLE_TARGET} OUTPUT_NAME )

   foreach( TGT ${ARGN} )
      if( TARGET ${TGT} )
         get_target_property( ARG_NAME ${TGT} OUTPUT_NAME )
         list( APPEND TARGET_LIST ${ARG_NAME} )
      else( TARGET ${TGT} )
         list( APPEND NON_TARGET_LIST ${TGT} )
      endif( TARGET ${TGT} )
   endforeach( TGT ${ARGN} )

   target_link_libraries( ${TARGET_LIST} ${NON_TARGET_LIST} )

endfunction( avr_target_link_libraries EXECUTABLE_TARGET )