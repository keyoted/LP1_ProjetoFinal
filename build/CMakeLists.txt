cmake_minimum_required (VERSION 3.0)

project (LP_PROJETO_FINAL VERSION 0.1.0)

set( CMAKE_C_STANDARD 11 )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin )
include_directories(${CMAKE_BINARY_DIR}/../dependencies)

add_executable( main.x86
                ../src/main.c
                ../src/menu.c
                ../src/morada.c
                ../src/artigo.c
                ../src/encomenda.c
                ../src/utilities.c
                ../src/utilizador.c)