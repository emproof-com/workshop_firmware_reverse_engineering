# Task

Many real-world license checks validate serials at runtime with schemes that are user-specific (the username participates in the computation) and also rely on hardcoded secrets (salts/keys). In this task we practice keygenning on a such sample that checks a username/serial pair.

1. Run the program with a few username/serial pairs to get a feeling for its behavior.

2. Reverse engineer the binary (Ghidra or Binary Ninja) and understand how the check works.

3. Find a valid serial for the username `procracker`.

4. Write a small keygen (e.g., in Python) that, given a username, outputs a valid serial such that the program prints `License accepted!`.

5. Verify by running the program with your generated serial.