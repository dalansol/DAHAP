// Copyright (c) 2026. All rights reserved.
//
// Pruebas unitarias para los algoritmos de la Situacion Problema 2.
// Verifica los tres casos de prueba de MST, TSP y flujo maximo,
// ademas de pruebas de distancia euclidiana, central cercana y validacion.
// Autores:
//      Adan Gonzalez Flores | A00841625
//      Patricio Javier Garza Rios | A00841942
//      Daniela Landin Solis | A00841757
//      Alana Martinez Morales | A00841524
// Fecha: 12 de junio 2026

#include "algoritmos.h"
#include <iostream>
#include <cmath>
#include <set>

int pruebasPasadas = 0;
int pruebasFallidas = 0;

// Funcion auxiliar que registra el resultado de una prueba unitaria
// Recibe la condicion a evaluar y el nombre descriptivo de la prueba
// Incrementa el contador de pruebas pasadas o fallidas
// Complejidad: O(1)
void verificar(bool condicion, const std::string& nombrePrueba) {
    if (condicion) {
        std::cout << "  PASO: " << nombrePrueba << std::endl;
        pruebasPasadas++;
    } else {
        std::cout << "  FALLO: " << nombrePrueba << std::endl;
        pruebasFallidas++;
    }
}

// Funcion auxiliar que convierte las aristas del MST a un conjunto de pares para comparacion
// Recibe un vector de aristas MST
// Regresa un conjunto de pares de strings (nodo1, nodo2)
// Complejidad: O(N log N) siendo N la cantidad de aristas
std::set<std::pair<std::string, std::string>> aristasMstAConjunto(
    const std::vector<AristaMst>& aristas) {
    std::set<std::pair<std::string, std::string>> conjunto;
    for (const auto& arista : aristas) {
        conjunto.insert({arista.nodo1, arista.nodo2});
    }
    return conjunto;
}

// --- Pruebas del algoritmo MST (Prim) ---

// Prueba del MST con el caso de prueba 1 (N=4)
// Verifica que las aristas del MST sean (A,B), (B,C), (C,D)
void probarMstCaso1() {
    std::cout << "MST Caso 1:" << std::endl;
    std::vector<std::vector<int>> distancias = {
        {0, 16, 45, 32},
        {16, 0, 18, 21},
        {45, 18, 0, 7},
        {32, 21, 7, 0}
    };

    std::vector<AristaMst> resultado = calcular_mst(4, distancias);

    std::set<std::pair<std::string, std::string>> esperado = {
        {"A", "B"}, {"B", "C"}, {"C", "D"}
    };

    verificar(resultado.size() == 3, "Cantidad de aristas es 3");
    verificar(aristasMstAConjunto(resultado) == esperado, "Aristas correctas");
}

// Prueba del MST con el caso de prueba 2 (N=5)
// Verifica que las aristas del MST sean (A,B), (B,D), (B,E), (C,E)
void probarMstCaso2() {
    std::cout << "MST Caso 2:" << std::endl;
    std::vector<std::vector<int>> distancias = {
        {0, 2, 9, 10, 7},
        {2, 0, 6, 4, 3},
        {9, 6, 0, 8, 5},
        {10, 4, 8, 0, 6},
        {7, 3, 5, 6, 0}
    };

    std::vector<AristaMst> resultado = calcular_mst(5, distancias);

    std::set<std::pair<std::string, std::string>> esperado = {
        {"A", "B"}, {"B", "D"}, {"B", "E"}, {"C", "E"}
    };

    verificar(resultado.size() == 4, "Cantidad de aristas es 4");
    verificar(aristasMstAConjunto(resultado) == esperado, "Aristas correctas");
}

// Prueba del MST con el caso de prueba 3 (N=6)
// Verifica que las aristas del MST sean (A,B), (A,C), (A,D), (B,E), (C,F)
void probarMstCaso3() {
    std::cout << "MST Caso 3:" << std::endl;
    std::vector<std::vector<int>> distancias = {
        {0, 3, 4, 2, 7, 9},
        {3, 0, 4, 6, 3, 8},
        {4, 4, 0, 5, 8, 2},
        {2, 6, 5, 0, 6, 7},
        {7, 3, 8, 6, 0, 4},
        {9, 8, 2, 7, 4, 0}
    };

    std::vector<AristaMst> resultado = calcular_mst(6, distancias);

    std::set<std::pair<std::string, std::string>> esperado = {
        {"A", "D"}, {"C", "F"}, {"A", "B"}, {"B", "E"}, {"A", "C"}
    };

    verificar(resultado.size() == 5, "Cantidad de aristas es 5");
    verificar(aristasMstAConjunto(resultado) == esperado, "Aristas correctas");
}

// --- Pruebas del algoritmo TSP ---

// Prueba del TSP con el caso de prueba 1 (N=4)
// Verifica ruta A-B-C-D-A con costo 73
void probarTspCaso1() {
    std::cout << "TSP Caso 1:" << std::endl;
    std::vector<std::vector<int>> distancias = {
        {0, 16, 45, 32},
        {16, 0, 18, 21},
        {45, 18, 0, 7},
        {32, 21, 7, 0}
    };

    ResultadoTsp resultado = calcular_tsp(4, distancias);
    std::vector<std::string> rutaEsperada = {"A", "B", "C", "D", "A"};

    verificar(resultado.costo == 73, "Costo es 73");
    verificar(resultado.ruta == rutaEsperada, "Ruta es A-B-C-D-A");
}

// Prueba del TSP con el caso de prueba 2 (N=5)
// Verifica ruta A-B-D-C-E-A con costo 26
void probarTspCaso2() {
    std::cout << "TSP Caso 2:" << std::endl;
    std::vector<std::vector<int>> distancias = {
        {0, 2, 9, 10, 7},
        {2, 0, 6, 4, 3},
        {9, 6, 0, 8, 5},
        {10, 4, 8, 0, 6},
        {7, 3, 5, 6, 0}
    };

    ResultadoTsp resultado = calcular_tsp(5, distancias);
    std::vector<std::string> rutaEsperada = {"A", "B", "D", "C", "E", "A"};

    verificar(resultado.costo == 26, "Costo es 26");
    verificar(resultado.ruta == rutaEsperada, "Ruta es A-B-D-C-E-A");
}

// Prueba del TSP con el caso de prueba 3 (N=6)
// Verifica ruta A-B-E-F-C-D-A con costo 19
void probarTspCaso3() {
    std::cout << "TSP Caso 3:" << std::endl;
    std::vector<std::vector<int>> distancias = {
        {0, 3, 4, 2, 7, 9},
        {3, 0, 4, 6, 3, 8},
        {4, 4, 0, 5, 8, 2},
        {2, 6, 5, 0, 6, 7},
        {7, 3, 8, 6, 0, 4},
        {9, 8, 2, 7, 4, 0}
    };

    ResultadoTsp resultado = calcular_tsp(6, distancias);
    std::vector<std::string> rutaEsperada = {"A", "B", "E", "F", "C", "D", "A"};

    verificar(resultado.costo == 19, "Costo es 19");
    verificar(resultado.ruta == rutaEsperada, "Ruta es A-B-E-F-C-D-A");
}

// --- Pruebas del algoritmo de Flujo Maximo (Edmonds-Karp) ---

// Prueba del flujo maximo con el caso de prueba 1 (N=4)
// Verifica que el flujo maximo sea 78
void probarFlujoCaso1() {
    std::cout << "Flujo Maximo Caso 1:" << std::endl;
    std::vector<std::vector<int>> capacidades = {
        {0, 48, 12, 18},
        {52, 0, 42, 32},
        {18, 46, 0, 56},
        {24, 36, 52, 0}
    };

    int resultado = calcular_flujo_maximo(4, capacidades);
    verificar(resultado == 78, "Flujo maximo es 78");
}

// Prueba del flujo maximo con el caso de prueba 2 (N=5)
// Verifica que el flujo maximo sea 15
void probarFlujoCaso2() {
    std::cout << "Flujo Maximo Caso 2:" << std::endl;
    std::vector<std::vector<int>> capacidades = {
        {0, 10, 5, 0, 0},
        {0, 0, 15, 10, 0},
        {0, 0, 0, 10, 10},
        {0, 0, 0, 0, 10},
        {0, 0, 0, 0, 0}
    };

    int resultado = calcular_flujo_maximo(5, capacidades);
    verificar(resultado == 15, "Flujo maximo es 15");
}

// Prueba del flujo maximo con el caso de prueba 3 (N=6)
// Verifica que el flujo maximo sea 23
void probarFlujoCaso3() {
    std::cout << "Flujo Maximo Caso 3:" << std::endl;
    std::vector<std::vector<int>> capacidades = {
        {0, 16, 13, 0, 0, 0},
        {0, 0, 10, 12, 0, 0},
        {0, 4, 0, 0, 14, 0},
        {0, 0, 9, 0, 0, 20},
        {0, 0, 0, 7, 0, 4},
        {0, 0, 0, 0, 0, 0}
    };

    int resultado = calcular_flujo_maximo(6, capacidades);
    verificar(resultado == 23, "Flujo maximo es 23");
}

// --- Pruebas de distancia euclidiana ---

// Prueba de la distancia euclidiana entre dos puntos conocidos
// Verifica que la distancia entre (0,0) y (3,4) sea 5.0
void probarDistanciaEuclidiana() {
    std::cout << "Distancia Euclidiana:" << std::endl;
    Punto a;
    a.x = 0.0;
    a.y = 0.0;

    Punto b;
    b.x = 3.0;
    b.y = 4.0;

    double distancia = calcular_distancia_euclidiana(a, b);
    verificar(std::abs(distancia - 5.0) < 0.001, "Distancia (0,0)-(3,4) es 5.0");

    Punto c;
    c.x = 1.0;
    c.y = 1.0;
    double distanciaMisma = calcular_distancia_euclidiana(c, c);
    verificar(std::abs(distanciaMisma) < 0.001, "Distancia de un punto a si mismo es 0");
}

// --- Pruebas de central mas cercana ---

// Prueba que encuentra la central mas cercana a un punto dado
// Verifica con las centrales del caso de prueba 1
void probarCentralCercana() {
    std::cout << "Central Cercana:" << std::endl;
    std::vector<Punto> centrales(4);
    centrales[0].x = 200.0;
    centrales[0].y = 500.0;
    centrales[1].x = 300.0;
    centrales[1].y = 100.0;
    centrales[2].x = 450.0;
    centrales[2].y = 150.0;
    centrales[3].x = 520.0;
    centrales[3].y = 480.0;

    // Punto cercano a la central (300, 100)
    Punto punto1;
    punto1.x = 280.0;
    punto1.y = 120.0;
    Punto resultado1 = encontrar_central_cercana(centrales, punto1);
    verificar(std::abs(resultado1.x - 300.0) < 0.001 &&
              std::abs(resultado1.y - 100.0) < 0.001,
              "Central cercana a (280,120) es (300,100)");

    // Punto cercano a la central (520, 480)
    Punto punto2;
    punto2.x = 500.0;
    punto2.y = 500.0;
    Punto resultado2 = encontrar_central_cercana(centrales, punto2);
    verificar(std::abs(resultado2.x - 520.0) < 0.001 &&
              std::abs(resultado2.y - 480.0) < 0.001,
              "Central cercana a (500,500) es (520,480)");
}

// --- Pruebas de validacion de entrada ---

// Prueba que la validacion rechaza valores invalidos de N
void probarValidacionN() {
    std::cout << "Validacion N:" << std::endl;
    bool excepcionLanzada = false;

    try {
        validar_n(0);
    } catch (const std::invalid_argument&) {
        excepcionLanzada = true;
    }
    verificar(excepcionLanzada, "N=0 lanza excepcion");

    excepcionLanzada = false;
    try {
        validar_n(-5);
    } catch (const std::invalid_argument&) {
        excepcionLanzada = true;
    }
    verificar(excepcionLanzada, "N=-5 lanza excepcion");

    excepcionLanzada = false;
    try {
        validar_n(3);
    } catch (const std::invalid_argument&) {
        excepcionLanzada = true;
    }
    verificar(!excepcionLanzada, "N=3 no lanza excepcion");
}

// Prueba que la validacion rechaza matrices con dimensiones incorrectas
void probarValidacionMatriz() {
    std::cout << "Validacion Matriz:" << std::endl;
    bool excepcionLanzada = false;

    // Matriz con menos filas
    std::vector<std::vector<int>> matrizMalFilas = {
        {0, 1},
        {1, 0}
    };
    try {
        validar_dimension_matriz(3, matrizMalFilas, "test");
    } catch (const std::invalid_argument&) {
        excepcionLanzada = true;
    }
    verificar(excepcionLanzada, "Matriz 2x2 para N=3 lanza excepcion");

    excepcionLanzada = false;
    // Matriz con fila de tamanio incorrecto
    std::vector<std::vector<int>> matrizMalColumnas = {
        {0, 1, 2},
        {1, 0},
        {2, 1, 0}
    };
    try {
        validar_dimension_matriz(3, matrizMalColumnas, "test");
    } catch (const std::invalid_argument&) {
        excepcionLanzada = true;
    }
    verificar(excepcionLanzada, "Matriz con fila corta lanza excepcion");

    excepcionLanzada = false;
    // Matriz correcta
    std::vector<std::vector<int>> matrizCorrecta = {
        {0, 1},
        {1, 0}
    };
    try {
        validar_dimension_matriz(2, matrizCorrecta, "test");
    } catch (const std::invalid_argument&) {
        excepcionLanzada = true;
    }
    verificar(!excepcionLanzada, "Matriz 2x2 para N=2 no lanza excepcion");
}

// Prueba que la central cercana rechaza una lista vacia de centrales
void probarCentralCercanaVacia() {
    std::cout << "Central Cercana Vacia:" << std::endl;
    bool excepcionLanzada = false;

    std::vector<Punto> centralesVacias;
    Punto punto;
    punto.x = 100.0;
    punto.y = 100.0;

    try {
        encontrar_central_cercana(centralesVacias, punto);
    } catch (const std::invalid_argument&) {
        excepcionLanzada = true;
    }
    verificar(excepcionLanzada, "Lista vacia de centrales lanza excepcion");
}

// --- Pruebas de caso especial ---

// Prueba del TSP con un solo nodo
// Verifica que la ruta sea A-A con costo 0
void probarTspNodoUnico() {
    std::cout << "TSP Nodo Unico:" << std::endl;
    std::vector<std::vector<int>> distancias = {{0}};

    ResultadoTsp resultado = calcular_tsp(1, distancias);
    std::vector<std::string> rutaEsperada = {"A", "A"};

    verificar(resultado.costo == 0, "Costo es 0");
    verificar(resultado.ruta == rutaEsperada, "Ruta es A-A");
}

// Prueba del MST con un solo nodo
// Verifica que no haya aristas en el MST
void probarMstNodoUnico() {
    std::cout << "MST Nodo Unico:" << std::endl;
    std::vector<std::vector<int>> distancias = {{0}};

    std::vector<AristaMst> resultado = calcular_mst(1, distancias);
    verificar(resultado.empty(), "MST vacio para un solo nodo");
}

// Prueba del flujo maximo con un solo nodo
// Verifica que el flujo sea 0
void probarFlujoNodoUnico() {
    std::cout << "Flujo Maximo Nodo Unico:" << std::endl;
    std::vector<std::vector<int>> capacidades = {{0}};

    int resultado = calcular_flujo_maximo(1, capacidades);
    verificar(resultado == 0, "Flujo maximo es 0 para un solo nodo");
}

// Prueba de resolver_problema con el caso de prueba 1 completo
// Verifica que los tres resultados sean correctos
void probarResolverProblemaCaso1() {
    std::cout << "Resolver Problema Caso 1:" << std::endl;
    std::vector<std::vector<int>> distancias = {
        {0, 16, 45, 32},
        {16, 0, 18, 21},
        {45, 18, 0, 7},
        {32, 21, 7, 0}
    };
    std::vector<std::vector<int>> capacidades = {
        {0, 48, 12, 18},
        {52, 0, 42, 32},
        {18, 46, 0, 56},
        {24, 36, 52, 0}
    };

    ResultadoProblema resultado = resolver_problema(4, distancias, capacidades);

    verificar(resultado.mst.size() == 3, "MST tiene 3 aristas");
    verificar(resultado.tsp.costo == 73, "TSP costo es 73");
    verificar(resultado.flujoMaximo == 78, "Flujo maximo es 78");
}

// Prueba que resolver_problema rechaza entrada invalida
void probarResolverProblemaInvalido() {
    std::cout << "Resolver Problema Invalido:" << std::endl;
    bool excepcionLanzada = false;

    std::vector<std::vector<int>> distancias = {{0}};
    std::vector<std::vector<int>> capacidades = {{0}};

    try {
        resolver_problema(0, distancias, capacidades);
    } catch (const std::invalid_argument&) {
        excepcionLanzada = true;
    }
    verificar(excepcionLanzada, "N=0 lanza excepcion en resolver_problema");
}

// Funcion principal que ejecuta todas las pruebas y reporta resultados
// No recibe parametros
// Regresa 0 si todas las pruebas pasan, 1 si alguna falla
int main() {
    std::cout << "=== Ejecutando pruebas unitarias ===" << std::endl << std::endl;

    // Pruebas de MST
    probarMstCaso1();
    probarMstCaso2();
    probarMstCaso3();
    probarMstNodoUnico();

    std::cout << std::endl;

    // Pruebas de TSP
    probarTspCaso1();
    probarTspCaso2();
    probarTspCaso3();
    probarTspNodoUnico();

    std::cout << std::endl;

    // Pruebas de flujo maximo
    probarFlujoCaso1();
    probarFlujoCaso2();
    probarFlujoCaso3();
    probarFlujoNodoUnico();

    std::cout << std::endl;

    // Pruebas de geometria y centrales
    probarDistanciaEuclidiana();
    probarCentralCercana();
    probarCentralCercanaVacia();

    std::cout << std::endl;

    // Pruebas de validacion
    probarValidacionN();
    probarValidacionMatriz();

    std::cout << std::endl;

    // Pruebas de integracion
    probarResolverProblemaCaso1();
    probarResolverProblemaInvalido();

    // Reporte final
    std::cout << std::endl << "=== Resultados ===" << std::endl;
    std::cout << "Pasadas: " << pruebasPasadas << std::endl;
    std::cout << "Fallidas: " << pruebasFallidas << std::endl;

    if (pruebasFallidas > 0) {
        return 1;
    }
    return 0;
}
