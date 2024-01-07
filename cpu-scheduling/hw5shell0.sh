gcc -o cpuHigh cpuHigh.c
gcc -o memRead memRead.c
gcc -o diskRead diskRead.c

time -v schedtool -0 -a 0x1 -e ./cpuHigh 
time -v schedtool -0 -a 0x1 -e ./memRead 
time -v schedtool -0 -a 0x1 -e ./diskRead test.txt  test1.txt test2.txt
