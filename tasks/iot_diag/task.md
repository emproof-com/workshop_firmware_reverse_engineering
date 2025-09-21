# Task

One of the biggest hurdles with bare-metal firmware is the memory map. Without telling your tools where flash, SRAM, data-flash/option bytes, and peripheral windows live, they can’t reliably distinguish code from data, build cross-references, or resolve vector tables. The result is noisy disassembly and broken analysis. To avoid that, you must recreate the **hardware-specific layout** inside the tool before doing anything else.

These addresses are defined by the MCU and the project’s linker script, so start with the vendor docs (datasheet/reference manual), then cross-check against the image itself (vector table, plausible base addresses). For STM32F103 that typically means flash at `0x08000000`, SRAM at `0x20000000`, and the APB/AHB peripheral address space starting at `0x40000000` (GPIO, USART, RCC, DMA, ...). Additionally, the Cortex-M system control space (NVIC/SCB) is mapped at `0xE0000000`. Set the correct image base, confirm the initial stack pointer and reset handler, and only then proceed.

In this task, you’re given a raw flash dump for an STM32F103. The firmware prompts for a password and, on success, enables a diagnostics mode—i.e., a UART-based field diagnostics console for service technicians. Your goal is to recover the password and understand the control flow. However, before that, you’ll have to lay out memory correctly in your analysis tool; only then will strings, cross-refs, and functions become reliable.

1. Open `iot_diag.bin` in Ghidra or Binary Ninja. Check for interesting strings, code, and cross-references. What do you notice?

2. Rebase the binary (image base) to `0x08000000`. What do you notice?

3. Check the [STM32F103 datasheet](https://www.st.com/resource/en/datasheet/stm32f103c8.pdf) and note the base addresses for flash, SRAM, peripherals and SCS.

4. Define memory segments: SRAM at `0x20000000` size `0x5000` (R/W, no-exec), peripherals at `0x40000000` size `0x100000` (R/W, no-exec), SCS at `0xE0000000` size `0x10000` (R/W, no-exec).

5. Re-analyze the binary. What do you notice? Note: if Ghidra does not find anything, enable “ARM Aggressive Instruction Finder”.

6. Figure out the required password to enable the diagnostics mode.
