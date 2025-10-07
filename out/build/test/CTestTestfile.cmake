# CMake generated Testfile for 
# Source directory: /home/ravi/workspace/IStudio
# Build directory: /home/ravi/workspace/IStudio/out/build/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[ipl_help_test]=] "/home/ravi/workspace/IStudio/out/build/test/IStudio" "--help")
set_tests_properties([=[ipl_help_test]=] PROPERTIES  WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;200;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_version_test]=] "/home/ravi/workspace/IStudio/out/build/test/IStudio" "--version")
set_tests_properties([=[ipl_version_test]=] PROPERTIES  WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;205;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_sample_lex_test]=] "/home/ravi/workspace/IStudio/out/build/test/IStudio" "--lex-ipl-samples")
set_tests_properties([=[ipl_sample_lex_test]=] PROPERTIES  WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;211;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_simple_compile_test]=] "/home/ravi/workspace/IStudio/out/build/test/IStudio" "compile" "examples/ipl/00_hello_world.ipl" "--grammar" "examples/grammar_rules.txt" "--translation" "examples/translation_rules.txt")
set_tests_properties([=[ipl_simple_compile_test]=] PROPERTIES  WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;217;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_invalid_unterminated_string]=] "/home/ravi/workspace/IStudio/out/build/test/IStudio" "compile" "examples/ipl_invalid/00_unterminated_string.ipl")
set_tests_properties([=[ipl_invalid_unterminated_string]=] PROPERTIES  WILL_FAIL "TRUE" WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;223;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_invalid_unknown_character]=] "/home/ravi/workspace/IStudio/out/build/test/IStudio" "compile" "examples/ipl_invalid/01_unknown_character.ipl")
set_tests_properties([=[ipl_invalid_unknown_character]=] PROPERTIES  WILL_FAIL "TRUE" WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;229;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_invalid_missing_paren]=] "/home/ravi/workspace/IStudio/out/build/test/IStudio" "compile" "examples/ipl_invalid/02_missing_paren.ipl")
set_tests_properties([=[ipl_invalid_missing_paren]=] PROPERTIES  WILL_FAIL "TRUE" WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;235;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_invalid_missing_semicolon]=] "/home/ravi/workspace/IStudio/out/build/test/IStudio" "compile" "examples/ipl_invalid/03_missing_semicolon.ipl")
set_tests_properties([=[ipl_invalid_missing_semicolon]=] PROPERTIES  WILL_FAIL "TRUE" WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;241;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_full_suite_test]=] "/home/ravi/workspace/IStudio/scripts/run_ipl_suite.sh")
set_tests_properties([=[ipl_full_suite_test]=] PROPERTIES  TIMEOUT "120" WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;251;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_samples_test]=] "/home/ravi/workspace/IStudio/scripts/test_ipl_samples.sh")
set_tests_properties([=[ipl_samples_test]=] PROPERTIES  TIMEOUT "120" WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;258;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
