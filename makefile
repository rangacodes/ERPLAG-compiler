make: compiler
	./compiler testcase.txt code.asm
compiler: ast.o driver.o ff.o codegen.o intermediate_code_generator.o lexer.o symbol_table.o typeChecker.o
	gcc ast.o driver.o ff.o codegen.o intermediate_code_generator.o lexer.o symbol_table.o typeChecker.o -o compiler
ast.o : ast.c
	gcc -c ast.c -o ast.o
driver.o: driver.c
	gcc -c driver.c -o driver.o
ff.o: ff.c
	gcc -c ff.c -o ff.o
codegen.o: codegen.c
	gcc -c codegen.c -o codegen.o
intermediate_code_generator.o: intermediate_code_generator.c 
	gcc -c intermediate_code_generator.c -o intermediate_code_generator.o
lexer.o: lexer.c 
	gcc -c lexer.c -o lexer.o
symbol_table.o: symbol_table.c 
	gcc -c symbol_table.c -o symbol_table.o
typeChecker.o: typeChecker.c 
	gcc -c typeChecker.c -o typeChecker.o
