This file should contain:

  - your name
  - your UNI
  - lab assignment number
  - description for each part
  
The description should indicate whether your solution for the part is
working or not.  You may also want to include anything else you would
like to communicate to the grader such as extra functionalities you
implemented or how you tried to fix your non-working code.

John Hui
jzh2106
lab5 cs3157
My solution compiles and works as expected.

--------

Lab 5 Part 1b:

This is the output for `valgrind --leak-check=yes ps afxfww`:

    ==23309== Memcheck, a memory error detector
    ==23309== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
    ==23309== Using Valgrind-3.13.0 and LibVEX; rerun with -h for copyright info
    ==23309== Command: ps ajxfww
    ==23309==
    [Output redacted]
    ==23309==
    ==23309== HEAP SUMMARY:
    ==23309==     in use at exit: 727,288 bytes in 792 blocks
    ==23309==   total heap usage: 968 allocs, 176 frees, 1,750,666 bytes allocated
    ==23309==
    ==23309== 14,552 bytes in 377 blocks are possibly lost in loss record 10 of 13
    ==23309==    at 0x4C31B25: calloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
    ==23309==    by 0x4E409CC: ??? (in /lib/x86_64-linux-gnu/libprocps.so.6.0.0)
    ==23309==    by 0x4E4208E: ??? (in /lib/x86_64-linux-gnu/libprocps.so.6.0.0)
    ==23309==    by 0x4E42F0F: ??? (in /lib/x86_64-linux-gnu/libprocps.so.6.0.0)
    ==23309==    by 0x4E438D5: ??? (in /lib/x86_64-linux-gnu/libprocps.so.6.0.0)
    ==23309==    by 0x4E44658: readproc (in /lib/x86_64-linux-gnu/libprocps.so.6.0.0)
    ==23309==    by 0x4E45129: readproctab2 (in /lib/x86_64-linux-gnu/libprocps.so.6.0.0)
    ==23309==    by 0x112EF2: ??? (in /bin/ps)
    ==23309==    by 0x5285B96: (below main) (libc-start.c:310)
    ==23309==
    ==23309== LEAK SUMMARY:
    ==23309==    definitely lost: 0 bytes in 0 blocks
    ==23309==    indirectly lost: 0 bytes in 0 blocks
    ==23309==      possibly lost: 14,552 bytes in 377 blocks
    ==23309==    still reachable: 712,736 bytes in 415 blocks
    ==23309==         suppressed: 0 bytes in 0 blocks
    ==23309== Reachable blocks (those to which a pointer was found) are not shown.
    ==23309== To see them, rerun with: --leak-check=full --show-leak-kinds=all
    ==23309==
    ==23309== For counts of detected and suppressed errors, rerun with: -v
    ==23309== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)

As we know, it is not possible for shell scripts to have a leak of memory.
However, this program clearly leaks memory, without any remorse.
Therefore, it mustn't be a shell script. Thanks.
