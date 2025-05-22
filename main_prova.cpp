#include "face.hpp"
#include "PolyhedronCollection.hpp"
#include "UCDUtilities.hpp"


void print_point(const PolyhedronCollection& p, unsigned int id_point){
    std::cout << "Id del punto: " << id_point << std::endl;
    std::cout << "Coordinate del punto " << id_point << ": " <<  p.Cell0DsCoordinates(0, id_point) << " " << p.Cell0DsCoordinates(1, id_point) << " "
    << p.Cell0DsCoordinates(2, id_point) << " " << std::endl;
}

void print_edge(const PolyhedronCollection& p, unsigned int id_edge){
    std::cout << "Id del lato: " << id_edge << std::endl;
    std::cout << "Estermi del lato " << id_edge << ": " <<  p.Cell1DsEndpoints(0, id_edge) << " " << p.Cell1DsEndpoints(1, id_edge) << std::endl;
}


void print_face(const PolyhedronCollection& p, unsigned int id_face){
    std::cout << "Id della faccia: " << id_face << std::endl;

    std::cout << "La faccia " << id_face << " ha " << p.NumCell2DsVertices[id_face] << " vertici." << std::endl;
    for(const auto elem : p.Cell2DsVertices[id_face])
        print_point(p, elem);

    std::cout << "La faccia " << id_face << " ha " << p.NumCell2DsEdges[id_face] << " lati." << std::endl;
    for(const auto elem : p.Cell2DsEdges[id_face])
        print_edge(p, elem);
}


void vertex_test(PolyhedronCollection& p);
void edge_test(PolyhedronCollection& p);
void face_test(PolyhedronCollection& p);

int main(){
    PolyhedronCollection p;
    std::cout << "********** TEST VERTICI ***************** " << std::endl;
    vertex_test(p);
    std::cout << "************************************** " << std::endl;
    std::cout << "********** TEST LATI ***************** " << std::endl;
    edge_test(p);
    std::cout << "************************************** " << std::endl;
    std::cout << "********** TEST FACCE ***************** " << std::endl;
    face_test(p);
    std::cout << "************************************** " << std::endl;
    
    Gedim::UCDUtilities utilities;
    {
        utilities.ExportPoints("./Cell0Ds.inp", p.Cell0DsCoordinates);
        utilities.ExportSegments("./Cell1Ds.inp",
                                 p.Cell0DsCoordinates,
                                 p.Cell1DsEndpoints);
    }
    

    return 0;
}

void vertex_test(PolyhedronCollection& p){
        // Inizializzazione del punto
        vertex::initialize(p, 5);
        std::cout << "Inizializzazione completata! È stata riservata una capacità pari a " << p.Cell0DsId.capacity() << "." << std::endl;
        unsigned int id_point;
    
        std::cout << "------ Aggiunta dei punti (con controllo) ------ " << std::endl;
        id_point = vertex::add(p, Eigen::Vector3d(0.0, 0.0, 0.0));
        print_point(p, id_point);
    
        id_point = vertex::add(p, Eigen::Vector3d(1.0, 0.0, 0.0));
        print_point(p, id_point);
    
        id_point = vertex::add(p, Eigen::Vector3d(0.0, 0.5, 0.0));
        print_point(p, id_point);
    
        id_point = vertex::add(p, Eigen::Vector3d(1.0, 1.0, 0.0));
        print_point(p, id_point);
    
        std::cout << "Quanti elementi ci sono: " << p.NumCell0Ds << std::endl;
    
        // Verifico se posso aggiungere un elemento che c'è già:
        id_point = vertex::add(p, Eigen::Vector3d(1.0, 1.0, 0.0));
        std::cout << "Id del punto (già esistente nella posizione 3): " << id_point << std::endl;
    
        std::cout << " ------- fine aggiunta controllata ---------" << std::endl;
    
        std::cout << "-------- inizio verifica funzioni con le coordinate ------- " << std::endl;
        // Verifico se il punto 1 è proeittato sulla sfera di raggio 1
        std::cout << "Il punto con id 1 è sulla sfera di raggio uno? " << (vertex::isPointOnSphere(p, 1)? "Sì" : "No") << std::endl;
        std::cout << "Il punto con id 2 è sulla sfera di raggio uno? " << (vertex::isPointOnSphere(p, 2)? "Sì" : "No") << std::endl;
        
        // Proiezione sulla sfera
        id_point = vertex::projectOnSphere(p, 2);
        std::cout << "Proietto il punto con id 2 sulla sfera." << std::endl;
        print_point(p, id_point);
    
        // Calcolo la distanza tra due punti
        std::cout << "Distanza tra il punto 0 e 1 è: " << vertex::distance(p, 0, 1) << std::endl;
    
        Eigen::VectorXd v = vertex::averagePoints(p, std::vector<unsigned int>{0, 1, 2, 3});
        std::cout << "La media dei quattro punti è: " << v(0) << " " << v(1) << " " << v(2) << std::endl;
    
        v = vertex::interpolatePoints(p, 0, 1, 0.4);
        std::cout << "L'interpolazione di 0.4 tra il punto 0 e 1 è: " << v(0) << " " << v(1) << " " << v(2) << std::endl;
    
        std::cout << "--------- fine funzioni con le coordinate ---------- " << std::endl;
    
        // creo un punto uguale
        std::cout << "creo un punto senza controllo (di uguali coordinate con un altro) " << std::endl;
        id_point = vertex::add(p, Eigen::Vector3d(0.0, 1.0, 0.0), false);
        print_point(p, id_point);
    
        // faccio la reshape
        std::cout << "------- faccio la reshape conservando i valori ----------" << std::endl;
        vertex::reshape(p, 6);
        std::cout << "Conservative reshape completata! È stata riservata una capacità pari a " << p.Cell0DsId.capacity() << "." << std::endl;
        std::cout << "Ci sono " << p.NumCell0Ds << " elementi." << std::endl;
        for(size_t i = 0; i < p.NumCell0Ds; i++)
            print_point(p, i);

}

void edge_test(PolyhedronCollection& p){
    // Inizializzazione del lato
    edge::initialize(p, 5);
    std::cout << "Inizializzazione completata! È stata riservata una capacità pari a " << p.Cell1DsId.capacity() << "." << std::endl;
    std::cout << "NUMERO DI COLONNE: " << p.Cell1DsEndpoints.cols() << std::endl;
    unsigned int id_edge;

    std::cout << "------ Aggiunta dei lati (con controllo) ------ " << std::endl;
    id_edge = edge::add(p, Eigen::Vector2i(0,1));
    print_edge(p, id_edge);

    id_edge = edge::add(p, Eigen::Vector2i(1, 3));
    print_edge(p, id_edge);

    id_edge = edge::add(p, Eigen::Vector2i(3, 2));
    print_edge(p, id_edge);

    id_edge = edge::add(p, Eigen::Vector2i(2, 1));
    print_edge(p, id_edge);

    std::cout << "Quanti elementi ci sono: " << p.NumCell1Ds << std::endl;

    //Proviamo ad aggiungere un elemento che già esiste
    id_edge = edge::add(p, Eigen::Vector2i(3, 1));
    std::cout << "Id del lato (già esistente nella posizione 1): " << id_edge << std::endl;
    std::cout << " ------- fine aggiunta controllata ---------" << std::endl;
    
    // calcoliamo la lunghezza di un segmento
    std::cout << "Lunghezza del segmento 0: " << edge::length(p, 0) << std::endl;


    std::cout << "creo un lato senza controllo (di uguali estremi con un altro) " << std::endl;
    id_edge = edge::add(p, Eigen::Vector2i(3, 2), false);
    print_edge(p, id_edge);

    // faccio la reshape
    std::cout << "------- faccio la reshape conservando i valori ----------" << std::endl;
    edge::reshape(p, 6);
    std::cout << "Conservative reshape completata! È stata riservata una capacità pari a " << p.Cell1DsId.capacity() << "." << std::endl;
    std::cout << "Ci sono " << p.NumCell1Ds << " elementi." << std::endl;
    for(size_t i = 0; i < p.NumCell1Ds; i++)
        print_edge(p, i);

    // faccio la reshape
    std::cout << "------- faccio la reshape non conservando i valori ----------" << std::endl;
    edge::reshape(p, 8, false);
    std::cout << "Reshape completata! È stata riservata una capacità pari a " << p.Cell1DsId.capacity() << "." << std::endl;
    std::cout << "Ci sono " << p.NumCell1Ds << " elementi." << std::endl;
    for(size_t i = 0; i < p.NumCell1Ds; i++)
        print_edge(p, i);


}


void face_test(PolyhedronCollection& p){
    // Inizializzazione della faccia
    face::initialize(p, 2);
    std::cout << "Inizializzazione completata! È stata riservata una capacità pari a " << p.Cell2DsId.capacity() << "." << std::endl;
    unsigned int id_face;

    std::cout << "------ Aggiunta delle facce (con controllo) ------ " << std::endl;
    id_face = face::add(p, std::vector<unsigned int>{0, 1, 2});
    print_face(p, id_face);

    id_face = face::add(p, std::vector<unsigned int>{2, 1, 3});
    print_face(p, id_face);

    std::cout << "Quanti elementi ci sono: " << p.NumCell2Ds << std::endl;

    //Proviamo ad aggiungere un elemento che già esiste
    id_face = face::add(p, std::vector<unsigned int>{2, 1, 0});
    std::cout << "Id del lato (già esistente nella posizione 0): " << id_face << std::endl;
    std::cout << " ------- fine aggiunta controllata ---------" << std::endl;

    std::cout << "-------- inizio verifica funzioni con le facce ------- " << std::endl;

    std::cout << "Calcoliamo il centroide della faccia 1." << std::endl;
    unsigned int id_point = face::getCentroid(p, 1);
    print_point(p, id_point);

    std::cout << "Le facce 0 e 1 sono adiacenti? " << (face::areAdjacent(p, 0, 1)? "Sì": "No") << std::endl;
    std::cout << "--------- fine funzioni con le coordinate ---------- " << std::endl;
    
    // creo una faccia uguale
    std::cout << "creo una faccia senza controllo (di uguaglianza con un'altra) " << std::endl;
    id_face = face::add(p, std::vector<unsigned int>{2, 3, 1}, false);
    print_face(p, id_face);

    // faccio la reshape
    std::cout << "------- faccio la reshape conservando i valori ----------" << std::endl;
    face::reshape(p, 6);
    std::cout << "Conservative reshape completata! È stata riservata una capacità pari a " << p.Cell2DsId.capacity() << "." << std::endl;
    std::cout << "Ci sono " << p.NumCell2Ds << " elementi." << std::endl;
    for(size_t i = 0; i < p.NumCell2Ds; i++)
        print_face(p, i);

    // faccio la reshape
    std::cout << "------- faccio la reshape non conservando i valori ----------" << std::endl;
    face::reshape(p, 8, false);
    std::cout << "Reshape completata! È stata riservata una capacità pari a " << p.Cell2DsId.capacity() << "." << std::endl;
    std::cout << "Ci sono " << p.NumCell2Ds << " elementi." << std::endl;
    for(size_t i = 0; i < p.NumCell2Ds; i++)
    print_face(p, i);



}
