#include "pebble.h"
#include "funciones.h"

#define MESES_TURNOS 20

#define COLOR_PRINCIPAL GColorBlack  // El color del lápiz es blanco
#define COLOR_FONDO GColorWhite  // y el fondo, negro

#define FUENTE FONT_KEY_GOTHIC_14
#define FUENTE_BOLD FONT_KEY_GOTHIC_14_BOLD

#define FUENTE_GRANDE FONT_KEY_GOTHIC_18
#define FUENTE_GRANDE_BOLD FONT_KEY_GOTHIC_18_BOLD

int turnos[MESES_TURNOS][33];
int total_turnos=0;
int cargando=0;

static const char *nombre_turno[9] =
{"vacio", "M", "T", "AM", "AT", "L", "FM", "FT", "D"};



// Matriz básica para transformar el número de mes en el nombre del mes.
static const char *nombre_mes[13] =
{ "vacio", "enero", "febrero", "marzo", "abril", "mayo", "junio", "julio", "agosto", "septiembre", "octubre",
    "noviembre", "diciembre" };

// Se define de forma global las variables día, mes y año (dado que pueden cambiar a lo largo de la ejecución)
// Mes_actual y dia_actual siempre guardarán el valor del día y el mes en el que se ejecuta el programa (no varían)
// La variable chkturnos puede tener valor 1 si se muestra el calendario de turnos y valor 0 si se muestra el de días.
int dia, mes, ano, mes_actual, dia_actual, chkturnos;

// Ventana principal
static Window *window;


//Capas del reloj
Layer *CapaLineas; // La capa principal donde se dibuja el calendario

void anade_datos(const char* input, int mes)
{
    total_turnos++;
    int y = 2;
    char dest[6];
    int dias;
    memset(dest, 0, 6);
    subString (input, 0, 2, dest);
    dias = atoi(dest);
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Dias %s. En número es %i", dest, atoi(dest));
    
    memset(dest, 0, 6);
    subString (input, 2, 4, dest);
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Año %s", dest);
    turnos[mes][0]=atoi(dest);
    
    memset(dest, 0, 6);
    subString (input, 6, 2, dest);
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "Mes %s", dest);
    turnos[mes][1]=atoi(dest);
    
    
    for (int x=8;x<dias+8;x++)
    {
        memset(dest, 0, 6);
        subString (input, x, 1, dest);
        //APP_LOG(APP_LOG_LEVEL_DEBUG, "Valor %i: %s", x-7,dest);
        turnos[mes][y]=atoi(dest);
        y++;
    }
}

void carga_datos()
{
    
    char username[64];
    persist_read_string(0, username, sizeof(username));
    if (strcmp(username, "")==0)
    {
        chkturnos=1;
    }
    else
    {
      for (int x=0;x<MESES_TURNOS;x++)
      {
        memset(username, 0, 64);
        persist_read_string(x, username, sizeof(username));
        //APP_LOG(APP_LOG_LEVEL_DEBUG, "Acabo de añadir %s", username);
        anade_datos(username, x);
        //APP_LOG(APP_LOG_LEVEL_DEBUG, "Los datos son %i, %i, %i", turnos[x][0], turnos[x][1], turnos[x][2]);
      }

    }
}



void process_tuple(Tuple *t)
{
    int key = t->key;
    char string_value[64];
    memset(string_value, 0, 64);
    strcpy(string_value, t->value->cstring);
    switch(key) {
        case 0:
            anade_datos(string_value, 0);
            persist_write_string(0, string_value);
            break;
        case 1:
            anade_datos(string_value, 1);
            persist_write_string(1, string_value);
            break;
        case 2:
            anade_datos(string_value, 2);
            persist_write_string(2, string_value);
            break;
        case 3:
            anade_datos(string_value, 3);
            persist_write_string(3, string_value);
            break;
        case 4:
            anade_datos(string_value, 4);
            persist_write_string(4, string_value);
            break;
        case 5:
            anade_datos(string_value, 5);
            persist_write_string(5, string_value);
            break;
        case 6:
            anade_datos(string_value, 6);
            persist_write_string(6, string_value);
            break;
        case 7:
            anade_datos(string_value, 7);
            persist_write_string(7, string_value);
            break;
        case 8:
            anade_datos(string_value, 8);
            persist_write_string(8, string_value);
            break;
        case 9:
            anade_datos(string_value, 9);
            persist_write_string(9, string_value);
            break;
        case 10:
            anade_datos(string_value, 10);
            persist_write_string(10, string_value);
            break;
        case 11:
            anade_datos(string_value, 11);
            persist_write_string(11, string_value);
            break;
        case 12:
            anade_datos(string_value, 12);
            persist_write_string(12, string_value);
            break;
        case 13:
            anade_datos(string_value, 13);
            persist_write_string(13, string_value);
            break;
        case 14:
            anade_datos(string_value, 14);
            persist_write_string(14, string_value);
            break;
    }
    
}

void in_received_handler(DictionaryIterator *iter, void *context)
{
    (void) context;
    Tuple *t = dict_read_first(iter);
    while(t != NULL)
    {
        process_tuple(t);
        t = dict_read_next(iter);
    }
    cargando=0;
    chkturnos=1;
    vibes_short_pulse();
    
    layer_mark_dirty(CapaLineas);
    
}

// Esta función se ejecutará cada vez que se refresque la CapaLineas
void CapaLineas_update_callback(Layer *me, GContext* ctx)
{
    // Color del fondo y color del trazo
    graphics_context_set_stroke_color(ctx, COLOR_PRINCIPAL);
    graphics_context_set_fill_color(ctx, COLOR_PRINCIPAL);
    graphics_context_set_text_color(ctx, COLOR_PRINCIPAL);
    
    // left, top, anchura, altura
    // 144x168
    
    if (cargando == 1)
    {
        graphics_draw_text(ctx, "Cargando...", fonts_get_system_font(FONT_KEY_GOTHIC_28), GRect(0, 0, 140, 30), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
    }
    
    else
    {
        // Se pintan las líneas de calendario
        
        // Lineas horizontales
        graphics_fill_rect(ctx, GRect(2, 30, 140, 2), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(2, 50, 140, 2), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(2, 70, 140, 1), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(2, 90, 140, 1), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(2, 110, 140, 1), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(2, 130, 140, 1), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(2, 150, 141, 2), 0, GCornerNone);
        
        // Lineas verticales
        graphics_fill_rect(ctx, GRect(2, 30, 1, 120), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(22, 30, 1, 120), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(42, 30, 1, 120), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(62, 30, 1, 120), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(82, 30, 1, 120), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(102, 30, 1, 120), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(122, 30, 1, 120), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(142, 30, 1, 120), 0, GCornerNone);
        
        // Se pinta la cabecera con los días de la semana
        graphics_draw_text(ctx, "L", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(2, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, "M", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(22, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, "X", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(42, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, "J", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(62, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, "V", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(82, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, "S", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(102, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, "D", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(122, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        
        // Pinta el nombre del mes
        graphics_draw_text(ctx, nombre_mes[mes], fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(0, 0, 72, 10), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        
        // Pasa el int del año a letra y pinta el año
        char temp[10]  = "";
        snprintf(temp, sizeof(temp), "%d", ano);
        graphics_draw_text(ctx, temp, fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(73, 0, 72, 10), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        
        // Pos es la columna en dónde se escribe el día de la semana.
        // Se inicia en 2 para centrar el dato en el cuadro del calendario
        int pos = 2;
        // Linea (fila) en la que se escribe el día de la semana. No tiene valor por que se asigna más adelante
        int linea;
        // La casilla de salida establece dónde está el día 1 de cada mes
        int casilla_salida = dweek(ano,mes,dia)-1;
        
        // Recorre todos los días del mes para dibujar el calendario
        for (int i = 0; i < numero_de_dias(mes,ano)+casilla_salida; ++i) {
            
            // Se establecen las posiciones dependiendo del valor de i (de las casillas del calendario)
            if (i > -1 && i < 7) linea = 51;
            if (i > 6 && i<14) linea = 71;
            if (i > 13 && i<21) linea = 91;
            if (i > 20 && i<28) linea = 111;
            if (i > 27 && i<35) linea = 131;
            if (i > 34) linea = 51;
            
            // Si es la última columna, vuelve al principio
            if (pos>122) pos = 2;
            
            // El año y el mes son los dos primeros valores de la matrix TURNOS
            // Se revisa toda la matriz (con un bucle) buscando si el mes y el año seleccionado tiene un calendario asignado
            
            // Se pasa el año y el mes a string
            char temp_ano[10];
            snprintf(temp_ano, 10, "%i",ano);
            char temp_mes[10];
            snprintf(temp_mes, 10, "%i",mes);
            // Se establece posicion_turno en un valor alto. Si sigue en valor alto, significa que no existe turno para
            // ese mes y ese año
            int posicion_turno=99;
            
            // Se inicia el bucle de búsqueda
            for (int bucle_matrix=0;bucle_matrix<MESES_TURNOS;bucle_matrix++)
            {
                // Se comparan las primeras posiciones de la matriz TURNOS para ver si el año y el mes existen
                if ((turnos[bucle_matrix][0] == ano) && (turnos[bucle_matrix][1] == mes))
                    // Si existe, se guarda la posicion de ese turno con respecto a la matriz (para dibujarlo luego)
                    posicion_turno = bucle_matrix;
            }
            // Si no se han encontrado coincidencias (valor 99), se evita pintar el turno poniendo chkturnos a 0
            if (posicion_turno==99) chkturnos = 0;
            
            // En este if se mira cual es la casilla de salida y se pinta a partir de ahí.
            if (i > casilla_salida-1)
            {
                // Se convierte el valor de i menos el de la casilla de salida (menos un ajuste) a string para pintarlo
                // en el calendario
                char str_dias[10];
                snprintf(str_dias, 10, "%i",i-casilla_salida+1);
                
                char str_turnos[10];
                snprintf(str_turnos, 10, "%i",turnos[posicion_turno][i-casilla_salida+2]);
                // A continuación se comprueba si el día del bucle es el día actual. En caso afirmativo, se pinta
                // ese día en negrita
                if (((i-casilla_salida+1)==dia_actual) && (mes==mes_actual))
                    graphics_draw_text(ctx, (chkturnos==1)?nombre_turno[turnos[posicion_turno][i-casilla_salida+2]]:str_dias, fonts_get_system_font(FUENTE_BOLD), GRect(pos, linea, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
                else
                    graphics_draw_text(ctx, (chkturnos==1)?nombre_turno[turnos[posicion_turno][i-casilla_salida+2]]:str_dias, fonts_get_system_font(FUENTE), GRect(pos, linea, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
            }
            // Se suma 20 a pos para saltar al siguiente cuadro y se continúa el bucle
            pos = pos+20;
            
        }
    }  // OJO, este es el cierre del if de cargando
    
}  // Y termina la función

// Se pulsa el botón arriba
static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
    mes--;
    if (mes==0)
    {
        mes = 12;
        ano--;
    }
    layer_mark_dirty(CapaLineas);
    // Se resta un mes al actual y si el mes es inferior a 1, se resta un año
}

void long_select_click_handler(ClickRecognizerRef recognizer, void *context) {
    cargando=1;
    send_int(5, 5);
    layer_mark_dirty(CapaLineas);
    
}

// Se pulsa el botón select
void select_click_handler(ClickRecognizerRef recognizer, void *context) {
    if (chkturnos==1)
        chkturnos=0;
    else
        chkturnos=1;
    layer_mark_dirty(CapaLineas);
    
    // Se usa el select para cambiar entre calendario normal y de turnos
}

// Se pulsa el botón abajo
void down_click_handler(ClickRecognizerRef recognizer, void *context) {
    mes++;
    if (mes==13)
    {
        mes = 1;
        ano++;
    }
    layer_mark_dirty(CapaLineas);
    // Se suma un mes al actual, y si supera el 12, se suma un año
}

// Se definen las funciones asociadas a la pulsación de botones
void click_config_provider(void *context) {
    window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
    window_long_click_subscribe(BUTTON_ID_SELECT, 1000 , long_select_click_handler, NULL);
    window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void init()
{
    window = window_create();
    window_set_click_config_provider(window, click_config_provider);
    //Register AppMessage events
    app_message_register_inbox_received(in_received_handler);
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    //Largest possible input and output buffer sizes
    
    
    window_stack_push(window, true /* Animado */);
    window_set_background_color(window, COLOR_FONDO);
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    
    
    
    //Se añade la CapaLineas
    CapaLineas = layer_create(bounds);
    layer_set_update_proc(CapaLineas, CapaLineas_update_callback); 
    layer_add_child(window_layer, CapaLineas); 
    
    
    //funcion para saber el día, el mes y el año actual
    time_t now = time(NULL);
    struct tm *tick_time = localtime(&now);  
    dia=1;
    dia_actual = tick_time->tm_mday;
    mes = tick_time->tm_mon+1;
    mes_actual = mes;
    ano = tick_time->tm_year+1900;
    
    // Se establece chkturnos a 1 para mostrar el calendario de turnos en primer lugar. Si es 0, se muestran los días
    chkturnos=1;
    
    // Línea de DEBUG, por si acaso. Debe estar desactivada siempre que sea posible
    //APP_LOG(APP_LOG_LEVEL_DEBUG, "El primer dia del mes %i, del año %i es %i", mes, ano, dweek(ano,mes,dia));
 
    carga_datos();
}



static void deinit() 
{
    
    //Al cerrar la aplicación, matamos las capas y desactivamos los procesos
    
    layer_destroy(CapaLineas);
    
    // Y por último se borra de la memoria la ventana principal
    window_destroy(window);
}

int main(void) 
{
    init();
    app_event_loop();
    deinit();
}