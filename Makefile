.PHONY: docs

all:
	cd build && \
	cmake ../ && \
	make && \
	./project

run:
	cd build && \
	make && \
	./project

test:
	cd build && \
	cmake ../ && \
	make && \
	./tests-main

docs:
	cd docs && \
	$(MAKE) html

open:
	firefox ./docs/_build/html/index.html

clean:
	rm -rf ./build/*
	rm -rf ./docs/_build/*
