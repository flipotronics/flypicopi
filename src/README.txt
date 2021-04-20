
Succesfull Python Test for the SD Card in Circuit Python!

from board import *
from time import *
import busio
import sdcardio
import storage
    
sleep(1)
    
spi = busio.SPI(GP14, MOSI=GP15, MISO=GP12)
cs = GP13
sd = sdcardio.SDCard(spi, cs)
    
vfs = storage.VfsFat(sd)
storage.mount(vfs, '/sd')
    
with open("/sd/pico.txt", "w") as file:
    file.write("1. Hello, world!\r\n")
    
with open("/sd/pico.txt", "a") as file:
    file.write("2. This is another line!\r\n")
    
with open("/sd/pico.txt", "a") as file:
    file.write("3. Last but not least!")
    
with open("/sd/pico.txt", "r") as file:
    print("Printing lines in file:")
        for line in file:
        print(line, end='')
