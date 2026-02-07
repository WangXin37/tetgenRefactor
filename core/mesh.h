#pragma once
#include <array>
#include "common_utils.h"
//The tetrahedron data structure
struct Tet
{
    int ID;
    std::array<int, 4> adjTet;
    std::array<int, 4> vertices;

};

// The subface data structure. It used to describe boundary or constraint surface.
struct shellface
{
    int ID;
    std::array<int, 3> adjTri;
    std::array<int, 3> vertices;
    std::array<int, 3> segments;
    std::array<int, 2> adjTet;
    int boundaryMark;
};

struct Node {
    int ID;
    Point2D cord;
    Tet* adjTet = nullptr;
};

// Handles
// Navigation and manipulation in a tetrahedralization
struct triface
{
    Tet* tet;
    int ver;
    triface() :tet(0), ver(0) {}
    triface& operator=(const triface& t) {
        tet = t.tet;
        ver = t.ver;
        return *this;
    }
};

struct face {
    shellface* sh;
    int ver;
    face() :sh(0), ver(0) {}
    face& operator=(const face& s) {
        sh = s.sh;
        ver = s.ver;
        return *this;
    }
};