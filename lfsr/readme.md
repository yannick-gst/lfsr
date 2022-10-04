**LFSR**

Implements a random number generator based on a maximal-length linear-feedback shift register.
A maximal-length LFSR of size N will cycle through all possible 2^N - 1 states within the shift register, except the state where all bits are zero.

The MaxLFSR class can be used to randomly generate unsigned integers from 1 to 2^N -1, without repeats.

When the LFSR has cycled through all states, it is reseeded so that the next cycle produces a different sequence of numbers.