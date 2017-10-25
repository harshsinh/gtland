add_executable (cam2global
				src/cam_to_global.cxx
				src/matlab/Cam_to_Global.cpp
				src/matlab/Cam_to_Global_initialize.cpp
				src/matlab/Cam_to_Global_terminate.cpp
				src/matlab/rtGetInf.cpp
				src/matlab/rtGetNaN.cpp
				src/matlab/rt_nonfinite.cpp
)
target_link_libraries (cam2global ${catkin_LIBRARIES})
