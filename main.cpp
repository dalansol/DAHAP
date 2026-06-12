// Programa API REST para la Situacion Problema 2: servicios de Internet para ISP.
// Expone endpoints HTTP para ejecutar algoritmos de grafos y geometria computacional,
// recibiendo datos en formato JSON y respondiendo con los resultados en JSON.
// Autores:
//      Adan Gonzalez Flores | A00841625
//      Patricio Javier Garza Rios | A00841942
//      Daniela Landin Solis | A00841757
//      Alana Martinez Morales | A00841524
// Fecha: 10 de junio 2026

#include "algoritmos.h"
#include "json.hpp"
#include "httplib.h"
#include <iostream>

const int PUERTO_SERVIDOR = 8080;

// Funcion principal que configura e inicia el servidor HTTP REST
// No recibe parametros
// Regresa 0 si la ejecucion fue exitosa
int main() {
    httplib::Server servidor;

    // Endpoint principal: ejecuta MST, TSP y flujo maximo
    servidor.Post("/", [](const httplib::Request& peticion, httplib::Response& respuesta) {
        try {
            auto cuerpo = nlohmann::json::parse(peticion.body);
            int n = cuerpo["N"].get<int>();
            auto distancias = cuerpo["distancias"].get<std::vector<std::vector<int>>>();
            auto capacidades = cuerpo["capacidades"].get<std::vector<std::vector<int>>>();

            ResultadoProblema resultado = resolverProblema(n, distancias, capacidades);

            // Construir JSON de respuesta con aristas del MST
            nlohmann::json salida;
            salida["mst"] = nlohmann::json::array();
            for (const auto& arista : resultado.mst) {
                salida["mst"].push_back({arista.nodo1, arista.nodo2});
            }

            // Agregar ruta y costo del TSP
            salida["tsp"]["ruta"] = resultado.tsp.ruta;
            salida["tsp"]["costo"] = resultado.tsp.costo;

            // Agregar flujo maximo
            salida["flujoMaximo"] = resultado.flujoMaximo;

            respuesta.set_content(salida.dump(2), "application/json");
        } catch (const std::exception& excepcion) {
            nlohmann::json error;
            error["error"] = excepcion.what();
            respuesta.status = 400;
            respuesta.set_content(error.dump(2), "application/json");
        }
    });

    // Endpoint para encontrar la central mas cercana a un punto dado
    servidor.Post("/centralCercana", [](const httplib::Request& peticion, httplib::Response& respuesta) {
        try {
            auto cuerpo = nlohmann::json::parse(peticion.body);

            // Parsear lista de centrales desde el JSON
            std::vector<Punto> centrales;
            for (const auto& coordenada : cuerpo["centrales"]) {
                Punto punto;
                punto.x = coordenada[0].get<double>();
                punto.y = coordenada[1].get<double>();
                centrales.push_back(punto);
            }

            // Parsear el punto de la nueva contratacion
            Punto nuevoPunto;
            nuevoPunto.x = cuerpo["punto"][0].get<double>();
            nuevoPunto.y = cuerpo["punto"][1].get<double>();

            Punto cercana = encontrarCentralCercana(centrales, nuevoPunto);

            nlohmann::json salida;
            salida["centralCercana"] = {cercana.x, cercana.y};
            respuesta.set_content(salida.dump(2), "application/json");
        } catch (const std::exception& excepcion) {
            nlohmann::json error;
            error["error"] = excepcion.what();
            respuesta.status = 400;
            respuesta.set_content(error.dump(2), "application/json");
        }
    });

    // Endpoint de salud para CI/CD (GitHub Actions / AWS deploy)
    servidor.Get("/health", [](const httplib::Request&, httplib::Response& respuesta) {
        respuesta.set_content("{\"status\": \"UP\"}", "application/json");
    });

    std::cout << "Servidor iniciado en puerto " << PUERTO_SERVIDOR << '\n';
    servidor.listen("0.0.0.0", PUERTO_SERVIDOR);

    return 0;
}
