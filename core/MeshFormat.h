#pragma once
#include<map>
#include<string>
#include <vector>
struct FemmshData
{
    struct Body
    {
        int body_id;
        std::string body_name;
        std::string material_name;
        std::string layer_name;
        std::string net_name;
        int sheet_body_flag;
        int nfaces_on_body;
        std::vector<int> face_ids;
    };

    struct Facet
    {
        int facet_id;
        int planar_flag;
        int nfaces_on_facet;
        std::vector<int> face_ids;
    };

    std::string version;
    double tolerance;
    std::string unit_name;
    int num_bodies;
    int num_facets;
    int num_points;
    int num_surf_elems;
    int num_vol_elems;
    std::map<int, Body> bodies;
    std::map<int, Facet> facets;
};