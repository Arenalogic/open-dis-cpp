# OpenDIS6 and OpenDIS7 both define namespace DIS, so each library gets its
# own test executable and the two are never linked into one binary.
if(BUILD_OPENDIS6)
  set(unit_test_suite_src
    UnitTestSuite.cpp
    DataStreamTests.cpp
    IncomingMessageTests.cpp
    PduMarshallTests.cpp
    PduUtils.cpp
  )

  add_executable(
    UnitTestSuite
    ${unit_test_suite_src}
  )

  target_link_libraries(UnitTestSuite OpenDIS6)
  target_include_directories(UnitTestSuite PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

  add_test(NAME UnitTestSuite
           COMMAND UnitTestSuite -r junit -o UnitTestSuite-results.xml)
endif()

if(BUILD_OPENDIS7)
  set(unit_test_suite7_src
    UnitTestSuite.cpp
    Dis7RadioAndAggregateTests.cpp
  )

  add_executable(
    UnitTestSuite7
    ${unit_test_suite7_src}
  )

  target_link_libraries(UnitTestSuite7 OpenDIS7)
  target_include_directories(UnitTestSuite7 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

  add_test(NAME UnitTestSuite7
           COMMAND UnitTestSuite7 -r junit -o UnitTestSuite7-results.xml)
endif()
