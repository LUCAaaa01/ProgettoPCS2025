#include "edge.hpp"
#include <string>
#include <fstream>

namespace edge{
    void initialize(PolyhedronCollection& p_coll, unsigned int n_edges){
        // inizializza le variabili legati ai lati
        p_coll.NumCell1Ds = 0;
        p_coll.Cell1DsId.clear();
        p_coll.Cell1DsId.reserve(n_edges); // riserva una capacità di n_edges
        p_coll.Cell1DsEndpoints = Eigen::MatrixXi(2, n_edges); //creo una matrice 2xNumeroLati (fromPointId - toPointId)
    }
    

    void reshape(PolyhedronCollection& p_coll, unsigned int new_n_edges, bool preservedata){
        if(preservedata){
            // Usiamo conservativeResize per preservare i lati esistenti
            p_coll.Cell1DsEndpoints.conservativeResize(2, new_n_edges);
            p_coll.Cell1DsId.reserve(new_n_edges); // riserva una capacità di new_n_edges
        }else{
            //Inizializziamo nuovamente le variabili legate ai lati
            initialize(p_coll, new_n_edges);
        }
    }

    bool isEqual(const Eigen::VectorXi& e1, const Eigen::VectorXi& e2){
        //controlla l'uguaglianza essendo un lato per sua natura un arco non orientato
        return (e1 == e2) || (e1 == e2.reverse());
    }

    
    unsigned int getId(const PolyhedronCollection& p_coll, const Eigen::VectorXi& endpoints_id){
        unsigned int index = p_coll.Cell1DsId.size();

        for(size_t i = 0; i < p_coll.Cell1DsId.size(); i++){
            // verifichiamo che il lato esista 
            if(isEqual(p_coll.Cell1DsEndpoints.col(i), endpoints_id)){
                index = i;
                break;
            }
        }
        return index;
    }

    unsigned int add(PolyhedronCollection& p_coll, const Eigen::Vector2i new_endpoints_id, bool checkedgeexists, bool shouldPrintEdgeExists){
        unsigned int old_id = p_coll.Cell1DsId.size();
        unsigned int new_id = 0;
        if(p_coll.NumCell1Ds > 0)
            new_id = p_coll.Cell1DsId.back() + 1; // creiamo un nuovo id
        
        // verifichiamo di poter creare un nuovo lato
        assert(p_coll.Cell1DsEndpoints.cols() >= new_id && "La matrice degli estremi è completa! Non è possibile aggiungere altri lati.");

        // facciamo il controllo dell'esistenza del lato (se richiesto)
        if(checkedgeexists)
            old_id = getId(p_coll, new_endpoints_id);

        // verifichiamo che il lato esista 
        if(old_id < p_coll.Cell1DsId.size()){
            if (shouldPrintEdgeExists)
                std::cout << "Il lato esiste già!" <<std::endl;
            new_id = old_id;
        }
        else{
            // nel caso non esista, lo aggiungiamo
            p_coll.Cell1DsEndpoints(0, new_id) = new_endpoints_id(0);
            p_coll.Cell1DsEndpoints(1, new_id) = new_endpoints_id(1);
            p_coll.Cell1DsId.push_back(new_id);
            p_coll.NumCell1Ds ++;
        }
        return new_id;
    }

    double length(PolyhedronCollection& p_coll, unsigned int edge_id){
        // controlliamo che l'id del lato esista
        assert(utils::contains(p_coll.Cell1DsId, edge_id) && "Lato non esistente! Impossibile effettuare l'operazione richiesta.");

        // calcolo la distanza tra gli estremi
        return vertex::distance(p_coll, p_coll.Cell1DsEndpoints(0, edge_id), p_coll.Cell1DsEndpoints(1, edge_id));
    }

    unsigned int countGeodesic(unsigned int q, unsigned int b, unsigned int c){
        unsigned int t = b*b + b*c + c*c;
        if(c != 0)
            t = 3*(b+1)*b; // formula aggiornata nel caso della classe II

        unsigned int n_edges = 0;
        switch (q) {
            case 3:
                n_edges = 6*t;
                break;
        
            case 4:
                n_edges = 12*t;
                break;

            case 5:
                n_edges = 30*t;
                break;
        
            default:
                std::cerr << "Error! q si not valid!" << std::endl;
                break;
        }
        return n_edges;
    }

    void exportTxt(const PolyhedronCollection& p_coll, const std::string& path){
        std::ofstream f(path + "Cell1Ds.txt");
        f << "id v0 v1\n";
        for (unsigned int i = 0; i < p_coll.NumCell1Ds; ++i) { 
            //estrae gli ID dei due vertici estremi della matrice Cell1DsExtrema
            int v0 = p_coll.Cell1DsEndpoints(0, i);
            int v1 = p_coll.Cell1DsEndpoints(1, i);
            f << i << " " << v0 << " " << v1 <<  "\n";
        }
    }
}