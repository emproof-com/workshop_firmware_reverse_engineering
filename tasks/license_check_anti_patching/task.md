# Task

Paid features often rely on a license check: the program validates user input to decide whether premium functionality is allowed. Bypassing a license check can be trivial—an attacker may patch a conditional jump so execution always goes to the "success" path, ignoring the underlying validation.

To raise the bar against such patches, a technique called code checksumming can be used. At runtime, the program computes a cryptographic (or simple) checksum over a protected code region and compares it to a constant. If the code was modified, the checksum no longer matches and the program aborts. In essence: `if compute_checksum(code_start, size) != EXPECTED_CHECKSUM { exit() }`.

In this exercise, you’ll inspect such a check and try to bypass it.

1. Open `license_check_crc` in Ghidra or Binary Ninja. Analyze the license check: how does it work? Note: Use the `*.armv7` for Binary Ninja Free; the free version doesn’t support AArch64.

2. Patch the binary in `main` so it always jumps to “Access granted!”. Run the binary. What happens?

3. Analyze the code checksumming logic. How is the checksum computed, which region is covered, and where is the expected value compared?

4. Find a way to bypass the checksum (e.g., relocate/neutralize the guard, adjust the constant, or move your patch outside the guarded region). Patch the binary accordingly and test again.

