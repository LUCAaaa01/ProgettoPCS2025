#include "face.hpp"
#include <unordered_set>
#include <cmath>

namespace face{
    
    void initialize(PolyhedronCollection& p_coll, unsigned int n_faces){
        // inizializza le variabili legate alle facce
        p_coll.NumCell2Ds = 0;
        p_coll.Cell2DsId = {};
        p_coll.NumCell2DsVertices = {};
        p_coll.NumCell2DsEdges = {};
        p_coll.Cell2DsId.reserve(n_faces); // riserva una capacità di n_faces per gli id delle facce
        p_coll.Cell2DsVertices.reserve(n_faces); // riserva una capacità di n_faces per memorizzare vettori di punti
        p_coll.Cell2DsEdges.reserve(n_faces); // riserva una capacità di n_faces per memorizzare vettori di lati
    }
    
    void reshape(PolyhedronCollection& p_coll, unsigned int new_n_faces, bool preservedata){
        if(preservedata){
            p_coll.Cell2DsId.reserve(new_n_faces); // riserva una capacità di new_n_faces per gli id delle facce
            p_coll.NumCell2DsVertices.reserve(new_n_faces); // riserva una capacità di new_n_faces per il vettore che contiene il numero di vertici delle facce
            p_coll.NumCell2DsEdges.reserve(new_n_faces); // riserva una capacità di new_n_faces per il vettore che contiene il numero di lati delle facce
            p_coll.Cell2DsVertices.reserve(new_n_faces); // riserva una capacità di new_n_faces per memorizzare vettori di punti
            p_coll.Cell2DsEdges.reserve(new_n_faces); // riserva una capacità di new_n_faces per memorizzare vettori di lati
        }else{
            //Inizializziamo nuovamente le variabili legate alle facce
            initialize(p_coll, new_n_faces);
        }
    }

    bool isEqual(const std::vector<unsigned int>& f1, const std::vector<unsigned int>& f2){
        // Due facce sono uguali se hanno gli stessi punti (e sono in sequenza).

        bool result = false;
        assert((f1.size() > 2 && f2.size() > 2) && "La faccia deve avere almeno tre punti! Impossibile effettuare l'operazione richiesta.");

        // verifichiamo che le facce abbiano lao stesso numero di lati
        if(f1.size() == f2.size()){
            //cerchiamo in che posizione della seconda faccia si trova il primo punto della prima faccia
            auto it = std::find(f2.begin(), f2.end(), f1[0]); 

            //se il primo elemento è presente nella seconda faccia
            if(it != f2.end()){
                size_t first_index = std::distance(f2.begin(), it); // ricavo il primo indice
                int direction = 1;

                // determina la direzione
                if(f1[1] == f2[(f2.size() + first_index - 1)%f2.size()])
                    direction = -1; // il secondo vettore va letto in senso inverso

                result = true;
                size_t i = 0;

                //verifico l'uguaglianza dei due vettori
                while(result && i < f1.size()){
                    result = f1[i] == f2[(f2.size() + first_index + direction*i)%f2.size()]; // verifico se gli elementi sono uguali
                    i++;
                }
            }
        }
        return result;
    }

    unsigned int getId(const PolyhedronCollection& p_coll, const std::vector<unsigned int> vertices_id){
        unsigned int index = p_coll.Cell2DsId.size();
        
        for(size_t i = 0; i < p_coll.Cell2DsId.size(); i++){
            // controlliamo che la faccia esista
            if(isEqual(p_coll.Cell2DsVertices[i], vertices_id)){
                index = i;
                break;
            }
        }
        return index;
    }

    unsigned int add(PolyhedronCollection& p_coll, const std::vector<unsigned int>& new_vertices_id, bool checkfaceexists){
        unsigned int old_id = p_coll.Cell2DsId.size();
        unsigned int new_id = 0;
        if(p_coll.NumCell2Ds > 0)
            new_id = p_coll.Cell2DsId.back() + 1; // creiamo un nuovo id

        // facciamo il controllo dell'esistenza della faccia (se richiesto)
        if(checkfaceexists)
            old_id = getId(p_coll, new_vertices_id);

        // verifichiamo che la faccia esista 
        if(old_id < p_coll.Cell2DsId.size()){
            std::cout << "La faccia esiste già!" <<std::endl;
            new_id = old_id;
        }
        else{
            // nel caso non esista, lo aggiungiamo
            unsigned int n_elem = new_vertices_id.size();

            // aggiungo i veritici alla faccia
            p_coll.Cell2DsVertices.push_back(std::vector<unsigned int>(new_vertices_id));  // copia di new_vertices_id

            // creo un vettore di lati
            std::vector<unsigned int> edges;
            edges.reserve(n_elem); //riservo una capacità pari al numero di lati

            for(size_t i = 0; i < n_elem; i++){
                //aggiungo un nuovo lato
                edges.push_back(edge::add(p_coll,Eigen::Vector2i(new_vertices_id[i], new_vertices_id[(i+1)%n_elem])));
            }

            p_coll.Cell2DsId.push_back(new_id);
            p_coll.Cell2DsEdges.push_back(edges); // aggiungo i lati alla faccia
            p_coll.NumCell2DsVertices.push_back(new_vertices_id.size());
            p_coll.NumCell2DsEdges.push_back(new_vertices_id.size()); // il numero di lati in una faccia è uguale al numero di punti
            p_coll.NumCell2Ds ++;
        }
        return new_id;
    }

    // rimuovo il punto, se esiste e non viene usato in nessun poliedro
    bool remove(PolyhedronCollection& p_coll, unsigned int face_id); // --- non so se ha senso

    unsigned int getCentroid(PolyhedronCollection& p_coll, unsigned int face_id, bool projection){
        assert(contains(p_coll.Cell2DsId, face_id) && "Faccia non esistente! Impossibile effettuare l'operazione richiesta.");
        
        // calcolo il centroide e lo aggiungo alla lista dei punti
        unsigned int point_id = vertex::add(p_coll, vertex::averagePoints(p_coll, p_coll.Cell2DsVertices[face_id]));
        if(projection)
            vertex::projectOnSphere(p_coll, point_id);
        return point_id;
    }
 
    bool areAdjacent(PolyhedronCollection& p_coll, unsigned int f1_id, unsigned int f2_id){
        bool result = false;
        //verifichiamo l'esistenza della faccia
        assert(contains(p_coll.Cell2DsId, f1_id) && contains(p_coll.Cell2DsId, f2_id)
                && "Faccia non esistente! Impossibile effettuare l'operazione richiesta.");

        // creiamo un set contenente gli elementi del primo vettore (complessità O(n))
        std::unordered_set<unsigned int> set1(p_coll.Cell2DsEdges[f1_id].begin(), p_coll.Cell2DsEdges[f1_id].end()); 

        // controlliamo se almeno un elemento del secondo vettore è nel set. Complessità (O(n))
        for (const auto elt : p_coll.Cell2DsEdges[f2_id]) {
            //verifichiamo se hanno un elemento in comune
            if (set1.find(elt) != set1.end()){
                result =  true;
                break;
            }
        }
        return result;
    }

    std::vector<unsigned int> computeCharacteristicTriangulation(PolyhedronCollection& p_coll, unsigned int face_id, unsigned int b){
        std::vector<unsigned int> new_faces_id = {};

        std::vector<unsigned int> vertices_id;
        vertices_id.reserve(factorial(b));
        vertices_id.push_back(p_coll.Cell2DsVertices[face_id][0]);

        for(size_t i = 1; i <= b; i++){
            // punto iniziale della riga
            vertices_id.push_back(vertex::add(p_coll, vertex::interpolatePoints(p_coll, p_coll.Cell2DsVertices[face_id][0], p_coll.Cell2DsVertices[face_id][1], i/(1.0*b))));

            for(size_t j = 0; j < i-1; j++){
                // creo il nodo intermedio
                vertices_id.push_back(vertex::reflect(p_coll, vertices_id[vertices_id.size()-1 - i], vertices_id[vertices_id.size()-1 - i + 1], vertices_id[vertices_id.size()-1 - (2*i-1)]));

                // creo le due facce triangolari
                new_faces_id.push_back(face::add(p_coll, {vertices_id.back(), vertices_id[vertices_id.size()-1 -1], vertices_id[vertices_id.size()-1 -1 -i]}));
                new_faces_id.push_back(face::add(p_coll, {vertices_id.back(), vertices_id[vertices_id.size()-1 -i], vertices_id[vertices_id.size()-1 -1 -i]}));
            }

            // punto finale della riga;
            vertices_id.push_back(vertex::add(p_coll, vertex::interpolatePoints(p_coll, p_coll.Cell2DsVertices[face_id][0], p_coll.Cell2DsVertices[face_id][2], i/(1.0*b))));

            // creo il triangolo finale:
            new_faces_id.push_back(face::add(p_coll, {vertices_id.back(), vertices_id[vertices_id.size()-1 -1], vertices_id[vertices_id.size()-1-1-i]}));
        }
        std::cout << "Ho appena terminato la triangolazione. Il numero di vertici che ho inserito sono: " << vertices_id.size() << std::endl;
        std::cout << "Il numero di vertici che sono stati inseriti sono: " << p_coll.Cell0DsId.size() << std::endl;
        return new_faces_id;
    }

    unsigned int countGeodesicClassI(unsigned int q, unsigned int b, unsigned int c){
        unsigned int t = b*b + 2*b*c + c*c;
        unsigned int n_faces = 0;
        switch (q) {
            case 3:
                n_faces = 4*t;
                break;
        
            case 4:
                n_faces = 8*t;
                break;

            case 5:
                n_faces = 20*t;
                break;
        
            default:
                std::cerr << "Error! q si not valid!" << std::endl;
                break;
        }
        return n_faces;
    }
}
