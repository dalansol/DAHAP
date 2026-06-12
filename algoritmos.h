// Funciones de algoritmos para la Situacion Problema 2: servicios de Internet para ISP.
// Implementa: Arbol de expansion minima (Prim), Ruta mas corta TSP (fuerza bruta),
//             Flujo maximo (Edmonds-Karp), Central mas cercana (distancia euclidiana).
// Autores:
//      Adan Gonzalez Flores | A00841625
//      Patricio Javier Garza Rios | A00841942
//      Daniela Landin Solis | A00841757
//      Alana Martinez Morales | A00841524
// Fecha: 10 de junio 2026

#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <vector>
#include <climits>
#include <algorithm>
#include <queue>
#include <cmath>
#include <string>
#include <stdexcept>

// Estructura que representa un punto en un plano coordenado bidimensional
struct Punto {
    double x = 0.0;
    double y = 0.0;
};

// Estructura que almacena una arista del arbol de expansion minima con sus nodos y peso
struct AristaMst {
    std::string nodo1 = "";
    std::string nodo2 = "";
    int peso = 0;
};

// Estructura que almacena el resultado del problema del viajante (TSP)
struct ResultadoTsp {
    std::vector<std::string> ruta;
    int costo = 0;
};

// Estructura que agrupa los resultados de los tres algoritmos principales
struct ResultadoProblema {
    std::vector<AristaMst> mst;
    ResultadoTsp tsp;
    int flujoMaximo = 0;
};

// Funcion que valida que N sea un valor positivo
// Recibe el numero de colonias n
// Lanza una excepcion si n no es valido
// Complejidad: O(1)
inline void validarN(int n) {
    if (n <= 0) {
        throw std::invalid_argument("N debe ser mayor que 0");
    }
}

// Funcion que valida que una matriz sea cuadrada de dimension N x N
// Recibe la dimension esperada n, la matriz a validar y el nombre de la matriz para mensajes
// Lanza una excepcion si la matriz no tiene las dimensiones correctas
// Complejidad: O(N)
inline void validarDimensionMatriz(int n, const std::vector<std::vector<int>>& matriz,
                                   const std::string& nombreMatriz) {
    if (static_cast<int>(matriz.size()) != n) {
        throw std::invalid_argument("La " + nombreMatriz + " debe tener " +
            std::to_string(n) + " filas");
    }

    for (int i = 0; i < n; i++) {
        if (static_cast<int>(matriz[i].size()) != n) {
            throw std::invalid_argument("La fila " + std::to_string(i) +
                " de " + nombreMatriz + " debe tener " + std::to_string(n) + " columnas");
        }
    }
}

// Funcion que implementa el algoritmo de Prim para encontrar el arbol de expansion minima
// Recibe la cantidad de nodos n y la matriz de adyacencias ponderada dist
// Regresa un vector de aristas del MST, ordenadas por peso ascendente
// Complejidad: O(V^2) siendo V la cantidad de colonias
inline std::vector<AristaMst> calcularMst(int n, const std::vector<std::vector<int>>& dist) {
    std::vector<bool> enArbol(n, false);
    std::vector<int> costoMinimo(n, INT_MAX);
    std::vector<int> padre(n, -1);
    std::vector<AristaMst> aristas;

    // Iniciamos desde el nodo 0
    costoMinimo[0] = 0;

    // Ciclo que agrega un nodo al arbol en cada iteracion
    for (int i = 0; i < n; i++) {
        // Buscar el nodo no incluido con menor costo
        int u = -1;
        for (int v = 0; v < n; v++) {
            if (!enArbol[v] && (u == -1 || costoMinimo[v] < costoMinimo[u])) {
                u = v;
            }
        }

        enArbol[u] = true;

        // Registrar la arista cuando un nodo distinto a la raiz es agregado al arbol
        if (padre[u] != -1) {
            AristaMst arista;
            int menor = std::min(padre[u], u);
            int mayor = std::max(padre[u], u);
            arista.nodo1 = std::string(1, static_cast<char>('A' + menor));
            arista.nodo2 = std::string(1, static_cast<char>('A' + mayor));
            arista.peso = costoMinimo[u];
            aristas.push_back(arista);
        }

        // Actualizar costos de los vecinos del nodo recien agregado
        for (int v = 0; v < n; v++) {
            if (!enArbol[v] && dist[u][v] != 0 && dist[u][v] < costoMinimo[v]) {
                costoMinimo[v] = dist[u][v];
                padre[v] = u;
            }
        }
    }

    // Ordenar aristas por peso ascendente y alfabeticamente para desempate
    std::sort(aristas.begin(), aristas.end(), [](const AristaMst& a, const AristaMst& b) {
        if (a.peso != b.peso) {
            return a.peso < b.peso;
        }
        if (a.nodo1 != b.nodo1) {
            return a.nodo1 < b.nodo1;
        }
        return a.nodo2 < b.nodo2;
    });

    return aristas;
}

// Funcion que resuelve el Problema del Viajante (TSP) por fuerza bruta
// Recibe la cantidad de nodos n y la matriz de adyacencias ponderada dist
// Regresa un ResultadoTsp con la ruta optima y su costo
// Complejidad: O(N!) siendo N la cantidad de colonias
inline ResultadoTsp calcularTsp(int n, const std::vector<std::vector<int>>& dist) {
    ResultadoTsp resultado;
    resultado.costo = INT_MAX;

    // Caso especial: un solo nodo
    if (n <= 1) {
        resultado.ruta.push_back("A");
        resultado.ruta.push_back("A");
        resultado.costo = 0;
        return resultado;
    }

    // Crear vector con indices de colonias excluyendo la de origen (0)
    std::vector<int> colonias;
    for (int i = 1; i < n; i++) {
        colonias.push_back(i);
    }

    std::vector<int> mejorRuta;

    // Evaluar todas las permutaciones posibles de las colonias restantes
    do {
        int costoActual = 0;
        int nodoActual = 0;
        bool rutaValida = true;

        // Calcular el costo de la ruta actual
        for (int i = 0; i < static_cast<int>(colonias.size()); i++) {
            if (dist[nodoActual][colonias[i]] == 0) {
                rutaValida = false;
            }
            costoActual += dist[nodoActual][colonias[i]];
            nodoActual = colonias[i];
        }

        // Agregar el costo de regreso al origen
        if (dist[nodoActual][0] == 0) {
            rutaValida = false;
        }
        costoActual += dist[nodoActual][0];

        // Actualizar la mejor ruta si el costo es menor
        if (rutaValida && costoActual < resultado.costo) {
            resultado.costo = costoActual;
            mejorRuta = colonias;
        }
    } while (std::next_permutation(colonias.begin(), colonias.end()));

    // Construir la ruta con letras comenzando y terminando en A
    resultado.ruta.push_back("A");
    for (int i = 0; i < static_cast<int>(mejorRuta.size()); i++) {
        resultado.ruta.push_back(std::string(1, static_cast<char>('A' + mejorRuta[i])));
    }
    resultado.ruta.push_back("A");

    return resultado;
}

// Funcion que realiza BFS para encontrar un camino aumentante en la red residual
// Recibe la red residual, el nodo fuente, el sumidero y el vector de padres
// Regresa true si existe un camino aumentante, false en caso contrario
// Complejidad: O(V + E) siendo V los vertices y E las aristas
inline bool buscarCaminoAumentante(const std::vector<std::vector<int>>& residual,
                                   int fuente, int sumidero, std::vector<int>& padre) {
    int cantidadNodos = static_cast<int>(residual.size());
    std::vector<bool> visitado(cantidadNodos, false);
    std::queue<int> cola;
    bool encontrado = false;

    cola.push(fuente);
    visitado[fuente] = true;
    padre[fuente] = -1;

    // Explorar nodos por niveles hasta encontrar el sumidero o agotar la busqueda
    while (!cola.empty() && !encontrado) {
        int u = cola.front();
        cola.pop();

        for (int v = 0; v < cantidadNodos && !encontrado; v++) {
            // Solo considerar aristas con capacidad residual positiva y nodos no visitados
            if (!visitado[v] && residual[u][v] > 0) {
                cola.push(v);
                padre[v] = u;
                visitado[v] = true;

                if (v == sumidero) {
                    encontrado = true;
                }
            }
        }
    }

    return encontrado;
}

// Funcion que implementa el algoritmo de Edmonds-Karp para calcular el flujo maximo
// Recibe la cantidad de nodos n y la matriz de capacidades maximas
// Regresa el valor del flujo maximo desde el nodo 0 hasta el nodo n-1
// Complejidad: O(V * E^2) siendo V los vertices y E las aristas
inline int calcularFlujoMaximo(int n, const std::vector<std::vector<int>>& capacidad) {
    // Crear la red residual como copia de las capacidades originales
    std::vector<std::vector<int>> residual = capacidad;
    std::vector<int> padre(n, -1);
    int flujoMaximo = 0;

    // Mientras exista un camino aumentante del nodo fuente al sumidero
    while (buscarCaminoAumentante(residual, 0, n - 1, padre)) {
        // Encontrar la capacidad minima en el camino encontrado
        int flujoRuta = INT_MAX;
        for (int v = n - 1; v != 0; v = padre[v]) {
            int u = padre[v];
            if (residual[u][v] < flujoRuta) {
                flujoRuta = residual[u][v];
            }
        }

        // Actualizar las capacidades residuales a lo largo del camino
        for (int v = n - 1; v != 0; v = padre[v]) {
            int u = padre[v];
            residual[u][v] -= flujoRuta;
            residual[v][u] += flujoRuta;
        }

        flujoMaximo += flujoRuta;
    }

    return flujoMaximo;
}

// Funcion que calcula la distancia euclidiana entre dos puntos en el plano
// Recibe dos puntos con coordenadas (x, y)
// Regresa la distancia euclidiana como valor double
// Complejidad: O(1)
inline double calcularDistanciaEuclidiana(const Punto& a, const Punto& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Funcion que encuentra la central mas cercana a un punto dado
// Recibe la lista de centrales y el punto de la nueva contratacion
// Regresa el punto de la central mas cercana
// Complejidad: O(N) siendo N la cantidad de centrales
inline Punto encontrarCentralCercana(const std::vector<Punto>& centrales, const Punto& nuevoPunto) {
    if (centrales.empty()) {
        throw std::invalid_argument("La lista de centrales no puede estar vacia");
    }

    int indiceCercana = 0;
    double distanciaMinima = calcularDistanciaEuclidiana(centrales[0], nuevoPunto);

    // Comparar la distancia a cada central para encontrar la menor
    for (int i = 1; i < static_cast<int>(centrales.size()); i++) {
        double distanciaActual = calcularDistanciaEuclidiana(centrales[i], nuevoPunto);
        if (distanciaActual < distanciaMinima) {
            distanciaMinima = distanciaActual;
            indiceCercana = i;
        }
    }

    return centrales[indiceCercana];
}

// Funcion que ejecuta los tres algoritmos principales y agrupa sus resultados
// Recibe la cantidad de nodos n, la matriz de distancias y la matriz de capacidades
// Regresa un ResultadoProblema con MST, TSP y flujo maximo
// Complejidad: depende de cada algoritmo individual
inline ResultadoProblema resolverProblema(int n,
                                          const std::vector<std::vector<int>>& distancias,
                                          const std::vector<std::vector<int>>& capacidades) {
    validarN(n);
    validarDimensionMatriz(n, distancias, "distancias");
    validarDimensionMatriz(n, capacidades, "capacidades");

    ResultadoProblema resultado;
    resultado.mst = calcularMst(n, distancias);
    resultado.tsp = calcularTsp(n, distancias);
    resultado.flujoMaximo = calcularFlujoMaximo(n, capacidades);

    return resultado;
}

#endif // ALGORITMOS_H
