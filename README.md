# Embedded C API

API to configure projects and perform common operations on embedded applications.

Provides common macros, definitions, types, structures and functions that are useful on embedded applications, abstracting common architecture dependencies that these implementations frequently need.

## Modules Available

### Common Data Structures

Common-used generic data structures

#### Ring Buffer

[Functions](https://github.com/arturnse/embedded-api/blob/master/includes/common/ring_buffer.h) that implement a decent FIFO ring buffer.
* Customizable element sizes
* Functions for popping and peeking elements 
* Does **NOT** turn off interruptions. It must be done outside the function calls when needed.

#### Entropy Pool

[Functions](https://github.com/arturnse/embedded-api/blob/master/includes/common/entropy_pool.h) that implement a data entropy pool.
* Data pushing is bit-aware, and can be done bit by bit.
* When the FIFO is full, the pool does XOR with previous elements for better entropy.

### Data Presentation

[Functions](https://github.com/arturnse/embedded-api/blob/master/includes/common/conv.h) to convert data presentation.
