# Task

The Mirai botnet is embedded malware targeting routers, IP cameras, and other IoT devices. It spreads by brute-forcing default usernames/passwords and, on success, copies itself to the remote device. For this it keeps a hardcoded list of encrypted credentials that are decrypted at runtime by a string-decryption routine. Its source code has been [published on GitHub]( https://github.com/jgamblin/Mirai-Source-Code).

In this task we’ll explore a more complex binary and look for interesting code constructs such as string-decryption routines. Then we’ll analyze the routine and learn how to **statically** decrypt some strings.

Note: This is active malware. **Do not execute** the sample (it runs on ARM32 Linux). To avoid AV detection/quarantine, the archive is password-protected with "infected". Unpack it manually or use the provided script [`samples/unzip.sh`](samples/unzip.sh).

1. Open the binary in Ghidra or Binary Ninja and try to locate the string-decryption function.  
   Hint: look for weird string blobs and follow cross-references from decryption call sites.

2. Analyze the string-decryption routine. How does it work?

3. Write a small decryption script and recover a subset of the strings.

4. What do you learn from the decrypted strings? What patterns or artifacts do you observe?

5. Explore other parts of the binary. What else can you infer from code, data, or references?