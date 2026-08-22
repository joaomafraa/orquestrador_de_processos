processflow: processflow.o
	gcc processflow.o -o processflow

processflow.o: processflow.c
	gcc -c processflow.c

run: processflow
	./processflow

clean:
	rm -f *.o processflow

.PHONY: run clean