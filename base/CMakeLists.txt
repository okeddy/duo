aux_source_directory(. base_SRCS)

add_library(duo_base ${base_SRCS})

link_libraries(duo_base pthread)