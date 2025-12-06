
#include <iostream> //imprime por consola 
#include <vector> // matriz del tablero
#include <queue> // BFS 
#include <random> // numeros aleatorios 
#include <ctime> // toma la hora actual 
#include <map>  // guardar padres 
#include <algorithm> // shuffle 

using namespace std; 

//  Inicia los numeros aleatorios con la hora actual, con los segundos 
mt19937 tablero_aleatorio (time(0)); // si uso otro valor que no sea el 0 siempre tendria el mismo laberinto


int filas, columnas; 

/*
- PUNTERO = es una variable que almacena la direccion de memoria de otra variable

*/

//estructura de una celda
struct Celda {

    bool celda_visitada=false;     
    bool pared_superior=true;      
    bool pared_inferior=true;       
    bool pared_izquierda=true;      
    bool pared_derecha=true;       

};


// FUNCION PARA CREAR EL LABERINTO CON RECURSIVIDAD (DFS Y BACKTRACKING)
void generar_laberinto(vector<vector<Celda>> &laberinto, int fil, int col) {
    laberinto[fil][col].celda_visitada = true;

    vector<pair<int, int>> direcciones = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    //mezcla aleatoriamente las direcciones para crear laberintos
    shuffle(direcciones.begin(), direcciones.end(), tablero_aleatorio); 
    /*
    SHUFFLE = es para mezclar o barajar todas las  
    Se usan begin y end porque shuffle necesita saber donde empezar a mezclar y donde terminar de mezclar
    BEGIN = donde empieza
    END = donde termina 
    y gracias a esto mezcla todas las dirreciones del primero hasta el ultimo 
    */
    // intenta moverse en cada direccion
    for (const auto& dir : direcciones) {
        int nueva_fila = fil + dir.first;
        int nueva_columna = col + dir.second;

        // si la celda vecina esta dentro de los límites y no ha sido visitada
        if (nueva_fila >= 0 && nueva_fila < filas && nueva_columna >= 0 && nueva_columna < columnas && !laberinto[nueva_fila][nueva_columna].celda_visitada) {
            
            //elimina las paredes entre la celda actual y la vecina
            if (dir.first == 1) {
                laberinto[fil][col].pared_inferior = false;
                laberinto[nueva_fila][nueva_columna].pared_superior = false;
            } else if (dir.first == -1) {
                laberinto[fil][col].pared_superior = false;
                laberinto[nueva_fila][nueva_columna].pared_inferior = false;
            } else if (dir.second == 1) {
                laberinto[fil][col].pared_derecha = false;
                laberinto[nueva_fila][nueva_columna].pared_izquierda = false;
            } else if (dir.second == -1) {
                laberinto[fil][col].pared_izquierda = false;
                laberinto[nueva_fila][nueva_columna].pared_derecha = false;
            }
            //llama recursivamente para continuar generando el laberinto desde la celda vecina
            generar_laberinto(laberinto, nueva_fila, nueva_columna);
        }
    }
}

//función para imprimir el laberinto en consola
//si se pasa una ruta, marca el camino con '*'
void imprimir_laberinto(const vector<vector<Celda>> &laberinto, const vector<pair<int,int>>* ruta = nullptr) {
                                // EL CONST ES PARA NO MODIFICAR EL LABERINTO Y SE USA ESTE SIGNO (&) PARA DECIR 
                                // QUE RECIBE EL LABERINTO YA HECHO Y NO LO COPIA Y NO LO MODIFICA PORQUE ESTA EL CONST 
    for (int i = 0; i < filas; ++i) {
        //imprime la fila superior de las celdas
        for (int j = 0; j < columnas; ++j) {
            cout << "+";
            if(laberinto[i][j].pared_superior){
                cout<< "---";
            }else{
                cout<< "   ";
            }
        }
        cout << "+" << endl;

        //imprime los laterales y el contenido de cada celda
        for (int j = 0; j < columnas; ++j) {
            if(laberinto[i][j].pared_izquierda){
                cout<< "|";
            }else{  
                cout<< " ";
            }
            if (i == 0 && j == 0)
                cout << " E "; // entrada
            else if (i == filas - 1 && j == columnas - 1)
                cout << " S "; // salida
            else if (ruta && find(ruta->begin(), ruta->end(), make_pair(i,j)) != ruta->end())
                cout << " * "; // camino de la rta
            else
                cout << "   "; // celda vacia
        }
        cout << "|" << endl;
    }
    //imprime la fila inferior del laberinto
    for (int j = 0; j < columnas; ++j) {
        cout << "+---";
    }
    cout << "+" << endl;
}

// Algoritmo BFS para encontrar la ruta más corta desde la entrada hasta la salida
// Devuelve un vector con las coordenadas de la ruta
vector<pair<int,int>> resolver_laberinto_BFS(const vector<vector<Celda>> &laberinto) {
    vector<vector<bool>> visitado(filas, vector<bool>(columnas, false));
    map<pair<int,int>, pair<int,int>> padre;
    queue<pair<int,int>> cola;
    vector<pair<int,int>> movimientos = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    cola.push({0,0});
    visitado[0][0] = true;
    

    while(!cola.empty()) {
        auto actual = cola.front();
        cola.pop();
        int f = actual.first, c = actual.second;
        // Si llegamos a la salida, terminamos
        if(f == filas-1 && c == columnas-1) {
            break;
        }
        // Explora las celdas vecinas
        for(const auto &mov : movimientos) {
            int nf = f + mov.first, nc = c + mov.second;
            if(nf < 0 || nf >= filas || nc < 0 || nc >= columnas) {
                continue;
            }
            if(visitado[nf][nc]) {
                continue;
            }
            // Verifica que no haya pared en la dirección del movimiento
            if(mov == make_pair(1,0) && laberinto[f][c].pared_inferior) {
                continue;
            }
            if(mov == make_pair(-1,0) && laberinto[f][c].pared_superior) {
                continue;
            }
            if(mov == make_pair(0,1) && laberinto[f][c].pared_derecha) {
                continue;
            }
            if(mov == make_pair(0,-1) && laberinto[f][c].pared_izquierda) {
                continue;
            }
            visitado[nf][nc] = true;
            padre[{nf,nc}] = {f,c};
            cola.push({nf,nc});
        }
    }
    // Reconstruye la ruta desde la salida hasta la entrada usando el mapa de padres
    vector<pair<int,int>> ruta;
    pair<int,int> actual = {filas-1, columnas-1};
    while(actual != make_pair(0,0)) {
        ruta.push_back(actual);
        actual = padre[actual];
    }
    ruta.push_back({0,0});
    reverse(ruta.begin(), ruta.end());
    return ruta;
}

// Función principal
int main() {
    filas = 6;
    columnas = 6;

    // Crea el laberinto vacío
    vector<vector<Celda>> laberinto(filas, vector<Celda>(columnas));
    // Genera el laberinto aleatoriamente
    generar_laberinto(laberinto, 0, 0);

    // Imprime el laberinto generado (sin ruta)
    cout << "Laberinto generado:\n";
    imprimir_laberinto(laberinto);

    // Resuelve el laberinto y muestra el camino con '*'
    cout << "\nLaberinto resuelto (camino con *):\n";
    vector<pair<int,int>> ruta = resolver_laberinto_BFS(laberinto);
    imprimir_laberinto(laberinto, &ruta);

    return 0;
}

