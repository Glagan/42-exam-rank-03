gcc -Wall -Wextra -Werror micro_paint.c -o micro_paint
rm -f output.res
./micro_paint example0 > output.res
./micro_paint example1 >> output.res
./micro_paint example2 >> output.res
./micro_paint example3 >> output.res
./micro_paint example4 >> output.res
rm -f coutput.res
./our_micro_paint example0 > coutput.res
./our_micro_paint example1 >> coutput.res
./our_micro_paint example2 >> coutput.res
./our_micro_paint example3 >> coutput.res
./our_micro_paint example4 >> coutput.res
diff -y --suppress-common-lines coutput.res output.res
rm -rf micro_paint output.res coutput.res