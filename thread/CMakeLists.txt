aux_source_directory(. DIR_SRCS)

add_library(thread ${DIR_SRCS})

link_libraries(thread pthread)