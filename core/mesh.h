#pragma once
#include <array>
#include "common_utils.h"


extern int orgpivot[12];
extern int destpivot[12];
extern int apexpivot[12];
extern int oppopivot[12];
extern int triorgpivot[6];
extern int tridestpivot[6];
extern int triapexpivot[6];
extern int nexttbl[12];
extern int prevtbl[12];
extern int symtbl[12];
extern int bondtbl[12][12];
extern int bondtettribl[12][6];
extern int bondtritetbl[12][6];


struct BaseEntity
{
    int ID;

    BaseEntity() = delete;
    BaseEntity(const int id) :ID(id) {}
};

// 前向声明，便于句柄类型定义
struct Tet;
struct Tri;
struct Node;

// Handles
// Navigation and manipulation in a tetrahedralization
struct TetHandle // triface in tetgen
{
    Tet* ptr;
    int ver;

    TetHandle() : ptr(nullptr), ver(0) {}
    TetHandle(Tet* p, int v) : ptr(p), ver(v) {}

    TetHandle& operator=(const TetHandle& t);

    Node* OrigNode();
    Node* DestNode();
    Node* OppoNode();
    Node* ApexNode();

    void GotoNextEdge();
    void GotoPrevEdge();
    void GotoSymEdge();
};

struct TriHandle // face in tetgen
{
    Tri* ptr;
    int ver;

    TriHandle() : ptr(nullptr), ver(0) {}
    TriHandle(Tri* p, int v) : ptr(p), ver(v) {}

    TriHandle& operator=(const TriHandle& t);

    Node* OrigNode();
    Node* DestNode();
    Node* ApexNode();
};

struct Node : BaseEntity {
    Point3D cord;
    Tet* adjTet = nullptr;
    Tri* adjTri = nullptr;

    Node() = delete;
    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    Node(const int id, const Point3D& pos) :BaseEntity{ id }, cord(pos) {}
};

//The tetrahedron data structure
struct Tet : BaseEntity
{
    std::array<Node*, 4> node;
    TetHandle adjTet[4];
    std::array<TriHandle, 4> adjTri;

    Tet() = delete;
    Tet(const Tet&) = delete;
    Tet& operator=(const Tet&) = delete;

    Tet(const int id, Node* a, Node* b, Node* c, Node* d)
        :BaseEntity{ id }, node{ a,b,c,d } {
    }
};

// The subface data structure. It used to describe boundary or constraint surface.
struct Tri : BaseEntity//shellface in tetgen
{
    std::array<Node*, 3> node;
    std::array<TriHandle, 3> adjTri;
    std::array<TetHandle, 2> adjTet;

    Tri() = delete;
    Tri(const Tri&) = delete;
    Tri& operator=(const Tri&) = delete;

    Tri(const int id, Node* a, Node* b, Node* c)
        :BaseEntity{ id }, node{ a,b,c } {
    }
};

inline void Bond(TetHandle& t1, TetHandle& t2) {
    t1.ptr->adjTet[t1.ver / 3] = TetHandle{ t2.ptr, bondtbl[t1.ver][t2.ver] };
    t2.ptr->adjTet[t2.ver / 3] = TetHandle{ t1.ptr, bondtbl[t2.ver][t1.ver] };
}

inline void Bond(TetHandle& t1, TriHandle& t2) {
    t1.ptr->adjTri[t1.ver / 3] = TriHandle{ t2.ptr, bondtettribl[t1.ver][t2.ver] };
    t2.ptr->adjTet[t2.ver / 3] = TetHandle{ t1.ptr, bondtritetbl[t2.ver][t1.ver] };
}

inline void Bond(TriHandle& t1, TriHandle& t2) {
    //t1.ptr->adjTri[t1.ver / 2] = { t2.ptr, bondtettribl[t1.ver][t2.ver] };
    //t2.ptr->adjTri[t2.ver / 2] = { t1.ptr, bondtritetbl[t2.ver][t1.ver] };
}

struct SNode3 :std::array<Node*, 3>
{
    SNode3() {
        (*this)[0] = 0;
        (*this)[1] = 0;
        (*this)[2] = 0;
    }

    SNode3(Node* a, Node* b, Node* c) {
        (*this)[0] = a;
        (*this)[1] = b;
        (*this)[2] = c;
        Sort();
    }

    SNode3(const SNode3& other) {
        (*this)[0] = other[0];
        (*this)[1] = other[1];
        (*this)[2] = other[2];
        Sort();
    }

    SNode3(Node **src){
        (*this)[0] = src[0];
        (*this)[1] = src[1];
        (*this)[2] = src[2];
        Sort();
    }

    void Set(Node* a, Node* b, Node* c){
        (*this)[0] = a;
        (*this)[1] = b;
        (*this)[2] = c;
        Sort();
    }

    void Set(Node **src){
        (*this)[0] = src[0];
        (*this)[1] = src[1];
        (*this)[2] = src[2];
        Sort();
    }

    //bool operator<(const SNode3& other) const {
        //return (*this)[0]->ID < other[0]->ID && (*this)[1]->ID < other[1]->ID && (*this)[2]->ID < other[2]->ID;
    //}

    bool operator==(const SNode3& other) const {
        return (*this)[0]->ID == other[0]->ID && 
                (*this)[1]->ID == other[1]->ID && 
                (*this)[2]->ID == other[2]->ID;
    }

    void Sort(){
        if ((*this)[1]->ID > (*this)[2]->ID) {
            std::swap((*this)[1], (*this)[2]);
        }
        if ((*this)[0]->ID > (*this)[1]->ID) {
            std::swap((*this)[0], (*this)[1]);
            if ((*this)[1]->ID > (*this)[2]->ID) {
                std::swap((*this)[1], (*this)[2]);
            }
        }
    }

    void Print() const {
        Log_Trace("SNode3: {} {} {}", (*this)[0]->ID, (*this)[1]->ID, (*this)[2]->ID);
    }
};

struct SNode4 :std::array<Node*, 4>
{
    SNode4() {
        (*this)[0] = 0;
        (*this)[1] = 0;
        (*this)[2] = 0;
        (*this)[3] = 0;
    }

    SNode4(Node* a, Node* b, Node* c, Node* d) {
        (*this)[0] = a;
        (*this)[1] = b;
        (*this)[2] = c;
        (*this)[3] = d;
        Sort();
    }

    SNode4(const SNode4& other) {
        (*this)[0] = other[0];
        (*this)[1] = other[1];
        (*this)[2] = other[2];
        (*this)[3] = other[3];
        Sort();
    }

    SNode4(Node **src){
        (*this)[0] = src[0];
        (*this)[1] = src[1];
        (*this)[2] = src[2];
        (*this)[3] = src[3];
        Sort();
    }

    void Set(Node* a, Node* b, Node* c, Node* d){
        (*this)[0] = a;
        (*this)[1] = b;
        (*this)[2] = c;
        (*this)[3] = d;
        Sort();
    }
    void Set(Node **src){
        (*this)[0] = src[0];
        (*this)[1] = src[1];
        (*this)[2] = src[2];
        (*this)[3] = src[3];
        Sort();
    }

    bool operator==(const SNode4& other) const {
        return (*this)[0]->ID == other[0]->ID && 
                (*this)[1]->ID == other[1]->ID && 
                (*this)[2]->ID == other[2]->ID && 
                (*this)[3]->ID == other[3]->ID;
    }

    void Sort(){
        if ((*this)[2]->ID > (*this)[3]->ID) 
            std::swap((*this)[2], (*this)[3]);
        if ((*this)[1]->ID > (*this)[2]->ID) {
            std::swap((*this)[1], (*this)[2]);
            if ((*this)[2]->ID > (*this)[3]->ID) 
                std::swap((*this)[2], (*this)[3]);
        }
        if ((*this)[0]->ID > (*this)[1]->ID) {
            std::swap((*this)[0], (*this)[1]);
            if ((*this)[1]->ID > (*this)[2]->ID) {
                std::swap((*this)[1], (*this)[2]);
                if ((*this)[2]->ID > (*this)[3]->ID) 
                    std::swap((*this)[2], (*this)[3]);
            }
        }
    }

    void Print() const {
        Log_Trace("SNode4: {} {} {} {}", (*this)[0]->ID, (*this)[1]->ID, (*this)[2]->ID, (*this)[3]->ID);
    }
};

//========= TetHandle inline implementations =========

inline TetHandle& TetHandle::operator=(const TetHandle& t) {
    ptr = t.ptr;
    ver = t.ver;
    return *this;
}

inline Node* TetHandle::OrigNode() {
    return this->ptr->node[orgpivot[this->ver]];
}
inline Node* TetHandle::DestNode() {
    return this->ptr->node[destpivot[this->ver]];
}
inline Node* TetHandle::OppoNode() {
    return this->ptr->node[oppopivot[this->ver]];
}
inline Node* TetHandle::ApexNode() {
    return this->ptr->node[apexpivot[this->ver]];
}

inline void TetHandle::GotoNextEdge() {
    ver = nexttbl[ver];
}
inline void TetHandle::GotoPrevEdge() {
    ver = prevtbl[ver];
}
inline void TetHandle::GotoSymEdge() {
    ver = symtbl[ver];
}

//========= TriHandle inline implementations =========

inline TriHandle& TriHandle::operator=(const TriHandle& t) {
    ptr = t.ptr;
    ver = t.ver;
    return *this;
}

inline Node* TriHandle::OrigNode() {
    return this->ptr->node[triorgpivot[this->ver]];
}
inline Node* TriHandle::DestNode() {
    return this->ptr->node[tridestpivot[this->ver]];
}
inline Node* TriHandle::ApexNode() {
    return this->ptr->node[triapexpivot[this->ver]];
}