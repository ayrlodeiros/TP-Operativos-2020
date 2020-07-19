#!/bin/sh
./gameboy-v2 BROKER CATCH_POKEMON Pikachu 9 3 #19b
./gameboy-v2 BROKER CATCH_POKEMON Squirtle 9 3 #20b

./gameboy-v2 BROKER CAUGHT_POKEMON 10 OK
./gameboy-v2 BROKER CAUGHT_POKEMON 11 FAIL

./gameboy-v2 BROKER CATCH_POKEMON Bulbasaur 1 7 #21
./gameboy-v2 BROKER CATCH_POKEMON Charmander 1 7 #22

./gameboy-v2 BROKER GET_POKEMON Pichu #9
./gameboy-v2 BROKER GET_POKEMON Raichu #10
