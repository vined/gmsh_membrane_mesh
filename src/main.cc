#include <chrono>
#include <iostream>
#include <limits>
#include <vector>

#include "lib/gmsh.h"

#include "parameters-utils.h"
#include "timing-utils.h"

namespace model = gmsh::model;
namespace factory = gmsh::model::occ;

int main(int argc, char *argv[]) {
    auto start = std::chrono::high_resolution_clock::now();


    // Setting double precission
    typedef std::numeric_limits<double> dbl;
    std::cout.precision(dbl::max_digits10);


    std::cout << "Reading prameters file \"" << argv[1] << "\"" << std::endl;
    auto startParameters = std::chrono::high_resolution_clock::now();
    parameters params = readParameters(argv[1]);
    std::cout << "Values scaled up by: " << SCALE << std::endl;
    params.print();
    std::cout << "Done reading parameters. Duration: " << getDuration(startParameters) << std::endl;


    std::cout << "Constructing elements..." << std::endl;
    auto startConstructingElements = std::chrono::high_resolution_clock::now();

    gmsh::initialize();
    gmsh::option::setNumber("General.Terminal", 1);
    gmsh::option::setNumber("General.NumThreads", 8);
    gmsh::option::setNumber("Geometry.OCCParallel", 1);
    gmsh::option::setNumber("Mesh.Algorithm", 8);
    gmsh::option::setNumber("Mesh.MaxNumThreads1D", 8);
    gmsh::option::setNumber("Mesh.MaxNumThreads2D", 8);
    gmsh::option::setNumber("Mesh.MaxNumThreads3D", 8);
    gmsh::option::setNumber("Mesh.RecombineAll", 1);
    gmsh::option::setNumber("Mesh.SurfaceFaces", 1);
    gmsh::option::setNumber("Mesh.CharacteristicLengthExtendFromBoundary", 0);
    gmsh::option::setNumber("Mesh.CharacteristicLengthFromPoints", 0);
    gmsh::option::setNumber("Mesh.CharacteristicLengthFromCurvature", 0);

    gmsh::model::add("membrane-mesh");

    double lcMin = params.defects[0].second * 0.4;
    double lcMax = 100;//params.defects[0].second * 3.5;
    double rMin = 1;
    double rMax = 300;
    double eps = params.defects[0].second + (params.defects[0].second * 0.2);


    std::cout << "- Creating layers" << std::endl;

    std::vector<int> verticesTags;
    for (std::pair<double, double> v : params.vertices) {
        verticesTags.push_back(factory::addPoint(v.first, v.second, 0, lcMin));
    }

    std::vector<int> linesTags;
    for (int i = 0; i < verticesTags.size() - 1; i++) {
        linesTags.push_back(factory::addLine(verticesTags[i], verticesTags[i + 1]));
    }
    linesTags.push_back(factory::addLine(verticesTags[verticesTags.size() - 1], verticesTags[0]));

    int wire = factory::addWire(linesTags, -1, true);
    int plane = factory::addPlaneSurface({wire});

    std::vector<std::pair<int, int>> membrane;


    std::cout << "- Creating defects" << std::endl;

    std::vector<std::pair<int, int>> defects;
    for (auto d : params.defects) {
        int circle = factory::addDisk(d.first.first, d.first.second, 0, d.second, d.second);
        defects.push_back({2, circle});
    }

    std::cout << "- Cutting defects" << std::endl;
    std::vector<std::pair<int, int>> ov;
    std::vector<std::vector<std::pair<int, int>>> ovv;


    factory::cut({{2, plane}}, defects, ov, ovv, -1, false, true);
    factory::extrude({{2, ov[0].second}}, 0, 0, params.h_membrane, membrane, {}, {}, true);
    factory::remove({{2, plane}});

    // std::cout << "- Adding physical groups" << std::endl;
    // int membraneGroup = model::addPhysicalGroup(3, {3});
    // model::setPhysicalName(3, membraneGroup, "membrane");
    std::cout << "Done constructing elements. Duration: " << getDuration(startConstructingElements) << std::endl;


    std::cout << "Synchronize CAD model with Gmsh..." << std::endl;
    auto startSynchronize = std::chrono::high_resolution_clock::now();
    factory::synchronize();
    std::cout << "Done synchronizing. Duration: " << getDuration(startSynchronize) << std::endl;


    std::cout << "Start configuring mesh..." << std::endl;
    auto startConfiguringMesh = std::chrono::high_resolution_clock::now();


    std::vector<double> fields;
    // Set finer mesh around defect
    for (auto d : params.defects) {
        model::getEntitiesInBoundingBox(d.first.first - eps, d.first.second - eps,
                                        -eps,
                                        d.first.first + eps, d.first.second + eps,
                                        params.h_membrane + eps, ov, 1);

        std::vector<double> curves;
        for (auto dimtag : ov) {
            curves.push_back(dimtag.second);
        }

        int distanceId = model::mesh::field::add("Distance");
        model::mesh::field::setNumber(distanceId, "NNodesByEdge", 50);
        model::mesh::field::setNumbers(distanceId, "EdgesList", curves);

        int thresholdId = model::mesh::field::add("Threshold");
        model::mesh::field::setNumber(thresholdId, "IField", distanceId);
        model::mesh::field::setNumber(thresholdId, "DistMin", rMin);
        model::mesh::field::setNumber(thresholdId, "DistMax", rMax);
        model::mesh::field::setNumber(thresholdId, "LcMin", lcMin);
        model::mesh::field::setNumber(thresholdId, "LcMax", lcMax);
        model::mesh::field::setNumber(thresholdId, "StopAtDistMax", 1);
        fields.push_back(thresholdId);
    }

    int minId = model::mesh::field::add("Min");
    model::mesh::field::setNumbers(minId, "FieldsList", fields);
    model::mesh::field::setAsBackgroundMesh(minId);

    std::cout << "Done configuring mesh. Duration: " << getDuration(startConfiguringMesh) << std::endl;


    std::cout << "Generating 3D mesh" << std::endl;
    auto startGeneratingMesh = std::chrono::high_resolution_clock::now();
    model::mesh::generate(3);
    std::cout << "Done generating 3D mesh. Duration: " << getDuration(startGeneratingMesh) << std::endl;


    std::cout << "End. Total duration: " << getDuration(start) << std::endl;

    // Write mesh to file
    // auto startWriteToFile = std::chrono::high_resolution_clock::now();
    // gmsh::write("membrane.msh");
    // std::cout << "Done writing to file. Duration: " << getDuration(startWriteToFile) << std::endl;

    // Start Gmsh GUI
    gmsh::fltk::run();
    // Must be called after finishing useing GmshAPI
    gmsh::finalize();

    return 0;
}
