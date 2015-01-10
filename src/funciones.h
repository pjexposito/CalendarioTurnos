#define MESES_TURNOS 20

#define LINEA_HORIZONTAL_INICIAL 30
#define LINEA_HORIZONTAL 17

#define LINEA_VERTICAL_INICIAL 2
#define LINEA_VERTICAL 20

#define COLOR_PRINCIPAL GColorBlack  // El color del lápiz es blanco
#define COLOR_FONDO GColorWhite  // y el fondo, negro

#define FUENTE FONT_KEY_GOTHIC_18
#define FUENTE_BOLD FONT_KEY_GOTHIC_18_BOLD

#define FUENTE_GRANDE FONT_KEY_GOTHIC_18
#define FUENTE_GRANDE_BOLD FONT_KEY_GOTHIC_18_BOLD
  
// La variable chkturnos puede tener valor 1 si se muestra el calendario de turnos y valor 0 si se muestra el de días.
// Se define de forma global las variables día, mes y año (dado que pueden cambiar a lo largo de la ejecución)
// Mes_actual y dia_actual siempre guardarán el valor del día y el mes en el que se ejecuta el programa (no varían)
int dia, mes, ano, mes_actual, dia_actual, chkturnos;

int turnos[MESES_TURNOS][33];

// Función para conseguir el día de la semana dado un día, un mes y un año
int dweek(int year, int month, int day);

// Función que devuelve el número de días que contiene un mes específico
int numero_de_dias(int month, int year);

void send_int(int key, int cmd);

char* subString (const char* input, int offset, int len, char* dest);

void carga_datos();

void anade_datos(const char* input, int mes);