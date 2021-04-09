# The set of languages for which implicit dependencies are needed:
set(CMAKE_DEPENDS_LANGUAGES
  "ASM"
  "C"
  "CXX"
  )
# The set of files for implicit dependencies of each language:
set(CMAKE_DEPENDS_CHECK_ASM
  "/home/mdietric/flypicopi/src/pico-extras/src/common/pico_audio/audio_utils.S" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-extras/src/common/pico_audio/audio_utils.S.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_divider/divider.S" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_divider/divider.S.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_irq/irq_handler_chain.S.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_bit_ops/bit_ops_aeabi.S.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_divider/divider.S" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_divider/divider.S.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_double/double_aeabi.S" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_double/double_aeabi.S.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_double/double_v1_rom_shim.S.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_float/float_aeabi.S" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_float/float_aeabi.S.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_float/float_v1_rom_shim.S.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_int64_ops/pico_int64_ops_aeabi.S.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_mem_ops/mem_ops_aeabi.S.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_standard_link/crt0.S" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_standard_link/crt0.S.obj"
  )

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_ASM
  "CFG_TUD_AUDIO=0"
  "CFG_TUD_MIDI=1"
  "CFG_TUD_MIDI_RX_BUFSIZE=64"
  "CFG_TUD_MIDI_TX_BUFSIZE=64"
  "CFG_TUSB_DEBUG=1"
  "CFG_TUSB_MCU=OPT_MCU_RP2040"
  "CFG_TUSB_OS=OPT_OS_PICO"
  "PICO_AUDIO_I2S_MONO_INPUT=0"
  "PICO_AUDIO_I2S_MONO_OUTPUT=0"
  "PICO_BIT_OPS_PICO=1"
  "PICO_BOARD=\"pico\""
  "PICO_BOOT2_NAME=\"boot2_w25q080\""
  "PICO_BUILD=1"
  "PICO_CMAKE_BUILD_TYPE=\"Debug\""
  "PICO_COPY_TO_RAM=0"
  "PICO_CXX_ENABLE_EXCEPTIONS=0"
  "PICO_DIVIDER_HARDWARE=1"
  "PICO_DOUBLE_PICO=1"
  "PICO_EXTRAS=1"
  "PICO_FLOAT_PICO=1"
  "PICO_INT64_OPS_PICO=1"
  "PICO_MEM_OPS_PICO=1"
  "PICO_MULTICORE=1"
  "PICO_NO_FLASH=0"
  "PICO_NO_HARDWARE=0"
  "PICO_ON_DEVICE=1"
  "PICO_PRINTF_PICO=1"
  "PICO_STDIO_USB=1"
  "PICO_TARGET_NAME=\"FlyPicoPi\""
  "PICO_USE_BLOCKED_RAM=0"
  "USE_AUDIO_I2S=1"
  )

# The include file search paths:
set(CMAKE_ASM_TARGET_INCLUDE_PATH
  "../pico-sdk/src/common/pico_stdlib/include"
  "../pico-sdk/src/rp2_common/hardware_gpio/include"
  "../pico-sdk/src/common/pico_base/include"
  "generated/pico_base"
  "../pico-sdk/src/boards/include"
  "../pico-sdk/src/rp2_common/pico_platform/include"
  "../pico-sdk/src/rp2040/hardware_regs/include"
  "../pico-sdk/src/rp2_common/hardware_base/include"
  "../pico-sdk/src/rp2040/hardware_structs/include"
  "../pico-sdk/src/rp2_common/hardware_claim/include"
  "../pico-sdk/src/rp2_common/hardware_sync/include"
  "../pico-sdk/src/rp2_common/hardware_uart/include"
  "../pico-sdk/src/rp2_common/hardware_divider/include"
  "../pico-sdk/src/common/pico_time/include"
  "../pico-sdk/src/rp2_common/hardware_timer/include"
  "../pico-sdk/src/common/pico_sync/include"
  "../pico-sdk/src/common/pico_util/include"
  "../pico-sdk/src/rp2_common/pico_runtime/include"
  "../pico-sdk/src/rp2_common/hardware_clocks/include"
  "../pico-sdk/src/rp2_common/hardware_resets/include"
  "../pico-sdk/src/rp2_common/hardware_watchdog/include"
  "../pico-sdk/src/rp2_common/hardware_xosc/include"
  "../pico-sdk/src/rp2_common/hardware_pll/include"
  "../pico-sdk/src/rp2_common/hardware_vreg/include"
  "../pico-sdk/src/rp2_common/hardware_irq/include"
  "../pico-sdk/src/rp2_common/pico_printf/include"
  "../pico-sdk/src/rp2_common/pico_bootrom/include"
  "../pico-sdk/src/common/pico_bit_ops/include"
  "../pico-sdk/src/common/pico_divider/include"
  "../pico-sdk/src/rp2_common/pico_double/include"
  "../pico-sdk/src/rp2_common/pico_int64_ops/include"
  "../pico-sdk/src/rp2_common/pico_float/include"
  "../pico-sdk/src/rp2_common/pico_malloc/include"
  "../pico-sdk/src/common/pico_binary_info/include"
  "../pico-sdk/src/rp2_common/pico_stdio/include"
  "../pico-sdk/src/rp2_common/pico_stdio_usb/include"
  "../pico-sdk/lib/tinyusb/src"
  "../pico-sdk/lib/tinyusb/src/common"
  "../pico-sdk/lib/tinyusb/hw"
  "../pico-sdk/src/rp2_common/pico_fix/rp2040_usb_device_enumeration/include"
  "pico_extras/src/rp2_common/pico_audio_i2s"
  "../pico-extras/src/rp2_common/pico_audio_i2s/include"
  "../pico-sdk/src/rp2_common/hardware_dma/include"
  "../pico-sdk/src/rp2_common/hardware_pio/include"
  "../pico-extras/src/common/pico_audio/include"
  "../pico-extras/src/common/pico_util_buffer/include"
  "pico_extras/src/rp2_common/pico_sd_card"
  "../pico-extras/src/rp2_common/pico_sd_card/include"
  "../pico-extras/src/common/pico_sd_card/include"
  "../pico-sdk/src/rp2_common/pico_multicore/include"
  "../pico-sdk/src/rp2_common/hardware_flash/include"
  "../pico-sdk/src/rp2_common/hardware_i2c/include"
  "../pico-sdk/src/rp2_common/hardware_adc/include"
  )
set(CMAKE_DEPENDS_CHECK_C
  "/home/mdietric/flypicopi/src/pico-extras/src/common/pico_util_buffer/buffer.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-extras/src/common/pico_util_buffer/buffer.c.obj"
  "/home/mdietric/flypicopi/src/pico-extras/src/rp2_common/pico_audio_i2s/audio_i2s.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-extras/src/rp2_common/pico_audio_i2s/audio_i2s.c.obj"
  "/home/mdietric/flypicopi/src/pico-extras/src/rp2_common/pico_sd_card/sd_card.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-extras/src/rp2_common/pico_sd_card/sd_card.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/class/audio/audio_device.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/class/audio/audio_device.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/class/cdc/cdc_device.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/class/cdc/cdc_device.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/class/dfu/dfu_rt_device.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/class/dfu/dfu_rt_device.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/class/hid/hid_device.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/class/hid/hid_device.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/class/midi/midi_device.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/class/midi/midi_device.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/class/msc/msc_device.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/class/msc/msc_device.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/class/net/net_device.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/class/net/net_device.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/class/usbtmc/usbtmc_device.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/class/usbtmc/usbtmc_device.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/class/vendor/vendor_device.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/class/vendor/vendor_device.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/common/tusb_fifo.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/common/tusb_fifo.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/device/usbd.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/device/usbd.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/device/usbd_control.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/device/usbd_control.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/portable/raspberrypi/rp2040/dcd_rp2040.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/portable/raspberrypi/rp2040/dcd_rp2040.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/portable/raspberrypi/rp2040/rp2040_usb.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/portable/raspberrypi/rp2040/rp2040_usb.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/lib/tinyusb/src/tusb.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/lib/tinyusb/src/tusb.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/common/pico_sync/critical_section.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/common/pico_sync/critical_section.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/common/pico_sync/lock_core.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/common/pico_sync/lock_core.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/common/pico_sync/mutex.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/common/pico_sync/mutex.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/common/pico_sync/sem.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/common/pico_sync/sem.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/common/pico_time/time.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/common/pico_time/time.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/common/pico_time/timeout_helper.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/common/pico_time/timeout_helper.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/common/pico_util/datetime.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/common/pico_util/datetime.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/common/pico_util/pheap.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/common/pico_util/pheap.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/common/pico_util/queue.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/common/pico_util/queue.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_adc/adc.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_adc/adc.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_claim/claim.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_claim/claim.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_clocks/clocks.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_clocks/clocks.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_dma/dma.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_dma/dma.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_flash/flash.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_flash/flash.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_gpio/gpio.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_gpio/gpio.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_i2c/i2c.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_i2c/i2c.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_irq/irq.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_irq/irq.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_pio/pio.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_pio/pio.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_pll/pll.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_pll/pll.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_sync/sync.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_sync/sync.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_timer/timer.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_timer/timer.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_uart/uart.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_uart/uart.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_vreg/vreg.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_vreg/vreg.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_watchdog/watchdog.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/hardware_xosc/xosc.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/hardware_xosc/xosc.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_bootrom/bootrom.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_double/double_init_rom.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_double/double_init_rom.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_double/double_math.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_double/double_math.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_fix/rp2040_usb_device_enumeration/rp2040_usb_device_enumeration.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_fix/rp2040_usb_device_enumeration/rp2040_usb_device_enumeration.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_float/float_init_rom.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_float/float_init_rom.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_float/float_math.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_float/float_math.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_malloc/pico_malloc.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_multicore/multicore.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_multicore/multicore.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_platform/platform.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_platform/platform.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_printf/printf.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_printf/printf.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_runtime/runtime.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_runtime/runtime.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_standard_link/binary_info.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_stdio/stdio.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_stdio/stdio.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_stdio_usb/reset_interface.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_stdio_usb/reset_interface.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_stdio_usb/stdio_usb.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_stdio_usb/stdio_usb.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_stdio_usb/stdio_usb_descriptors.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_stdio_usb/stdio_usb_descriptors.c.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_stdlib/stdlib.c.obj"
  )
set(CMAKE_C_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_C
  "CFG_TUD_AUDIO=0"
  "CFG_TUD_MIDI=1"
  "CFG_TUD_MIDI_RX_BUFSIZE=64"
  "CFG_TUD_MIDI_TX_BUFSIZE=64"
  "CFG_TUSB_DEBUG=1"
  "CFG_TUSB_MCU=OPT_MCU_RP2040"
  "CFG_TUSB_OS=OPT_OS_PICO"
  "PICO_AUDIO_I2S_MONO_INPUT=0"
  "PICO_AUDIO_I2S_MONO_OUTPUT=0"
  "PICO_BIT_OPS_PICO=1"
  "PICO_BOARD=\"pico\""
  "PICO_BOOT2_NAME=\"boot2_w25q080\""
  "PICO_BUILD=1"
  "PICO_CMAKE_BUILD_TYPE=\"Debug\""
  "PICO_COPY_TO_RAM=0"
  "PICO_CXX_ENABLE_EXCEPTIONS=0"
  "PICO_DIVIDER_HARDWARE=1"
  "PICO_DOUBLE_PICO=1"
  "PICO_EXTRAS=1"
  "PICO_FLOAT_PICO=1"
  "PICO_INT64_OPS_PICO=1"
  "PICO_MEM_OPS_PICO=1"
  "PICO_MULTICORE=1"
  "PICO_NO_FLASH=0"
  "PICO_NO_HARDWARE=0"
  "PICO_ON_DEVICE=1"
  "PICO_PRINTF_PICO=1"
  "PICO_STDIO_USB=1"
  "PICO_TARGET_NAME=\"FlyPicoPi\""
  "PICO_USE_BLOCKED_RAM=0"
  "USE_AUDIO_I2S=1"
  )

# The include file search paths:
set(CMAKE_C_TARGET_INCLUDE_PATH
  "../pico-sdk/src/common/pico_stdlib/include"
  "../pico-sdk/src/rp2_common/hardware_gpio/include"
  "../pico-sdk/src/common/pico_base/include"
  "generated/pico_base"
  "../pico-sdk/src/boards/include"
  "../pico-sdk/src/rp2_common/pico_platform/include"
  "../pico-sdk/src/rp2040/hardware_regs/include"
  "../pico-sdk/src/rp2_common/hardware_base/include"
  "../pico-sdk/src/rp2040/hardware_structs/include"
  "../pico-sdk/src/rp2_common/hardware_claim/include"
  "../pico-sdk/src/rp2_common/hardware_sync/include"
  "../pico-sdk/src/rp2_common/hardware_uart/include"
  "../pico-sdk/src/rp2_common/hardware_divider/include"
  "../pico-sdk/src/common/pico_time/include"
  "../pico-sdk/src/rp2_common/hardware_timer/include"
  "../pico-sdk/src/common/pico_sync/include"
  "../pico-sdk/src/common/pico_util/include"
  "../pico-sdk/src/rp2_common/pico_runtime/include"
  "../pico-sdk/src/rp2_common/hardware_clocks/include"
  "../pico-sdk/src/rp2_common/hardware_resets/include"
  "../pico-sdk/src/rp2_common/hardware_watchdog/include"
  "../pico-sdk/src/rp2_common/hardware_xosc/include"
  "../pico-sdk/src/rp2_common/hardware_pll/include"
  "../pico-sdk/src/rp2_common/hardware_vreg/include"
  "../pico-sdk/src/rp2_common/hardware_irq/include"
  "../pico-sdk/src/rp2_common/pico_printf/include"
  "../pico-sdk/src/rp2_common/pico_bootrom/include"
  "../pico-sdk/src/common/pico_bit_ops/include"
  "../pico-sdk/src/common/pico_divider/include"
  "../pico-sdk/src/rp2_common/pico_double/include"
  "../pico-sdk/src/rp2_common/pico_int64_ops/include"
  "../pico-sdk/src/rp2_common/pico_float/include"
  "../pico-sdk/src/rp2_common/pico_malloc/include"
  "../pico-sdk/src/common/pico_binary_info/include"
  "../pico-sdk/src/rp2_common/pico_stdio/include"
  "../pico-sdk/src/rp2_common/pico_stdio_usb/include"
  "../pico-sdk/lib/tinyusb/src"
  "../pico-sdk/lib/tinyusb/src/common"
  "../pico-sdk/lib/tinyusb/hw"
  "../pico-sdk/src/rp2_common/pico_fix/rp2040_usb_device_enumeration/include"
  "pico_extras/src/rp2_common/pico_audio_i2s"
  "../pico-extras/src/rp2_common/pico_audio_i2s/include"
  "../pico-sdk/src/rp2_common/hardware_dma/include"
  "../pico-sdk/src/rp2_common/hardware_pio/include"
  "../pico-extras/src/common/pico_audio/include"
  "../pico-extras/src/common/pico_util_buffer/include"
  "pico_extras/src/rp2_common/pico_sd_card"
  "../pico-extras/src/rp2_common/pico_sd_card/include"
  "../pico-extras/src/common/pico_sd_card/include"
  "../pico-sdk/src/rp2_common/pico_multicore/include"
  "../pico-sdk/src/rp2_common/hardware_flash/include"
  "../pico-sdk/src/rp2_common/hardware_i2c/include"
  "../pico-sdk/src/rp2_common/hardware_adc/include"
  )
set(CMAKE_DEPENDS_CHECK_CXX
  "/home/mdietric/flypicopi/src/font.cc" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/font.cc.obj"
  "/home/mdietric/flypicopi/src/main.cpp" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/main.cpp.obj"
  "/home/mdietric/flypicopi/src/oled.cc" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/oled.cc.obj"
  "/home/mdietric/flypicopi/src/pico-extras/src/common/pico_audio/audio.cpp" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-extras/src/common/pico_audio/audio.cpp.obj"
  "/home/mdietric/flypicopi/src/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/pico-sdk/src/rp2_common/pico_standard_link/new_delete.cpp.obj"
  "/home/mdietric/flypicopi/src/splash.cc" "/home/mdietric/flypicopi/src/build/CMakeFiles/FlyPicoPi.dir/splash.cc.obj"
  )
set(CMAKE_CXX_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
set(CMAKE_TARGET_DEFINITIONS_CXX
  "CFG_TUD_AUDIO=0"
  "CFG_TUD_MIDI=1"
  "CFG_TUD_MIDI_RX_BUFSIZE=64"
  "CFG_TUD_MIDI_TX_BUFSIZE=64"
  "CFG_TUSB_DEBUG=1"
  "CFG_TUSB_MCU=OPT_MCU_RP2040"
  "CFG_TUSB_OS=OPT_OS_PICO"
  "PICO_AUDIO_I2S_MONO_INPUT=0"
  "PICO_AUDIO_I2S_MONO_OUTPUT=0"
  "PICO_BIT_OPS_PICO=1"
  "PICO_BOARD=\"pico\""
  "PICO_BOOT2_NAME=\"boot2_w25q080\""
  "PICO_BUILD=1"
  "PICO_CMAKE_BUILD_TYPE=\"Debug\""
  "PICO_COPY_TO_RAM=0"
  "PICO_CXX_ENABLE_EXCEPTIONS=0"
  "PICO_DIVIDER_HARDWARE=1"
  "PICO_DOUBLE_PICO=1"
  "PICO_EXTRAS=1"
  "PICO_FLOAT_PICO=1"
  "PICO_INT64_OPS_PICO=1"
  "PICO_MEM_OPS_PICO=1"
  "PICO_MULTICORE=1"
  "PICO_NO_FLASH=0"
  "PICO_NO_HARDWARE=0"
  "PICO_ON_DEVICE=1"
  "PICO_PRINTF_PICO=1"
  "PICO_STDIO_USB=1"
  "PICO_TARGET_NAME=\"FlyPicoPi\""
  "PICO_USE_BLOCKED_RAM=0"
  "USE_AUDIO_I2S=1"
  )

# The include file search paths:
set(CMAKE_CXX_TARGET_INCLUDE_PATH
  "../pico-sdk/src/common/pico_stdlib/include"
  "../pico-sdk/src/rp2_common/hardware_gpio/include"
  "../pico-sdk/src/common/pico_base/include"
  "generated/pico_base"
  "../pico-sdk/src/boards/include"
  "../pico-sdk/src/rp2_common/pico_platform/include"
  "../pico-sdk/src/rp2040/hardware_regs/include"
  "../pico-sdk/src/rp2_common/hardware_base/include"
  "../pico-sdk/src/rp2040/hardware_structs/include"
  "../pico-sdk/src/rp2_common/hardware_claim/include"
  "../pico-sdk/src/rp2_common/hardware_sync/include"
  "../pico-sdk/src/rp2_common/hardware_uart/include"
  "../pico-sdk/src/rp2_common/hardware_divider/include"
  "../pico-sdk/src/common/pico_time/include"
  "../pico-sdk/src/rp2_common/hardware_timer/include"
  "../pico-sdk/src/common/pico_sync/include"
  "../pico-sdk/src/common/pico_util/include"
  "../pico-sdk/src/rp2_common/pico_runtime/include"
  "../pico-sdk/src/rp2_common/hardware_clocks/include"
  "../pico-sdk/src/rp2_common/hardware_resets/include"
  "../pico-sdk/src/rp2_common/hardware_watchdog/include"
  "../pico-sdk/src/rp2_common/hardware_xosc/include"
  "../pico-sdk/src/rp2_common/hardware_pll/include"
  "../pico-sdk/src/rp2_common/hardware_vreg/include"
  "../pico-sdk/src/rp2_common/hardware_irq/include"
  "../pico-sdk/src/rp2_common/pico_printf/include"
  "../pico-sdk/src/rp2_common/pico_bootrom/include"
  "../pico-sdk/src/common/pico_bit_ops/include"
  "../pico-sdk/src/common/pico_divider/include"
  "../pico-sdk/src/rp2_common/pico_double/include"
  "../pico-sdk/src/rp2_common/pico_int64_ops/include"
  "../pico-sdk/src/rp2_common/pico_float/include"
  "../pico-sdk/src/rp2_common/pico_malloc/include"
  "../pico-sdk/src/common/pico_binary_info/include"
  "../pico-sdk/src/rp2_common/pico_stdio/include"
  "../pico-sdk/src/rp2_common/pico_stdio_usb/include"
  "../pico-sdk/lib/tinyusb/src"
  "../pico-sdk/lib/tinyusb/src/common"
  "../pico-sdk/lib/tinyusb/hw"
  "../pico-sdk/src/rp2_common/pico_fix/rp2040_usb_device_enumeration/include"
  "pico_extras/src/rp2_common/pico_audio_i2s"
  "../pico-extras/src/rp2_common/pico_audio_i2s/include"
  "../pico-sdk/src/rp2_common/hardware_dma/include"
  "../pico-sdk/src/rp2_common/hardware_pio/include"
  "../pico-extras/src/common/pico_audio/include"
  "../pico-extras/src/common/pico_util_buffer/include"
  "pico_extras/src/rp2_common/pico_sd_card"
  "../pico-extras/src/rp2_common/pico_sd_card/include"
  "../pico-extras/src/common/pico_sd_card/include"
  "../pico-sdk/src/rp2_common/pico_multicore/include"
  "../pico-sdk/src/rp2_common/hardware_flash/include"
  "../pico-sdk/src/rp2_common/hardware_i2c/include"
  "../pico-sdk/src/rp2_common/hardware_adc/include"
  )

# Targets to which this target links.
set(CMAKE_TARGET_LINKED_INFO_FILES
  )

# Fortran module output directory.
set(CMAKE_Fortran_TARGET_MODULE_DIR "")
