# Task

Reverse engineering is not always about analyzing single binaries. Sometimes we get embedded Linux firmware images that contain entire file systems where additional data can be stored—source code (e.g., of websites or servers), configuration files, private SSH keys, VPN configs, and even standard Linux files containing login information.

In this task we’ll use [Firm-1](https://github.com/sajjadium/ctf-archives/tree/main/ctfs/WORMCON/2021/iot/Firm-1) from Wormcon CTF 2021 to learn how to analyze full firmware blobs, find interesting data in the firmware, and finally crack the root password of an embedded system so that we could, for example, log in as root via SSH (assuming it’s open on the device). A step-by-step solution can be found [here](https://medium.com/@x3rz/iot-story-of-wormcon-0x01-60dd571c1675).

1. Analyze `Firm-1.bin` with the `file` command. What do you learn?

2. Use `binwalk Firm-1.bin` to scan the file and detect embedded data.

3. Extract the embedded data using `binwalk -e Firm-1.bin`. Note: In the provided docker add `--run-as=root`.

4. Inspect the extracted data. What can you find there?

While the CTF firmware may not contain many services, let’s assume it runs an SSH server and the SSH configuration (which we could read in `/etc/ssh/ssh_config`) allows password-based authentication for the root account. Our goal is to crack the root password so we can obtain credentials and log in via SSH. To do this, we need to locate the files that store user login credentials and figure out how to brute-force the password using a tool like [John the Ripper](https://en.wikipedia.org/wiki/John_the_Ripper) with a common-password dictionary.

5. Locate the `etc/passwd` file that lists all users of the Linux system and take a look at it.

6. Locate the `etc/shadow` file which contains password hashes of all Linux users.

7. Before cracking the password, preprocess the files:  
   `unshadow passwd shadow > unshadowed.txt`

8. Obtain and unpack a dictionary of common passwords, then inspect it:  
   `cp /usr/share/wordlists/rockyou.txt.gz . && gzip -d rockyou.txt.gz`

9. Crack the password with John the Ripper:  
   `john -w=rockyou.txt unshadowed.txt`
