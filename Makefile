all:
	make -C src
	cp src/crender .

clean:
	make -C src clean

run: all
	./crender
