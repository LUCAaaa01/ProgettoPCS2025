#pragma once

#include <iostream>
#include <algorithm>
#include <cassert>
#include "Eigen/Eigen"
#include "PolyhedronCollection.hpp"

namespace vertex{

    /**
    * @brief Inizializza i valori legati a punto.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param n_points numero di punti
    */
    void initialize(PolyhedronCollection& p_coll, unsigned int n_points);


    /**
    * @brief Ridimensiona la matrice delle coordinate.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param new_n_points nuovo numero di punti
    * @param preservedata indica se vogliamo preservare i punti già esistenti 
    */
    void reshape(PolyhedronCollection& p_coll, unsigned int new_n_points, bool preservedata = true);


    /**
    * @brief Confronta se due punti sono uguali.
    * 
    * @param p1 coordinate del primo punto
    * @param p2 coordinate del secondo punto
    * 
    * @return indica se i punti sono uguali 
    */
    bool isEqual(const Eigen::VectorXd& p1, const Eigen::VectorXd& p2);


    /**
    * @brief Resistituisce l'id di un punto date le coordinate.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param point_coord coordinate del punto
    * 
    * @return ritorna l'id del punto se esiste o la dimensione del vettore nel caso non esista
    */
    unsigned int getId(const PolyhedronCollection& p_coll, const Eigen::VectorXd& point_coord);


    /**
    * @brief Verifica se il punto appartiene alla sfera.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param point_id id del punto
    * 
    * @return ritorna se il punto appartiene alla sfera
    */
    bool isPointOnSphere(const PolyhedronCollection& p_coll, unsigned int point_id);


    /**
    * @brief Aggiunge un punto, se non esiste ancora.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param new_point coordinate del nuovo punto
    * @param checkpointexists indica se effettuare il controllo sull'esistenza del punto
    * @param shouldPrintVertexExists indica se stampare se il vertice esiste già
    * 
    * @return ritorna l'id del nuovo punto o di quello già esistente
    */
    unsigned int add(PolyhedronCollection& p_coll, const Eigen::Vector3d new_point, bool checkpointexists = true, bool shouldPrintVertexExists = false);


    /**
    * @brief Proietta il punto sulla sfera.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param point_id id del punto
    * @param inplace indica se sostituire le coordinate del punto proiettate (true) o creare un nuovo punto (false)
    * 
    * @return ritorna l'id della proiezione
    */
   unsigned int projectOnSphere(PolyhedronCollection& p_coll, unsigned int point_id, bool inplace = true);


    /**
    * @brief Calcola la distanza tra due punti.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param p1_id id del primo punto
    * @param p2_id id del secondo punto
    * 
    * @return ritorna la distanza tra i due punti.
    */
    double distance(const PolyhedronCollection& p_coll, unsigned int p1_id, unsigned int p2_id);


    /**
    * @brief Calcola la media aritmetica dei punti.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param id_points vettore degli id dei punti
    * 
    * @return ritorna la media aritmetica delle coordinate dei punti
    */
    Eigen::VectorXd averagePoints (const PolyhedronCollection& p_coll, const std::vector<unsigned int>& id_points);
    

    /**
    * @brief Calcola la media pesata (interpolazione lineare) tra due punti.
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param p1_id id del primo punto
    * @param p2_id id del secondo punto
    * @param weight peso verso il primo punto
    * 
    * @return ritorna le coordinate dell'interpolazione lineare.
    */
    Eigen::VectorXd interpolatePoints(const PolyhedronCollection& p_coll, unsigned int p1_id, unsigned int p2_id, double weight);


    /**
    * @brief Calcola il punto riflesso di un segmento
    * 
    * @param p_coll oggetto di PolyhedronCollection
    * @param first_endpoints id del primo punto del lato
    * @param second_endpoints id del secondo punto del lato
    * @param point_to_reflect id del punto da riflettere
    * 
    * @return ritorna l'id del punto riflesso (che è stato memorizzato).
    */
    unsigned int reflect(PolyhedronCollection& p_coll, unsigned int first_endpoints, unsigned int second_endpoints, unsigned int point_to_reflect);


    /**
    * @brief Conta il numero di vertici del solido geodetico di classe I e II
    * 
    * @param q denota il numero di vertici del poligono che si osserva guardando ciascun vertice
    * @param b parametro di triangolazione
    * @param c parametro di triangolazione
    * 
    * @return ritorna il numero di vertici del solido geodetico di classe I.
    */ 
    unsigned int countGeodesic(unsigned int q, unsigned int b, unsigned int c = 0);
}