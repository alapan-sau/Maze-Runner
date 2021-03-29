// #include <iostream>
// #include <vector>
// #include <algorithm> // std::random_shuffle
// #include <ctime>     // std::time
// #include <cstdlib>   // std::rand, std::srand
// #include <random>
// #include <ctime>
// using namespace std;

// struct edge{
//     int ori;
//     int row;
//     int col;
//     int wall;
// };

// int find_root(int node, int parent[]){
//     if (parent[node] == node)
//         return node;
//     else
//         return find_root(parent[node], parent);
// }

// int do_union(int a, int b, int size[], int parent[]){
//     int root_a = find_root(a, parent);
//     int root_b = find_root(b, parent);

//     if (root_a == root_b)
//         return 0;

//     if (size[root_a] > size[root_b])
//     {
//         parent[root_b] = root_a;
//         size[root_a] += size[root_b];
//         return 1;
//     }
//     else
//     {
//         parent[root_a] = root_b;
//         size[root_b] += size[root_a];
//         return 1;
//     }
// }

// int main(){
//     int val = 4;
//     vector <edge> e;

//     int parent[val*val];
//     int size[val*val];

//     for(int i=0;i<val;i++){
//         for(int j=0;j<val;j++){
//             parent[i*val+j] = i*val+j;
//             size[i*val+1] = 1;
//             if(j!=val-1)e.push_back({0, i, j, 1}); // right edge of cell
//             if(i!=val-1)e.push_back({1, i, j, 1}); // down edge of cell
//         }
//     }
//     shuffle(e.begin(), e.end(), default_random_engine(time(0)));
//     int edge_len = e.size();

//     for(int i=0;i<edge_len;i++){
//         auto ed = e[i];

//         int a, b;
//         if(ed.ori==1){
//             a = ed.row * val + ed.col;
//             b = (ed.row+1) * val + ed.col;
//         }
//         else{
//             a = ed.row * val + ed.col;
//             b = (ed.row) * val + ed.col+1;
//         }

//         if(do_union(a,b,size,parent)){
//             ed.wall=0;
//         }
//         else cout << ed.row << " " << ed.col << " " << ((ed.ori)? "D" : "R") << " " << ed.wall << endl;
//     }
// }