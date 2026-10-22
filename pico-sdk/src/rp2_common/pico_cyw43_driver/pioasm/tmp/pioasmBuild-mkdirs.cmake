# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/vincent/pico-sdk/tools/pioasm"
  "/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/pioasm"
  "/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/pioasm-install"
  "/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
  "/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/vincent/Desktop/ECE6785-AdvEmb/ECE6785-Lab3/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
