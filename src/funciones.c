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

