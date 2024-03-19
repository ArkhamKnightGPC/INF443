
#include "terrain.hpp"


using namespace cgp;

// Evaluate 3D position of the terrain for any (x,y)
float evaluate_terrain_height(float x, float y)
{

    vec2 p_i[4] = { {-10,-10}, {5,5}, {-3,4}, {6,4} };
    float h_i[4] = {3.0f, -1.5f, 1.0f, 2.0f};
    float sigma_i[4] = {10, 3, 4, 4};

    //vec2 p_0 = { 0, 0 };
    //float h_0 = 2.0f;
    //float sigma_0 = 3.0f;

    float z = 0;
    for(int i=0; i<4; i++){
        float d_i = norm(vec2(x, y) - p_i[i]) / sigma_i[i];

        float z_i = h_i[i] * std::exp(-d_i * d_i);

        z += z_i;
    }

    return z;
}

mesh create_terrain_mesh(int N, float terrain_length)
{

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*N);

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // Compute the real coordinates (x,y) of the terrain in [-terrain_length/2, +terrain_length/2]
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_terrain_height(x,y);

            // Store vertex coordinates
            terrain.position[kv+N*ku] = {x,y,z};
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(int ku=0; ku<N-1; ++ku)
    {
        for(int kv=0; kv<N-1; ++kv)
        {
            unsigned int idx = kv + N*ku; // current vertex offset

            uint3 triangle_1 = {idx, idx+1+N, idx+1};
            uint3 triangle_2 = {idx, idx+N, idx+1+N};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    // need to call this function to fill the other buffer with default values (normal, color, etc)
	terrain.fill_empty_field(); 

    return terrain;
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
