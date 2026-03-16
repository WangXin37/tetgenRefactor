#include "common_utils.h"
#include "predicates.h"
#include "MeshData.h"
#include <iostream>
int main() {
    Logger::GetInstance().Init();
    MeshData meshdata;
    meshdata.LoadMesh_Femmesh("D:\\gitroot\\tetgenRefactor\\case\\init.femmsh");
    return 0;
}