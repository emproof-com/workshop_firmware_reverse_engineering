# Task

Often, we can extract a lot of information from binaries **before** looking at any code. In the following, we'll use common command-line tools to extract basic characteristics from ELF files and reason about program behavior. Later on, we'll jump into some basic reverse engineering tools and become comfortable with their workflows.

1. Which CPU architecture (including bitness and endianness) is the binary compiled for? Use `file hello_world` to find out.

2. Which human-readable strings does the binary contain, and what do they reveal about expected inputs or behavior? Use `strings -a hello_world` to find out.

3. Which API functions does the program import from shared libraries? Use `readelf -Ws hello_world` to find out. *(Alternatively, `objdump -T hello_world`.)*

4. Is the binary dynamically linked, and which dynamic loader will be used at runtime? Use `readelf -l hello_world` and look for the `INTERP` segment to find out.

5. Is the file a PIE (ET_DYN) or a non-PIE executable (ET_EXEC)? Use `readelf -h hello_world` to find out.

6. What is the program’s entry point address? Use `readelf -h hello_world` to find out.

7. Where are the dynamic symbol and string tables located, and can you directly see import names in the file? Use `readelf -S hello_world` to get the offsets of `.dynsym`/`.dynstr`, then verify with `xxd -s 0x<OFFSET> -l 256 hello_world` to find out.

8. Which shared libraries will be loaded at runtime? Use `readelf -d hello_world` and list the needed entries to find out.

9. Are there hints about the compiler/toolchain or build metadata (e.g., GCC/Clang version, build ID)? Use `readelf -p .comment hello_world` and `strings hello_world | grep -i -E 'gcc|clang|build|version'` to find out.

10. Optional: Does the binary include usage/help text or format strings that reveal argument order or output formatting? Use `strings -a hello_world | grep -i -E 'usage|error|%d|%s'` to find out.

At this point we’ve learned a lot from metadata alone. As a next step, let’s peek at the actual code: first `main`, then `compute and let's try to understand what's going on.

11. Show the disassembly for `main`. Use `objdump -d --disassemble=main hello_world`. Tip: verify that `main` is present with `readelf -Ws hello_world | grep ' main$'`. What do you see there?

12. Show the disassembly for `compute`. Use `objdump -d --disassemble=compute hello_world`. Tip: verify that `compute` is present with `readelf -Ws hello_world | grep ' compute$'`. What do you see there?

So far we assumed that **symbols** were available. Now we'll strip them away. After stripping, name-based disassembly (e.g., `--disassemble=main`) will fail because the names are gone. Next, we’ll see how to disassemble **without** symbols.

13. Remove symbols and observe the effect.  
    - Make a copy and strip it:  
      `cp hello_world hello_world_stripped && strip hello_world_stripped`  
    - Compare symbol tables before/after:  
      `readelf -Ws hello_world` vs. `readelf -Ws hello_world_stripped`  
    - Try name-based disassembly and note the failure:  
      `objdump -d --disassemble=main hello_world_stripped`  
    
    What changed, and why does disassembling by function name fail now?

14. Disassemble without symbols. Dump just the code section to browse functions by sight:  
      `objdump -d -j .text hello_world_stripped | less`  
      Show the addresses of the regions you believe correspond to `main` and `compute`, and briefly justify your choice (e.g., nearby `atoi@plt` / `printf@plt` for `main`.

We’ve now seen how `file`, `strings`, `readelf`, and `objdump` already reveal a lot. Next, let’s look at unified reverse-engineering tools that streamline these workflows. We’ll start with Ghidra and then do the same in Binary Ninja. Both provide powerful navigation, cross-references, and (where available) decompiler views that make reasoning about code much faster.


15. Get comfortable with Ghidra. Open the binary `hello_world` (or `hello_world_stripped`) and analyze it.
    - Import into a new project and enable analysis (Language: AARCH64:LE:64).
    - Browse strings, imports and functions; open `main` and `compute`.
    - Use the decompiler.
  
    What does `main` do, and where is `compute` called? Compare the decompiler output to the source code.

16. Do the same in Binary Ninja. Open the same binary and let it analyze.
    - Inspect the API imports, strings and functions. Navigate to `main` / `compute`.
    - Switch to a higher-level view (IL/HLIL)/decompiler.
  
    What does `main` do, and where is `compute` called? Compare the decompiler output to the source code. Compare the workflow to Ghidra.

    Note: Use the `*.armv7` for Binary Ninja Free; the free version doesn’t support AArch64.