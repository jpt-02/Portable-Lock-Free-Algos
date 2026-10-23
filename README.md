# Portable-Lock-Free-Algos
Portable, simple, copy+pastable lock-free algorithms written in C++. Algos are added here as I write them for other projects.

## How to Interact with this Repo

The purpose of this repo is to be copy+pasted from. Each algorithm fits in a single file that uses only C++ standard library headers, so if you want to use one for your project, just take the file you want.

Files are stored in the `algos` folder.

### Why do it this way?

Contained here are a number of lock-free algorithms that are useful for making programs as fast as possible. None of these algorithms are new or original, but many of them:
- Are hard to find on the internet as standalone files that make sense to newcomers
- *Can* be found online, but do not work on any system (work only on Linux, for example)
- Come bundled in packages that have large file sizes

You will likely find this repo useful if:
- You are rapidly prototyping something
- Whatever you are making needs to be as portable as possible
- You are a moron (like me)

Otherwise, you are definitely better off using a more popular library like Folly or Liburcu.

## Testing

Each algorithm is tested for race conditions using Relacy. The test files are in `algos_relacy` folder. 

> **Note:** LLMs are god awful at memory ordering. They will tell you that working code has race conditions, and that buggy code is perfectly fine. Lock-free programming is, in my experience, best done and tested by hand.

### Saving Output

VScode can truncate Relacy's output in the "Test Results" tab, so if you want to save it to a file just run `.\build\01_test.exe *> 01_output.txt`.

## List of Algos

### 01 - TripleBuffer

- File: `01_triplebuffer.hpp`
- Triple Buffer algorithm that uses a dirty bit to swap buffers.
- SPSC (Single Producer, Single Consumer)
- **Relies on uintptr_t for the dirty bit logic**, which may not be available on some microcontrollers. Should be perfectly portable in any user-space application, though.
- References: https://github.com/brilliantsugar/trio/tree/main

| Methods | |
| :--- | :--- |
`read()` | Lock-free if atomics on host machine are lock-free <br> Wait-free if atomics on host machine are wait-free
`get_write()` | Always lock-free <br> Always wait-free
`commit_write()` | Lock-free if atomics on host machine are lock-free <br> Wait-free if atomics on host machine are wait-free

More coming soon!
