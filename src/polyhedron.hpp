#pragma once

#include <iostream>
#include "PolyhedronCollection.hpp"
#include "vertex.hpp"
#include "edge.hpp"
#include "face.hpp"

namespace polyhedron{
    /**
    * @brief Inizializza i valori legati ai poliedri.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param n_polyhedron numero di poliedri
    */
   void initialize(PolyhedronCollection& p_coll, unsigned int n_polyhedron);
    

   /**
   * @brief Riserva una capacità aggiuntiva al vettore di punti, lati, facce e id dei poliedri.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param new_n_polyhedron nuovo numero di poliedri
   * @param preservedata indica se vogliamo preservare le facce già esistenti 
   */
   void reshape(PolyhedronCollection& p_coll, unsigned int new_n_polyhedron, bool preservedata = true);


   /**
   * @brief Confronta se due poliedri sono uguali.
   * 
   * @param p1 facce del primo poliedro
   * @param p2 facce del secondo poliedro
   * 
   * @return indica se i poliedri sono uguali 
   */
   bool isEqual(const std::vector<unsigned int>& p1, const std::vector<unsigned int>& p2);

   
   /**
   * @brief Resistituisce l'id di un poliedro date le facce.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param faces_id punti della faccia
   * 
   * @return ritorna l'id del poliedro se esiste o la dimensione del vettore nel caso non esista
   */
   unsigned int getId(const PolyhedronCollection& p_coll, const std::vector<unsigned int> faces_id);


   /**
   * @brief Aggiunge un poliedro, se non esiste ancora.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param new_faces_id facce del nuovo poliedro 
   * @param checkpolyexists indica se effettuare il controllo sull'esistenza del poliedro
   * 
   * @return ritorna l'id della nuova faccia o di quella già esistente
   */
   unsigned int add(PolyhedronCollection& p_coll, const std::vector<unsigned int>& new_faces_id, bool checkpolyexists = true);


   // rimuovo il punto, se esiste e non viene usato in nessun poliedro
   bool remove(PolyhedronCollection& p_coll, unsigned int face_id); // --- non so se ha senso


   /**
   * @brief Crea il solido platonico dato in input
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param p numero di vertici del poligono che si osserva guardando ciascuna faccia
   * @param q denota il numero di vertici del poligono che si osserva guardando ciascun vertice
   * 
   * @return ritorna l'id del poliedro creato (-1 se non esiste la combinazione p e q)
   */
   int build_platonic_solid(PolyhedronCollection& p_coll, unsigned int p, unsigned int q);


   /**
   * @brief Identifica quale poliedro devo creare
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param prefix nome del solido platonico che si vuole costruire
   * @param p numero di vertici del poligono che si osserva guardando ciascuna faccia
   * @param q denota il numero di vertici del poligono che si osserva guardando ciascun vertice
   * @param n_vertices numero di vertici del poliedro
   * @param n_edges numero di lati del poliedro
   * @param n_faces numero di facce del poliedro
   * 
   * @return ritorna l'id del solido platonico creato
   */
   unsigned int import_platonic_solid(PolyhedronCollection& p_coll, const std::string& prefix, unsigned int n_vertices, unsigned int n_edges, unsigned int n_faces);


   /**
   * @brief Crea il duale del poliedro.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param poly_id id del poliedro di cui si vuole calcolare il duale
   * @param q denota il numero di vertici del poligono che si osserva guardando ciascun vertice
   * 
   * @return ritorna l'id del duale del poliedro
   */
   unsigned int createDual(PolyhedronCollection& p_coll, unsigned int poly_id, const unsigned int q);


   /**
   * @brief Crea il poliedro geodetico di classe I.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param poly_id id del poliedro di cui si vuole calcolare il duale
   * @param q denota il numero di vertici del poligono che si osserva guardando ciascun vertice
   * @param b parametro di triangolazione
   * 
   * @return ritorna l'id del poliedro 
   */
   unsigned int buildGeodesicClassI(PolyhedronCollection& p_coll, unsigned int poly_id, unsigned int q, unsigned int b);

}