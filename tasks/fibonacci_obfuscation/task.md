# Task

Without additional protections, analyzing and understanding algorithms in binaries is often not very hard—especially with decompilers. To increase analysis effort, code obfuscation techniques rewrite the syntax of a program so it’s harder to follow, while preserving the same semantics. Common techniques include obscuring control flow, hiding computations behind algebraic identities, and adding extra analysis layers that must be peeled away step by step.

In this exercise you’ll see a clean Fibonacci implementation and several obfuscated variants. Your goal is to examine each style, understand its impact on analysis, and explain what’s really going on.

1. Open `fib_obfuscated` in Ghidra or Binary Ninja. Look at `fib_ref`. What does it do? Note: Use the `*.armv7` for Binary Ninja Free; the free version doesn’t support AArch64.

2. Analyze `fib_flat_switch`. How does it work, and what does it compute? How would you "bypass" the obfuscation (i.e., simplify the control flow back to a normal loop/pseudocode)?

3. Analyze `fib_flat_goto`. How does the computed-goto dispatcher work, and what does it compute? How would you de-flatten or otherwise normalize the control flow?

4. Analyze `fib_opaque` (the opaque-predicate variant). What makes certain branches always true/false, and how can you simplify them away?

5. Analyze the arithmetic/bitwise obfuscation in `fib_arithmetic` (e.g., using `s = (a ^ b) + ((a & b) << 1)` instead of `s = a + b`). Where is this used, and how would you normalize it back to standard arithmetic?
   
6. Analyze `fib_vm`.  How does it work, and what does it compute? How would you "bypass" the obfuscation?
