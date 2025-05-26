#include "polyhedron.hpp"
#include <unordered_set>
#include <fstream>

namespace polyhedron{
    void initialize(PolyhedronCollection& p_coll, unsigned int n_polyhedron){
        // inizializza le variabili legate ai poliedri
        p_coll.NumCell3Ds = 0;
        p_coll.Cell3DsId = {};
        p_coll.NumCell3DsVertices = {};
        p_coll.NumCell3DsEdges = {};
        p_coll.NumCell3DsFaces = {};
        p_coll.Cell3DsId.reserve(n_polyhedron); // riserva una capacità di n_polyhedron per gli id dei poliedri
        p_coll.Cell3DsVertices.reserve(n_polyhedron); // riserva una capacità di n_polyhedron per memorizzare vettori di punti
        p_coll.Cell3DsEdges.reserve(n_polyhedron); // riserva una capacità di n_polyhedron per memorizzare vettori di lati
        p_coll.Cell3DsFaces.reserve(n_polyhedron); // riserva una capacità di n_polyhedron per memorizzare vettori di facce
    }

    void reshape(PolyhedronCollection& p_coll, unsigned int new_n_polyhedron, bool preservedata){
        if(preservedata){
            p_coll.Cell3DsId.reserve(new_n_polyhedron); // riserva una capacità di new_n_polyhedron per gli id delle facce
            p_coll.NumCell3DsVertices.reserve(new_n_polyhedron); // riserva una capacità di new_n_polyhedron per il vettore che contiene il numero di vertici del poliedro
            p_coll.NumCell3DsEdges.reserve(new_n_polyhedron); // riserva una capacità di new_n_polyhedron per il vettore che contiene il numero di lati del poliedro
            p_coll.NumCell3DsFaces.reserve(new_n_polyhedron); // riserva una capacità di new_n_polyhedron per il vettore che contiene il numero di facce del poliedro
            p_coll.Cell3DsVertices.reserve(new_n_polyhedron); // riserva una capacità di new_n_polyhedron per memorizzare vettori di punti
            p_coll.Cell3DsEdges.reserve(new_n_polyhedron); // riserva una capacità di new_n_polyhedron per memorizzare vettori di lati
            p_coll.Cell3DsFaces.reserve(new_n_polyhedron); // riserva una capacità di new_n_polyhedron per memorizzare vettori di facce
        }else{
            //Inizializziamo nuovamente le variabili legate ai poliedri
            initialize(p_coll, new_n_polyhedron);
        }
    }

    bool isEqual(const std::vector<unsigned int>& p1, const std::vector<unsigned int>& p2){
        // due poliedri sono uguali se hanno le stesse facce
        bool result = true;

        // controllo se hanno lo stesso numero di facce
        if (p1.size() == p2.size()){
             // creiamo un set contenente gli elementi del primo vettore (complessità O(n))
            std::unordered_set<unsigned int> set1(p1.begin(), p1.end()); 

            // controlliamo se almeno un elemento del secondo vettore è nel set. Complessità (O(n))
            for (const auto face_id : p2) {
                //verifichiamo se non hanno un elemento in comune
                if (set1.find(face_id) == set1.end()){
                    result =  false;
                    break;
                }
            }
        }else{
            result = false;
        }

        return result;
    }

    unsigned int getId(const PolyhedronCollection& p_coll, const std::vector<unsigned int> faces_id){
        unsigned int index = p_coll.Cell3DsId.size();
            
        for(size_t i = 0; i < p_coll.Cell3DsId.size(); i++){
            // controlliamo che la faccia esista
            if(isEqual(p_coll.Cell3DsFaces[i], faces_id)){
                index = i;
                break;
            }
        }
        return index;
    }

    unsigned int add(PolyhedronCollection& p_coll, const std::vector<unsigned int>& new_faces_id, bool checkpolyexists){
        unsigned int old_id = p_coll.Cell3DsId.size();
        unsigned int new_id = 0;
        if(p_coll.NumCell3Ds > 0)
            new_id = p_coll.Cell3DsId.back() + 1; // creiamo un nuovo id

        // facciamo il controllo dell'esistenza del poliedro (se richiesto)
        if(checkpolyexists)
            old_id = getId(p_coll, new_faces_id);

        // verifichiamo che il poliedro esista 
        if(old_id < p_coll.Cell3DsId.size()){
            std::cout << "Il poliedro esiste già!" <<std::endl;
            new_id = old_id;
        }
        else{
            // nel caso non esista, lo aggiungiamo
            // aggiungo le facce al poliedro
            p_coll.Cell3DsFaces.push_back(std::vector<unsigned int>(new_faces_id));  // copia di new_faces_id

            // creo i nodi e i vertici
            std::unordered_set<unsigned int> edges = {};
            std::unordered_set<unsigned int> vertices = {};

            for(const auto face_id : new_faces_id){
                edges.insert(p_coll.Cell2DsEdges[face_id].begin(), p_coll.Cell2DsEdges[face_id].end());
                vertices.insert(p_coll.Cell2DsVertices[face_id].begin(), p_coll.Cell2DsVertices[face_id].end());
            }
        
            // converto in vettori
            p_coll.Cell3DsId.push_back(new_id);
            p_coll.Cell3DsVertices.push_back(std::vector <unsigned int> (vertices.begin(), vertices.end()));
            p_coll.Cell3DsEdges.push_back(std::vector <unsigned int> (edges.begin(), edges.end()));
            p_coll.NumCell3DsVertices.push_back(vertices.size());
            p_coll.NumCell3DsEdges.push_back(edges.size());
            p_coll.NumCell3DsFaces.push_back(new_faces_id.size());
            p_coll.NumCell3Ds ++;
        }
        return new_id;
    }

    unsigned int import_platonic_solid(PolyhedronCollection& p_coll, const std::string& prefix, 
                unsigned int n_vertices, unsigned int n_edges, unsigned int n_faces)
    {
        vertex::initialize(p_coll, n_vertices);
        edge::initialize(p_coll, n_edges);
        face::initialize(p_coll, n_faces);

        std::vector<unsigned int> vertices;
        std::string line;

        // 1) Lettura dei vertici
        std::ifstream file_v("./" + prefix + "_vertex.txt");
        if (!file_v) {
            std::cerr << "Error opening file " << prefix << "_vertex.txt\n";
            return {};
        }
        std::getline(file_v, line); // skip header
        while (std::getline(file_v, line)) {
            if (line.empty()) continue;
            std::istringstream ss(line);
            unsigned int vid; double x,y,z;
            if (!(ss >> vid >> x >> y >> z)) {
                std::cerr << "Invalid vertex line: '" << line << "'\n";
                return {};
            }
            vertices.push_back(vertex::add(p_coll, Eigen::Vector3d(x, y, z)));
        }
        file_v.close();

        // 2) Lettura delle facce (solo vertici)
        std::vector<unsigned int> faces_id;
        std::ifstream file_f("./" + prefix + "_face.txt");
        if (!file_f) {
            std::cerr << "Error opening file " << prefix << "_face.txt\n";
            return {};
        }
        std::getline(file_f, line); // skip header
        while (std::getline(file_f, line)) {
            if (line.empty()) continue;
            std::istringstream ss(line);
            unsigned int fid, nV;
            ss >> fid >> nV;
            std::vector<unsigned int> vertIds(nV);
            for (unsigned int i = 0; i < nV; ++i) ss >> vertIds[i];
            faces_id.push_back(face::add(p_coll, vertIds));
        }
        file_f.close();
        return add(p_coll, faces_id);
    }

    int build_platonic_solid(PolyhedronCollection& p_coll, unsigned int p, unsigned int q){
        //identifica prefisso
        unsigned int poly_id = -1;

        if (p==3 && q==3) {
            poly_id = import_platonic_solid(p_coll, "tetrahedron", 4, 6, 4);
        }

        else if (p==4 && q==3) {
            poly_id = import_platonic_solid(p_coll, "cube", 8, 12, 6);
        }

        else if (p==5 && q==3) {
            poly_id = import_platonic_solid(p_coll, "dodecahedron",20, 30, 12);
        }

        else if (p==3 && q==4) { //ottaedro dal dualee del cubo
            poly_id = import_platonic_solid(p_coll, "cube", 8, 12, 6);
            std::cout << "Cubo creato" << std::endl;
            poly_id = createDual(p_coll, poly_id, p);
        }

        else if (p==3 && q==5) {
            poly_id = import_platonic_solid(p_coll, "dodecahedron", 20, 30, 12);
            poly_id = createDual(p_coll, poly_id, p);
        }

        else {
            //caso non valido
            std::cerr << "Errore: combinazione {p,q} non valida\n";
        }
        return poly_id;
    }


    // rimuovo il punto, se esiste e non viene usato in nessun poliedro
    bool remove(PolyhedronCollection& p_coll, unsigned int face_id); // --- non so se ha senso

    unsigned int createDual(PolyhedronCollection& p_coll, unsigned int poly_id, const unsigned int q){
        unsigned int new_n_vertices = p_coll.Cell3DsFaces[poly_id].size();
        unsigned int new_n_faces = p_coll.Cell3DsVertices[poly_id].size();
    
        //ridimensiono i vettori
        vertex::reshape(p_coll, p_coll.Cell0DsId.capacity() + new_n_vertices);
        edge::reshape(p_coll, p_coll.Cell1DsId.capacity()*2); // il numero di lati è uguale al numero di lati del poliedro originale
        face::reshape(p_coll, p_coll.Cell2DsId.capacity() + new_n_faces);
    
        // creo i centroidi
    
        //abbiamo scelto la unordered_map perché nel caso medio la complessità computazionale è O(1) (per la insert e la search)
        std::unordered_map<unsigned int, unsigned int> map_face_id_centroid; 
        std::vector<unsigned int> dual_poly_id_faces;
        dual_poly_id_faces.reserve(new_n_faces);
    
        for(const auto face_id:p_coll.Cell3DsFaces[poly_id]){
            // calcolo il centroide della faccia, di id face_id e lo memorizzo nella mappa con key face_id
            map_face_id_centroid[face_id] = face::getCentroid(p_coll, face_id);
        }
    
        // Calcolo (per ogni nodo del poliedro originale), le facce a cui il punto appartiene
        for(const auto v:p_coll.Cell3DsVertices[poly_id]){
            std::vector<unsigned int> incident_faces; 
            incident_faces.reserve(q);
            // trovo tutte le facce che contengono il nodo v (da spostare in una funzione di face)
            for(const auto face_id:p_coll.Cell3DsFaces[poly_id]){
                if(std::find(p_coll.Cell2DsVertices[face_id].begin(), p_coll.Cell2DsVertices[face_id].end(), v)
                                            != p_coll.Cell2DsVertices[face_id].end()){
                    incident_faces.push_back(face_id);
                    //if(incident_faces.size() == q) //avendo la nuova faccia un numero di vertici noto, esco prima nel caso le abbia già trovate tutte
                        //break;
                }
            }
    
            // Ordino le facce - si potrebbero usare alcuni algoritmi di ordinamento con opportune modifiche. Non vedo come applicare quelli visti che
            // hanno complessità computazionale O(nlogn), quindi lo faremo per il momento standard.
            std::vector<unsigned int> centroid_vertices_id;
            centroid_vertices_id.reserve(q);
    
            //scelgo l'elemento di partenza
            centroid_vertices_id.push_back(map_face_id_centroid.at(incident_faces[0]));
    
            for(size_t i = 0; i < incident_faces.size(); i++){ 
                for(size_t j = i+1; j < incident_faces.size(); j++){
                    if(face::areAdjacent(p_coll, incident_faces[i], incident_faces[j])){
                        std::swap(incident_faces[i+1], incident_faces[j]);
                        centroid_vertices_id.push_back(map_face_id_centroid.at(incident_faces[i+1]));
                        break;
                    }
                }
                
            }

    
            // Creo la nuova faccia e la aggiungo alla lista delle facce
            dual_poly_id_faces.push_back(face::add(p_coll, centroid_vertices_id));
    
        }
        // Creo il nuovo poliedro
        return add(p_coll, dual_poly_id_faces);
    }

    unsigned int buildGeodesicClassI(PolyhedronCollection& p_coll, unsigned int poly_id, unsigned int q, unsigned int b){
        vertex::reshape(p_coll, p_coll.Cell0DsId.capacity() + vertex::countGeodesicClassI(q, b, 0));
        edge::reshape(p_coll, p_coll.Cell1DsId.capacity() + edge::countGeodesicClassI(q, b, 0));
        face::reshape(p_coll, p_coll.Cell2DsId.capacity() + face::countGeodesicClassI(q, b, 0));

        std::vector<unsigned int> faces_id = {};
        for(const auto face_id: p_coll.Cell3DsFaces[poly_id]){
            std::vector<unsigned int> el = face::computeCharacteristicTriangulation(p_coll, face_id, b);
            faces_id.insert(faces_id.end(), el.begin(), el.end());
        }

        unsigned int pid = add(p_coll, faces_id);
        for(const auto vertex_id: p_coll.Cell3DsVertices[pid])
            vertex::projectOnSphere(p_coll, vertex_id);

        return pid;
    }

}