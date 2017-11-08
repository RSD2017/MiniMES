#!/bin/bash
echo -e  "GET ORDER 1\n" | nc -q 1 localhost 4567 | xxd -c1

echo -e "GET PART 1\n" | nc -q 1 localhost 4567  | xxd -c 1
echo -e  "POST PARTSTATUS\n0001 0" | nc -q 1 localhost 4567

echo -e "GET PART 1\n" | nc -q 1 localhost 4567  | xxd -c 1
echo -e  "POST PARTSTATUS\n0002 1" | nc -q 1 localhost 4567
echo -e "GET PART 1\n" | nc -q 1 localhost 4567  | xxd -c 1
echo -e  "POST PARTSTATUS\n0003 0" | nc -q 1 localhost 4567
echo -e "GET PART 1\n" | nc -q 1 localhost 4567  | xxd -c 1
echo -e  "POST PARTSTATUS\n0004 1" | nc -q 1 localhost 4567
echo -e "GET PART 1\n" | nc -q 1 localhost 4567  | xxd -c 1
echo -e  "POST PARTSTATUS\n0005 0" | nc -q 1 localhost 4567
echo -e "GET PART 1\n" | nc -q 1 localhost 4567  | xxd -c 1
echo -e  "POST PARTSTATUS\n0006 1" | nc -q 1 localhost 4567
echo -e "GET PART 1\n" | nc -q 1 localhost 4567  | xxd -c 1
echo -e  "POST PARTSTATUS\n0007 0" | nc -q 1 localhost 4567
echo -e "GET PART 1\n" | nc -q 1 localhost 4567  | xxd -c 1
echo -e  "POST PARTSTATUS\n0008 1" | nc -q 1 localhost 4567
echo -e "GET PART 1\n" | nc -q 1 localhost 4567  | xxd -c 1
echo -e  "POST PARTSTATUS\n0009 0" | nc -q 1 localhost 4567
echo -e "GET PART 1\n" | nc -q 1 localhost 4567  | xxd -c 1
echo -e  "POST PARTSTATUS\n0010 1" | nc -q 1 localhost 4567
echo -e "GET PART 1\n" | nc -q 1 localhost 4567  | xxd -c 1
echo -e  "POST PARTSTATUS\n0011 1" | nc -q 1 localhost 4567







#echo -e "GET PART 1\n" | nc -q 1 localhost 4567  | xxd -c 1
#echo -e  "POST PARTSTATUS\n0001 1 2 7 8" | nc -q 1 localhost 4567
echo -e  "POST ORDERSTATUS\n0001 1 2 7 8\n" | nc -q 1 localhost 4567 
echo -e  "POST ORDERSTATUS\n0002 2 6 10 11\n" | nc -q 1 localhost 4567 



