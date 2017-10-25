add_executable (detector_node src/detector2.cxx)
target_link_libraries (detector_node
						${catkin_LIBRARIES}
						${openCV_LIBRARIES}
	)
