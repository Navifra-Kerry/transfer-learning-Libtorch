project(transfer-learning)

cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 14)

add_definitions(-D__CUDA_NO_HALF_OPERATORS__)

include_directories(./include)
include_directories(./include/model)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")


if(WIN32)
	set(CMAKE_PREFIX_PATH "D:/local/libtorch/")
	set(OpenCV_DIR "D:/local/opencv/build")
	set(CUDNN_INCLUDE_PATH "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v10.2/include")
endif(WIN32)

find_package(Torch REQUIRED)
find_package(OpenCV REQUIRED)

set(SRC_FILE src/model/resnet.cpp
			 src/ImageNetDataSet.cpp
			 src/transfer-learning.cpp
			 )

add_executable(${PROJECT_NAME} ${SRC_FILE})
target_link_libraries(${PROJECT_NAME} "${TORCH_LIBRARIES}")
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} )

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 14)
