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
   * @brief Resistituisce l'id di una faccia dati i punti.
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
   * @param shouldPrintFaceExists indica se stampare se la faccia esiste già
   * 
   * @return ritorna l'id della nuova faccia o di quella già esistente
   */
   unsigned int add(PolyhedronCollection& p_coll, const std::vector<unsigned int>& new_vertices_id, bool checkfaceexists = true, bool shouldPrintFaceExists = false);


   /**
   * @brief Calcola il centroide di una faccia e lo memorizza ai punti esistenti
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param face_id id della faccia
   * @param projection indica se si vuole proiettare il centroide sulla sfera unitaria nell'origine
   *  
   * @return ritorna l'id del nuovo punto o di quello già esistente
   */
   unsigned int getCentroid(PolyhedronCollection& p_coll, unsigned int face_id, bool projection = true);


   /**
   * @brief Confronta se due facce sono adiacenti.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param f1_id id della prima faccia
   * @param f2_id id della seconda faccia
   * 
   * @return indica se le facce sono adiacenti 
   */   
   bool areAdjacent(PolyhedronCollection& p_coll, unsigned int f1_id, unsigned int f2_id);


   /**
   * @brief Calcola la triangolazione caratterizzante la faccia dei poliedri geodetici di classe I.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param face_id id della faccia di cui effettuare la triangolazione
   * @param b parametro di triangolazione
   * 
   * @return ritorna il vettore di indici delle facce
   */
   std::vector<unsigned int> computeClassICharacteristicTriangulation(PolyhedronCollection& p_coll, unsigned int face_id, unsigned int b);

   /**
   * @brief Calcola la triangolazione caratterizzante la faccia dei poliedri geodetici di classe II.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param face_id id della faccia di cui effettuare la triangolazione
   * @param b parametro di triangolazione
   * 
   * @return ritorna il vettore di indici delle facce
   */
   std::vector<unsigned int> computeClassIICharacteristicTriangulation(PolyhedronCollection& p_coll, unsigned int face_id, unsigned int b);


    /**
    * @brief Conta il numero di facce del solido geodetico di classe I 0 II
    *
    * @param q denota il numero di vertici del poligono che si osserva guardando ciascun vertice
    * @param b parametro di triangolazione
    * @param c parametro di triangolazione
    * 
    * @return ritorna il numero di facce del solido geodetico di classe I.
    */ 
    unsigned int countGeodesic(unsigned int q, unsigned int b, unsigned int c = 0);



}
