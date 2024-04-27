cmake_minimum_required(VERSION 3.5)
project(hicore)
 
set(CMAKE_C_STANDARD 99)

aux_source_directory(src  SOURCES)
aux_source_directory(demo  SOURCES2)

add_library(sjon  ${SOURCES} )

add_executable(test_sjon   ${SOURCES2} ${SOURCES})

target_link_libraries(test_sjon sjon)


