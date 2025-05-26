#pragma once

#include <iostream>
#include "vertex.hpp"
#include "PolyhedronCollection.hpp"

namespace edge{

    /**
    * @brief Inizializza i valori legati al lato.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param n_edges numero di lati
    */
    void initialize(PolyhedronCollection& p_coll, unsigned int n_edges);
    

    /**
    * @brief Ridimensiona la matrice degli estremi dei lati.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param new_n_edges nuovo numero di lati
    * @param preservedata indica se vogliamo preservare gli lati già esistenti 
    */
    void reshape(PolyhedronCollection& p_coll, unsigned int new_n_edges, bool preservedata = true);


    /**
    * @brief Confronta se due lati sono uguali.
    * 
    * @param e1 estremi del primo lato
    * @param e2 estremi del secondo lato
    * 
    * @return indica se i lati sono uguali 
    */
    bool isEqual(const Eigen::VectorXi& e1, const Eigen::VectorXi& e2);

    
    /**
    * @brief Resistituisce l'id di un lato dati gli estremi.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param endpoints_id estremi del lato
    * 
    * @return ritorna l'id del lato se esiste o la dimensione del vettore nel caso non esista
    */
    unsigned int getId(const PolyhedronCollection& p_coll, const Eigen::VectorXi& endpoints_id);


    /**
    * @brief Aggiunge un lato, se non esiste ancora.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param new_endpoints_id estremi del nuovo lato
    * @param checkedgeexists indica se effettuare il controllo sull'esistenza del lato
    * 
    * @return ritorna l'id del nuovo lato o di quello già esistente
    */
    unsigned int add(PolyhedronCollection& p_coll, const Eigen::Vector2i new_endpoints_id, bool checkedgeexists = true);

    // rimuovo il lato, se esiste e non viene usato in nessuna faccia
    bool remove(PolyhedronCollection& p_coll, unsigned int edge_id); // --- non so se ha senso


    /**
    * @brief Calcola la lunghezza del lato.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param edge_id id del lato
    * 
    * @return ritorna la lunghezza del lato.
    */
    double length(PolyhedronCollection& p_coll, unsigned int edge_id);


    /**
    * @brief Conta il numero di lati del solido geodetico di classe I
    * 
    * @param q denota il numero di vertici del poligono che si osserva guardando ciascun vertice
    * @param b parametro di triangolazione
    * @param c parametro di triangolazione
    * 
    * @return ritorna il numero di lati del solido geodetico di classe I.
    */ 
    unsigned int countGeodesicClassI(unsigned int q, unsigned int b, unsigned int c);

}