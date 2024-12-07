# DarkStorm Base X64

A base for Team Fortress 2's 64-bit version, based off of the DarkStorm 2015 base. \
Original base can be found at [DarkStorm-2015-Base](https://bitbucket.org/gir489/darkstorm-2015-base/src/master/). \
This does not have any features other than some hooks and basic game stuff. 

---

## Usage

You need a 64-bit injector to be able to inject this.
To use this, just inject the DLL into tf_win64.exe.

## Building the project

### Prerequisites

Visual Studio 2022 ( Might work on older versions but tested with 2022 ).
CMake.

### Building with CMake

Open a command prompt in the directory that has CMakeLists.txt
Then run the following commands to build the project:

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Credits

[gir489](https://www.unknowncheats.me/forum/members/5420895.html) [old account](https://www.unknowncheats.me/forum/members/225272.html) - The posted / latest coder of the project.
Kalvin - according to gir489 coding the original DarkStorm 2.6.1 base in [this post](https://www.unknowncheats.me/forum/team-fortress-2-a/74558-tf2-darkstorm-3-4-1-a.html)
[TsudaKageyu](https://github.com/TsudaKageyu) - Coder of [minhook](https://github.com/TsudaKageyu/minhook).
