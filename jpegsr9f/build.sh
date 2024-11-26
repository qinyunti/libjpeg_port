#!/usr/bin/env bash

gcc jpeg_decode_buffer.c decode_test.c src/*.c port/jioport.c port/jmemansi.c -I./ -Iinc/  -g -o decode_test
gcc jpeg_encode_buffer.c encode_test.c src/*.c port/jioport.c port/jmemansi.c -I./ -Iinc/  -g -o encode_test