# Zpacker

## instructions

Zpacker is used to packer ELF64 file.

It is still under development.

## reference

Some of the code is borrowed from grumbach/woody_woodpacker

https://github.com/grumbach/woody_woodpacker


## purpose

This project is only used for learning and exchanging shell technology

## How to build it

``` bash

make

```

## How to use it

``` bash

./Zpacker filename

```

## How to test

## build packed test file and run

``` bash

gcc test.c -o test

./Zpacker test

./packed_test

```
### result

``` bash
zpacker:hello world

```
## How to clean redundancy

``` bash

make clean

```
