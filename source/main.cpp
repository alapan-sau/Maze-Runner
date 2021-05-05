#include <iostream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>
#include <ctime>
#include <chrono>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>
#define inf 100000


// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

#define GLT_IMPLEMENTATION
#include "gltext.h"


struct edge
{
    int ori;
    int row;
    int col;
    int wall;
};

struct cell
{
    int row;
    int col;
    int s;
    int n;
    int e;
    int w;
    float left_margin;
    float right_margin;
    float top_margin;
    float bottom_margin;
};

/////////////// UTILITY //////////////
glm::vec3 playerPosition;
glm::vec3 imposterPosition;

int imposter_visible = 1;
int task1_visible = 1;
int task2_visible = 1;

int tasks = 0;
int score = 0;

int game_won = 0;
int game_lost = 0;

    std::vector<float>
    get_arena_location_from_coordinates(std::vector<cell> arena[], int row, int col, float scale_x, float scale_y, float origin_x, float origin_y, glm::vec3 objectPosition)
{
    float dist_x = objectPosition.x - origin_x;
    float dist_y = origin_y - objectPosition.y;

    int pos_y = (dist_x / scale_x);
    int pos_x = (dist_y / scale_y);

    float excess_x = dist_x - pos_y * scale_x;
    float excess_y = dist_y - pos_x * scale_y;

    std::vector<float> position;
    position.push_back(pos_x);
    position.push_back(pos_y);
    position.push_back(excess_x);
    position.push_back(excess_y);

    // std:: cout << dist_x << " "<< dist_y << std::endl;

    return position;
}

void button_handler(GLFWwindow *window, std::vector<cell> arena[], int row, int col, float scale_x, float scale_y, float origin_x, float origin_y, float player_size_x, float player_size_y){

    std::vector<float> position = get_arena_location_from_coordinates( arena, row, col, scale_x, scale_y, origin_x,origin_y, playerPosition);
    int pos_x = position[0];
    int pos_y = position[1];

    float excess_x = position[2];
    float excess_y = position[3];

    // std::cout << pos_x << " " << pos_y << std::endl;

    const float playerSpeed = 0.005f;
    // up

    // if(excess_x + player_size_x > scale_x) return;
    // if(excess_y + player_size_y > scale_y) return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        // std::cout << pos_x << " " << pos_y << " " << excess_x << " "<< excess_y << std::endl;
        if(arena[pos_x][pos_y].n==1 && (excess_y - playerSpeed) < 0){
            // std::cout << "CASE1" << std::endl;
            return;
        }
        if (excess_x + player_size_x > scale_x){
            // std::cout << "CASE2" << std::endl;
            if ((excess_y - playerSpeed) < 0)
                return;
        }
        playerPosition.y += playerSpeed;
    }
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        // std::cout << pos_x << " " << pos_y << " " << excess_x << " " << excess_y << std::endl;
        if (arena[pos_x][pos_y].s == 1 && excess_y + player_size_y + playerSpeed > scale_y){
            // std::cout << "CASE3" << std::endl;
            return;
        }
        if (excess_x + player_size_x > scale_x){
            // std::cout << "CASE4" << std::endl;
            if (excess_y + player_size_y + playerSpeed > scale_y > scale_y)
                return;
        }
        playerPosition.y -= playerSpeed;
    }

    // left
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        // std::cout << pos_x << " " << pos_y << " " << excess_x << " " << excess_y << std::endl;
        if (arena[pos_x][pos_y].w == 1 && excess_x - playerSpeed < 0){
            // std::cout << "CASE5" << std::endl;

            return;
        }
        if (excess_y + player_size_y > scale_y){
            // std::cout << "CASE6" << std::endl;
            if (excess_x - playerSpeed < 0)
                return;
        }
        playerPosition.x -= playerSpeed;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        // std::cout << pos_x << " " << pos_y << " " << excess_x << " " << excess_y << std::endl;
        if (arena[pos_x][pos_y].e == 1 && excess_x + player_size_x +  playerSpeed > scale_x){
            // std::cout << "CASE7" << std::endl;

            return;
        }
        if (excess_y + player_size_y > scale_y){
            // std::cout << "CASE8" << std::endl;
            if (excess_x + player_size_x + playerSpeed > scale_x)
                return;
        }
        playerPosition.x += playerSpeed;
    }

    // std::cout << glm::to_string(playerPosition) << std::endl;
}

void floyd_warshall_path_reconstructor(std::vector <cell> arena[], std::vector <int> imposter_path[], int row, int col){

    int all_cells = row*col;

    int mat[all_cells][all_cells];

    for(int i=0;i<all_cells;i++){
        for(int j=0;j<all_cells;j++){
            mat[i][j] = inf;
        }
    }
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(i != (row - 1) && arena[i][j].s == 0){ // check south
                mat[i*col+j][(i+1)*col+j] = 1;
                mat[(i+1)*col+j][i*col+j] = 1;
                imposter_path[i*col+j][(i+1)*col+j]= (i+1)*col+j;
                imposter_path[(i+1)*col+j][i*col+j] = i*col+j;
            }
            if(j!=(col-1) && arena[i][j].e==0){// check right
                mat[i*col+j][i*col+j+1] = 1;
                mat[i*col+j+1][i*col+j] = 1;
                imposter_path[i*col+j][i*col+j+1] = i*col+j+1;
                imposter_path[i*col+j+1][i*col+j] = i*col+j;
            }
            mat[i*col+j][i*col+j]=0;
            imposter_path[i*col+j][i*col+j]=i*col+j;
        }
    }

    // FLOYD WARSHALL
    for(int k=0;k<all_cells;k++){
        for(int i=0;i<all_cells;i++){
            for(int j=0;j<all_cells;j++){
                if(mat[i][j] > mat[i][k]+mat[k][j]){
                    mat[i][j] = mat[i][k] + mat[k][j];
                    imposter_path[i][j] = imposter_path[i][k];
                }
            }
        }
    }
}


int steps = 0;
int direction = 0;
void imposter_movement_handler(std::vector<cell> arena[], int row, int col, float scale_x, float scale_y, float origin_x, float origin_y, float imposter_size_x, float imposter_size_y, std::vector<int>imposter_path[])
{
    std::vector<float> position = get_arena_location_from_coordinates(arena, row, col, scale_x, scale_y, origin_x, origin_y, imposterPosition);
    int pos_x = position[0];
    int pos_y = position[1];
    float excess_x = position[2];
    float excess_y = position[3];

    std::vector<float> target = get_arena_location_from_coordinates(arena, row, col, scale_x, scale_y, origin_x, origin_y, playerPosition);
    int tar_x = target[0];
    int tar_y = target[1];
    float tar_excess_x = target[2];
    float tar_excess_y = target[3];

    // ALL THE MEASUREMENTS ARE TOP LEFT CORNER
    // printf("%d %d %d %d\n", pos_x, pos_y, tar_x, tar_y);


    if(!steps){
        if(pos_x == tar_x && pos_y == tar_y) return ;

        int ser_pos = pos_x * col + pos_y;
        int ser_tar = tar_x * col + tar_y;

        int ser_next = imposter_path[ser_pos][ser_tar];
        if(ser_next - ser_pos == 1){// go right
            direction = 1;
        }
        else if(ser_next - ser_pos == -1){// go left
            direction = 2;
        }
        else if(ser_next - ser_pos == col){// go down
            direction = 3;
        }
        else if(ser_next - ser_pos == -col){// go up
            direction = 4;
        }
        else{
            return;
        }
        steps=50;
        return;
    }
    else{
        if(direction == 1){// go right
            imposterPosition.x+=(scale_x/50);
        }
        else if(direction == 2){// go left
            imposterPosition.x-=(scale_x/50);
        }
        else if(direction == 3){// go down
            imposterPosition.y-=(scale_y/50);
        }
        else{// go up
            imposterPosition.y+=(scale_y/50);
        }
        steps--;
    }
}

std::vector<float> CreateCircleArray(float radius, float x, float y, int fragments)
{
     const float PI = 3.1415926f;

     std::vector<float> result;

     float increment = 2.0f * PI / fragments;

     for (float currAngle = 0.0f; currAngle <= 2.0f * PI; currAngle += increment)
     {
        result.push_back(x + radius * cos(currAngle));
        result.push_back(y + radius * sin(currAngle));
     }

     return result;
}

void handle_player_task1_collision(std::vector<cell> arena[], int row, int col, float scale_x, float scale_y, float origin_x, float origin_y, int task1_x,int task1_y){
    std::vector<float>position = get_arena_location_from_coordinates(arena, row, col, scale_x, scale_y, origin_x, origin_y, playerPosition);
    int pos_x = position[0];
    int pos_y = position[1];
    float excess_x = position[2];
    float excess_y = position[3];

    // printf("%d %d %d %d\n", pos_x, pos_y, task1_x ,task1_y);

    if(task1_x == pos_x && task1_y == pos_y){
        task1_visible=0;
        imposter_visible = 0;
        tasks++;
    }
    return;
}


void handle_player_task2_collision(std::vector<cell> arena[], int row, int col, float scale_x, float scale_y, float origin_x, float origin_y, int task2_x,int task2_y, int power_up_visible[], int obstacle_visible[]){
    std::vector<float>position = get_arena_location_from_coordinates(arena, row, col, scale_x, scale_y, origin_x, origin_y, playerPosition);
    int pos_x = position[0];
    int pos_y = position[1];
    float excess_x = position[2];
    float excess_y = position[3];

    // printf("%d %d %d %d\n", pos_x, pos_y, task1_x ,task1_y);

    if(task2_x == pos_x && task2_y == pos_y){
        task2_visible=0;
        tasks++;
        for(int i=0;i<2;i++){
            power_up_visible[i]=1;
            obstacle_visible[i]=1;

        }
    }
    return;
}

void handle_power_up_collision(int index, int power_up_x[], int power_up_y[], int power_up_visible[], std::vector<cell> arena[], int row, int col, float scale_x, float scale_y, float origin_x, float origin_y){
    std::vector<float>position = get_arena_location_from_coordinates(arena, row, col, scale_x, scale_y, origin_x, origin_y, playerPosition);
    int pos_x = position[0];
    int pos_y = position[1];
    float excess_x = position[2];
    float excess_y = position[3];

    if(pos_x == power_up_x[index]  && pos_y == power_up_y[index]){
        power_up_visible[index]=0;
        score+=20;
    }
    return;
}

void handle_obstacle_collision(int index, int obstacle_x[], int obstacle_y[], int obstacle_visible[], std::vector<cell> arena[], int row, int col, float scale_x, float scale_y, float origin_x, float origin_y){
    std::vector<float>position = get_arena_location_from_coordinates(arena, row, col, scale_x, scale_y, origin_x, origin_y, playerPosition);
    int pos_x = position[0];
    int pos_y = position[1];
    float excess_x = position[2];
    float excess_y = position[3];

    if(pos_x == obstacle_x[index]  && pos_y == obstacle_y[index]){
        obstacle_visible[index]=0;
        score-=10;
    }
    return;
}


void handle_imposter_player_collision( float player_size_x, float player_size_y){
    if(pow((playerPosition.x - imposterPosition.x),2) + pow((playerPosition.y - imposterPosition.y),2) <= pow(player_size_x,2)){
        game_lost =1;
    }
    return;
}


void handle_game_winning(std::vector<cell> arena[], int row, int col, float scale_x, float scale_y, float origin_x, float origin_y){
    std::vector<float>position = get_arena_location_from_coordinates(arena, row, col, scale_x, scale_y, origin_x, origin_y, playerPosition);
    int pos_x = position[0];
    int pos_y = position[1];
    float excess_x = position[2];
    float excess_y = position[3];

    if(pos_x==row-1 && pos_y==col-1) game_won=1;
}

////////////////////////////////////////////////////////////////////
//////                  MAZE INITIALIZATION                   //////

int find_root(int node, int parent[])
{
    if (parent[node] == node)
        return node;
    else
        return find_root(parent[node], parent);
}

int do_union(int a, int b, int size[], int parent[])
{
    int root_a = find_root(a, parent);
    int root_b = find_root(b, parent);

    if (root_a == root_b)
        return 0;

    if (size[root_a] > size[root_b])
    {
        parent[root_b] = root_a;
        size[root_a] += size[root_b];
        return 1;
    }
    else
    {
        parent[root_a] = root_b;
        size[root_b] += size[root_a];
        return 1;
    }
}


// creates the maze in the arena and sends back the vector of edges
std::vector <float> maze_creator(const int row, const int col, std::vector<cell> arena[], const float origin_x, const float origin_y){
    std::vector <edge> e;

    int parent[row * col];
    int size[row * col];

    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            parent[i * col + j] = i * col + j;
            size[i * col + 1] = 1;
            if (j != col - 1)
                e.push_back({0, i, j, 1}); // right edge of cell
            if (i != row - 1)
                e.push_back({1, i, j, 1}); // down edge of cell
        }
    }

    std::shuffle(e.begin(), e.end(), std::default_random_engine(time(0)));
    int edge_len = e.size();

    float scale_x = (float)2 * (abs(origin_x)) / col;
    float scale_y = (float)2 * (abs(origin_y)) / row;

    std::vector<float> maze_edges;

    // SIDE 1
    maze_edges.push_back(origin_x);
    maze_edges.push_back(origin_y);
    maze_edges.push_back(origin_x);
    maze_edges.push_back(-origin_y);

    // SIDE 2
    maze_edges.push_back(origin_x);
    maze_edges.push_back(-origin_y);
    maze_edges.push_back(-origin_x);
    maze_edges.push_back(-origin_y);

    // SIDE 3
    maze_edges.push_back(-origin_x);
    maze_edges.push_back(origin_y);
    maze_edges.push_back(-origin_x);
    maze_edges.push_back(-origin_y);

    // SIDE 4
    maze_edges.push_back(origin_x);
    maze_edges.push_back(origin_y);
    maze_edges.push_back(-origin_x);
    maze_edges.push_back(origin_y);


    // route through all edges
    for (int i = 0; i < edge_len; i++){
        auto ed = e[i];
        int a, b;
        if (ed.ori == 1){ // D
            a = ed.row * col + ed.col;
            b = (ed.row + 1) * col + ed.col;

            if (do_union(a, b, size, parent)){
                ed.wall = 0;
                arena[ed.row][ed.col].s = 0;
                arena[ed.row + 1][ed.col].n = 0;
            }
            else{
                float left_end_x = origin_x + (ed.col * scale_x);
                float left_end_y = origin_y - (ed.row + 1) * scale_y;

                float right_end_x = origin_x + (ed.col + 1) * scale_x;
                float right_end_y = origin_y - (ed.row + 1) * scale_y;

                maze_edges.push_back(left_end_x);
                maze_edges.push_back(left_end_y);
                maze_edges.push_back(right_end_x);
                maze_edges.push_back(right_end_y);

            }
        }
        else{ // R
            a = ed.row * col + ed.col;
            b = (ed.row) * col + ed.col + 1;
            if (do_union(a, b, size, parent)){
                ed.wall = 0;
                arena[ed.row][ed.col].e = 0;
                arena[ed.row][ed.col + 1].w = 0;
            }
            else{
                float top_end_x = origin_x + (ed.col + 1) * scale_x;
                float top_end_y = origin_y - (ed.row) * scale_y;
                float bottom_end_x = origin_x + (ed.col + 1) * scale_x;
                float bottom_end_y = origin_y - (ed.row + 1) * scale_y;

                maze_edges.push_back(top_end_x);
                maze_edges.push_back(top_end_y);
                maze_edges.push_back(bottom_end_x);
                maze_edges.push_back(bottom_end_y);
            }
        }
    }
    return maze_edges;
}




///////////////////////////////////////////////////////////////////////////
///////////                      OPEN GL                       ////////////
//vertes shader source code
const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "uniform mat4 model; \n"
    "void main()\n"
    "{\n"
    "gl_Position = model * vec4(aPos,0.0,1.0f);\n"
    "}\0";

// fragment shader source code
const char *fragmentShaderSource="#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec3 color; \n"
    "void main()\n"
    "{\n"
    "       FragColor = vec4(color,1.0f);\n"
    "}\n\0";


// window size change handler
void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

// error handler
void errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW error %d: %s\n", error, description);
}

// generate a shader from a source code
unsigned int createShader(std :: string type){
    unsigned int shader;
    // std :: cout << type << std::endl;
    if(type == "vertex")shader = glCreateShader(GL_VERTEX_SHADER);
    else shader = glCreateShader(GL_FRAGMENT_SHADER);

    if(type == "vertex"){
        glShaderSource(shader, 1, &vertexShaderSource, NULL);
    }
    else {
        glShaderSource(shader, 1, &fragmentShaderSource, NULL);
    }
    glCompileShader(shader);


    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std :: cout << "ERROR : Unable to Compile " << type <<  " Shader\n" << std :: endl;
        std :: cout << infoLog << std :: endl;
        return 0;
    }
    return shader;
}

// create the program(pipeline)
unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader){
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std :: cout <<  "Unable to Link Shaders\n" << std::endl;
        std :: cout << infoLog << std :: endl;
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
}

// // creates a texture
// unsigned int createTexture(){
//     unsigned int texture;
//     glGenTextures(1, &texture);
//     glBindTexture(GL_TEXTURE_2D, texture);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


//     int width, height, nrChannels;
//     stbi_set_flip_vertically_on_load(true);
//     unsigned char *data = stbi_load("./../source/wall.jpg", &width, &height, &nrChannels, 0);
//     if (data)
//     {
//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//         glGenerateMipmap(GL_TEXTURE_2D);
//     }
//     else
//     {
//         fprintf(stderr, "Unable to load texture 2 \n");
//         return 0;
//     }
//     stbi_image_free(data);
//     return texture;
// }

// sets up a window using glfw
GLFWwindow* initialize() {
    int glfwInitRes = glfwInit();
    if (!glfwInitRes) {
        fprintf(stderr, "Unable to initialize GLFW\n");
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "InitGL", nullptr, nullptr);
    if (!window) {
        fprintf(stderr, "Unable to create GLFW window\n");
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int gladInitRes = gladLoadGL();
    if (!gladInitRes) {
        fprintf(stderr, "Unable to initialize glad\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return nullptr;
    }

    return window;
}

int main(){
    // CODE STARTS


    // The callBack functions which handle all errors thrown by the glfw
    glfwSetErrorCallback(errorCallback);

    GLFWwindow* window = initialize();
    if (!window) {
        return 0;
    }

    // Text adding lib
    gltInit();


    // Create the pipeline
    unsigned int vertexShader = createShader("vertex");
    unsigned int fragmentShader = createShader("fragment");
    unsigned int shaderProgram = createShaderProgram(vertexShader,fragmentShader);


    // // to make the depth shit not happen
    // glEnable(GL_DEPTH_TEST);

    // create the arena
    const int row = 12;
    const int col = 12;
    const float origin_x = -0.8;
    const float origin_y = 0.8;

    float scale_x = (float)2 * (abs(origin_x)) / col;
    float scale_y = (float)2 * (abs(origin_y)) / row;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, row-1);


    std::vector<cell>
        arena[row];

    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            float left_margin = origin_x+ j*scale_x;
            float right_margin = left_margin + scale_x;
            float top_margin = origin_y - i * scale_y;
            float bottom_margin = top_margin - scale_y;
            arena[i].push_back({i, j, 1, 1, 1, 1, left_margin, right_margin, top_margin, bottom_margin});
        }
    }

    // create the maze
    std::vector<float> maze_edges = maze_creator(row,col,arena,origin_x, origin_y);

    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            std::cout << arena[i][j].n << arena[i][j].s << arena[i][j].e << arena[i][j].w << "   ";
        }
        std::cout << std::endl;
    }

    float line[maze_edges.size()];
    for(int i=0;i<maze_edges.size();i++){
        line[i] = maze_edges[i];
    }

    // Local Space Set Up for MAZE
    unsigned int MBO, MAO;
    glGenVertexArrays(1, &MAO);
    glGenBuffers(1, &MBO);

    glBindVertexArray(MAO);
    glBindBuffer(GL_ARRAY_BUFFER, MBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);


    // Create the player

    float player_size_x = scale_x*0.6;
    float player_size_y = scale_y*0.6;

    playerPosition = glm::vec3(-0.8f, +0.8f, 0.0f);

    // float player[] = {
    //     0.0f, 0.0f,
    //     player_size_x, -player_size_y,
    //     0.0f, -player_size_y,

    //     0.0f, 0.0f,
    //     player_size_x, -player_size_y,
    //     player_size_x, 0.0f};

    int size_player = 24;


    float player []= {
        // bag
        0.05f, -0.25,
        0.25f,-0.25f,
        0.05f, -0.7f,

        0.05f, -0.7f,
        0.25f, -0.25f,
        0.25f, -0.7f,
    // };

    // float body[] ={
        0.25f, -0.3f,
        0.25f, -0.85f,
        0.85f, -0.85,

        0.25f, -0.3f, // -0.375
        0.85f, -0.3f,
        0.85f, -0.85f,
    // };

    // float leg1[]={
        0.25f, -0.75f,
        0.25f, -1.0f,
        0.45f, -0.75f,

        0.25f, -1.0f,
        0.45f, -0.75f,
        0.45f, -1.0f,
    // };

    // float leg2[]={
        0.65f, -0.75f,
        0.65f, -1.0f,
        0.85f, -0.75f,

        0.65, -1.0f,
        0.85f, -0.75f,
        0.85f, -1.0f,
    };
    float glass_pane[]={
    // glass pane
        0.5f, -0.3f,
        0.5f, -0.65f,
        0.95f, -0.3f,

        0.95, -0.65f,
        0.5f, -0.65f,
        0.95f, -0.3f,
    };

    std::vector<float> head_vertices;
    const float PI = 3.1415926f;
    float increment = (2.0f * PI) / 100.0f;


    for (float currAngle = 0.2f; currAngle <= 2.0f * PI ; currAngle += increment){
        head_vertices.push_back(0.55 + 0.3 * cos(currAngle));
        head_vertices.push_back(-0.3 + 0.3 * sin(currAngle));
        // printf("%f\n", currAngle);
    }

    for(int i=0;i<2*size_player;i++){
        if(i%2==0)player[i]= player[i] * player_size_x;
        else player[i]= player[i] * player_size_y;
    }
    float head[head_vertices.size()];
    for(int i=0;i<head_vertices.size();i++){
        if(i%2==0)head[i] = head_vertices[i] * player_size_x;
        else head[i] = head_vertices[i] * player_size_y;
    }

    for(int i=0;i<12;i++){
        if(i%2==0)glass_pane[i] = glass_pane[i] * player_size_x;
        else glass_pane[i] = glass_pane[i] * player_size_y;
    }

    // Local Space Set Up for PLAYER
    unsigned int PBO, PAO;
    glGenVertexArrays(1, &PAO);
    glGenBuffers(1, &PBO);

    glBindVertexArray(PAO);
    glBindBuffer(GL_ARRAY_BUFFER, PBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(player), player, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);


    unsigned int HBO, HAO;
    glGenVertexArrays(1, &HAO);
    glGenBuffers(1, &HBO);

    glBindVertexArray(HAO);
    glBindBuffer(GL_ARRAY_BUFFER, HBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(head), head, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);


    unsigned int GBO, GAO;
    glGenVertexArrays(1, &GAO);
    glGenBuffers(1, &GBO);

    glBindVertexArray(GAO);
    glBindBuffer(GL_ARRAY_BUFFER, GBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glass_pane), glass_pane, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // FLOYD WARSHALL ALL SOURCE ALL SINK PATH

    std::vector<int> imposter_path[row*col];
    for (int i=0; i<row*col;i++){
        for(int j=0;j<row*col;j++){
            imposter_path[i].push_back(0);
        }
    }

    floyd_warshall_path_reconstructor(arena, imposter_path, row, col);

    // IMPOSTER

    float imposter_size_x = scale_x * 0.6;
    float imposter_size_y = scale_y * 0.6;

    int imposter_row = distrib(gen);
    int imposter_col = distrib(gen);

    float l_margin = arena[imposter_row][imposter_col].left_margin;
    float t_margin = arena[imposter_row][imposter_col].top_margin;

    float center_x = l_margin+ scale_x*0.5;
    float center_y = t_margin - scale_y*0.5;

    imposterPosition.x = center_x - imposter_size_x * 0.5;
    imposterPosition.y = center_y + imposter_size_y * 0.5;


    // imposterPosition = glm::vec3(-origin_x - ((float)5/6) * scale_x, -origin_y+ ((float)5/6) * scale_y, 0.0f);

    // TASK 1

    int task1_row = distrib(gen);
    int task1_col = distrib(gen);

    l_margin = arena[task1_row][task1_col].left_margin;
    t_margin = arena[task1_row][task1_col].top_margin;

    center_x = l_margin+ scale_x*0.5;
    center_y = t_margin- scale_y*0.5;


    std::vector <float> circle_vertices = CreateCircleArray(scale_x * 0.2 , center_x , center_y , 100);
    float circle[circle_vertices.size()];

    for(int i=0;i<circle_vertices.size();i++) circle[i] = circle_vertices[i];

    unsigned int CBO, CAO;
    glGenVertexArrays(1, &CAO);
    glGenBuffers(1, &CBO);

    glBindVertexArray(CAO);
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle), circle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);


    // create Task2 POWER UP/OBS BUTTON
    // std::uniform_int_distribution<> distrib(0, row-1);

    int task2_row = distrib(gen);
    int task2_col = distrib(gen);

    l_margin = arena[task2_row][task2_col].left_margin;
    t_margin = arena[task2_row][task2_col].top_margin;

    center_x = l_margin+scale_x*0.5;
    center_y = t_margin-scale_y*0.5;

    std::vector <float> circle_vertices1 = CreateCircleArray(scale_x * 0.2 , center_x , center_y , 100);
    float circle1[circle_vertices1.size()];
    for(int i=0;i<circle_vertices1.size();i++) circle1[i] = circle_vertices1[i];

    unsigned int CBO1, CAO1;
    glGenVertexArrays(1, &CAO1);
    glGenBuffers(1, &CBO1);

    glBindVertexArray(CAO1);
    glBindBuffer(GL_ARRAY_BUFFER, CBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(circle1), circle1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);


    //POWER_UPS/OBSTACLES

    int power_up_x[2];
    int power_up_y[2];
    int power_up_visible[2];

    int obstacle_x[2];
    int obstacle_y[2];
    int obstacle_visible[2];

    for(int i=0;i<2;i++){
        power_up_x[i] = distrib(gen);
        power_up_y[i] = distrib(gen);
        obstacle_x[i] = distrib(gen);
        obstacle_y[i] = distrib(gen);
        power_up_visible[i] = 0;
        obstacle_visible[i] = 0;
    }

    std::vector <float> centred_circle_vertices = CreateCircleArray(scale_x * 0.2 , 0, 0 , 100);
    float centred_circle[centred_circle_vertices.size()];
    for(int i=0;i<centred_circle_vertices.size();i++) centred_circle[i] = centred_circle_vertices[i];


    unsigned int CCBO, CCAO;
    glGenVertexArrays(1, &CCAO);
    glGenBuffers(1, &CCBO);

    glBindVertexArray(CCAO);
    glBindBuffer(GL_ARRAY_BUFFER, CCBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(centred_circle), centred_circle, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);



    // WINNING BOX

    float win[] ={
        arena[row-1][col-1].left_margin, arena[row-1][col-1].top_margin,
        arena[row-1][col-1].left_margin, arena[row-1][col-1].bottom_margin,
        arena[row-1][col-1].right_margin, arena[row-1][col-1].top_margin,

        arena[row-1][col-1].left_margin, arena[row-1][col-1].bottom_margin,
        arena[row-1][col-1].right_margin, arena[row-1][col-1].top_margin,
        arena[row-1][col-1].right_margin, arena[row-1][col-1].bottom_margin,
    };

    unsigned int WBO, WAO;
    glGenVertexArrays(1,  &WAO);
    glGenBuffers(1,  &WBO);

    glBindVertexArray(WAO);
    glBindBuffer(GL_ARRAY_BUFFER, WBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(win), win, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);




    auto start_time = std::chrono::system_clock::now();
    // white screen
    glClearColor(0, 0, 0, 0);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto current_time = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds> (current_time - start_time).count();
        if(duration >= 200) game_lost=1;

        if(game_lost){
            glfwSwapBuffers(window);
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            char dialogue[1000];
            sprintf(dialogue,"Score:%d\nTasks:%d/2\n\n\n\n You Lost", score, tasks);
            GLTtext *text = gltCreateText();
            gltSetText(text,dialogue);
            gltBeginDraw();
            gltColor(0.0f, 1.0f, 1.0f, 0.0f);
            gltDrawText2D(text, -1.0f, 1.0f, 3.0f);
            gltEndDraw();
            glfwSwapBuffers(window);
            glfwPollEvents();
            sleep(3);
            return 0;
        }
        if(game_won){
            glfwSwapBuffers(window);
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            char dialogue[1000];
            sprintf(dialogue,"Score:%d\nTasks:%d/2\n\n\n\n You Won", score, tasks);
            GLTtext *text = gltCreateText();
            gltSetText(text,dialogue);
            gltBeginDraw();
            gltColor(0.0f, 1.0f, 1.0f, 0.0f);
            gltDrawText2D(text, -1.0f, 1.0f, 3.0f);
            gltEndDraw();
            glfwSwapBuffers(window);
            glfwPollEvents();
            sleep(3);
            return 0;
        }

        glUseProgram(shaderProgram);

        button_handler(window, arena, row, col, scale_x, scale_y, origin_x, origin_y, player_size_x, player_size_y);
        //clear the screen

        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        int colorLoc = glGetUniformLocation(shaderProgram, "color");
        // MATRIX
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.0f);

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(colorLoc, 1, glm::value_ptr(color));

        glBindVertexArray(MAO);
        glDrawArrays(GL_LINES, 0, maze_edges.size()/2);

        // PLAYER
        model = glm::translate(model, playerPosition);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        color = glm::vec3(0.074f, 0.062f, 0.635f);
        glUniform3fv(colorLoc, 1, glm::value_ptr(color));

        glBindVertexArray(PAO);
        glDrawArrays(GL_TRIANGLES, 0, size_player);

        color = glm::vec3(0.074f, 0.062f, 0.635f);
        glUniform3fv(colorLoc, 1, glm::value_ptr(color));

        glBindVertexArray(HAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, head_vertices.size()/2);


        color = glm::vec3(0.705f, 0.760f, 0.894f);
        glUniform3fv(colorLoc, 1, glm::value_ptr(color));

        glBindVertexArray(GAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);


//

        // IMPOSTER
        if(imposter_visible){
            handle_imposter_player_collision(player_size_x, player_size_y);
            imposter_movement_handler(arena, row,col, scale_x, scale_y, origin_x, origin_y, imposter_size_x, imposter_size_y, imposter_path);
            model = glm::mat4(1.0f);
            model = glm::translate(model, imposterPosition);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            color = glm::vec3(0.941f, 0.0f, 0.180f);
            glUniform3fv(colorLoc, 1, glm::value_ptr(color));

            glBindVertexArray(PAO);
            glDrawArrays(GL_TRIANGLES, 0, size_player);

            color = glm::vec3(0.941f, 0.0f, 0.180f);
            glUniform3fv(colorLoc, 1, glm::value_ptr(color));

            glBindVertexArray(HAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, head_vertices.size()/2);


            color = glm::vec3(0.705f, 0.760f, 0.894f);
            glUniform3fv(colorLoc, 1, glm::value_ptr(color));

            glBindVertexArray(GAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
        }

        // TASK1
        if(task1_visible==1){
            handle_player_task1_collision(arena, row,col, scale_x, scale_y, origin_x, origin_y,task1_row, task1_col);
            model = glm::mat4(1.0f);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            color = glm::vec3(0.023f, 0.070f, 0.898f);
            glUniform3fv(colorLoc, 1, glm::value_ptr(color));

            glBindVertexArray(CAO);
            glDrawArrays(GL_TRIANGLE_FAN, 0, circle_vertices.size()/2);
        }

        // TASK2
        if(task2_visible==1){
            handle_player_task2_collision(arena, row,col, scale_x, scale_y, origin_x, origin_y,task2_row, task2_col, power_up_visible, obstacle_visible);
            model = glm::mat4(1.0f);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            color = glm::vec3(0.058f, 1.0f, 0.070f);
            glUniform3fv(colorLoc, 1, glm::value_ptr(color));

            glBindVertexArray(CAO1);
            glDrawArrays(GL_TRIANGLE_FAN, 0, circle_vertices1.size()/2);
        }

        // POWER_UP
        for(int i=0;i<2;i++){
            if(power_up_visible[i]){
                handle_power_up_collision(i,power_up_x, power_up_y, power_up_visible, arena, row,col, scale_x, scale_y, origin_x, origin_y );
                model = glm::mat4(1.0f);
                glm::vec3 power_up_position;
                power_up_position.x = arena[power_up_x[i]][power_up_y[i]].left_margin + scale_x*0.5;
                power_up_position.y = arena[power_up_x[i]][power_up_y[i]].top_margin - scale_y*0.5;
                power_up_position.z = 0;

                model = glm::translate(model, power_up_position);
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                color = glm::vec3(1.0f, 0.964f, 0.058f);
                glUniform3fv(colorLoc, 1, glm::value_ptr(color));

                glBindVertexArray(CCAO);
                glDrawArrays(GL_TRIANGLE_FAN, 0, centred_circle_vertices.size()/2);
            }


        }

        // OBSTACLES
        for(int i=0;i<2;i++){
            if(obstacle_visible[i]){
                handle_obstacle_collision(i,obstacle_x, obstacle_y, obstacle_visible, arena, row,col, scale_x, scale_y, origin_x, origin_y);
                model = glm::mat4(1.0f);
                glm::vec3 obstacle_position;
                obstacle_position.x = arena[obstacle_x[i]][obstacle_y[i]].left_margin + scale_x*0.5;
                obstacle_position.y = arena[obstacle_x[i]][obstacle_y[i]].top_margin - scale_y*0.5;
                obstacle_position.z = 0;

                model = glm::translate(model, obstacle_position);
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glBindVertexArray(CCAO);

                color = glm::vec3(0.972f, 0.070f, 0.133f);
                glUniform3fv(colorLoc, 1, glm::value_ptr(color));

                glDrawArrays(GL_TRIANGLE_FAN, 0, centred_circle_vertices.size()/2);
            }
        }

        if(tasks==2){
            handle_game_winning(arena, row,col, scale_x, scale_y, origin_x, origin_y);
            model = glm::mat4(1.0f);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(WAO);

            color = glm::vec3(0.0f, 1.0f, 0.0f);
            glUniform3fv(colorLoc, 1, glm::value_ptr(color));
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        char dialogue[1000];
        sprintf(dialogue,"Score:%d\nTasks:%d/2 Duration:%lld", score, tasks, std::max(0ll, 200-duration));
        GLTtext *text = gltCreateText();
        gltSetText(text,dialogue);
        gltBeginDraw();
        gltColor(0.0f, 1.0f, 1.0f, 0.0f);
        gltDrawText2D(text, -1.0f, 1.0f, 3.0f);
        gltEndDraw();


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &MAO);
    glDeleteBuffers(1, &MBO);
    glDeleteVertexArrays(1, &PAO);
    glDeleteBuffers(1, &PBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
}