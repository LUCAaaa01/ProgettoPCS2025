#pragma once

#include <iostream>
#include "vertex.hpp"
#include "edge.hpp"
#include "PolyhedronCollection.hpp"

namespace face{
    /**
    * @brief Inizializza i valori legati alla faccia.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param n_faces numero di facce
    */
   void initialize(PolyhedronCollection& p_coll, unsigned int n_faces);
    

   /**
   * @brief Riserva una capacità aggiuntiva al vettore di punti, lati e id della faccia.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param new_n_faces nuovo numero di facce
   * @param preservedata indica se vogliamo preservare le facce già esistenti 
   */
   void reshape(PolyhedronCollection& p_coll, unsigned int new_n_faces, bool preservedata = true);


   /**
   * @brief Confronta se due facce sono uguali.
   * 
   * @param f1 punti della prima faccia
   * @param f2 punti della seconda faccia
   * 
   * @return indica se le facce sono uguali 
   */
   bool isEqual(const std::vector<unsigned int>& f1, const std::vector<unsigned int>& f2);

   
   /**
   * @brief Resistituisce l'id di una faccia dati i lati.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param vertices_id punti della faccia
   * 
   * @return ritorna l'id della faccia se esiste o la dimensione del vettore nel caso non esista
   */
   unsigned int getId(const PolyhedronCollection& p_coll, const std::vector<unsigned int> vertices_id);


   /**
   * @brief Aggiunge una faccia, se non esiste ancora.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param new_vertices_id punti della nuova faccia ordinati (in senso orario o antiorario)
   * @param checkfaceexists indica se effettuare il controllo sull'esistenza della faccia
   * 
   * @return ritorna l'id della nuova faccia o di quella già esistente
   */
   unsigned int add(PolyhedronCollection& p_coll, const std::vector<unsigned int>& new_vertices_id, bool checkfaceexists = true);


   // rimuovo il punto, se esiste e non viene usato in nessun poliedro
   bool remove(PolyhedronCollection& p_coll, unsigned int face_id); // --- non so se ha senso


   /**
   * @brief Calcola il centroide di una faccia e lo memorizza ai punti esistenti
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param face_id id della faccia
   *  
   * @return ritorna l'id del nuovo punto o di quello già esistente
   */
   unsigned int getCentroid(PolyhedronCollection& p_coll, unsigned int face_id);


   /**
   * @brief Confronta se due facce sono adiacenti.
   * 
   * @param f1_id id della prima faccia
   * @param f2_id id della seconda faccia
   * 
   * @return indica se le facce sono adiacenti 
   */   
   bool areAdjacent(PolyhedronCollection& p_coll, unsigned int f1_id, unsigned int f2_id);



}