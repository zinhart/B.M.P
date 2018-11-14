# bmp unit test src
  set(bmp_unit_tests_src
   run_all.cc
   bmp_test.cc
   )
add_executable(bmp_unit_tests ${bmp_unit_tests_src})

target_link_libraries(bmp_unit_tests 
					  gtest
					  #gmock_main # for google mock
					  ${CMAKE_THREAD_LIBS_INIT}
					  )
add_test(
  NAME bmp_unit_tests
  COMMAND bmp_unit_tests
  )
