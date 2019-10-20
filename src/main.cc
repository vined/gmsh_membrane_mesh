#include <chrono>
#include <iostream>

#include "lib/gmsh.h"

#include "parameters-utils.h"
#include "timing-utils.h"


int main(int argc, char *argv[]) {
    auto start = std::chrono::high_resolution_clock::now();


    std::cout << "Reading prameters file \"" << argv[1] << "\"" << std::endl;
    auto startParameters = std::chrono::high_resolution_clock::now();

    parameters params = readParameters(argv[1]);
    std::cout << params.toString() << std::endl;

    std::cout << "Done reading parameters. Duration: " << getDuration(startParameters) << std::endl;


    std::cout << "Generating mesh" << std::endl;
    auto startGeneratingMesh = std::chrono::high_resolution_clock::now();

    gmsh::initialize(argc, argv);
    gmsh::option::setNumber("General.Terminal", 1);

    gmsh::model::add("test");

    // add discrete surface with tag 1
    gmsh::model::addDiscreteEntity(2, 1);

    // add 4 mesh nodes
    gmsh::model::mesh::addNodes(2, 1,
                                {1, 2, 3, 4}, // node tags: 1, 2, 3, and 4
                                {0., 0., 0., // coordinates of node 1
                                 1., 0., 0., // coordinates of node 2
                                 1., 1., 0., // ...
                                 0., 1., 0.});

    // add 2 triangles
    gmsh::model::mesh::addElements(2, 1,
                                   {2}, // single type : 3-node triangle
                                   {{1, 2}}, // triangle tags: 1 and 2
                                   {{1, 2, 3, // triangle 1: nodes 1, 2, 3
                                            1, 3, 4}}); // triangle 2: nodes 1, 3, 4

//    gmsh::write("result.msh");

    gmsh::finalize();

    std::cout << "Done generating mesh. Duration: " << getDuration(startGeneratingMesh) << std::endl;


    std::cout << "End. Total duration: " << getDuration(start) << std::endl;
}
