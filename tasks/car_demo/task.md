# Task

Not all firmware runs on embedded Linux. Quite often you’ll face bare-metal images: no OS, no filesystem—just a single binary where startup code, drivers, and application logic live side by side. In these cases, the first hurdle is understanding the memory map—which ranges belong to flash, SRAM, data-flash/option bytes, or peripheral windows. Because these addresses are hardware-specific and shaped by the project’s linker script, you should consult the vendor’s documentation (datasheet/reference manual) and then cross-check your findings against the ELF’s sections and load segments. 

Even without a filesystem, such firmware images can still hide secrets (hard-coded passwords, keys, endpoints) that become recoverable once the layout is clear. In this exercise, the goal is to extract as much as possible from a bare-metal ELF starting with minimal prior knowledge.

The file `car_demo.elf` contains the firmware of a remote-controlled model car. Reverse engineer the binary and answer the questions below.

1. Which CPU architecture is the firmware compiled for?

2. Which IDE / compiler tool-chain was used to build the firmware?

3. Which microcontroller is the firmware intended to run on? Hint: Use clues in the binary and verify them with online research about the hardware components.

4. From the ELF sections/segments, infer addresses/sizes for `.data`, `.bss`, heap/stack, vector table, and any device-specific flash areas (e.g., data-flash/option bytes). Then cross-check your inference against the official memory map of the target MCU/board (datasheet/reference manual). 

5. Determine the date (and, if present, the time) the firmware was most likely compiled.  Point to the exact location in the binary where you obtained the value.

6. The firmware joins a Wi-Fi network automatically. Extract the SSID, and the password / pre-shared key and show where in the code those literals are passed to the Wi-Fi driver.

7. The code contacts a fixed server to fetch updates. Identify the IP address, the port number, and the application-layer protocol. Then name the function that performs the connection and describe its purpose in one sentence.

8. Reconstruct one HTTP request (method, path, and likely headers) based on the code and strings.
