FastHamming
===========

This is a fast implementation of a truncateable extended (127,120) Hamming code.

It features an encoder and decoder that allow for any number of bytes to be
transferred. If the amount of data that needs to be transferred is too large
for one block it is automatically broken up into chunks of 15 bytes with the
last block having all the "undefined" bits past the end silently skipped.

The encoding is performed as a separateable code by appending the redundancy
information at the end of each block. All coding assumes overall even parity.

The library is mostly targeted towards STM32 microcontrollers, but should be
portable enough to run on essentially everything with enough RAM to hold the
encode and decode buffers.

Inspiration
===========

This library was inspired by the explanations presented by [@3blue1brown](https://twitter.com/3blue1brown) in
the following Youtube videos:

- [Explanation Part 1](https://youtu.be/X8jsijhllIA)
- [Explanation Part 2](https://youtu.be/b3NxrZOu_CE)
- [HW implementation](https://youtu.be/h0jloehRKas)

License
=======

This library is licensed under AGPLv3.
