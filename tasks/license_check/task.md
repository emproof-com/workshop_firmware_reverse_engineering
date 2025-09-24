# Task

Paid features often rely on a license check: the program validates user input to decide whether premium functionality is allowed. In this exercise, your goal is to understand how the check works and either recover the valid serial or bypass the check. We’ll discuss multiple routes to reach that goal.

1. Use `strings` to see if anything interesting is embedded in the binary.  
   If you spot a candidate license key, try it and observe the program’s behavior.

It’s not always that simple. A common workflow is to load the binary in an analysis tool (e.g., **Ghidra** or **Binary Ninja**), navigate to an informative string (e.g., “Access granted!”), follow cross-references into code, and study the control flow to locate the comparison. From there, work backward and forward to understand what must hold for the success path.

2. Do this workflow in **Ghidra**.  
   Use strings/xrefs to reach the check, inspect the code around it, and note the condition that leads to acceptance.

3. Repeat the same workflow in **Binary Ninja**.  
   Navigate from data to code, identify the comparison, and map the path that results in “Access granted!”. Note: Use the `*.armv7` for Binary Ninja Free; the free version doesn’t support AArch64.

Another route is **patching**: modify the executable so the check always succeeds. This typically means neutralizing a conditional check or forcing the validator to return success. Save the patched binary and confirm that the premium feature unlocks.

4. Apply a minimal patch in **Ghidra** to unlock the premium feature.  
   Verify the result.

5. Repeat the patching approach in **Binary Ninja**.  
   Verify that the program now always accepts. Note: Use the `*.armv7` for Binary Ninja Free; the free version doesn’t support AArch64.