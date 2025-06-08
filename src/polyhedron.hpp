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
   * @param shouldPrintPolyExists indica se stampare se il poliedro esiste già
   * 
   * @return ritorna l'id della nuova faccia o di quella già esistente
   */
   unsigned int add(PolyhedronCollection& p_coll, const std::vector<unsigned int>& new_faces_id, bool checkpolyexists = true, bool shouldPrintPolyExists = false);


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
   * @param poly_id id del poliedro di cui si vuole calcolare la triangolazione di classe I
   * @param q denota il numero di vertici del poligono che si osserva guardando ciascun vertice
   * @param b parametro di triangolazione
   * 
   * @return ritorna l'id del poliedro 
   */
   unsigned int buildGeodesicClassI(PolyhedronCollection& p_coll, unsigned int poly_id, unsigned int q, unsigned int b);


   /**
   * @brief Crea il poliedro geodetico di classe II.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param poly_id id del poliedro di cui si vuole calcolare la triangolazione di classe II
   * @param q denota il numero di vertici del poligono che si osserva guardando ciascun vertice
   * @param b parametro di triangolazione (uguale a c)
   * 
   * @return ritorna l'id del poliedro 
   */
   unsigned int buildGeodesicClassII(PolyhedronCollection& p_coll, unsigned int poly_id, unsigned int q, unsigned int b);


   /**
   * @brief Crea il poliedro geodetico.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param p numero di vertici del poligono che si osserva guardando ciascuna faccia
   * @param q denota il numero di vertici del poligono che si osserva guardando ciascun vertice
   * @param b parametro di triangolazione
   * @param c 
   * 
   * @return ritorna l'id del poliedro (-1 se dà errore)
   */
   int createGeodesicPolyhedron(PolyhedronCollection& p_coll, unsigned int p, unsigned int q, unsigned int b, unsigned int c);


   /**
   * @brief Crea il poliedro di Goldberg.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param p numero di vertici del poligono che si osserva guardando ciascuna faccia
   * @param q denota il numero di vertici del poligono che si osserva guardando ciascun vertice
   * @param b parametro di triangolazione
   * @param c 
   * 
   * @return ritorna l'id del poliedro (-1 se dà errore)
   */
   int createGoldbergPolyhedron(PolyhedronCollection& p_coll, unsigned int p, unsigned int q, unsigned int b, unsigned int c);


   /**
   * @brief Trova il percorso più breve (in termini di distanza) tra due vertici di un poliedro utilizzando l'algoritmo di Dijkstra.
   * 
   * @param p_coll oggetto di PolyhedronCollection
   * @param poly_id id del poliedro dove si vuole calcolare il percorso
   * @param id1 id del vertice di partenza
   * @param id2 id del vertice di arrivo
   * @param vertices_path è il vettore dei vertici del percorso
   * @param edges_path è il vettore dei lati del percorso
   * @param graphIsWeighted indica se il grafo è pesato
   * 
   * @return ritorna la distanza tra i due vertici
   */
   double findShortestPath(PolyhedronCollection& p_coll, unsigned int poly_id, unsigned int id1, unsigned int id2, 
    std::vector<int>& cammino_vertici, std::vector<int>& cammino_lati, bool graphIsWeighted = false);

}