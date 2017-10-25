add_executable (cam2global
				src/cam_to_global.cxx
				src/Cam_to_Global/Cam_to_Global.cpp
				src/Cam_to_Global/Cam_to_Global_initialize.cpp
				src/Cam_to_Global/Cam_to_Global_terminate.cpp
				src/Cam_to_Global/rtGetInf.cpp
				src/Cam_to_Global/rtGetNan.cpp
				src/Cam_to_Global/rt_nonfinite.cpp
)
target_link_libraries (cam2global ${catkin_LIBRARIES})
