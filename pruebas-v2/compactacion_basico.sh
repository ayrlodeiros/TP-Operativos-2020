#!/bin/sh
./gameboy-v2 BROKER CAUGHT_POKEMON 1 OK
./gameboy-v2 BROKER CAUGHT_POKEMON 2 FAIL

./gameboy-v2 BROKER CATCH_POKEMON Pikachu 2 3
./gameboy-v2 BROKER CATCH_POKEMON Squirtle 5 2

./gameboy-v2 BROKER CATCH_POKEMON Onyx 4 5

./gameboy-v2 SUSCRIPTOR CAUGHT_POKEMON 10

./gameboy-v2 BROKER CATCH_POKEMON Vaporeon 4 5
