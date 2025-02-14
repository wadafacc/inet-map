all:
	@gcc -Wall -c ./*.c
	@gcc -o build.o ./*.o

clean:
	@rm -rf ./*.o 

clean-data: 
	@rm -rf ./data/*