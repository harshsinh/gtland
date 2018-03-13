add_executable(gtland_ukf
			  src/ukf.cxx
			  src/matlab/UKF_main_c.cpp
			  src/matlab/xgehrd.cpp
			  src/matlab/rtGetInf.cpp
			  src/matlab/UKF_main_c_emxutil.cpp
			  src/matlab/xgerc.cpp
			  src/matlab/rtGetNaN.cpp
			  src/matlab/UKF_main_c_initialize.cpp
			  src/matlab/xhseqr.cpp
			  src/matlab/rt_nonfinite.cpp
			  src/matlab/UKF_main_c_rtwutil.cpp
			  src/matlab/xnrm2.cpp
			  src/matlab/UKF_main_c_terminate.cpp
			  src/matlab/xrot.cpp
			  src/matlab/schur.cpp
			  src/matlab/xungorghr.cpp
			  src/matlab/sqrt.cpp
			  src/matlab/xdhseqr.cpp
			  src/matlab/xzlarfg.cpp
			  src/matlab/sqrtm.cpp
			  src/matlab/xdlanv2.cpp
)
target_link_libraries(gtland_ukf ${catkin_LIBRARIES})
add_dependencies (gtland_ukf ${PROJECT_NAME}_generated_messages_cpp ${catkin_EXPORTED_TARGETS})
