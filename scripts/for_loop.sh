#!/bin/bash

extra_space=30  #spaces to be add before PASS/FAIL for right alignment


echo "see error log in zerror.txt"
echo "see more details in zresult.txt"
echo ""

RED='\033[1;31m'
BLUE='\033[1;36m'
GREEN='\033[1;32m'
WHITE='\033[0m'


pass_file_list=();
fail_file_list=();


for file in all/*/*.c;
do
echo "sir______________________________________________";
./A4-sclp   -tokens -ast -tac  "$file";
sir_result=$?;
echo "my_______________________________________________";
./sclp      -tokens -ast -tac  "$file";
my_result=$?;


if [[ $sir_result -eq $my_result ]]
then 
	# if [[ $sir_result -eq 0 ]]
	# then 
		echo "diff_____________________________________________";
		diff -Bw "${file}.tac" "${file}.tac1";
		diff_file=$?;
	# else
	# 	diff_file=0;
	# fi

	if [[ $diff_file -eq 0 ]]
	then 

		echo "_________________________________________________";
		echo "sir=$sir_result, my=$my_result, diff_file=$diff_file"
		# printf "${BLUE}${file} ${WHITE}";
		# printf "$GREEN%${extra_space}s$WHITE\n" "|    PASS"
		echo "${file} | PASS"
		pass_file_list+=("${file}");
	else
		echo "_________________________________________________";
		echo "FEEDBACK: output file missmatch"
		echo "sir=$sir_result, my=$my_result, diff_file=$diff_file"
		# printf "${BLUE}${file} ${WHITE}";
		# printf "$RED%${extra_space}s$WHITE\n" "|    FAIL"
		echo "${file} | FAIL"
		fail_file_list+=("${file}");
	fi
else
	echo "_________________________________________________";
	echo "FEEDBACK: return value not match"
	echo "sir=$sir_result, my=$my_result"
	# printf "${BLUE}${file} ${WHITE}";
	# printf "$RED%${extra_space}s$WHITE\n" "|    FAIL"
	echo "${file} | FAIL"
	fail_file_list+=("${file}");
fi
echo "#################################################";
echo ""
done > "zresult.txt" 2>| "zerror.txt";


	printf "${GREEN}      PASS : ${#pass_file_list[@]}  |  ${WHITE}";
	printf "${RED}FAIL : ${#fail_file_list[@]}${WHITE}\n";

for ffff in "${pass_file_list[@]}";
do
	printf "${GREEN}PASS    ${WHITE}";
	printf "${BLUE}${ffff}${WHITE}\n";
done | sort

for ffff in "${fail_file_list[@]}";
do
	printf "${RED}FAIL    ${WHITE}";
	printf "${BLUE}${ffff}${WHITE}\n";
done

echo "_________________________________________________";
