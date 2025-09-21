# Task

Reverse engineering is not always about analyzing single binaries. Sometimes we get embedded Linux firmware images that contain entire file systems where additional data can be stored—source code (e.g., of websites or servers), configuration files, private SSH keys, VPN configs, and even standard Linux files containing login information.

In this task we’ll use [Firm-2](https://github.com/sajjadium/ctf-archives/tree/main/ctfs/WORMCON/2021/iot/Firm-2) from WORMCON CTF 2021 to learn how to analyze full firmware blobs. In this firmware, there is a hidden binary that needs to be found and analyzed. A step-by-step solution can be found [here](https://medium.com/@x3rz/iot-story-of-wormcon-0x01-60dd571c1675).

1. Analyze `Firm-2.bin` with the `file` command. What do you learn?

2. Use `binwalk Firm-2.bin` to scan the file and detect embedded data.

3. Extract the embedded data using `binwalk -e Firm-2.bin`.  
   Note: In the provided Docker environment, add `--run-as=root`.

4. In the extracted file system, there is a hidden executable (in an unusual location where you usually don’t find binaries). Find it.

5. Analyze the binary. What does it do?
