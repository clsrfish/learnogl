#!/bin/bash

cmake -S . -B build --graphviz=build/dependencies.dot
dot -v -Twebp -o build/dependencies.webp build/dependencies.dot
code build/dependencies.webp
