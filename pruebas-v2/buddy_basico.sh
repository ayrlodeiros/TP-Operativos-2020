#!/bin/sh
./gameboy-v2 BROKER CAUGHT_POKEMON 1 OK
./gameboy-v2 BROKER CAUGHT_POKEMON 2 FAIL

./gameboy-v2 BROKER NEW_POKEMON Pikachu 2 3 1

./gameboy-v2 BROKER CATCH_POKEMON Onyx 4 5

./gameboy-v2 SUSCRIPTOR NEW_POKEMON 10

./gameboy-v2 BROKER CATCH_POKEMON Charmander 4 5
