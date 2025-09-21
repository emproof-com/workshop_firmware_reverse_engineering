# Task

Sometimes we encounter binaries that perform symmetric encryption/decryption with a hardcoded key. With reverse engineering we can extract that key by locating the crypto routine, identifying the key/IV parameters, and backtracking where they come from (often `.rodata`). In this task you will do exactly that on an dynamically-linked Linux binary.

1. Familiarize yourself with the binary. Run it on the provided ciphertext and verify the output file: `./aes_detect --in secret.bin --out plain.txt`

2. Open the binary in Ghidra or Binary Ninja and locate the AES decryption call (EVP AES-128-CBC). Identify the function that performs the decryption.

3. Backtrack the parameters passed to the decryption call. Recover the **IV** and the **key**, and note where they originate (address/section).

4. Verify your findings by decrypting manually with OpenSSL: `openssl enc -aes-128-cbc -d -K <16-byte-hex-key> -iv <16-byte-hex-iv> -in secret.bin -out plain_decrypted.txt`