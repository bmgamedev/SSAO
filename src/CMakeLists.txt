add_executable(${PROJECT_NAME} 
	main.cc
	#plus any other files I make
)

target_link_libraries(${PROJECT_NAME} 
	PUBLIC 
		Engin3D 
)

target_compile_features(${PROJECT_NAME} 
	PUBLIC 
		cxx_std_17 
)

set_target_properties(${PROJECT_NAME} 
	PROPERTIES 
		CXX_EXTENSIONS OFF # not essential
		#FOLDER ${PROJECT_NAME} 
)

#add_dependencies(${PROJECT_NAME} 
#	copy_resources
#)
