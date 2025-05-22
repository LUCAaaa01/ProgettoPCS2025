#include "vertex.hpp"

inline constexpr double PRECISION_TOLLERANCE = 1e-6;

namespace vertex{

    void initialize(PolyhedronCollection& p_coll, unsigned int n_points){
        // inizializziamo le variabili legate ai punti
        p_coll.NumCell0Ds = 0;
        p_coll.Cell0DsId = {};
        p_coll.Cell0DsId.reserve(n_points); // riserva una capacità di n_points
        p_coll.Cell0DsCoordinates = Eigen::MatrixXd::Zero(3, n_points); //creo una matrice 3xNumeroPunti
    }

    void reshape(PolyhedronCollection& p_coll, unsigned int new_n_points, bool preservedata){
        if(preservedata){
            // Usiamo conservativeResize per preservare i punti esistenti
            p_coll.Cell0DsCoordinates.conservativeResize(3, new_n_points);
            p_coll.Cell0DsId.reserve(new_n_points); // riserva una capacità di new_n_points
        }else{
            // inizializziamo nuovamente le variabili legati ai punti
            initialize(p_coll, new_n_points);
        }
    }

    bool isEqual(const Eigen::VectorXd& p1, const Eigen::VectorXd& p2){
        return std::abs(p1(0) - p2(0)) < PRECISION_TOLLERANCE &&
               std::abs(p1(1) - p2(1)) < PRECISION_TOLLERANCE &&
               std::abs(p1(2) - p2(2)) < PRECISION_TOLLERANCE;
    }
    
    unsigned int getId(const PolyhedronCollection& p_coll, const Eigen::VectorXd& point_coord){
        unsigned int index = p_coll.Cell0DsId.size();
        
        for(size_t i = 0; i < p_coll.Cell0DsId.size(); i++){
            // verifichiamo che il punto esista 
            if(isEqual(p_coll.Cell0DsCoordinates.col(i), point_coord)){
                index = i;
                break;
            }
        }
        return index;
    }

    bool isPointOnSphere(const PolyhedronCollection& p_coll, unsigned int point_id){
        //  verifichiamo che l'id del punto sia valido
        assert(contains(p_coll.Cell0DsId, point_id) && "Punto non esistente! Impossibile effettuare l'operazione richiesta.");
        return p_coll.Cell0DsCoordinates.col(point_id).norm() == 1; //controlliamo se appartiene alla sfera 
    }

    unsigned int add(PolyhedronCollection& p_coll, const Eigen::Vector3d new_point, bool checkpointexists){
        unsigned int old_id = p_coll.Cell0DsId.size(); 

        unsigned int new_id = 0;
        if(p_coll.NumCell0Ds > 0)
            new_id = p_coll.Cell0DsId.back() + 1; // creiamo un nuovo id
        
        // verifichiamo di poter creare un nuovo punto
        assert(p_coll.Cell0DsCoordinates.cols() >= new_id && "La matrice delle coordinate è completa! Non è possibile aggiungere altri punti.");

        // facciamo il controllo dell'esistenza del punto se richiesto
        if(checkpointexists)
            old_id = getId(p_coll, new_point);
        
        // verifichiamo che il punto esista 
        if(old_id < p_coll.Cell0DsId.size()){
            std::cout << "Il punto esiste già!" <<std::endl;
            new_id = old_id; //memorizziamo l'id del punto già esistente
        }
        else{
            // nel caso non esista, lo aggiungiamo
            p_coll.Cell0DsCoordinates(0, new_id) = new_point(0);
            p_coll.Cell0DsCoordinates(1, new_id) = new_point(1);
            p_coll.Cell0DsCoordinates(2, new_id) = new_point(2);
            p_coll.Cell0DsId.push_back(new_id);
            p_coll.NumCell0Ds ++;
        }
        return new_id;
    }


    // rimuoviamo il punto, se esiste e non viene usato in nessun segmento o faccia
    bool remove(PolyhedronCollection& p_coll, unsigned int point_id); // --- non so se utile

    unsigned int projectOnSphere(PolyhedronCollection& p_coll, unsigned int point_id, bool inplace){
        unsigned int result = point_id;
        // controlliamo che l'id del punto esista
        assert(contains(p_coll.Cell0DsId, point_id) && "Punto non esistente! Impossibile effettuare l'operazione richiesta.");
        Eigen::VectorXd v = p_coll.Cell0DsCoordinates.col(point_id).normalized(); //creo il punto proiettato sulla sfera di raggio 1

        if(inplace)
            p_coll.Cell0DsCoordinates.col(point_id) = v; //modifichiamo le coordinate del punto esistente
        else
            result = add(p_coll, v); //aggiungiamo il punto
        
        return result;
    }

    double distance(const PolyhedronCollection& p_coll, unsigned int p1_id, unsigned int p2_id){
        assert((contains(p_coll.Cell0DsId, p1_id) && contains(p_coll.Cell0DsId, p2_id))
                && "Punto non esistente! Impossibile effettuare l'operazione richiesta.");
        
        return (p_coll.Cell0DsCoordinates.col(p1_id) - p_coll.Cell0DsCoordinates.col(p2_id)).norm();

    }

    Eigen::VectorXd averagePoints (const PolyhedronCollection& p_coll, const std::vector<unsigned int>& id_points){
        // controlliamo che il vettore degli id contenga almeno un elemento
        assert(id_points.size() > 0 && "Impossibile fare la media! Non sono stati dati punti in input!");

        Eigen::VectorXd v(3);
        v << 0.0, 0.0, 0.0;
        for(const auto elt : id_points){
            // controlliamo che l'id del punto esista
            assert(contains(p_coll.Cell0DsId, elt) && "Punto non esistente! Impossibile effettuare l'operazione richiesta.");
            v += p_coll.Cell0DsCoordinates.col(elt);
        }
        return v/id_points.size();
    }

    Eigen::VectorXd interpolatePoints(const PolyhedronCollection& p_coll, unsigned int p1_id, unsigned int p2_id, double weight){
        // controlliamo che l'id dei punti esistano
        assert((contains(p_coll.Cell0DsId, p1_id) && contains(p_coll.Cell0DsId, p2_id))
        && "Punto non esistente! Impossibile effettuare l'operazione richiesta.");

        //verifichiamo che il peso sia valido
        assert(weight >= 0 && weight <= 1 && "Peso non valido! Impossibile effettuare l'interpolazione.");

        return (1-weight)*p_coll.Cell0DsCoordinates.col(p1_id) + weight*p_coll.Cell0DsCoordinates.col(p2_id);
    }
}