# Portable-Lock-Free-Algos
Portable, simple, copy+pastable lock-free algorithms written in C++.

## How to Interact with this Repo

The purpose of this repo is to be copy+pasted from. Each algorithm fits in a single file that uses only C++ standard library headers, so if you want to use one for your project, just take the file you want.

Files are stored in `TODO: folder name`.

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

Each algorithm is tested for race conditions using Relacy. The test files are in `TODO: folder`. 

> **Important Note:** LLMs are god awful at memory ordering. They will tell you that working code has race conditions, and that buggy code is perfectly fine. Lock-free programming is, in my experience, best done and tested by hand.

## List of Algos

Coming soon!