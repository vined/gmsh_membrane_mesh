#include <chrono>
#include <iostream>

#include "lib/gmsh.h"

#include "parameters-utils.h"
#include "timing-utils.h"

namespace model = gmsh::model;
namespace factory = gmsh::model::occ;

int main(int argc, char *argv[]) {
    auto start = std::chrono::high_resolution_clock::now();


    std::cout << "Reading prameters file \"" << argv[1] << "\"" << std::endl;
    auto startParameters = std::chrono::high_resolution_clock::now();

    parameters params = readParameters(argv[1]);
    std::cout << params.toString() << std::endl;

    std::cout << "Done reading parameters. Duration: " << getDuration(startParameters) << std::endl;


    std::cout << "Initializing Gmsh" << std::endl;
    auto startGeneratingMesh = std::chrono::high_resolution_clock::now();

    gmsh::initialize();
    gmsh::option::setNumber("General.Terminal", 1);
    gmsh::model::add("mebrane-mesh");


    std::cout << "Adding base" << std::endl;


    int bigBox = factory::addBox(0, 0, 0, params.l, params.l, params.h);

    int hole = factory::addCylinder(params.l / 2., params.l / 2., -params.h, 0, 0, params.h*3., params.r);

    std::vector<std::pair<int, int> > ov;
    std::vector<std::vector<std::pair<int, int> > > ovv;
    factory::cut({{3, bigBox}}, {{3, hole}}, ov, ovv, 3, true, false);


    // synchronize the CAD model with the Gmsh model
    factory::synchronize();


    std::cout << "Setting meshing parameters" << std::endl;
    double lcar1 = .1;
    double lcar2 = .01;
    double lcar3 = .02;

    model::getEntities(ov, 0);
    model::mesh::setSize(ov, lcar1);

    double eps = 0.3;
    model::getEntitiesInBoundingBox(0.5 - eps, 0.5 - eps, -params.h - eps,
                                    0.5 + eps, 0.5 + eps, params.h*3 + eps, ov, 0);
    model::mesh::setSize(ov, lcar2);

    model::getBoundary({{3, hole}}, ov, false, false, true);
    model::mesh::setSize(ov, lcar3);



    // generate 3D mesh
    model::mesh::generate(3);


    std::cout << "Done generating mesh. Duration: " << getDuration(startGeneratingMesh) << std::endl;


    std::cout << "End. Total duration: " << getDuration(start) << std::endl;

    // Write mesh to file
    // auto startWriteToFile = std::chrono::high_resolution_clock::now();
    // gmsh::write("membrane.msh");
    // std::cout << "Done writing to file. Duration: " << getDuration(startWriteToFile) << std::endl;

    // Start Gmsh GUI
    gmsh::fltk::run();
    // Must be called after finishing to use GmshAPI
    gmsh::finalize();

    return 0;
}
