echo PASS
python -c "print('*'*512 + '\n' + '*'*512 + '\n' + '*'*512)" > test.txt
./gameoflife -i test.txt -i test.txt -g 5 -t -s

echo PASS
./test_script.sh

echo PASS
python -c "print(str('*'*16 + '\n') * 16)" > test.txt
./gameoflife -i test.txt -i test.txt -g 0 -t -s

echo FAIL
python -c "print('*'*513 + '\n' + '*'*513 + '\n' + '*'*513)" > test.txt
./gameoflife -i test.txt -i test.txt -g 5 -t -s

echo FAIL
python -c "print('*'*16 + '\n' + '*'*4 + '\n')" > test.txt
./gameoflife -i test.txt

echo PASS
python -c "print('*'*1)" > test.txt
./gameoflife -i test.txt -t -s

echo FAIL
python -c "print(str('*'*16 + '\n') * 16)" > test.txt
./gameoflife -i test.txt -i glider.txt -g 0 -t -s

echo FAIL
python -c "print(str('*'*16 + '\n') * 16)" > test.txt
./gameoflife -i test.txt -o output.txt -o foo.txt

echo FAIL
python -c "print(str('*'*16 + '\n') * 16)" > test.txt
./gameoflife -i test.txt -g -1

echo PASS 
python -c "print(str('*'*16 + '\n') * 16)" > test.txt
./gameoflife -i test.txt -s -s -s -s -s -s -s -s

echo FAIL
./gameoflife -i foo.txt

echo FAIL
python -c "print(str('x'*16 + '\n') * 16)" > test.txt
./gameoflife -i test.txt
