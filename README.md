# Compact Suffix Tree Implementation in C++

## Overview

This project involves the implementation of a compact Suffix Tree data structure in C++ without the use of any Standard Template Library (STL) components, except for basic I/O and string handling functions. The Suffix Tree allows efficient storage of substrings and provides a `Search()` operation to find any given substring within the original string.

## Specifications

- **Allowed Headers**: `<cstdlib>`, `<cstdio>`, `<cstring>`, `<iostream>`
- **Prohibited**: Usage of `string` class, `vector`, and any STL libraries.

## Suffix Tree Node Structure

Each node in the Suffix Tree contains:
1. A linked list of children nodes.
2. The starting index of the characters on the edge leading to this node.
3. The starting index of the suffix ending at this node (for leaf nodes only; otherwise, it is -1).

## Complexity Requirements

- **Tree Construction Time Complexity**: \( O(n^2) \)
- **Tree Space Complexity**: \( O(n) \)
- **Search Complexity**: \( O(m + occ) \)
  - Where \( n \) is the length of the original string.
  - \( m \) is the length of the searched pattern.
  - \( occ \) is the number of occurrences of the pattern.

## Implementation Details

The Suffix Tree is implemented using a compact structure as described in the lecture, with a focus on minimizing the number of components used. The `Search()` operation is designed to efficiently locate any substring within the tree.

## Test Bench

The code includes a test bench that verifies the functionality of the Suffix Tree with at least 40 test cases, covering various input values and patterns.

## Compilation and Execution

The provided C++ code can be compiled and executed following standard procedures, ensuring compatibility with the constraints set forth in the project specifications.

## Documentation

Extensive comments are provided throughout the code to explain the implementation details and the logic behind each operation.

## Usage

To utilize this Suffix Tree implementation, simply include the provided C++ files in your project, compile, and execute as per the instructions.

