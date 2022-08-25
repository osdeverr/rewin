set(CMW_ProjectName rewin-example)

include(../find_source_files.cmake)
find_source_files_curr_dir(SRCS)

add_executable (${CMW_ProjectName} ${SRCS})
set_target_properties(${CMW_ProjectName} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${CMW_ProjectName} PUBLIC .)
target_link_libraries(${CMW_ProjectName} PUBLIC rewin)
