# Embedded C API

API to configure projects and perform common operations on embedded applications.

It provides common macros, definitions and functions useful on embedded applications, abstracting common architecture dependencies that these implementations need.

## Modules Available

Quick brief description of the modules available in this project.

### Common Data Structures

Common-used generic data structures

#### Ring Buffer

[Function Prototypes](https://github.com/arturnse/embedded-api/blob/master/includes/common/ring_buffer.h) functions that implement a decent FIFO ring buffer.
* Customizable element sizes
* Functions for popping and peeking elements 
* Does **NOT** turn off interruptions. It must be done outside the function calls when needed.

#### Entropy Pool

[Function Prototypes](https://github.com/arturnse/embedded-api/blob/master/includes/common/entropy_pool.h) functions that implement a data entropy pool.
* Data pushing is bit-aware, and can be done bit by bit.
* When the FIFO is full, the pool does XOR with previous elements for better entropy.

### Data Presentation

[Function Prototypes](https://github.com/arturnse/embedded-api/blob/master/includes/common/conv.h) functions that convert data presentation.
