# Concurrent Mergesort
Time analysis and comparision of concurrent mergesort with normal mergesort.

## Details
- There are three types of merge sort I have implemented.
    1. `normal` (without forking, just one process)
    2. `concurrent` (forking for every part containing more than 5 elements.)
    3. `concurrent_controlled` (forking up to some level. i.e. there are logn levels. So I am here forking up to half or one third level.)
- `test.c` is to generate input file.

## Test Runs

### For n=10^5

#### normal
```
real	0m0.036s
user	0m0.035s
sys	    0m0.001s
```

#### concurrent
```
real	0m5.005s
user	0m0.227s
sys	    0m17.835s
```

#### concurrent_controlled
- Forking up to logn/2 levels
```
real	0m0.034s
user	0m0.033s
sys	    0m0.021s
```

- It is clear that we should not fork for all levels. Forking has its overhead. For smaller segments, it is wise to do normal merge sort.
- Also after each level we are creating two more processes. Parent process is waiting for its two children. So whenever parent is scheduled, it takes CPU time just for waiting. At a time at max two processes are doing work.
- There is a level where we should stop forking. I have taken as middle level which is working fine.
- Though sys time is more(due to forking), real time is almost same. Thus, limited forking is always better.
- One thing is clear that CPU is doing more work in forking than normal. So it is not reducing complexity. But real time is less.

### For n=10^7
#### normal
```
real	0m4.142s
user	0m4.065s
sys	    0m0.044s
```
```
real	0m4.267s
user	0m4.206s
sys	    0m0.057s
```

#### concurrent
- concurrent can't run for this big input. It will fork too much. CPU won't be able to allocate this much resources.

#### concurrent_controlled
- Forking up to logn/2 levels
```
real	0m2.588s
user	0m5.372s
sys	0m0.347s
```
```
real	0m2.557s
user	0m5.330s
sys	0m0.321s
```

- Forking up to logn/1.5 levels
```
real	0m2.691s
user	0m5.324s
sys	0m0.851s
```

- Forking up to logn/3 levels
```
real	0m2.515s
user	0m5.284s
sys	0m0.206s
```

- Here we can see concurrency at its best. Normal merge sort is taking significant time. But taking advantage of concurrency and limited forking concurrent_controlled merge sort is taking almost half real time.


### For n=10^8

#### normal
```
real	0m44.374s
user	0m43.938s
sys	    0m0.427s
```

#### concurrent_controlled
- Forking up to logn/2 levels
```
real	0m26.336s
user	0m58.542s
sys	    0m3.052s
```

- Forking up to logn/3 levels
```
real	0m26.340s
user	0m58.522s
sys	    0m2.197s
```

- Forking up to logn/1.5 levels
```
real	0m28.335s
user	0m59.911s
sys	    0m5.763s
```

- Forking up to logn/1.25 levels
real	0m31.286s
user	0m59.070s
sys 	0m19.807s

- Forking up to logn/1.3 levels
real	0m31.227s
user	0m58.946s
sys 	0m19.725s

- Forking up to logn/1.4 levels
real	0m29.861s
user	0m58.472s
sys 	0m11.723s
