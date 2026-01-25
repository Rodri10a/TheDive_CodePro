'''
JUEGO DEL LABERINTO EL RATON VS EL GATO (con minimax)
El gato intenta acercarse al raton (minimiza), y el raton intenta escapar (maximiza)
'''
import random 


def mostrar (tablero):
    for fila in tablero: 
        print ("".join(fila)) 
        
        
        
# distancia manhattan
def manhattan (tablero,xr,yr,xg,yg):  
    distancia = abs(xr - xg) + abs(yr - yg)  
    return distancia 


def movimientos (x,y, dimension):
    movimientos = []  
    posibles_mov = [(-1,0),(1,0),(0,-1),(0,1)] 
    for mov_x , mov_y in posibles_mov: 
        mov_x = x + mov_x 
        mov_y = y + mov_y 
        if 0 <= mov_x < dimension and 0 <= mov_y < dimension and tablero[mov_x][mov_y] != "⬛": 
            movimientos.append((mov_x,mov_y))
    return movimientos 

# Minimax 
def gato (xg,yg,xr,yr, tablero, profundidad, max_turnos, dimension): 
    # BUSCA FINALIZAR MANHATTAN Y TERMINA 
    if profundidad == 0 or (xg == xr and yg == yr): # CASO BASE PARA LA RECURSIVIDAD 
        distancia = manhattan (tablero, xr, yr, xg, yg) 
        return distancia, (xg,yg) 
    
    
    #MINIMIZA LA DISTANCIA
    #JUEGA EL GATO
    if max_turnos:
        minimo = float ("inf") 
        best_mov = (xg,yg)    
        mov_permitidos = movimientos (xg,yg, dimension)
        
        #RECORRE TODOS LOS MOVIMIENTOS Y BUSCA LA MENOR DISTANCIA 
        for mov_x, mov_y in mov_permitidos: 
            distancia, _ = gato (mov_x, mov_y,xr,yr,tablero,profundidad -1, False, dimension) 
            if distancia < minimo: 
                minimo = distancia 
                best_mov = (mov_x,mov_y)        
        return minimo, best_mov # devuelve posicion mas cerca al raton 
    
    #MAXIMIZA LA DISTANCIA 
    #JUEGA EL RATON 
    else:
        #maximixo la distancia 
        maximo = -float ("inf")
        worst_mov = (xr,yr)
        mov_permitidos = movimientos (xr,yr, dimension)
        
        #RECORRE TODOS LOS MOVIMIENTOS Y BUSCA LA MAYOR DISTANCIA 
        for mov_x, mov_y in mov_permitidos:
            distancia, _ = gato (xg,yg,mov_x,mov_y,tablero,profundidad -1, True, dimension) # si pongo true indica que es el turno del gato ahora 
            if distancia > maximo: 
                maximo = distancia  
                worst_mov = (mov_x,mov_y)        
        return maximo, worst_mov # devuelve posicion mas alejada al raton
  
    # EL MINIMAX ELIGE LA JUGADA QUE PERMITE MEJORES POSICIONES FUTURAS
    
turno = 0

def raton(tablero,xr,yr,turno):
    while True: 
        movs = input("1- w = Arriba\n2- a = Izquierda\n3- s = Abajo\n4- d = Derecha\nIngrese el movimiento del raton: ").lower() 
        turno = turno + 1 
        if movs == "w" and xr -1 >=0 and tablero[xr-1][yr] != "⬛":
            tablero[xr][yr] = "⬜"
            xr -= 1 
            tablero[xr][yr] = "🐭"
            print(f"Es el turno {turno}. Suerte tu puedes !! 🎉 ")
            break 
        elif movs == "a" and yr -1 >= 0 and tablero[xr][yr-1] != "⬛":
            tablero[xr][yr] = "⬜"
            yr -= 1 
            tablero[xr][yr] = "🐭"
            print(f"Es el turno {turno}. Suerte tu puedes !! 🎉 ")
            break 
        elif movs == "s" and xr + 1 < dimension and tablero[xr+1][yr] != "⬛":
            tablero[xr][yr] = "⬜"
            xr += 1 
            tablero[xr][yr] = "🐭" 
            print(f"Es el turno {turno}. Suerte tu puedes !! 🎉 ")
            break
        elif movs == "d" and yr + 1 < dimension and tablero[xr][yr+1] != "⬛":
            tablero[xr][yr] = "⬜"
            yr += 1
            tablero[xr][yr] = "🐭"
            print(f"Es el turno {turno}. Suerte tu puedes !! 🎉 ")
            break 
        else:
            print ("Movimiento no valido. Intente de nuevo")
            
    return tablero, xr,yr, turno 


# configuracion inicial del tablero 
dimension = 8 
tablero = [["⬜" for _ in range(dimension)] for _ in range(dimension)] 
Gato =  "😺"
Raton =  "🐭"
xr, yr = 0,0 
xg, yg = 7,0
tablero [xr][yr] =  Raton
tablero [xg][yg] = Gato
meta_x, meta_y = 7,7
tablero[meta_x][meta_y] = "🏁"

#coloca muros en el tablero
def muro_aleatorio (tablero, cantidad,xr,yr,xg,yg): 
    for _ in range(cantidad):
        x = random.randint (0, dimension -1)
        y = random.randint (0, dimension -1)
        
        if tablero[x][y] == "⬜" and (x,y) not in [(xr,yr), (xg,yg)]: # solo se puede colocor un muro si no esta encima de el raton o el gato 
            tablero [x][y] = "⬛" 
muro_aleatorio (tablero,15,xr,yr,xg,yg) 


#bucle hasta que termine el juego 
while True: # repite hasta que el movimiento sea valido 
    print ("\nTurno del gato 😺\n")
    _,(new_xg, new_yg)= gato (xg,yg,xr,yr,tablero,3,True,dimension) # la raiz 
    # estado inicial donde se generan todos los movimientos del gato 
    tablero[xg][yg] = "⬜"
    xg, yg = new_xg, new_yg
    tablero[xg][yg] = Gato 
    mostrar(tablero)
    
    if (xr,yr)== (xg,yg) :
        print("El gato te ha atrapado. Lo siento, haz perdido 😢😢")
        break 
    
    print ("\nTurno del raton 🐭\n")
    tablero, xr, yr, turno = raton(tablero, xr, yr, turno)
    mostrar(tablero)
    
    if (xr,yr)== (xg,yg) :
        print("El gato te ha atrapado. Lo siento, haz perdido 😢😢")
        break 
    elif (xr, yr) == (meta_x, meta_y):
        print("¡El ratón llegó a la meta 🏁🐭\nFELICIDADES HAZ GANADO !! ✨✨")
        break

