# Long Addition in Parallel

## Description

Addition. Such a simple task, you might have learned the algorithm for long addition when you were in grade
school. Now you will have the opportunity and re-live this childhood experience by adding two large numbers
(so large that they cannot be stored into an int or even an unsigned long long), thus you will need to
store the number into a dynamic array or a vector. Thus each element will be a digit in its appropriate
place value, so you can write the iterative code to implement long addition. To make matters worse, we will
do this in parallel using a vector of threads.

Suppose we want to add the two 32 digit numbers: 56781257987110923417082367650001 and
74321168999909850024784138764000. Of course this can be done sequentially in O(n) time where n is the
size of the two numbers. Suppose we had 8 threads that could run in parallel, we can break the problem
where each thread adds a section of size 4 and add them up in parallel since the each thread can perform
its own addition without the result of any other thread.

```
5678 1257 9871 1092 3417 0823 6765 0001
+
7432 1168 9999 0985 0024 7841 3876 4000
```
```
1 3110 0 2425 1 9870 0 2077 0 3441 0 8664 1 0641 0 4001
```
As you can see above, each portion was added (using long addition) and the number in red is the carry over
that would be added to the section to the left of where the carry over bit was computed, and so on. Once
all the carry overs are added to each section, the numbers can be concatenated to obtain the final answer.

Your program will read two large numbers (64 digits and larger), and you will need to store each digit
into an array or vector. Then you will need to determine the maximum number of threads that can run
concurrently that is also a power of 2 (You can assume the number of digits for the numbers used will also
be powers of 2). You then need to divide up the work such that each thread performs the addition on a
specific portion of the number, compute its sum and store the carry overs for each portion. Then at the end
you will perform add the carry overs to the correct section of the number, this will be done sequentially.

## Specifications

- Document your code
- Use the C++thread library to implement the brick sort algorithm in parallel
- Do not spawn more threads than your system can handle
- Global variables are ok in this project especially since the threads might share some memory

## Example Output

```
$ g++ -pthread laip.cpp
```
```
$ cat huge_nums.txt
3377364598435445778809903612001058603382305856774546614102313493
8080212596483814704504943572350673964488834590322416825886403781
```
```
$ ./a.out
Enter input file: huge_nums.txt
```
```
11457577194919260483314847184351732567871140447096963439988717274
```
