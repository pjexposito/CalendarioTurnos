#!/usr/bin/python
# -*- coding: utf-8 -*-

import sqlite3 as lite
import sys
import calendar



con = lite.connect('ShiftCal.db')
cadena = ""
calendarios = 0

with con:    
    
    cur = con.cursor()    
    cur.execute("SELECT * FROM calendar,shifts WHERE calendar.cal_shift_id1 = shifts.shift_rowid ORDER BY calendar.cal_date")
    #cur.execute("SELECT * FROM calendar")

    rows = cur.fetchall()
    ano = ""
    mes = ""

    for row in rows:
        este_mes = row[1][4:6]
        este_ano = row[1][:4]
        este_dia = row[1][6:8]
        este_turno = row[19] 
        if ano == este_ano:
          if mes == este_mes:
            cadena = cadena + "\"" + este_dia + "_" + este_turno + "\", "
          else:
            mes = este_mes
            calendarios = calendarios+1
            cadena = cadena[:len(cadena)-2] + "}, " + "{\"" + ano +"\", \"" + mes + "\", \""+ este_dia + "_" +este_turno + "\", "
        else:
          ano = este_ano
          mes = este_mes
          cadena = cadena[:len(cadena)-2] + "}, " +  "{\"" + ano +"\", \"" + mes + "\", \"" + este_dia + "_" + este_turno + "\", "
          calendarios = calendarios+1


cadena = cadena[3:len(cadena)-2] + "}, };"	
cadena = "static const char *turnos[MESES_TURNOS][33] = {" + cadena	  
print ("#define MESES_TURNOS "+ str(calendarios))

print (cadena)
#print row[1][4:6], row[1][6:8], row[18], row[19]
#print(calendar.month(2015,01))
#print(calendar.monthrange(2015,1)[1])
