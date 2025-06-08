#pragma once

#include <iostream>
#include "PolyhedronCollection.hpp"
#include "vertex.hpp"
#include "edge.hpp"
#include "face.hpp"
#include "polyhedron.hpp"

namespace utils{

    /**
    * @brief Verifica se un elemento appartiene a un vettore.
    * 
    * @param vec vettore di elementi
    * @param element elemento che si vuole verificare se appartiene al vettore
    * 
    * @return ritorna se l'elemento appartiene al vettore
    */
    bool contains(const std::vector<unsigned int>& vec, unsigned int element);


    /**
    * @brief Calcola il fattoriale di un intero senza segno
    * 
    * @param n il numero di cui si vuole calcolare il fattoriale
    * 
    * @return ritorna il fattoriale
    */
    unsigned int factorial(unsigned int n);


    /**
    * @brief Esporto tutti i file txt
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param path percorso della cartella in cui si vuole salvare il file
    * 
    * @return ritorna se l'elemento appartiene al vettore
    */
    void exportToTxt(const PolyhedronCollection& p_coll, const std::string& path = "");
}