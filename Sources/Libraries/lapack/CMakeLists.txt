FILE(GLOB SRCS libF77/*.h libF77/*.c libI77/*.h libI77/*.c Blas/*.h Blas/*.c Lapack/*.h Lapack/*.c *.h)

# définitions générales
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
ADD_DEFINITIONS(-O4 -DSkip_f2c_Undefs -DnoBlasWrap)

# Création d'une librairie
ADD_LIBRARY(lapack ${SRCS})

