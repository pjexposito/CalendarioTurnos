#!/usr/bin/python
# -*- coding: utf-8 -*-

#Código sucio y tal, pero es mi primer programa en Python :D

import sqlite3 as lite
import sys
import calendar



con = lite.connect('ShiftCal.db')
cadena = ""
calendarios = 0

with con:
    
    cur = con.cursor()
    # Se seleccionan dos tablas (calendar y shifts), se unen y se ordenan por fecha
    cur.execute("SELECT * FROM calendar,shifts WHERE calendar.cal_shift_id1 = shifts.shift_rowid ORDER BY calendar.cal_date")
    #cur.execute("SELECT * FROM calendar")
    
    rows = cur.fetchall()
    ano = ""
    mes = ""
    salto_ahora = 0
    # Se recorre toda la tabla
    for row in rows:
        este_mes = row[1][4:6]
        # Si el mes empieza por cero, se borra el cero
        if este_mes[0:1] == "0":
            este_mes = este_mes[1:2]
        # Se guarda el año, el día y el turno también en una variable
        este_ano = row[1][:4]
        este_dia = row[1][6:8]
        este_turno = row[19]
        # Se discrimina el año, para saber si ya se ha usado o no
        if ano == este_ano:
            # De nuevo, se discrimina por meses
            if mes == este_mes:
                cadena = cadena + "\"" + este_turno + "\", "
                # Se suma uno a la variable salto_ahora. Si es 7, se mete un salto de carro, para que se lea mejor la array
                salto_ahora = salto_ahora + 1
                if salto_ahora == 7:
                    cadena = cadena + "\n"
                    # Al meter el salto, se vuelve la variable a 0
                    salto_ahora = 0
            else:
                mes = este_mes
                # Con esto, se termina el turno. Por eso, se añade 1 a la variable calendario. Se cierra ese turno y se abre otro
                calendarios = calendarios+1
                cadena = cadena[:len(cadena)-2] + "}, \n" + "{\"" + ano +"\", \"" + mes + "\", \n\"" + este_turno + "\", "
                # Salto se establece en 1 por que se ha metido un turno en la línea anterior
                salto_ahora = 1
        else:
            ano = este_ano
            mes = este_mes
            # De nuevo, como en el punto anterior, se cierra el turno y se añade 1 a calendario
            calendarios = calendarios+1
            cadena = cadena[:len(cadena)-2] + "}, \n" +  "{\"" + ano +"\", \"" + mes + "\", \n\"" + este_turno + "\", "
            # Salto se establece en 1 por que se ha metido un turno en la línea anterior
            salto_ahora = 1

# Se sale del bucle general. Empezamos a cerrar cosas
# Se eliminan dos carácteres de la cadena, que se han metido en el punto anterior (como dije, es código sucio).
# Luego, se añade un salto de carro para dejarlo más legible
cadena = cadena[3:len(cadena)-2] + "}, \n};"
# Se mete la definición de la variable y esas cosas
cadena = "static const char *turnos[MESES_TURNOS][33] = {" + cadena

# Y aquí ya se imprime todo. Primero el define y el se cuentan todos los calendarios que suman el array
#print ("#define MESES_TURNOS "+ str(calendarios) + "\n")

# Y para terminar, se pinta el array
#print (cadena)

# Se crea el archivo y se pinta todo
print ("Creando archivo...")
file = open("matrizturnos.h", "w")
file.write("#define MESES_TURNOS "+ str(calendarios) + "\n" +"\n")
file.write(cadena+"\n")
print ("Archivo \"matrizturnos.h\" creado.")


# Todo lo siguiente es código de depuración que no sirve para nada. Por eso se queda así.
#print row[1][4:6], row[1][6:8], row[18], row[19]
#print(calendar.month(2015,01))
#print(calendar.monthrange(2015,1)[1])

