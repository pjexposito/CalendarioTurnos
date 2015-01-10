#include "pebble.h"
#include "funciones.h"


int cargando=0;

static const char *nombre_turno[9] =
{"vacio", "M", "T", "AA", "AT", "L", "FM", "FT", "D"};



// Matriz básica para transformar el número de mes en el nombre del mes.
static const char *nombre_mes[13] =
{ "vacio", "enero", "febrero", "marzo", "abril", "mayo", "junio", "julio", "agosto", "septiembre", "octubre",
    "noviembre", "diciembre" };



// Ventana principal
static Window *window;


//Capas del reloj
Layer *CapaLineas; // La capa principal donde se dibuja el calendario





void process_tuple(Tuple *t)
{
    int key = t->key;
    char string_value[64];
    memset(string_value, 0, 64);
    strcpy(string_value, t->value->cstring);
    anade_datos(string_value, key);
    persist_write_string(key, string_value);
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
        // Se pinta la cabecera con los días de la semana
        graphics_draw_text(ctx, "L", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(2, 27, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, "M", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(22, 27, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, "X", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(42, 27, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, "J", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(62, 27, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, "V", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(82, 27, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, "S", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(102, 27, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        graphics_draw_text(ctx, "D", fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(122, 27, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        
        // Pinta el nombre del mes
        graphics_draw_text(ctx, nombre_mes[mes], fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(0, 5, 72, 10), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        
        // Pasa el int del año a letra y pinta el año
        char temp[10]  = "";
        snprintf(temp, sizeof(temp), "%d", ano);
        graphics_draw_text(ctx, temp, fonts_get_system_font(FUENTE_GRANDE_BOLD), GRect(73, 5, 72, 10), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        
        // Pos es la columna en dónde se escribe el día de la semana.
        // Se inicia en 2 para centrar el dato en el cuadro del calendario
        int pos = 2;
        // Linea (fila) en la que se escribe el día de la semana. No tiene valor por que se asigna más adelante
        int linea;
        // La casilla de salida establece dónde está el día 1 de cada mes
        int casilla_salida = dweek(ano,mes,dia)-1;
        // nueva_fila indica si el mes tiene 5 o 6 filas. Si es igual a 0, tiene 5, si es 1, tiene 6.
        int nueva_fila = 0;
        
        // Recorre todos los días del mes para dibujar el calendario
        for (int i = 0; i < numero_de_dias(mes,ano)+casilla_salida; ++i) {
            
            // Se establecen las posiciones dependiendo del valor de i (de las casillas del calendario)
            if (i > -1 && i < 7) linea = 47;
            if (i > 6 && i<14) linea = 64;
            if (i > 13 && i<21) linea = 81;
            if (i > 20 && i<28) linea = 98;
            if (i > 27 && i<35) linea = 115;
            if (i > 34)
            {
                // Hay que añadir una nueva línea a la tabla
                nueva_fila = 1;
                linea = 132;
            }

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
                    graphics_draw_text(ctx, (chkturnos==1)?nombre_turno[turnos[posicion_turno][i-casilla_salida+2]]:str_dias, fonts_get_system_font(FUENTE_BOLD), GRect(pos, linea-3, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
                else
                    graphics_draw_text(ctx, (chkturnos==1)?nombre_turno[turnos[posicion_turno][i-casilla_salida+2]]:str_dias, fonts_get_system_font(FUENTE), GRect(pos, linea-3, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
            }
            // Se suma 20 a pos para saltar al siguiente cuadro y se continúa el bucle
            pos = pos+20;
            
        }
        // Se pintan las líneas de calendario
        
        // Lineas horizontales
        for (int x=0; x<6+nueva_fila; x++)
            graphics_fill_rect(ctx, GRect(2, LINEA_HORIZONTAL_INICIAL+(x*LINEA_HORIZONTAL), 140, 1), 0, GCornerNone);
        graphics_fill_rect(ctx, GRect(2, LINEA_HORIZONTAL_INICIAL+((6+nueva_fila)*LINEA_HORIZONTAL), 140, 2), 0, GCornerNone);
        
        
        // Lineas verticales
        for (int x=0; x<8;x++)
            graphics_fill_rect(ctx, GRect(LINEA_VERTICAL_INICIAL+(x*LINEA_VERTICAL), 30, 1, ((6+nueva_fila)*LINEA_HORIZONTAL)+2), 0, GCornerNone);

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
    for (int x=0;x<MESES_TURNOS;x++)      
      persist_delete(x);
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