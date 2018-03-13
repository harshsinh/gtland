add_executable (detector_node src/detector2.cxx)
target_link_libraries (detector_node
						${catkin_LIBRARIES}
						${OpenCV_LIBRARIES}
	)
add_executable (detector_node_old src/detector1.cxx)
target_link_libraries (detector_node_old
						${catkin_LIBRARIES}
						${OpenCV_LIBRARIES}
	)
add_dependencies (detector_node ${catkin_EXPORTED_TARGETS})
add_dependencies (detector_node_old ${catkin_EXPORTED_TARGETS})
