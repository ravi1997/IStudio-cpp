# CMake generated Testfile for 
# Source directory: /home/ravi/workspace/IStudio
# Build directory: /home/ravi/workspace/IStudio/out/build/Configure preset using toolchain file
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[ipl_help_test]=] "/home/ravi/workspace/IStudio/out/build/Configure preset using toolchain file/IStudio" "--help")
set_tests_properties([=[ipl_help_test]=] PROPERTIES  WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;198;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_version_test]=] "/home/ravi/workspace/IStudio/out/build/Configure preset using toolchain file/IStudio" "--version")
set_tests_properties([=[ipl_version_test]=] PROPERTIES  WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;203;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_sample_lex_test]=] "/home/ravi/workspace/IStudio/out/build/Configure preset using toolchain file/IStudio" "--lex-ipl-samples")
set_tests_properties([=[ipl_sample_lex_test]=] PROPERTIES  WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;209;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_simple_compile_test]=] "/home/ravi/workspace/IStudio/out/build/Configure preset using toolchain file/IStudio" "compile" "examples/ipl/00_hello_world.ipl" "--grammar" "examples/grammar_rules.txt" "--translation" "examples/translation_rules.txt")
set_tests_properties([=[ipl_simple_compile_test]=] PROPERTIES  WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;215;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_invalid_unterminated_string]=] "/home/ravi/workspace/IStudio/out/build/Configure preset using toolchain file/IStudio" "compile" "examples/ipl_invalid/00_unterminated_string.ipl")
set_tests_properties([=[ipl_invalid_unterminated_string]=] PROPERTIES  WILL_FAIL "TRUE" WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;221;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_invalid_unknown_character]=] "/home/ravi/workspace/IStudio/out/build/Configure preset using toolchain file/IStudio" "compile" "examples/ipl_invalid/01_unknown_character.ipl")
set_tests_properties([=[ipl_invalid_unknown_character]=] PROPERTIES  WILL_FAIL "TRUE" WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;227;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_invalid_missing_paren]=] "/home/ravi/workspace/IStudio/out/build/Configure preset using toolchain file/IStudio" "compile" "examples/ipl_invalid/02_missing_paren.ipl")
set_tests_properties([=[ipl_invalid_missing_paren]=] PROPERTIES  WILL_FAIL "TRUE" WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;233;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_invalid_missing_semicolon]=] "/home/ravi/workspace/IStudio/out/build/Configure preset using toolchain file/IStudio" "compile" "examples/ipl_invalid/03_missing_semicolon.ipl")
set_tests_properties([=[ipl_invalid_missing_semicolon]=] PROPERTIES  WILL_FAIL "TRUE" WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;239;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_full_suite_test]=] "/home/ravi/workspace/IStudio/scripts/run_ipl_suite.sh")
set_tests_properties([=[ipl_full_suite_test]=] PROPERTIES  TIMEOUT "120" WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;249;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
add_test([=[ipl_samples_test]=] "/home/ravi/workspace/IStudio/scripts/test_ipl_samples.sh")
set_tests_properties([=[ipl_samples_test]=] PROPERTIES  TIMEOUT "120" WORKING_DIRECTORY "/home/ravi/workspace/IStudio" _BACKTRACE_TRIPLES "/home/ravi/workspace/IStudio/CMakeLists.txt;256;add_test;/home/ravi/workspace/IStudio/CMakeLists.txt;0;")
