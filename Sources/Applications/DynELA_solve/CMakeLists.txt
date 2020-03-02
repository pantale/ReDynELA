FILE(GLOB SRCS *.h *.C)

# Projet global : Application DynELA solve
project(DynELA_solve)

# définitions générales

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR} 
	${SOURCE_LIBRARY_DIRECTORY}/basicTools 
	${SOURCE_LIBRARY_DIRECTORY}/lapack 
	${SOURCE_LIBRARY_DIRECTORY}/linearAlgebra 
	${SOURCE_LIBRARY_DIRECTORY}/finiteElement
	${SOURCE_LIBRARY_DIRECTORY}/interpretor)

LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

#ADD_DEFINITIONS(-Wall -O2)

# Création d'un exécutable
ADD_EXECUTABLE(DynELA_solve ${SRCS})
TARGET_LINK_LIBRARIES(DynELA_solve dnlDYN dnlFE dnlLA dnlBT lapack fl)
#INSTALL(TARGETS DynELA_solve DESTINATION ${DynELA_base}/bin)
