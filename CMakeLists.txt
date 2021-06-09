cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(ChipTool)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
#file( GLOB LIB_SOURCES include/dlib/*.cpp )
#file( GLOB LIB_HEADERS include/dlib/*.h )
#aadd_library( dlib STATIC ${LIB_SOURCES} ${LIB_HEADERS} )

add_subdirectory(include/dlib dlib_build)


add_executable( imglab 
   imglab/src/main.cpp
   imglab/src/metadata_editor.h
   imglab/src/metadata_editor.cpp
   imglab/src/convert_pascal_xml.h
   imglab/src/convert_pascal_xml.cpp
   imglab/src/convert_pascal_v1.h
   imglab/src/convert_pascal_v1.cpp
   imglab/src/convert_idl.h
   imglab/src/convert_idl.cpp
   imglab/src/common.h
   imglab/src/common.cpp
   imglab/src/cluster.cpp
   imglab/src/flip_dataset.cpp
)
add_executable( seal sealdnn/src/dnn_seal_chip.cpp)
add_executable( program sealdnn/src/sealFindr.cpp)

#target_include_directories(imglab PUBLIC include)
#target_include_directories(seal PUBLIC include)
#target_include_directories(program PUBLIC include)


target_link_libraries( imglab dlib::dlib)
target_link_libraries( seal dlib::dlib)
target_link_libraries( program dlib::dlib)

install(TARGETS imglab seal program 
  DESTINATION bin
  DESTINATION bin
  DESTINATION bin
  )
      #install(PROGRAMS convert_imglab_paths_to_relative copy_imglab_dataset DESTINATION bin )

