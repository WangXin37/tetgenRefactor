#pragma once
#include "string.h"
#include "MeshFormat.h"
#include "memorypool.h"
#include "common_utils.h"
struct MeshData {
private:
    FemmshData* femmshData;
    ObjectPool<Node> nodePool;
    ObjectPool<Tri> triPool;
    ObjectPool<Tet> tetPool;

    //Record pointer of dummy node
    Node* dummyNode = nullptr;
public:
    MeshData():femmshData(nullptr), nodePool(1024), triPool(1024), tetPool(1024) {}
    ~MeshData() {
        if (femmshData != nullptr) {
            delete femmshData;
        }
    }
    void LoadMesh_Femmesh(const std::string& filename);

    void CreateDummyNode();

    // Construct mesh topology
    void ConstructMeshTopology();
};