#include "polyhedron.hpp"
#include <unordered_set>
#include <fstream>
#include <queue>
#include <map>

namespace polyhedron{
    void initialize(PolyhedronCollection& p_coll, unsigned int n_polyhedron){
        // inizializza le variabili legate ai poliedri
        p_coll.NumCell3Ds = 0;
        p_coll.Cell3DsId.clear();
        p_coll.NumCell3DsVertices.clear();
        p_coll.NumCell3DsEdges.clear();
        p_coll.NumCell3DsFaces.clear();
        p_coll.Cell3DsVertices.clear();
        p_coll.Cell3DsEdges.clear();
        p_coll.Cell3DsFaces.clear();

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

    unsigned int add(PolyhedronCollection& p_coll, const std::vector<unsigned int>& new_faces_id, bool checkpolyexists, bool shouldPrintPolyExists){
        unsigned int old_id = p_coll.Cell3DsId.size();
        unsigned int new_id = 0;
        if(p_coll.NumCell3Ds > 0)
            new_id = p_coll.Cell3DsId.back() + 1; // creiamo un nuovo id

        // facciamo il controllo dell'esistenza del poliedro (se richiesto)
        if(checkpolyexists)
            old_id = getId(p_coll, new_faces_id);

        // verifichiamo che il poliedro esista 
        if(old_id < p_coll.Cell3DsId.size()){
            if (shouldPrintPolyExists)
                std::cout << "Il poliedro esiste già!" <<std::endl;
            new_id = old_id;
        }
        else{
            // nel caso non esista, lo aggiungiamo
            // aggiungiamo le facce al poliedro
            p_coll.Cell3DsFaces.push_back(std::vector<unsigned int>(new_faces_id));  // copia di new_faces_id

            // creiamo i nodi e i vertici
            std::unordered_set<unsigned int> edges = {};
            std::unordered_set<unsigned int> vertices = {};

            for(const auto face_id : new_faces_id){
                edges.insert(p_coll.Cell2DsEdges[face_id].begin(), p_coll.Cell2DsEdges[face_id].end());
                vertices.insert(p_coll.Cell2DsVertices[face_id].begin(), p_coll.Cell2DsVertices[face_id].end());
            }
        
            // convertiamo in vettori
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

        else if (p==3 && q==4) { //creiamo l'ottaedro come duale del cubo
            poly_id = import_platonic_solid(p_coll, "cube", 8, 12, 6);
            poly_id = createDual(p_coll, poly_id, p);
        }

        else if (p==3 && q==5) { //creiamo l'icosaedro come il duale del dodecaedro
            poly_id = import_platonic_solid(p_coll, "dodecahedron", 20, 30, 12);
            poly_id = createDual(p_coll, poly_id, p);
        }

        else {
            //caso non valido
            std::cerr << "Errore: combinazione {p,q} non valida\n";
        }
        return poly_id;
    }

    unsigned int createDual(PolyhedronCollection& p_coll, unsigned int poly_id, const unsigned int q){
        // alcuni controlli sull'input
        assert((contains(p_coll.Cell3DsId, poly_id)) && "Errore! L'id del poliedro non è valido!");

        unsigned int new_n_vertices = p_coll.Cell3DsFaces[poly_id].size();
        unsigned int new_n_faces = p_coll.Cell3DsVertices[poly_id].size();
    
        //ridimensioniamo i vettori
        vertex::reshape(p_coll, p_coll.Cell0DsId.capacity() + new_n_vertices);
        edge::reshape(p_coll, p_coll.Cell1DsId.capacity()*2); // il numero di lati è uguale al numero di lati del poliedro originale
        face::reshape(p_coll, p_coll.Cell2DsId.capacity() + new_n_faces);
    
        // creiamo i centroidi
    
        //abbiamo scelto la unordered_map perché nel caso medio la complessità computazionale è O(1) (per la insert e la search)
        std::map<unsigned int, unsigned int> map_face_id_centroid; 
        std::vector<unsigned int> dual_poly_id_faces;
        dual_poly_id_faces.reserve(new_n_faces);
    
        for(const auto face_id:p_coll.Cell3DsFaces[poly_id]){
            // calcoliamo il centroide della faccia, di id face_id e lo memorizzo nella mappa con key face_id
            map_face_id_centroid[face_id] = face::getCentroid(p_coll, face_id);
        }
    
        // Calcoliamo (per ogni nodo del poliedro originale), le facce a cui il punto appartiene
        for(const auto v:p_coll.Cell3DsVertices[poly_id]){
            std::vector<unsigned int> incident_faces; 
            incident_faces.reserve(q);
            // troviamo tutte le facce che contengono il nodo v (da spostare in una funzione di face)
            for(const auto face_id:p_coll.Cell3DsFaces[poly_id]){
                if(std::find(p_coll.Cell2DsVertices[face_id].begin(), p_coll.Cell2DsVertices[face_id].end(), v)
                                            != p_coll.Cell2DsVertices[face_id].end()){
                    incident_faces.push_back(face_id);
                }
            }
    
            // Ordiniamo le facce

            std::vector<unsigned int> centroid_vertices_id;
            centroid_vertices_id.reserve(q);
    
            //scegliamo l'elemento di partenza
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

    
            // Creiamo la nuova faccia e la aggiungiamo alla lista delle facce
            dual_poly_id_faces.push_back(face::add(p_coll, centroid_vertices_id));
    
        }
        // Creiamo il nuovo poliedro
        return add(p_coll, dual_poly_id_faces);
    }

    unsigned int buildGeodesicClassI(PolyhedronCollection& p_coll, unsigned int poly_id, unsigned int q, unsigned int b){
        // alcuni controlli sull'input
        assert((contains(p_coll.Cell3DsId, poly_id)) && "Errore! L'id del poliedro non è valido!");

        // effettuiamo la reshape dei vertici, lati e facce
        vertex::reshape(p_coll, p_coll.Cell0DsId.capacity() + vertex::countGeodesic(q, b));
        edge::reshape(p_coll, p_coll.Cell1DsId.capacity() + edge::countGeodesic(q, b));
        face::reshape(p_coll, p_coll.Cell2DsId.capacity() + face::countGeodesic(q, b));

        // effettuiamo la triangoalzione di classe I per ogni faccia del poliedro
        std::vector<unsigned int> faces_id = {};
        for(const auto face_id: p_coll.Cell3DsFaces[poly_id]){
            std::vector<unsigned int> el = face::computeClassICharacteristicTriangulation(p_coll, face_id, b);
            // memorizziamo le nuove facce
            faces_id.insert(faces_id.end(), el.begin(), el.end());
        }

        // creiamo il nuovo poliedro
        unsigned int pid = add(p_coll, faces_id);

        // proiettiamo tutti i punti sulla sfera
        for(const auto vertex_id: p_coll.Cell3DsVertices[pid])
            vertex::projectOnSphere(p_coll, vertex_id);

        return pid;
    }

    unsigned int buildGeodesicClassII(PolyhedronCollection& p_coll, unsigned int poly_id, unsigned int q, unsigned int b){
        // alcuni controlli sull'input
        assert((contains(p_coll.Cell3DsId, poly_id)) && "Errore! L'id del poliedro non è valido!");

        // effettuiamo la reshape dei vertici, lati e facce
        vertex::reshape(p_coll, p_coll.Cell0DsId.capacity() + vertex::countGeodesic(q, b, 0) + vertex::countGeodesic(q, b, b));
        edge::reshape(p_coll, p_coll.Cell1DsId.capacity() + edge::countGeodesic(q, b, 0) + edge::countGeodesic(q, b, b));
        face::reshape(p_coll, p_coll.Cell2DsId.capacity() + face::countGeodesic(q, b, 0) + face::countGeodesic(q, b, b));

        // effettuiamo la triangoalzione di classe II per ogni faccia del poliedro
        std::vector<unsigned int> faces_id = {};
        for(const auto face_id: p_coll.Cell3DsFaces[poly_id]){
            std::vector<unsigned int> el = face::computeClassIICharacteristicTriangulation(p_coll, face_id, b);
            // memorizziamo le nuove facce
            faces_id.insert(faces_id.end(), el.begin(), el.end());
        }

        // creiamo il nuovo poliedro
        unsigned int pid = add(p_coll, faces_id);

        // proiettiamo tutti i punti sulla sfera
        for(const auto vertex_id: p_coll.Cell3DsVertices[pid])
            vertex::projectOnSphere(p_coll, vertex_id);

        return pid;
    }
    
    int createGeodesicPolyhedron(PolyhedronCollection& p_coll, unsigned int p, unsigned int q, unsigned int b, unsigned int c){
        initialize(p_coll, 2);
        //costruiamo il solido platonico
        int poly_id = build_platonic_solid(p_coll, p, q);
        if(poly_id == -1)
            std::cout << "Errore! Non è stato possibile creare il solido platonico!" << std::endl;
        else{
            // costruiamo il poliedro geodetico
            if(c == 0)
                poly_id = buildGeodesicClassI(p_coll, poly_id, q, b);
            else
                poly_id = buildGeodesicClassII(p_coll, poly_id, q, b);
            }

        return poly_id;
    }
    
    
    int createGoldbergPolyhedron(PolyhedronCollection& p_coll, unsigned int p, unsigned int q, unsigned int b, unsigned int c){
        int poly_id = createGeodesicPolyhedron(p_coll, q, p, b, c);
        if(poly_id != -1){
            reshape(p_coll, 3);
             // creiamo il duale (del duale)
            poly_id = createDual(p_coll, poly_id, p);
        }
        return poly_id;
    }

    double findShortestPath(PolyhedronCollection& p_coll, unsigned int poly_id, unsigned int id1, unsigned int id2, 
        std::vector<int>& vertices_path, std::vector<int>& edges_path, bool graphIsWeighted){
        
        // alcuni controlli sull'input
        assert((contains(p_coll.Cell3DsId, poly_id)) && "Errore! L'id del poliedro non è valido!");
        assert((contains(p_coll.Cell3DsVertices[poly_id], id1) && contains(p_coll.Cell3DsVertices[poly_id], id2)) &&
                "Errore! Gli id scelgono non esistono o non appartengono al poliedro!");

        std::map<int, int> id_to_index; // Mappa: ID reale -> indice interno
        std::vector<int> index_to_id;   // Mappa inversa: indice interno -> ID reale

        const auto& vertex_ids = p_coll.Cell3DsVertices[poly_id];
        // definiamo il numero di vertici
        const int n = vertex_ids.size();

        for (int i = 0; i < n; ++i) {
            id_to_index[vertex_ids[i]] = i;
            index_to_id.push_back(vertex_ids[i]);
        }

        // definiamo il vettore della distanza (inizialmente "infinito")
        std::vector<double> dist(n, std::numeric_limits<double>::infinity());
        // definiamo i vettori che contengono rispettivamente l'id dei vertici predecessori e l'id degli archi predecessori
        std::vector<int> prevV(n, -1);
        std::vector<int> prevE(n, -1);

        // definiamo la lista di adiacenza, dove il vettore i-esimo contiene gli elementi adiacenti,
        // memorizzati come una coppia (id vertice adiacente, id arco che li unisce)
        std::vector<std::vector<std::pair<int,int>>> adj(n);

        // costruiamo la lista di adiacenza 
        for(const auto id_edge:p_coll.Cell3DsEdges[poly_id]){
            // ricaviamo l'id dei vertici dell'arco
            unsigned int u = id_to_index[p_coll.Cell1DsEndpoints(0, id_edge)];
            unsigned int v = id_to_index[p_coll.Cell1DsEndpoints(1, id_edge)];

            // li aggiungiamo alla lista di adiacenza
            adj[u].emplace_back(v, id_edge);
            adj[v].emplace_back(u, id_edge);
        }

        // applichiamo l'algoritmo di Dijkstra utilizzando una coda con priorità
        using P = std::pair<double,int>;
        std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
        dist[id_to_index[id1]] = 0.0;
        pq.emplace(0.0, id_to_index[id1]); //aggiungiamo alla coda il nodo sorgente con distanza 0 (da se stesso)

        while (!pq.empty()) {
            //estriamo il primo elemento
            auto [d, u] = pq.top();
            pq.pop();

            //controlliamo se la distanza è maggiore di quella migliore già trovata
            if (d > dist[u]) 
                continue; // salta questa iterazione

            // controlliamo se il vertice u è quello di destinazione
            if (u == id_to_index[id2]) 
            break;  // abbiamo già trovato la distanza minima per id2

            // per ogni vertice adiacente a u
            for (auto& edge : adj[u]) {
                // estriamo dalla lista di adiacenza l'id del vertice adiacente e l'id dell'arco che li collega
                int v       = edge.first;
                int edgeIdx = edge.second;

                // controlliamo che il grafo sia pesato
                double w = 1;
                if(graphIsWeighted){
                    // se è pesato calcoliamo il peso come la distanza euclidea tra i due punti
                     w = vertex::distance(p_coll, index_to_id[u], index_to_id[v]);
                } // se non è pesato il peso è unitario per tutti i nodi
            
                // se passando per u abbiamo trovato un percorso più veloce per v
                if (dist[v] > dist[u] + w) {
                    // aggiorniamo il percorso migliore per v
                    dist[v] = dist[u] + w;
                    prevV[v] = u;
                    prevE[v] = edgeIdx;
                    pq.emplace(dist[v], v); // lo aggiungiamo alla coda
                }
            }
        }

        // costruiamo il percorso
    
        // risaliamo dai predecessori per ricostruire i vettori di cammino
        for (int v = id_to_index[id2]; v != -1; v = prevV[v]) {
            vertices_path.push_back(index_to_id[v]);
            if (prevE[v] != -1) 
                edges_path.push_back(prevE[v]);
        }

        // inverto i vettori
        std::reverse(vertices_path.begin(), vertices_path.end());
        std::reverse(edges_path.begin(),    edges_path.end());

        return dist[id_to_index[id2]];
    }
            
}
