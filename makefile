
all: diferencias tiempos.txt output.txt evolution.png times.png

diferencias: diferencias.c
	gcc diferencias.c -o diferencias -lm -fopenmp

tiempos.txt:
	touch tiempos.txt

output.txt: diferencias 
	@echo "{ time ./diferencias 1 ; } |& grep "user">>tiempos.txt"
	rm output.txt
	@echo "{ time ./diferencias 2 ; } |& grep "user">>tiempos.txt"
	rm output.txt
	@echo "{ time ./diferencias 4 ; } |& grep "user">>tiempos.txt"

evolution.png: output.txt Graf_resultado.py
	python Graf_resultado.py

times.png: tiempos.txt Graf_tiempo.py
	python Graf_tiempo.py

clean:
	rm diferencias output.txt tiempos.txt evolution.png times.png Energies.txt energies.png
