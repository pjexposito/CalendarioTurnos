#include "pebble.h"
  
#define COLOR_PRINCIPAL GColorWhite  // El color del lápiz es blanco
#define COLOR_FONDO GColorBlack  // y el fondo, negro

#define MESES_TURNOS 3

static Window *window;


static const char *dias_en_numero[31] = 
	{ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", 
   "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", 
   "21", "22", "23", "24", "25", "26", "27", "28", "29", "30",
   "31"};

static const char *turnos[MESES_TURNOS][33] = {
	{ "2015", "1",
    "L", "T", "AT", "FT", "AM", "L", "AM", 
    "M", "L", "L", "FM", "T", "T", "T", 
    "T", "T", "AT", "D", "AM", "M", "M",
    "M", "M", "M", "D", "T", "T", "T", 
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
};

static const char *nombre_mes[13] = 
	{ "vacio", "enero", "febrero", "marzo", "abril", "mayo", "junio", "julio", "agosto", "septiembre", "octubre", 
   "noviembre", "diciembre" };

int dia, mes, ano, mes_actual, dia_actual, chkturnos;

//Capas del reloj
Layer *CapaLineas; // La capa principal donde se dibuja el calendario


int dweek(int year, int month, int day)
   {
     int h,q,m,k,j;
  static int conv[] = {6,7,1,2,3,4,5,6};
    if(month == 1)
    {
    month = 13;
    year--;
    }
    if (month == 2)
    {
      month = 14;
      year--;
    }
    q = day;
    m = month;
    k = year % 100;
    j = year / 100;
    h = q + 13*(m+1)/5 + k + k/4 + j/4 + 5*j;
    h = h % 7;
    return conv[h];
   }
 
int numero_de_dias(int month, int year)
  {
  
  int numberOfDays;  
  if (month == 4 || month == 6 || month == 9 || month == 11)  
  numberOfDays = 30;  
  else if (month == 2)  
  { 
    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);  
    if (isLeapYear)  
      numberOfDays = 29;  
    else  
      numberOfDays = 28;  
  }  
  else  
    numberOfDays = 31; 
  return numberOfDays;
  }


// Esta función se ejecutará cada vez que se refresque la CapaLineas
void CapaLineas_update_callback(Layer *me, GContext* ctx) 
{


  // Color del fondo y color del trazo
    graphics_context_set_stroke_color(ctx, COLOR_PRINCIPAL);
    graphics_context_set_fill_color(ctx, COLOR_PRINCIPAL);

    // left, top, anchura, altura
    // 144x168
  
    graphics_fill_rect(ctx, GRect(2, 30, 140, 2), 0, GCornerNone);  

    graphics_fill_rect(ctx, GRect(2, 50, 140, 2), 0, GCornerNone);  
    graphics_fill_rect(ctx, GRect(2, 70, 140, 1), 0, GCornerNone);  
    graphics_fill_rect(ctx, GRect(2, 90, 140, 1), 0, GCornerNone);  
    graphics_fill_rect(ctx, GRect(2, 110, 140, 1), 0, GCornerNone);  
    graphics_fill_rect(ctx, GRect(2, 130, 140, 1), 0, GCornerNone);  

    graphics_fill_rect(ctx, GRect(2, 150, 142, 2), 0, GCornerNone);  
  
  
    graphics_fill_rect(ctx, GRect(2, 30, 1, 120), 0, GCornerNone); 

    graphics_fill_rect(ctx, GRect(22, 30, 1, 120), 0, GCornerNone); 
    graphics_fill_rect(ctx, GRect(42, 30, 1, 120), 0, GCornerNone); 
    graphics_fill_rect(ctx, GRect(62, 30, 1, 120), 0, GCornerNone); 
    graphics_fill_rect(ctx, GRect(82, 30, 1, 120), 0, GCornerNone); 
    graphics_fill_rect(ctx, GRect(102, 30, 1, 120), 0, GCornerNone); 
    graphics_fill_rect(ctx, GRect(122, 30, 1, 120), 0, GCornerNone); 
  
    graphics_fill_rect(ctx, GRect(142, 30, 1, 120), 0, GCornerNone); 


		graphics_draw_text(ctx, "L", fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(2, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
		graphics_draw_text(ctx, "M", fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(22, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
		graphics_draw_text(ctx, "X", fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(42, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
		graphics_draw_text(ctx, "J", fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(62, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
		graphics_draw_text(ctx, "V", fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(82, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
		graphics_draw_text(ctx, "S", fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(102, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
 		graphics_draw_text(ctx, "D", fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(122, 28, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
    
  	graphics_draw_text(ctx, nombre_mes[mes], fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(0, 0, 80, 10), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);
    char temp[10]  = "";
    snprintf(temp, sizeof(temp), "%d", ano);
    	graphics_draw_text(ctx, temp, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD), GRect(90, 0, 50, 10), GTextOverflowModeTrailingEllipsis, GTextAlignmentLeft, NULL);

    int pos = 2;
    int linea;
    int casilla_salida = dweek(ano,mes,dia)-1;

    for (int i = 0; i < numero_de_dias(mes,ano)+casilla_salida; ++i) {
      
      
      if (i > -1 && i < 7) linea = 51;
      if (i > 6 && i<14) linea = 71;
      if (i > 13 && i<21) linea = 91;
      if (i > 20 && i<28) linea = 111;
      if (i > 27 && i<35) linea = 131;
      if (i > 34) linea = 51;

      if (pos>122) pos = 2;
      
      
      // Para encontrar el turno correcto busco en toda la matrix TURNOS
      /*
      int posicion_turno=99;
      for (int bucle_matrix=0;bucle_matrix<MESES_TURNOS;bucle_matrix++)
        {
        if (((int)turnos[bucle_matrix][0] == ano) && ((int)turnos[bucle_matrix][1] == mes))
          posicion_turno = bucle_matrix;
        }
      
      if (posicion_turno==99) chkturnos = 0;
      */
      
      if (mes>MESES_TURNOS) chkturnos = 0;

      
 		  if (i > casilla_salida-1)
        {
        // Sumo 2 a casilla_salida por que los dos primeros espacios son de año y mes
        
        if (((i-casilla_salida+1)==dia_actual) && (mes==mes_actual))
          graphics_draw_text(ctx, (chkturnos==1)?turnos[mes-1][i-casilla_salida+2]:dias_en_numero[i-casilla_salida], fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD), GRect(pos, linea, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        else
          graphics_draw_text(ctx, (chkturnos==1)?turnos[mes-1][i-casilla_salida+2]:dias_en_numero[i-casilla_salida], fonts_get_system_font(FONT_KEY_GOTHIC_14), GRect(pos, linea, 20, 20), GTextOverflowModeTrailingEllipsis, GTextAlignmentCenter, NULL);
        }
      pos = pos+20;

  }

}  // Y termina la función


static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
      mes--;
      if (mes==0) 
        {
        mes = 12;
        ano--;
      }
  layer_mark_dirty(CapaLineas);
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if (chkturnos==1)
    chkturnos=0;
  else
    chkturnos=1;
  layer_mark_dirty(CapaLineas);
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
      mes++;
      if (mes==13) 
        {
        mes = 1;
        ano++;
      }
  layer_mark_dirty(CapaLineas);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);


}

static void init() 
{
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);

  window_stack_push(window, true /* Animado */);
  window_set_background_color(window, COLOR_FONDO);
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  
 
  
  //Se añade la CapaLineas
  CapaLineas = layer_create(bounds);
  layer_set_update_proc(CapaLineas, CapaLineas_update_callback); 
  layer_add_child(window_layer, CapaLineas); 
  
  
  //funcion para saber el día de la semana
  time_t now = time(NULL);
  struct tm *tick_time = localtime(&now);  
  dia=1;
  dia_actual = tick_time->tm_mday;
  mes = tick_time->tm_mon+1;
  mes_actual = mes;
  ano = tick_time->tm_year+1900;
  chkturnos=1;
  APP_LOG(APP_LOG_LEVEL_DEBUG, "El primer dia del mes %i, del año %i es %i", mes, ano, dweek(ano,mes,dia));    
  for (int bucle_matrix=0;bucle_matrix<MESES_TURNOS;bucle_matrix++)
   {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Para año %i el valor es %i. Para mes %i, el valor es %i",(int)turnos[bucle_matrix][0],ano,(int)turnos[bucle_matrix][1],mes);
  }

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
