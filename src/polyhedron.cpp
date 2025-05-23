#include "polyhedron.hpp"
#include <unordered_set>

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
            unsigned int n_elem = new_faces_id.size();

            // aggiungo le facce al poliedro
            p_coll.Cell3DsFaces.push_back(std::vector<unsigned int>(new_faces_id));  // copia di new_faces_id

            // creo i nodi e i vertici
            std::vector<unsigned int> edges;
            std::unordered_set<unsigned int> vertices;
            for(size_t i = 0; i < n_elem; i++){
                edges.insert(p_coll.Cell2DsEdges[i]);
                vertices.insert(p_coll.Cell2DsVertices[i]);
            }
        
            // converto in vettori
            p_coll.Cell3DsId.push_back(new_id);
            p_coll.Cell3DsVertices.push_back(vertices.begin(), vertices.end());
            p_coll.Cell3DsEdges.push_back(edges.begin(), edges.end());
            p_coll.NumCell3DsVertices.push_back(vertices.size());
            p_coll.NumCell3DsEdges.push_back(edges.size());
            p_coll.NumCell3DsFaces.push_back(new_faces_id.size());
            p_coll.NumCell3Ds ++;
        }
        return new_id;
   }

   unsigned int import_platonic_solid(PolyhedronCollection& p_coll, int p, int q){
        std::vector<unsigned int> vertices;
        std::string line;

        // 1) Lettura dei vertici
        std::ifstream file_v(prefix + "_vertex.txt");
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
        std::ifstream file_f(prefix + "_face.txt");
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

   int build_platonic_solid(PolyhedronCollection& p_coll, int p, int q){
        //identifica prefisso
        unsigned int poly_id = -1;

        if (p==3 && q==3) {
            poly_id = import_platonic_solid("tetrahedron");
        }

        else if (p==4 && q==3) {
            poly_id = import_platonic_solid("cube");
        }

        else if (p==5 && q==3) {
            poly_id = import_platonic_solid("dodecahedron");
        }

        else if (p==3 && q==4) { //ottaedro dal dualee del cubo
            poly_id = import_platonic_solid("cube");
            poly_id = createDual(poly_id);
        }

        else if (p==3 && q==5) {
            poly_id = import_platonic_solid("dodecahedron");
            poly_id = createDual(poly_id);
        }

        else {
            //caso non valido
            std::cerr << "Errore: combinazione {p,q} non valida\n";
        }
        return poly_id;
   }


   // rimuovo il punto, se esiste e non viene usato in nessun poliedro
   bool remove(PolyhedronCollection& p_coll, unsigned int face_id); // --- non so se ha senso

   unsigned int createDual(PolyhedronCollection& p_coll, unsigned int poly_id);
}