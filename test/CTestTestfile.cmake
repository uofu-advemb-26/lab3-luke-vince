# CMake generated Testfile for 
# Source directory: /home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/test
# Build directory: /home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/test
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(simulate_mytest "/usr/bin/renode" "--disable-xwt" "--port" "-2" "--pid-file" "renode.pid" "--console" "-e" "\$ELF=@/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/test/mytest.elf; \$WORKING=@/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3; include @/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/test/simulate.resc; start")
set_tests_properties(simulate_mytest PROPERTIES  _BACKTRACE_TRIPLES "/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/test/CMakeLists.txt;39;add_test;/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/test/CMakeLists.txt;0;")
