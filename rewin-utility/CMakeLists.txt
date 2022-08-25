set(CMW_ProjectName rewin-utility)

include(../find_source_files.cmake)
find_source_files_curr_dir(SRCS)

add_library (${CMW_ProjectName} STATIC ${SRCS})
set_target_properties(${CMW_ProjectName} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${CMW_ProjectName} PUBLIC .)
