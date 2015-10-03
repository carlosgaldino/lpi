Even though in `mem_segments` there is an array of around 10 MB in size (`mbuf`)
the executable file for `mem_segments` is much smaller than that because the
mentioned array is not initialized. This means that the executable only needs to
store the location and size required for the array which will be together with
any other uninitialized variable in the uninitialized data segment. The required
memory will be only allocated at run time.
