sudo useradd -m usera -p Sudoku111
sudo useradd -m userb -p Sudoku101
sudo useradd -m userc -p Sudoku100
sudo groupadd hw5 
sudo adduser usera hw5 
sudo adduser userb hw5 
sudo adduser userc hw5 
sudo chown userc DataIn.txt
sudo chown userc DataOut.txt
sudo chown userc DataProcessor.c
gcc -o DataProcess DataProcessor.c
echo "1. User C commands:"
echo "chmod 740 DataIn.txt"
echo "chmod 720 DataOut.txt"
echo "chmod 710 DataProcessor.c"
echo "su - userc"
echo "2. Now, User B commands:"
echo "su - userb"
echo "./DataProcess DataIn.txt DataOut.txt"
echo "3. Lastly, User C again:"
echo "User C"
echo "su - userc"
echo "cat DataOut.txt"
