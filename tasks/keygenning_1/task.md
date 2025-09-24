# Task 

In most real-world license checks, the serial isn’t hardcoded; it’s validated at runtime by an algorithm (e.g., checksums, simple math properties, lightweight ciphers). In the cracking scene, one typically writes a keygen that, for a given username, computes a matching serial. 
In this task we practice keygenning on a small sample that checks a username/serial pair.

1. Run the program with a few username/serial pairs to get a feeling for its behavior.

2. Reverse engineer the binary (Ghidra or Binary Ninja) and understand how the check works. Note: Use the `*.armv7` for Binary Ninja Free; the free version doesn’t support AArch64.

3. Find a valid serial for the username "procracker".

4. Write a small keygen (e.g., in Python) that, given a username, outputs a valid serial such that the program prints `License accepted!`.

5. Verify by running the program with your generated serial.