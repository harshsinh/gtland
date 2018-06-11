find_package(Matlab COMPONENTS MX_LIBRARY REQUIRED)

add_executable (position_controller
				main.c
				matlab/Position_Controller.c
				matlab/Position_Controller_data.c
				matlab/Position_Controller_initialize.c
				matlab/Position_Controller_terminate.c
				matlab/rtGetInf.c
				matlab/rtGetNaN.c
				matlab/rt_nonfinite.c
        #        matlab/interface/_coder_Position_Controller_api.c
        #        matlab/interface/_coder_Position_Controller_info.c
        #        matlab/interface/_coder_Position_Controller_mex.c
)

target_link_libraries (position_controller m ${catkin_LIBRARIES})
add_dependencies (position_controller ${PROJECT_NAME}_generated_messages_cpp ${catkin_EXPORTED_TARGETS})
