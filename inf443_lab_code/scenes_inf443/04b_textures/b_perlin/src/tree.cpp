#include "tree.hpp"
#include <math.h>       /* sin, cos */

# define M_PI           3.14159265358979323846  /* pi */

using namespace cgp;


mesh create_cylinder_mesh(float radius, float height)
{
    mesh m; 
    // To do: fill this mesh ...
    // ...
    // To add a position: 
    //   m.position.push_back(vec3{x,y,z})
    // Or in pre-allocating the buffer:
    //   m.position.resize(maximalSize);
    //   m.position[index] = vec3{x,y,z}; (with 0<= index < maximalSize)
    // 
    // Similar with the triangle connectivity:
    //  m.connectivity.push_back(uint3{index_1, index_2, index_3});

    float mesh_size = 100;// 50 points in the base, 50 points at the top

    // Fill cylinder geometry
    for(int ku=0; ku<mesh_size; ++ku){
        float scale_factor = 2.0/(mesh_size-2);
        float x,y,z;

        float theta = 2*M_PI*scale_factor*(ku/2);
        x = radius*cos(theta);
        y = radius*sin(theta);

        if(ku%2 == 0){//point in base
            z = 0;
        }else{//point at the top
            z = height;
        }

        m.position.push_back(vec3{x,y,z});
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(int ku=0; ku < mesh_size-2; ku++){

            uint3 triangle = {ku, ku+1, ku+2};

            m.connectivity.push_back(triangle);
    }

    // Need to call fill_empty_field() before returning the mesh 
    //  this function fill all empty buffer with default values (ex. normals, colors, etc).
    m.fill_empty_field();

    return m;
}

mesh create_cone_mesh(float radius, float height, float z_offset)
{
    mesh m; 
    // To do: fill this mesh ...
    // ...

    float mesh_size = 100;// 50 points in the base, 50 points at the top

    // Fill cylinder geometry
    for(int ku=0; ku<mesh_size; ++ku){
        float scale_factor = 2.0/(mesh_size-2);
        float x,y,z;

        float theta = 2*M_PI*scale_factor*(ku/2);

        if(ku%2 == 0){//point in base
            x = radius*cos(theta);
            y = radius*sin(theta);
            z = 0;
        }else{//point at the top
            x = 0;
            y = 0;
            z = height + z_offset;
        }

        m.position.push_back(vec3{x,y,z});
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(int ku=0; ku < mesh_size-2; ku++){

            uint3 triangle = {ku, ku+1, ku+2};

            m.connectivity.push_back(triangle);
    }

    m.fill_empty_field();
    return m;
}

mesh create_tree()
{
    float h = 0.7f; // trunk height
    float r = 0.1f; // trunk radius

    // Create a brown trunk
    mesh trunk = create_cylinder_mesh(r, h);
    trunk.color.fill({0.4f, 0.3f, 0.3f});


    // Create a green foliage from 3 cones
    mesh foliage = create_cone_mesh(4*r, 6*r, 0.0f);      // base-cone
    foliage.push_back(create_cone_mesh(4*r, 6*r, 2*r));   // middle-cone
    foliage.push_back(create_cone_mesh(4*r, 6*r, 4*r));   // top-cone
    foliage.translate({0,0,h});       // place foliage at the top of the trunk
    foliage.color.fill({0.4f, 0.6f, 0.3f});
       
    // The tree is composed of the trunk and the foliage
    mesh tree = trunk;
    tree.push_back(foliage);

    return tree;
}

std::vector<cgp::vec3> generate_positions_on_terrain(int N, float terrain_length){

    std::vector<cgp::vec3> positions;

    for(int i=0; i<N; i++){
        float u = rand_uniform();
        float v = rand_uniform();

        //real coordinates (x,y) in the terrain
        float x = (u - 0.5f) * terrain_length;
        float y = (v - 0.5f) * terrain_length;
        float z = evaluate_terrain_height(x,y);

        bool chk = true;

        for(int j=0; j<positions.size(); j++){
            cgp::vec3 pj = positions[j];
            float dist = pow(pow(x - pj[0], 2) + pow(y - pj[1], 2) + pow(z - pj[2], 2), 0.5);
            if(dist < 2){
                chk = false;
            }
        }

        if(chk){
            positions.push_back(vec3{x,y,z});
        }else{
            i--;
        }
    }

    return positions;
}