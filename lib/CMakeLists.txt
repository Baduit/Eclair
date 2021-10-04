find_package(Threads)

add_library(Eclair INTERFACE)
target_sources(Eclair INTERFACE ${CMAKE_CURRENT_LIST_DIR}/Eclair/Eclair.hpp)
target_compile_features(Eclair INTERFACE cxx_std_20)
target_compile_options(Eclair
                       INTERFACE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)
target_include_directories(Eclair INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(Eclair INTERFACE ${CMAKE_THREAD_LIBS_INIT})