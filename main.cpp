#include "common_utils.h"
#include "predicates.h"
#include <iostream>
int main() {
    double pa[] = {0,0,0};
    double pb[] = {1,0,0};
    double pc[] = {0,1,0};
    double pd[] = {0,0,1};

    double res= orient3d(pa, pb, pc, pd);
    Logger::GetInstance().Init(true, true);
    Logger::GetInstance().PrintVersion();
    Log_Info("res {}", res);
    return 0;
}