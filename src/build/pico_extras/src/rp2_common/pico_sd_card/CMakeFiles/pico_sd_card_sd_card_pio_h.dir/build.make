# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mdietric/flypicopi/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mdietric/flypicopi/src/build

# Utility rule file for pico_sd_card_sd_card_pio_h.

# Include the progress variables for this target.
include pico_extras/src/rp2_common/pico_sd_card/CMakeFiles/pico_sd_card_sd_card_pio_h.dir/progress.make

pico_extras/src/rp2_common/pico_sd_card/CMakeFiles/pico_sd_card_sd_card_pio_h: pico_extras/src/rp2_common/pico_sd_card/sd_card.pio.h


pico_extras/src/rp2_common/pico_sd_card/sd_card.pio.h: ../pico-extras/src/rp2_common/pico_sd_card/sd_card.pio
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/mdietric/flypicopi/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating sd_card.pio.h"
	cd /home/mdietric/flypicopi/src/build/pico_extras/src/rp2_common/pico_sd_card && ../../../../pioasm/pioasm -o c-sdk /home/mdietric/flypicopi/src/pico-extras/src/rp2_common/pico_sd_card/sd_card.pio /home/mdietric/flypicopi/src/build/pico_extras/src/rp2_common/pico_sd_card/sd_card.pio.h

pico_sd_card_sd_card_pio_h: pico_extras/src/rp2_common/pico_sd_card/CMakeFiles/pico_sd_card_sd_card_pio_h
pico_sd_card_sd_card_pio_h: pico_extras/src/rp2_common/pico_sd_card/sd_card.pio.h
pico_sd_card_sd_card_pio_h: pico_extras/src/rp2_common/pico_sd_card/CMakeFiles/pico_sd_card_sd_card_pio_h.dir/build.make

.PHONY : pico_sd_card_sd_card_pio_h

# Rule to build all files generated by this target.
pico_extras/src/rp2_common/pico_sd_card/CMakeFiles/pico_sd_card_sd_card_pio_h.dir/build: pico_sd_card_sd_card_pio_h

.PHONY : pico_extras/src/rp2_common/pico_sd_card/CMakeFiles/pico_sd_card_sd_card_pio_h.dir/build

pico_extras/src/rp2_common/pico_sd_card/CMakeFiles/pico_sd_card_sd_card_pio_h.dir/clean:
	cd /home/mdietric/flypicopi/src/build/pico_extras/src/rp2_common/pico_sd_card && $(CMAKE_COMMAND) -P CMakeFiles/pico_sd_card_sd_card_pio_h.dir/cmake_clean.cmake
.PHONY : pico_extras/src/rp2_common/pico_sd_card/CMakeFiles/pico_sd_card_sd_card_pio_h.dir/clean

pico_extras/src/rp2_common/pico_sd_card/CMakeFiles/pico_sd_card_sd_card_pio_h.dir/depend:
	cd /home/mdietric/flypicopi/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mdietric/flypicopi/src /home/mdietric/flypicopi/src/pico-extras/src/rp2_common/pico_sd_card /home/mdietric/flypicopi/src/build /home/mdietric/flypicopi/src/build/pico_extras/src/rp2_common/pico_sd_card /home/mdietric/flypicopi/src/build/pico_extras/src/rp2_common/pico_sd_card/CMakeFiles/pico_sd_card_sd_card_pio_h.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : pico_extras/src/rp2_common/pico_sd_card/CMakeFiles/pico_sd_card_sd_card_pio_h.dir/depend

