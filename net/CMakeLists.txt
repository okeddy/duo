aux_source_directory(. net_SRCS)

include_directories(../base)

add_library(duo_net ${net_SRCS})

target_link_libraries(duo_net duo_base pthread)