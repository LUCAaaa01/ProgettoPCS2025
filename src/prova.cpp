#include "PolyhedronCollection.hpp"
#include "face.hpp"

bool createDual(PolyhedronCollection& p_coll, unsigned int poly_id, unsigned int p, unsigned int q){ // !!! p e q sono i numerid schfff
    unsigned int new_n_vertices = p_coll.Cell3DsFaces[poly_id].size();
    unsigned int new_n_faces = p_coll.Cell3DsVertices[poly_id].size();

    //ridimensiono i vettori
    vertex::reshape(p_coll, p_coll.Cell0DsId.capacity() + new_n_vertices);
    edge::reshape(p_coll, p_coll.Cell1DsId.capacity() + new_n_vertices); // il numero di vertici è uguale al numero di lati
    face::reshape(p_coll, p_coll.Cell2DsId.capacity() + new_n_faces);

    // creo i centroidi

    //abbiamo scelto la unordered_map perché nel caso medio la complessità computazionale è O(1) (per la insert e la search)
    std::unordered_map<unsigned int, unsigned int> map_face_id_centroid; 
    std:.vector<unsigned int> dual_poly_id_faces = {};
    dual_poly_id_faces.reserve(new_n_faces);

    for(const auto face_id:p_coll.Cell3DsFaces[poly_id]){
        // calcolo il centroide della faccia, di id face_id e lo memorizzo nella mappa con key face_id
        vec_centroid[face_id] = vertex::getCentroid(p_coll, face_id);
    }

    // Calcolo (per ogni nodo del poliedro originale), le facce a cui il punto appartiene
    //for(size_t i = 0; i < p_coll.Cell3DsVertices[poly_id].size(); i++){
    for(const auto v:p_coll.Cell3DsVertices[poly_id]){
        unsigned int i = 0;
        std::array<unsigned int, p> incident_faces; //!!!! È p o q (il numero di shifff)
        // trovo tutte le facce che contengono il nodo v (da spostare in una funzione di face)
        for(const auto face_id:p_coll.Cell3DsFaces[poly_id]){
        //for(size_t j = 0; j < p_coll.Cell3DsFaces[poly_id].size(); j++){
            if(std::find(p_coll.Cell2DsVertices[face_id].begin(), p_coll.Cell2DsVertices[face_id].end(), v)
                                        != p_coll.Cell2DsVertices[face_id].end()){
                incident_faces[i] = _coll.Cell2DsVertices[face_id];
                i++;
                if(i == incident_faces.size()) //avendo la nuova faccia un numero di vertici noto, esco prima nel caso le abbia già trovate tutte
                    break;
            }
        }

        // Ordino le facce - si potrebbero usare alcuni algoritmi di ordinamento con opportune modifiche. Non vedo come applicare quelli visti che
        // hanno complessità computazionale O(nlogn), quindi lo faremo per il momento standard.
        std::array<unsigned int, p> centroid_vertices_id;//!!!!!! È p o q (il numero di shifff)

        //scelgo l'elemento di partenza
        centroid_vertices_id[0] = map_face_id_centroid.at(incident_faces[0]);

        for(size_t i = 0; i < incident_faces.size(); i++){ 
            for(size_t j = i+1; j < incident_faces.size(); j++){
                if(face::areAdjacent(p_coll, incident_faces[i], incident_faces[j])){
                    std::swap(incident_faces[i+1], incident_faces[j]);
                    centroid_vertices_id[i+1] = map_face_id_centroid.at(incident_faces[i+1]);
                    break;
                }
            }
        }

        // Creo la nuova faccia e la aggiungo alla lista delle facce
        dual_poly_id_faces.push_back(face::add(p_coll, centroid_vertices_id));

    }
    // Creo il nuovo poliedro
    polyhedron::add(p_coll, dual_poly_id_faces);
}

// Sarebbe comodo avere una funzione che semplicemente passando il vettore delle facce costruisca il poliedro