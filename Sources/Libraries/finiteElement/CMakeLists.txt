FILE(GLOB SRCS *.h *.C)

ADD_LIBRARY(dnlFE ${SRCS})
#TARGET_LINK_LIBRARIES(libdnlLA dnlKernel lapacke lapack blas)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Include of previous directories
INCLUDE_DIRECTORIES(${SOURCE_LIBRARY_DIRECTORY}/basicTools ${SOURCE_LIBRARY_DIRECTORY}/lapack ${SOURCE_LIBRARY_DIRECTORY}/linearAlgebra)

SOURCE_GROUP(base             REGULAR_EXPRESSION ".*\\.(C|i|h)")
SOURCE_GROUP(generated        REGULAR_EXPRESSION ".*\\.(cxx|py)")

