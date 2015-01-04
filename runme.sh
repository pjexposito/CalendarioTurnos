#!/bin/bash

clear
echo "Programa para crear un calendario de turnos actualizado"
echo " "
python crea_matriz.py
rm ./src/matrizturnos.h > /dev/null
mv matrizturnos.h ./src/matrizturnos.h
rm ShiftCal.db
echo "Creando ejecutable para Pebble"
pebble clean > /dev/null 2>&1
pebble build > /dev/null 2>&1
echo "Ejecutable creado. Limpiando..."
mv ./build/calendario_de_turnos.pbw ./calendario_de_turnos.pbw
pebble clean > /dev/null 2>&1
echo "Finalizado. El archivo calendario_de_turnos.pbw ha sido creado"
