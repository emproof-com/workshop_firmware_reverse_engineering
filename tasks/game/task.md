# Task

In this task we practice code understanding and cracking on a small number-guessing game. Your goal is to understand the game logic, recover the serial to unlock the full game, and then crack the game with a minimal patch.

1. Play the game and get a feeling for how it works (inputs, messages, trial vs full).

2. Open the game in your analysis tool (Ghidra or Binary Ninja) and reverse engineer the game logic (input handling, random number generation, attempt loop, success/failure flow). Note: Use the `*.armv7` for Binary Ninja Free; the free version doesn’t support AArch64.

3. Find the serial that unlocks the full game mode (locate where it is compared and recover the accepted value).

4. Patch the binary so the trial mode is more powerful (e.g., increase attempts from 3 to 10) and verify.

5. Crack the game so it always starts in full mode (modify the decision or force the check to succeed) and verify.
