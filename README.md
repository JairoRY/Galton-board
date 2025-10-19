# Galton Board Experiment

This program simulates a Galton board to compare the observed ball distribution with the theoretical binomial and normal distributions. It also computes the mean squared errors (MSE) between them.

---

## Requirements

A C++17 or newer compiler is required, such as:

* g++
* clang++
* MSVC

No external libraries are needed.

---

## Compilation

To compile the program, run:

```bash
g++ -std=c++17 -o galtonboard galtonboard.cpp
```

This will produce an executable named `galtonboard`.

---

## Execution

Run the program with three arguments:

```bash
./galtonboard <levels n> <balls N> <repetitions x>
```

Where:

* `n` = number of levels (rows of pins)
* `N` = number of balls
* `x` = number of repetitions (results are averaged)

Example:

```bash
./galtonboard 10 100000 5
```

---

## Output

The program prints two tables:

1. A comparison of observed, binomial, and normal expected counts for each bin.
2. A summary of mean squared errors (MSE) between the distributions.

Example:

```
Results after 100000 balls with 10 levels:

         k    Observed   Expected(binomial)   Expected(normal)
--------------------------------------------------------------
         0    98.24000          97.65625          121.98540
         1  1008.56000         976.56250         1033.43580
         ...
--------------------------------------------------------------

Mean Squared Errors
---------------------------------------------
Observed vs Binomial:       0.0000000321
Observed vs Normal:         0.0000000548
Binomial vs Normal:         0.0000000217
---------------------------------------------
```
