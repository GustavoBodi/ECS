.PHONY: docs

all:
	mkdir -p build/ && \
	cd build && \
	cmake ../ && \
	make -j12 && \
	./project

setup_docs:
	python -m venv ./docs/.venv && \
	. ./docs/.venv/bin/activate && \
	pip install sphinx sphinx-sitemap sphinx-rtd-theme breathe exhale==0.3.6

run:
	cd build && \
	make -j12 && \
	./project

test:
	cd build && \
	cmake ../ && \
	make -j12 && \
	./tests-main

docs:
	cd docs && \
	$(MAKE) html

open:
	firefox ./docs/_build/html/index.html

clean:
	rm -rf ./build/*
	rm -rf ./docs/_build/*
