# Multithreaded-graph-coloring-implementation

This program is meant to be used in CLion IDE.
However, it is possible to generate the .exe on Windows 
through CMake

## Usage from cmd

Launch the following commands in the root folder

### Create build directory

```bash
mkdir build && cd build/
```

### Compile

```bash
cmake ..
```

### Create .exe

```bash
cmake --build . --config Release
```

### Run

```bash
./Release/${name}.exe
```
