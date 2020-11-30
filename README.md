# Final Project CS225 Fall 2020 - Voyager



## Summary

This is UIUC CS225 course final project aims to build heatmap visualization of worldwide airports based on their importance (calculated by betweenness centrality). Check the result [here](result/RESULTS.md). 


## Contributors

listed in alphabetically order

**Jessica Wang** (NetID: ziyu4)

**Yucheng Jiang** (NetID: yj17)

**Yueqi Jiang** (NetID: yueqij2)



## Compiling and Running

- Clone the [repository](https://github-dev.cs.illinois.edu/cs225-fa20/yj17-ziyu4-yueqij2.git)

- Make sure you have these packages installed

  `clang-6.0`, `libc++abi-dev`, `libc++-dev`,  `git`,  `gdb`,  `make`

- compile the executable by linking your code with the provided code with the following command `make`

- The executable created is called `voyager`. You can run it as follows:

  `./voyager [input file addr] [output file addr]`

  Parameters in `[square brackets]` are optional. Below are the defaults:

  | Parameter           | Default        | Notes                                                    |
  | ------------------- | -------------- | -------------------------------------------------------- |
  | input file address  | "worldMap.png" |                                                          |
  | output file address | "outMap.png"   | `.png`, `.jpg`, `.gif`, and `.tiff` files also supported |



## Document files

  - [Project goals](documents/GOALS.md)
  - [development.md]((documents/DEVELOPMENT.md))