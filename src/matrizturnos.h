#define MESES_TURNOS 4
  
// Matriz de turnos. Se compone de dos dimensiones. La primera marca los meses incluidos en la matrix.
// Si hay 4 meses de turnos, MESES_TURNOS será 4. Cada turno se marca entre corchetes, como elemento de 
// la matriz. 
// La segunda dimensión son los días (o elementos de la matriz MESES_TURNOS). Se compone de 33 elementos.
// Estos 33 elementos son la suma de los días máximos del mes (31) junto con dos valores iniciales correspondientes
// al año y al mes al que pertenece la matriz de turnos. Estos valores se usarán más adelante para saber si el
// mes seleccionado consta de matriz de turnos o no.
static const char *turnos[MESES_TURNOS][33] = {
	{ "2015", "1",
    "L", "T", "AT", "FT", "AM", "L", "AM", 
    "M", "L", "L", "FM", "M", "M", "T", 
    "T", "T", "AT", "L", "AM", "M", "M",
    "M", "M", "M", "L", "T", "T", "T", 
    "T", "T", "AT"},
  { "2015", "2",
    "L", "L", "M", "M", "M", "M", "M", 
    "V", "V", "V", "V", "V", "V", "V",
    "V", "V", "V", "V", "M", "M", "M", 
    "L", "T", "T", "T", "T", "AT", "AT",
    " ", " ", " "},
  { "2015", "3",
    "L", "M", "M", "M", "M", "M", "L",
    "L", "T", "T", "T", "T", "T", "AT",
    "L", "M", "M", "M", "L", "M", "M", 
    "L", "T", "T", "T", "T", "T", "AT", 
    "L", "L", "M"}, 
  { "2014", "12",
    "M", "M", "T", "T", "AT", "FT", "L",
    "L", "M", "M", "M", "M", "AM", "FM",
    "AT", "T", "T", "AT", "M", "AM", "FT", 
    "AM", "AM", "AM", "L", "M", "M", "FM", 
    "FT", "AT", "T"},     
};