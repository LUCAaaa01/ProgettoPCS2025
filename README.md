# ProgettoPCS2025
/**
 * @mainpage Struttura e utilizzo del progetto
 *
 * Questo progetto implementa una struttura dati leggera per rappresentare poliedri, tramite la struct PolyhedronCollection.
 *
 * I dati (vertici, lati, facce, poliedri) sono accessibili tramite appositi namespace:
 * - vertex::
 * - edge::
 * - face::
 * - polyhedron::
 *
 * Ogni namespace contiene funzioni standard (`add`, `getId`, `initialize`, ecc.) e funzioni specifiche (es. `createDual`, `projectOnSphere`, ecc.).
 *
 * Importante:
 * - L'utente **non dovrebbe accedere direttamente agli attributi di PolyhedronCollection**
 * - Tutte le operazioni vanno fatte tramite le funzioni fornite
 */

 /**
 * @example
 * @brief Esempio d'uso corretto delle funzioni del namespace vertex e face.
 *
 * PolyhedronCollection p_coll;
 * unsigned int vertex_id, face_id;
 * vertex_id = vertex::add(p_coll, Eigen::Vector3d(x, y, z));
 * face_id = face::add(p_coll, vertIds);
 */


