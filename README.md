# cpp_gallery
Some features are introduced in c++14, c++17 and c++20 so that when using **cl.exe** to build on Windows, use 
```dos
cl /std:c++latest main.cpp
```
If using G++, then
```terminal
g++-9 libc++=2x
```

## Language
### ParameterPack
1. Combine hash: fold expression and variadic function.
2. Cast: store double and struct in an array with reinterpret_cast. This method can be used to write an allocator.

## Math
### SteepestDecent
SteepestDecent: optimization using armadillo. Two methods for calculating step length: Armijo and 0.618.

## OS
### CopyFiles
Usage of filesystem.

## Algorithm
### TreeTraverse
Given an array $y[i]=j$ which means i's parent is j. First construct a map to store the tree. Then travere it and return path for each leaf.