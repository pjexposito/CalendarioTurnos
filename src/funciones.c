#include <pebble.h>
#include "funciones.h"
  
// Este código no es mío, así que poco puedo comentar. El caso es que funciona perfectamente.
// Sacado de: http://www.codecodex.com/wiki/Calculate_the_number_of_days_in_a_month#C.2FC.2B.2B  
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

// De nuevo, este código no es mío. Tan sólo he hecho una pequeña variación sobre el código encontrado
// en: http://stackoverflow.com/questions/15127615/determining-day-of-the-week-using-zellers-congruence
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


void send_int(int key, int cmd)
{
	DictionaryIterator *iter;
 	app_message_outbox_begin(&iter);
 	
 	Tuplet value = TupletInteger(key, cmd);
 	dict_write_tuplet(iter, &value);
 	
 	app_message_outbox_send();
}

char* subString (const char* input, int offset, int len, char* dest)
{
  int input_len = strlen (input);
  if (offset + len > input_len)
  {
     return NULL;
  }
  strncpy (dest, input + offset, len);
  return dest;
}

void anade_datos(const char* input, int mes)
{
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
        if (strcmp(username, "")!=0)
          anade_datos(username, x);
      }

    }
}
