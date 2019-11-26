#!/bin/bash
echo "> start"
echo "$ compile"
rm -f mini_paint gen
gcc -Wall -Wextra -Werror -lm mini_paint.c -o mini_paint
gcc -Wall -Wextra -Werror -lm our_mini_paint.c -o _our_mini_paint
g++ -Wall -Wextra -Werror generate_example.cpp -o gen
echo "$ test"
counter=1
max=2
our_res=-1
bad_res=-1
while [ $counter -le $max ]
do
	./gen
	if [ $? ]
	then
		sleep .01
		./_our_mini_paint example_ > coutput 2>&1
		our_res=$?
		./mini_paint example_ > output 2>&1
		bad_res=$?
		if [ $our_res -ne $bad_res ]
		then
			printf "\n: different return result, our \e[1;31m$our_res\e[0m and yours \e[1;32m$bad_res\e[0m !\n"
			exit 1
		fi
		diff -y --suppress-common-lines coutput output
		if [ $? -ne 0 ]
		then
			printf "\e[1;31m: difference in output, coutput is our, output yours and the example is in example_ !\e[0m\n"
			exit 2
		fi
		if ! (( counter % 50 ))
		then
			printf "\e[1;34m[$counter]\e[0m"
		fi
		if [ $our_res -ne 0 ]
		then
			printf "\e[1;33m$our_res\e[0m"
		else
			printf "\e[1;32m$our_res\e[0m"
		fi
	fi
	((max++))
	((counter++))
done
rm -rf gen mini_paint example_ output coutput
printf "\n> done"
