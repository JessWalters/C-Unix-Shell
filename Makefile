make:
	gcc -o project1 project1.c

.PHONY: run
run: ## Compile and run the module
	gcc -o project1 project1.c
	./project1