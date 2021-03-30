#include <iostream>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <random>
#include <ctime>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glad/glad.h>
#define inf 100000


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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

    if(pos_x == tar_x && pos_y == tar_y) return ;

    if(!steps){
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
    "void main()\n"
    "{\n"
    "       FragColor = vec4(0.0f,0.0f,0.0f,1.0f);\n"
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
unsigned int createShader( std :: string type){
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

// creates a texture
unsigned int createTexture(){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./../source/wall.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        fprintf(stderr, "Unable to load texture 2 \n");
        return 0;
    }
    stbi_image_free(data);
    return texture;
}

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

    GLFWwindow* window = glfwCreateWindow(800, 600, "InitGL", nullptr, nullptr);
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

    // Create the pipeline
    unsigned int vertexShader = createShader("vertex");
    unsigned int fragmentShader = createShader("fragment");
    unsigned int shaderProgram = createShaderProgram(vertexShader,fragmentShader);
    glUseProgram(shaderProgram);

    // // to make the depth shit not happen
    // glEnable(GL_DEPTH_TEST);

    // create the arena
    const int row = 15;
    const int col = 20;
    const float origin_x = -0.8;
    const float origin_y = 0.8;

    float scale_x = (float)2 * (abs(origin_x)) / col;
    float scale_y = (float)2 * (abs(origin_y)) / row;

    std::vector<cell>
        arena[row];

    for (int i = 0; i < row; i++){
        for (int j = 0; j < col; j++){
            float left_margin = origin_x+ i*scale_x;
            float right_margin = left_margin + scale_x;
            float top_margin = origin_y - j * scale_y;
            float bottom_margin = top_margin - scale_x;
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

    float player[] = {
        0.0f, 0.0f,
        player_size_x, -player_size_y,
        0.0f, -player_size_y,

        0.0f, 0.0f,
        player_size_x, -player_size_y,
        player_size_x, 0.0f};

    // Local Space Set Up for PLAYER
    unsigned int PBO, PAO;
    glGenVertexArrays(1, &PAO);
    glGenBuffers(1, &PBO);

    glBindVertexArray(PAO);
    glBindBuffer(GL_ARRAY_BUFFER, PBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(player), player, GL_STATIC_DRAW);

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

    // for (int i = 0; i < row * col; i++){
    //     for (int j = 0; j < row * col; j++){
    //         std::cout  << imposter_path[i][j]  << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // Create the player

    float imposter_size_x = scale_x * 0.6;
    float imposter_size_y = scale_y * 0.6;

    imposterPosition = glm::vec3(-origin_x - ((float)5/6) * scale_x, -origin_y+ ((float)5/6) * scale_y, 0.0f);

    float imposter[] = {
        0.0f, 0.0f,
        imposter_size_x, -imposter_size_y,
        0.0f, -imposter_size_y,

        0.0f, 0.0f,
        imposter_size_x, -imposter_size_y,
        imposter_size_x, 0.0f};

    // Local Space Set Up for PLAYER
    unsigned int IBO, IAO;
    glGenVertexArrays(1, &IAO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(IAO);
    glBindBuffer(GL_ARRAY_BUFFER, IBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(imposter), imposter, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // white screen
    glClearColor(1, 1, 1, 1);
    while (!glfwWindowShouldClose(window)) {

        button_handler(window, arena, row, col, scale_x, scale_y, origin_x, origin_y, player_size_x, player_size_y);
        //clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(MAO);
        glDrawArrays(GL_LINES, 0, maze_edges.size()/2);

        model = glm::translate(model, playerPosition);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(PAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        imposter_movement_handler(arena, row,col, scale_x, scale_y, origin_x, origin_y, imposter_size_x, imposter_size_y, imposter_path);
        model = glm::mat4(1.0f);
        model = glm::translate(model, imposterPosition);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(IAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

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