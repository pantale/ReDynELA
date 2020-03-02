FILE(GLOB SRCS *.h *.C)

find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser InputDyn.y ${CMAKE_CURRENT_BINARY_DIR}/InputDyn_yacc.cpp COMPILE_FLAGS "-p InputDyn")
FLEX_TARGET(MyScanner InputDyn.l  ${CMAKE_CURRENT_BINARY_DIR}/InputDyn_lex.cpp COMPILE_FLAGS "-PInputDyn")
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

# Include of previous directories
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} ${SOURCE_LIBRARY_DIRECTORY}/basicTools ${SOURCE_LIBRARY_DIRECTORY}/linearAlgebra ${SOURCE_LIBRARY_DIRECTORY}/finiteElement)

# Création d'une librairie
ADD_LIBRARY(dnlDYN  ${SRCS} ${BISON_MyParser_OUTPUTS} ${FLEX_MyScanner_OUTPUTS})
