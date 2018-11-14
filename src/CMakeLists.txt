 set(bmp_lib bmp.cc )	

 add_library(bmp ${LIB_TYPE} ${bmp_lib})

 #link cpu threading library
 target_link_libraries(bmp ${CMAKE_THREAD_LIBS_INIT} )

if(BuildShared)
  #install  libbmp.so to <cmake output dir>/${CMAKE_CURRENT_DIR}/
  install(TARGETS bmp LIBRARY DESTINATION lib DESTINATION lib)
else()
  # install  libbmp.a to <cmake output dir>/${CMAKE_CURRENT_DIR}/
  install(TARGETS bmp LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
endif()

#to do
install(DIRECTORY ../include/bmp DESTINATION include)

