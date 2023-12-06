# Advent of Code 2023

Advent of Code 2023

Odd numbered days (1st December, 3rd December, etc) are in C++23, where as even numbered days are in Rust. This is because I wanted to practise both but did not want to commit to doing each challenge twice (and they are more fun the first time round).


## Odd days: C++23

Compiled and run from repo root with `g++ --std=c++23 -O3 day1/a/main.cpp -o day1/build/1a`, etc. Locally on Arch (btw) the `--std` flag is required to allow for `std::views` though this still flags as an error in vscode...


## Even days: Rust 1.74.0

Compiled and run from cargo root with the good 'ol `cargo run --release`.


## Run

Or you can build and run with:
```sh
# cpp or rust
./run 1         # build and run day 1 with time

# cpp
./run 1 1,2     # same as `./run 1`
./run 1 1       # build and run only day 1 part 1
```


## Results

Day     | Part One      | Part Two      | Execution Time    |
:-------|--------------:|--------------:|-------------------:
1 | 56049 | 54530 | 0.002s [1]
2 | 2679 | 77607 | 0.001s
3 | 522726 | 81721933 | 0.005s [2]
4 | 23941 | 5571760 | 0.002s
5 | 806029445 | 59370572 | 2m24.628s [3]

[1] - Part one and part two were separate executables taking 0.001s and 0.002s respectively, combining would likely be less than the sum of execution times

[2] - Due to naive algorithm which iterated over all part and symbol lexemes in O(|N| * |S|), could be made faster by only checking the symbols in rows (n-1) to (n+1), not 0..N, for a part on row n. But this was deemed pointless given that it was a single run on once input of only 140x140 possible symbols (reality: 730) and 70x140 numbers (reality: 1192) the execution was basically instant.

[3] - Todo: fast version from notes.
