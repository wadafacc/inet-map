all:
	@gcc -Wall -c ./*.c
	@gcc -o build.o ./*.o

clean:
	@rm -rf ./*.o 

cleand: 
	@rm -rf ./data/*
	@rm -rf ./*.txt