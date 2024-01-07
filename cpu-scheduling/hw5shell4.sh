sudo -S /usr/bin/time -v schedtool -4 -a 0x1 -p 1 -e ./cpuHigh
sudo -S /usr/bin/time -v schedtool -4 -a 0x1 -p 2 -e ./memRead
sudo -S /usr/bin/time -v schedtool -4 -a 0x1 -p 3 -e ./diskRead test.txt  test1.txt test2.txt
