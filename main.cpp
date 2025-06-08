#include "polyhedron.hpp"
#include "PolyhedronCollection.hpp"
#include "UCDUtilities.hpp"
#include <set>
#include <string>

void exportPolyhedronCollectionOnParaview(const PolyhedronCollection& p_coll, unsigned int poly_id,
                                            std::vector<int>& vertices_path, std::vector<int>& edges_path);

int main(int argc, char* argv[]){
    // 4 0 6 parametri più il nome del programma
    if(argc != 5 and argc != 7){
        std::cerr << "Errore! Numero di parametri non valido!" << std::endl;
        return 1;
    }

    // leggiamo i valori in input
    unsigned int p = std::stoul(argv[1]);
    unsigned int q = std::stoul(argv[2]);

    unsigned int b = std::stoul(argv[3]);
    unsigned int c = std::stoul(argv[4]);

    // controllo che la triangolazione non sia di classe III
    if(c != 0 and c != b){
        std::cerr << "Errore! Il parametro c non è corretto!" << std::endl;
        return 2;
    }

    // creiamo l'oggetto di PolyhedronCollection
    PolyhedronCollection p_coll;

    unsigned int poly_id;
    bool graphIsWeighted = true;
    if(p == 3){
        std::cout << "Creazione del poliedro geodetico ... " << std::endl;
        poly_id = polyhedron::createGeodesicPolyhedron(p_coll, p, q, b, c);
        std::cout << "Poliedro geodetico creato!" << std::endl;
        graphIsWeighted = false;
    }
    else{
        std::cout << "Creazione del poliedro di Goldberg ... " << std::endl;
        poly_id = polyhedron::createGoldbergPolyhedron(p_coll, p, q, b, c);
        std::cout << "Poliedro di Goldberg creato!" << std::endl;
    }
    
    //creiamo se richiesto lo shortest path
    std::vector<int> vertices_path = {};
    std::vector<int> edges_path = {};

    if(argc == 7){
        unsigned int id1 = std::stoul(argv[5]);
        unsigned int id2 = std::stoul(argv[6]);
        while((!utils::contains(p_coll.Cell3DsVertices[poly_id], id1) || !utils::contains(p_coll.Cell3DsVertices[poly_id], id2))){
            std::cout << "Sembra ci sia un errore con i vertici che hai inserito! Questi potrebbero non appartenere al poliedro o non esistere." << std::endl;
            std::cout << "I vertici che appartengono al poliedro sono: " << std::endl;
            for(const auto vertice_id : p_coll.Cell3DsVertices[poly_id])
                std::cout << vertice_id << " - ";
            std::cout << std::endl;

            std::cout << "Inserire l'id del vertice di partenza: ";
            std::cin >> id1;
            std::cout << "Inserire l'id del vertice di arrivo: ";
            std::cin >> id2;
        }

        std::cout << "Cerco il cammino migliore tra il vertice con id " << id1 << " e il vertice con id " << id2 << " ..." << std::endl;
        double distance_path = polyhedron::findShortestPath(p_coll, poly_id, id1, id2, vertices_path, edges_path, graphIsWeighted);
        std::cout << "Cammino trovato!" << std::endl;


        // Stampo informazioni del cammino
        std::cout << "Lunghezza complessiva del cammino: " << distance_path << std::endl;
        std::cout << "Numero di lati nel cammino minimo: " << edges_path.size() << std::endl;
    }

    std::cout << "Esportazione dei poliedri creati in corso ... " << std::endl;
    exportPolyhedronCollectionOnParaview(p_coll, poly_id, vertices_path, edges_path);
    std::cout << "Esportazione completata correttamente!" << std::endl;

    std::cout << "Esportazione dei dati nei file txt ..." << std::endl;
    utils::exportToTxt(p_coll);
    std::cout << "Esportazione completata!" << std::endl;

    return 0;
}


void exportPolyhedronCollectionOnParaview(const PolyhedronCollection& p_coll, unsigned int poly_id,
                                            std::vector<int>& vertices_path, std::vector<int>& edges_path){
    Gedim::UCDUtilities utilities;
    {
        std::vector<Gedim::UCDProperty<double>> cell0Ds_properties(2);

        // etichetta del marker per sapere se il vertice appartiene al poliedro
        cell0Ds_properties[0].Label = "FinalPolyhedron";
        cell0Ds_properties[0].UnitLabel = "-";
        cell0Ds_properties[0].NumComponents = 1;

        // etichetta del marker per sapere se il vertice appartiene al cammino minimo
        cell0Ds_properties[1].Label = "ShortPath";
        cell0Ds_properties[1].UnitLabel = "-";
        cell0Ds_properties[1].NumComponents = 1;

        
        std::vector<double> pfv(p_coll.NumCell0Ds, 0.0);
        std::vector<double> spv(p_coll.NumCell0Ds,   0.0);

        // mettiamo a 1 i marker dei vertici che appartengono al poliedro finale
        for(const auto idv_p : p_coll.Cell3DsVertices[poly_id])
            pfv.at(idv_p) = 1;

        // mettiamo a 1 i marker dei vertici che appartengono al cammino più breve
        for(const auto idv_sp : vertices_path)
            spv.at(idv_sp) = 1;

        cell0Ds_properties[0].Data = pfv.data();
        cell0Ds_properties[1].Data = spv.data();

        utilities.ExportPoints("./Cell0Ds.inp",
                               p_coll.Cell0DsCoordinates,
                               cell0Ds_properties);
    }

    {

        std::vector<Gedim::UCDProperty<double>> cell1Ds_properties(2);

        // etichetta del marker per sapere se il lato appartiene al poliedro
        cell1Ds_properties[0].Label = "FinalPolyhedron";
        cell1Ds_properties[0].UnitLabel = "-";
        cell1Ds_properties[0].NumComponents = 1;

        // etichetta del marker per sapere se il lato appartiene al cammino minimo
        cell1Ds_properties[1].Label = "ShortPath";
        cell1Ds_properties[1].UnitLabel = "-";
        cell1Ds_properties[1].NumComponents = 1;

        std::vector<double> pfe(p_coll.NumCell1Ds, 0.0);
        std::vector<double> spe(p_coll.NumCell1Ds,   0.0);

        // mettiamo a 1 i marker dei vertici che appartengono al poliedro finale
        for(const auto ide_p : p_coll.Cell3DsEdges[poly_id])
            pfe.at(ide_p) = 1;

        // mettiamo a 1 i marker dei vertici che appartengono al cammino più breve
        for(const auto ide_sp : edges_path)
            spe.at(ide_sp) = 1;

        cell1Ds_properties[0].Data = pfe.data();
        cell1Ds_properties[1].Data = spe.data();

        utilities.ExportSegments("./Cell1Ds.inp",
                                 p_coll.Cell0DsCoordinates,
                                 p_coll.Cell1DsEndpoints,
                                 {},
                                 cell1Ds_properties);
    }

}