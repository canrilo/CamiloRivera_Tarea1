
all: diferencias Times.txt Energies.txt evolution.png times.png

diferencias: diferencias.c
	gcc diferencias.c -o diferencias -lm -fopenmp

Times.txt output.txt Energies.txt: diferencias 
	#@echo "{ time ./diferencias 1 ; } |& grep "user">>tiempos.txt"
	./diferencias 1
	./diferencias 2
	./diferencias 4	

energies.png evolution.png: Energies.txt output.txt Graf_resultado.py
	python Graf_resultado.py

times.png: Times.txt Graf_tiempo.py
	python Graf_tiempo.py

clean:
	rm diferencias output.txt Times.txt evolution.png times.png Energies.txt energies.png
