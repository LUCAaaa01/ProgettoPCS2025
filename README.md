# ProgettoPCS2025
## Struttura e utilizzo del progetto
### Obiettivo del progetto
 * Il codice ha lo scopo di rappresentare in modo efficiente e modulare una struttura dati per poliedri
 * (geodetici, di Goldberg, ecc.), gestendone vertici, lati, facce e celle 3D.
 *
 * ## Struttura dati principale
 * Tutti i dati sono contenuti in una sola struct: `PolyhedronCollection`, che include:
 * - una lista di vertici (0D)
 * - una lista di lati (1D)
 * - una lista di facce (2D)
 * - una lista di poliedri (3D)
 *
 * Gli elementi sono identificati tramite ID numerici e riferimenti incrociati (ad esempio, un lato contiene gli ID dei vertici estremi).
 *
 * ## Organizzazione del codice: namespace
 * Il progetto è suddiviso logicamente in namespace:
 * - `vertex::` per tutte le operazioni sui vertici
 * - `edge::` per i lati
 * - `face::` per le facce
 * - `polyhedron::` per i poliedri
 *
 * Ogni namespace gestisce **solo i dati di sua competenza**.
 * Tutte le modifiche o letture a `PolyhedronCollection` dovrebbero avvenire attraverso le funzioni nei namespace.
 *
 * ## Funzioni standard e specifiche
 * Ogni namespace include:
 * - **Funzioni standard**: `add`, `initialize`, `getId`, `isEqual`, ecc.
 * - **Funzioni specifiche** (legate al contesto):
 *   - Esempio: `face::triangulateGeodesicClassI`, `vertex::projectOnSphere`, ecc.
 *
 * ## Esempio d'uso consigliato
 * ```cpp
 * PolyhedronCollection p_coll;
 * vertex::add(p_coll, 1.0, 0.0, 0.0);          // Aggiunge un vertice
 * face::add(p_coll, {v0, v1, v2});             // Aggiunge una faccia specificando solo i vertici
 * ```
 *
 * ## Nota sulla sicurezza
 * I dati di `PolyhedronCollection` sono pubblici, per ragioni di efficienza e semplicità.
 * Tuttavia, l'accesso diretto **è sconsigliato**:
 * - L'interfaccia tramite namespace fornisce controlli e coerenza
 * - Il codice è progettato per essere utilizzato in modo sicuro **tramite le funzioni fornite**

