# Multithreaded-graph-coloring-implementation


[![GitHub Build Status](https://github.com/team-diana/DIANA-TemplateCPP/workflows/C/C++%20CI/badge.svg)](https://github.com/team-diana/DIANA-TemplateCPP/actions)

Use this template for new repositories.

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
cmake ../src
```

### Create .exe

```bash
cmake --build . --config Release 
```

### Run

```bash
./Release/program.exe
```

##Input file
.graph files are in the ./test folder. To launch the program on rgg_n_2_15_s0.graph, the filename has to be ../../test/rgg_n_2_15_s0.graph
