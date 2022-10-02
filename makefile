CP = gcc
all: exit
	@echo "Compiling..."
	@echo "Compilation complete!"
	@echo
	@echo "Running..."
	./exe

exit: main.o imagem.o base.o filtros2d.o geometria.o cores.o desenho.o compression.o
	$(CP) -o exe main.o imagem.o base.o filtros2d.o geometria.o cores.o desenho.o compression.o -lm -Wall

imagem.o: imagem.c
	$(CP) -o imagem.o -c imagem.c

base.o: base.c
	$(CP) -o base.o -c base.c

filtros2d.o: filtros2d.c
	$(CP) -o filtros2d.o -c filtros2d.c

geometria.o: geometria.c
	$(CP) -o geometria.o -c geometria.c

cores.o: cores.c
	$(CP) -o cores.o -c cores.c

desenho.o: desenho.c
	$(CP) -o desenho.o -c desenho.c
compression.o: compression.c
	$(CP) -o compression.o -c compression.c

main.o: main.c
	$(CP) -o main.o -c main.c

clean:
	rm -rf *.o
	rm -rf exe