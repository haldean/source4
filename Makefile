all:
	make -C src
	cp src/source4 .

clean:
	make -C src clean

run: all
	./source4
