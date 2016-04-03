default:
	gcc lab0.c -o lab0

check:
	echo "asdf" > input.txt  
	./lab0 --input=input.txt --output=output.txt 
	([ $$? -eq 0 ] && echo "1. PASSED: Return code for input / output")
	diff input.txt output.txt && (echo "2. PASSED: Output matches input")
	./lab0 --input=fake.txt || ([ $$? -eq 1 ] && echo "3. PASSED: Invalid input file handled")
	echo "asdfasdf" > input.txt
	./lab0 --input=input.txt --output=output.txt 
	diff input.txt output.txt && (echo "4. PASSED: Output file is overwritten")
	./lab0 --segfault --catch || ([ $$? -eq 3 ] && echo "5. PASSED: Return code for catch")
	echo "asdfasdfasdf" > input.txt
	rm output.txt
	./lab0 --input=input.txt > output.txt
	diff input.txt output.txt && (echo "6. PASSED: Input successfully redirects to stdout")
	chmod 400 output.txt
	./lab0 --input=input.txt --output=output.txt || ([ $$? -eq 2 ] && echo "7. PASSED: Invalid output file handled")
	chmod 777 output.txt

clean:
	rm ./lab0
	find . -name "*.txt" | xargs rm 

dist:
	tar -cvzf lab0-704291153.tar.gz lab0.c Makefile README.md backtrace.png breakpoint.png
