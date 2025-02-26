#!/bin/bash
cd ..
cd ..
git submodule update --remote --merge
git submodule update --init --recursive
git commit -m "Update submodule to version v1.2.3"