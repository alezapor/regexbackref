configure_file(Config.h.in Config.h)

find_package(Boost COMPONENTS system filesystem unit_test_framework REQUIRED)


add_executable(matcher main.cpp
			   ast.cpp automaton.cpp tape.cpp
			   avdfa.cpp lexer.cpp matcher.cpp
			   memorystate.cpp ndtm.cpp parser.cpp
			   )

add_executable(test_init test.cpp
			   ast.cpp automaton.cpp tape.cpp
			   avdfa.cpp lexer.cpp matcher.cpp
			   memorystate.cpp ndtm.cpp parser.cpp
			   )

target_include_directories(matcher PRIVATE include)

target_include_directories(test_init PRIVATE include)

target_link_libraries(test_init
		${Boost_FILESYSTEM_LIBRARY}
		${Boost_SYSTEM_LIBRARY}
		${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
		)

target_include_directories(matcher PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")

add_test(NAME RegexFinale
		COMMAND test_init)