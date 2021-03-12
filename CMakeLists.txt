cmake_minimum_required(VERSION 2.8)
project( Video )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( Fourier Fourier.cpp )
target_link_libraries( Fourier ${OpenCV_LIBS} )


cmake_minimum_required(VERSION 2.8)
project( Record )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( Fourier_data Fourier_data.cpp )
target_link_libraries( Fourier_data ${OpenCV_LIBS} )
