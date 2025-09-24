# Task

Malware often hides context-revealing strings to increase analysis effort. A common trick is to store strings in an encrypted form (e.g., XOR with a hardcoded key) so they are not visible during static analysis, but are decrypted at run-time by calling a string decryption function (e.g., `printf("%s", decrypt(enc))`). In this exercise you’ll learn to spot string-decryption routines and recover the plaintext without relying on execution.

1. Execute the binary once and observe how it behaves (inputs, outputs, notable messages).

2. Use `strings` on the binary and check whether those messages appear. Note any suspicious "gibberish" bytes or other interesting artifacts.

Even if we saw the plaintext at runtime, we often cannot (or do not want to) execute binaries, for example in the case of malware or a missing execution environment. Therefore, we now take a look how to statically deal with encrypted strings.

3. Open the binary in Ghidra or Binary Ninja and locate the string-decryption function (hint: a small routine called many times with byte arrays that look non-ASCII). Note: Use the `*.armv7` for Binary Ninja Free; the free version doesn’t support AArch64.

4. Reverse engineer the routine: how does it work?

5. Re-implement the routine (e.g., in Python) and use it to decrypt several encrypted strings from the binary.