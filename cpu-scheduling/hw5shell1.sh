sudo -S /usr/bin/time -v schedtool -1 -p 1 -a 0x1 -e ./cpuHigh 
sudo -S /usr/bin/time -v schedtool -1 -p 2 -a 0x1 -e ./memRead 
sudo -S /usr/bin/time -v schedtool -1 -p 3 -a 0x1 -e ./diskRead test.txt  test1.txt test2.txt
