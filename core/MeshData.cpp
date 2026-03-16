#include "MeshData.h"

#include <fstream>

void MeshData::LoadMesh_Femmesh(const std::string& filename)
{
    Log_Info("Start loading mesh data from Femmesh ...");
    Log_Info("  file name: {}", filename.c_str());
    std::ifstream input(filename);
    if (!input.is_open())
    {
        //ETMESH_EXIT("Fail to open Femmesh input file, file name = {}", filename.c_str());
    }
    std::string line;
    femmshData = new FemmshData();
    input >> line >> femmshData->version;
    input >> line >> femmshData->tolerance;
    input >> line >> femmshData->unit_name;
    input >> line >> femmshData->num_bodies;
    input >> line >> femmshData->num_facets;
    input >> line >> femmshData->num_points;
    input >> line >> femmshData->num_surf_elems;
    input >> line >> femmshData->num_vol_elems;

    input >> line;
    FIND_MESH_FILE_TAG(line, "begin_body_data");

    FemmshData::Body tmpbody;

    for (int i = 0; i < femmshData->num_bodies; ++i)
    {
        input >> line >> tmpbody.body_id;
        input >> line >> tmpbody.body_name;
        input >> line;
        getline(input, tmpbody.material_name);
        tmpbody.material_name.erase(0, 1); // delete space
        input >> line >> tmpbody.layer_name;
        input >> line >> tmpbody.net_name;
        input >> line >> tmpbody.sheet_body_flag;
        input >> line >> tmpbody.nfaces_on_body;

        input >> line;
        tmpbody.face_ids.resize(tmpbody.nfaces_on_body);
        for (int j = 0; j < tmpbody.nfaces_on_body; ++j)
        {
            input >> tmpbody.face_ids[j];
        }
    }

    femmshData->bodies.emplace(tmpbody.body_id, tmpbody);

    input >> line;
    FIND_MESH_FILE_TAG(line, "end_body_data");

    input >> line;

    FIND_MESH_FILE_TAG(line, "begin_facet_data");

    FemmshData::Facet tmpfacet;
    for (int i = 0;i < femmshData->num_facets;++i)
    {
        input >> line >> tmpfacet.facet_id;
        input >> line >> tmpfacet.planar_flag;
        input >> line >> tmpfacet.nfaces_on_facet;

        input >> line;
        tmpfacet.face_ids.resize(tmpfacet.nfaces_on_facet);
        for (int j = 0;j < tmpfacet.nfaces_on_facet;++j)
        {
            input >> tmpfacet.face_ids[j];
        }

        femmshData->facets.emplace(tmpfacet.facet_id, tmpfacet);
    }

    input >> line;
    FIND_MESH_FILE_TAG(line, "end_facet_data");

    input >> line;
    FIND_MESH_FILE_TAG(line, "begin_point_data");

    // Load node information
    CreateDummyNode();

    int nodeID;
    double x, y, z;
    for (int i = 1; i <= femmshData->num_points; ++i)
    {
        input >> nodeID >> x >> y >> z;
        if (nodeID != i)
        {
            TETMESH_EXIT("Invalid mesh input: node ID is not continuous, input {} / count {}", nodeID, i);
        }
        // Allocate memory and construct a new node
        nodePool.construct(nodeID, Point3D{ x, y, z });
    }

    input >> line;
    FIND_MESH_FILE_TAG(line, "end_point_data");

    input >> line;
    FIND_MESH_FILE_TAG(line, "begin_surf_elem_data");

    // Load element information
    int tetID, triID, geomID, nNodes;
    int nodeIDs[4] = { -1, -1, -1, -1 };

    // Used to check if there are triangles which have exactly same three nodes
    std::map<SNode3, int> checkTri;
    SNode3 tmpTri;
    std::map<SNode4, int> checkTet;
    SNode4 tmpTet;

    // Load triangles
    triPool.construct(-1, nullptr, nullptr, nullptr);
    for (int i = 1; i <= femmshData->num_surf_elems; ++i)
    {
        input >> triID >> geomID >> nNodes >> nodeIDs[0] >> nodeIDs[1] >> nodeIDs[2];
        if (triID != i)
        {
            TETMESH_EXIT("Invalid input: triangle ID is not continuous, input {} / count {}", triID, i);
        }
        tmpTri.Set(nodePool[nodeIDs[0]], nodePool[nodeIDs[1]], nodePool[nodeIDs[2]]);

        if (checkTri.find(tmpTri) != checkTri.end())
        {
            TETMESH_EXIT("Invalid input: find repeated triangle, {} and {}, nodes = {{}, {}, {}}",
                triID, checkTri[tmpTri],
                tmpTri[0]->ID, tmpTri[1]->ID, tmpTri[2]->ID);
        }
        checkTri.insert({ tmpTri, triID });

        //Allocate memory and construct a new triangle
        triPool.construct(triID,
            nodePool[nodeIDs[0]],
            nodePool[nodeIDs[1]],
            nodePool[nodeIDs[2]]);
    }

    input >> line;
    FIND_MESH_FILE_TAG(line, "end_surf_elem_data");

    input >> line;
    FIND_MESH_FILE_TAG(line, "begin_vol_elem_data");

    //Load tetrahedrons
    tetPool.construct(-1, nullptr, nullptr, nullptr, nullptr);
    for (int i = 1;i <= femmshData->num_vol_elems;++i)
    {
        input >> tetID >> geomID >> nNodes >>
            nodeIDs[0] >> nodeIDs[1] >> nodeIDs[2] >> nodeIDs[3];

        if (tetID != i)
        {
            TETMESH_EXIT("Invalid mesh input: tetrahedron ID is not continuous,input {} / count {}", tetID, i);
        }

        //Check if there exists repeated tetrahedron
        tmpTet.Set(nodePool[nodeIDs[0]], nodePool[nodeIDs[1]],
            nodePool[nodeIDs[2]], nodePool[nodeIDs[3]]);
        if (checkTet.find(tmpTet) != checkTet.end())
        {
            TETMESH_EXIT("Invalid input:find repeated tetrahedron,{ and {, nodes =({,{},{}）",
                tetID, checkTet[tmpTet],
                tmpTet[0]->ID, tmpTet[1]->ID, tmpTet[2]->ID, tmpTet[3]->ID);
        }

        checkTet.insert({ tmpTet, tetID });

        //Allocate memory and construct a new tetrahedron
        tetPool.construct(tetID,
            nodePool[nodeIDs[0]], nodePool[nodeIDs[1]],
            nodePool[nodeIDs[2]], nodePool[nodeIDs[3]]);
    }

    input >> line;
    FIND_MESH_FILE_TAG(line, "end_vol_elem_data");

    input.close();

    //Construct tetrahedron IDs to index of tetpool mapping,it might be used by refine list
    //for (int i = 1;i < tetPool.GetMaxIndex();++i)
    //{
    //    tetID2TetPoolIdx.insert({ tetPool[i]->ID,i });
    //}

    // Construct tetrahedron IDs to index of tetpool mapping, it might be used by refine list
    //for (int i = 1; i < tetPool.GetMaxIndex(); ++i)
    //{
    //    tetID2TetPoolIdx.insert({ tetPool[i]->ID, i });
    //}
    
    Log_Info("   { } nodes, { } triangles and { } tetrahedrons are input",
        nodePool.GetItemUsed() - 1, triPool.GetItemUsed() - 1, tetPool.GetItemUsed() - 1);

    return;
}

void MeshData::CreateDummyNode()
{
    if (dummyNode != nullptr) {
        TETMESH_EXIT("Try to create dummy node twice");
    }
    else {
        dummyNode = nodePool.construct(-1, Point3D(0,0,0));
    }
}

void MeshData::ConstructMeshTopology()
{

}
