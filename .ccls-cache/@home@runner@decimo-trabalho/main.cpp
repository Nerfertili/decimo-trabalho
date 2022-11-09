#include <array>
#include <bits/stdc++.h>
#include <chrono>
#include <ctime>
#include <experimental/random>
#include <fstream>
#include <iostream>
#include <queue>
#include <random>
#include <stack>
#include <vector>

using namespace std;
int get_ind(array<int, 20> base, int searched) {
  for (int i = 0; i < base.size(); i++) {
    if (base[i] == searched)
      return i;
  }
  return 0;
}
class grafo_lista {
public:
  array<int, 20> vert;
  array<array<array<int, 2>, 3>, 20> adj;
  grafo_lista(array<int, 20> vert_) {
    vert = vert_;
    for (int i = 0; i < 20; i++) {
      for (int w = 0; w < 3; w++) {
        for (int k = 0; k < 2; k++) {
          adj[i][w][k] = -1;
        }
      }
    }
  }
  void adicionarAresta(int v1, int v2, int peso) {
    int index = get_ind(vert, v1);
    for (int w = 0; w < 3; w++) {
      if (adj[index][w][0] == -1 && adj[index][w][1] == -1) {
        adj[index][w] = {v2, peso};
        break;
      }
    }
  }
  array<int, 20> get_vert() { return vert; }
  array<array<array<int, 2>, 3>, 20> get_adj() { return adj; }
};
int buscar(stack<int> base, int searched) {
  int size = base.size();
  for (int i = 0; i < size; i++) {
    if (base.top() == searched) {
      if (searched) {
        return searched;
      } else {
        return -1;
      }
    }
    base.pop();
  }
  return 0;
}
int get_ind(stack<int> base, int searched) {
  for (int i = 0; i < base.size(); i++) {
    if (base.top() == searched)
      return i;
    base.pop();
  }
  return 0;
}

int get_ind(int base[20], int searched) {
  for (int i = 0; i < 20; i++) {
    if (base[i] == searched)
      return i;
  }
  return 0;
}
void print_visited(stack<int> visited) {
  cout << "visited: " << endl;
  int size = visited.size();
  for (int i = 0; i < size; i++) {
    cout << visited.top() << " ";
    visited.pop();
  }
  cout << endl;
}
void print_matriz(array<array<int, 20>, 20> base, array<int, 20> smallest) {
  cout << "matriz de distancias: " << endl;
  for (int i = 0; i < 20; i++) {
    for (int k = 0; k < 20; k++) {
      if (k == smallest[i]) {
        cout << "*" << base[i][k] << " ";
      } else {
        cout << base[i][k] << " ";
      }
    }
    cout << endl;
  }
  cout << endl;
}
vector<int> get_smallest(array<int, 20> base) {
  sort(base.begin(), base.end());
  vector<int> small;
  for (int i = 0; i < 20; i++) {
    if (base[i] != 9999)
      small.push_back(base[i]);
  }
  return small;
}

stack<int> deleta_from_stack(stack<int> base, int reference) {
  stack<int> reserva = {};
  int size = base.size();
  bool retirado = false;
  for (int i = 0; i < size; i++) {
    if (base.top() != reference) {
      reserva.push(base.top());
    } else if (base.top() == reference && !retirado) {
      retirado = true;
    } else {
      reserva.push(base.top());
    }
    base.pop();
  }
  size = reserva.size();
  stack<int> n_inverso = {};
  for (int i = 0; i < size; i++) {
    n_inverso.push(reserva.top());
    reserva.pop();
  }
  return n_inverso;
}

array<int, 2> get_small_ex(array<int, 20> base, stack<int> excluidos) {
  int small = 9999;
  int index;
  while (true) {
    for (int i = 0; i < base.size(); i++) {
      // cout <<"base[i]: "<<base[i]<<endl;
      if (base[i] < small) {
        small = base[i];
        index = i;
      }
    }
    // c//out <<"small: "<<small;
    // cout<<endl;
    if (buscar(excluidos, small)) {
      base[index] = 9999;
      excluidos = deleta_from_stack(excluidos, small);
      small = 9999;
    } else {
      return {small, index};
    }
  }
}
void dijkstra(grafo_lista grafo) {
  array<int, 20> vertices = grafo.get_vert();
  array<array<array<int, 2>, 3>, 20> adj = grafo.get_adj();
  stack<int> visited;
  array<int, 20> smallest;
  array<array<int, 20>, 20> dists;
  for (int i = 0; i < 20; i++) {
    for (int k = 0; k < 20; k++) {
      dists[i][k] = 9999;
    }
  }
  dists[0] = {0,    9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999,
              9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999, 9999};
  int current = vertices[0];
  int current_index = 0;
  smallest[0] = 0;
  for (int rolagem = 1; rolagem < 20; rolagem++) {
    cout << "current: " << current << endl;
    array<array<int, 2>, 3> ligs = adj[current_index];
    visited.push(current);

    for (int w = 0; w < 20; w++) {
      int old_path = dists[rolagem - 1][w];
      cout << "old path: " << old_path << endl;
      int new_path = old_path;
      for (int j = 0; j < ligs.size(); j++) {
        int index_of_now_lig = get_ind(vertices, ligs[j][0]);
        cout << "index_of_now_lig: " << index_of_now_lig << " w: " << w << endl;
        if (index_of_now_lig == w && ligs[j][0] != -1) {
          int lig_path = ligs[j][1] + smallest[rolagem - 1];
          cout << "lig-path: " << lig_path << endl;
          if (lig_path < new_path) {
            new_path = lig_path;
          }
        }
      }
      cout << "new_path: " << new_path << endl;
      dists[rolagem][w] = new_path;
    }
    stack<int> excluded = {};
    array<int, 20> line_rol = dists[rolagem];
    cout << "line_rol:"
         << "rolagem: " << rolagem << endl;
    for (int l = 0; l < 20; l++) {
      cout << line_rol[l] << " ";
    }
    cout << endl;
    while (true) {
      array<int, 2> small_of_line = get_small_ex(line_rol, excluded);
      cout << "small of line: " << small_of_line[0]
           << " index: " << small_of_line[1] << endl;
      if (buscar(visited, vertices[small_of_line[1]])) {
        if (small_of_line[0] == 9999) {
          smallest[rolagem] = -1;
          current_index = small_of_line[1];
          current = vertices[small_of_line[1]];
          break;
        }
        excluded.push(small_of_line[0]);
        cout << "excluido: " << small_of_line[0] << endl;
        cout << endl;
      } else {
        cout << "jkfdjk" << endl;
        smallest[rolagem] = small_of_line[0];
        current_index = small_of_line[1];
        current = vertices[small_of_line[1]];
        break;
      }
    }
  }

  print_visited(visited);
  print_matriz(dists, smallest);
}

int valor_dos_vertices() {
  int aleatorio = 1 + rand() % 100;
  return aleatorio;
}
int numero_de_arestas() {
  int aleatorio = 1 + rand() % 3;
  return aleatorio;
}
int preso_aleatorio() {
  int aleatorio = 1 + rand() % 20;
  return aleatorio;
}
int ligacao_aleatoria() {
  int aleatorio = rand() % 19;
  return aleatorio;
}
int main() {
  auto start = std::chrono::high_resolution_clock::now();

  array<int, 20> vertices = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                             11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  // cout << "vertices: "<<endl;
  // for(int i=0;i<20;i++){
  //  vertices[i] = valor_dos_vertices();
  //  cout << vertices[i] << " ";
  //}
  // cout << endl;
  auto finish = std::chrono::high_resolution_clock::now();
  srand(98797);

  grafo_lista grafo(vertices);
  int tmp;
  for (int i = 0; i < 20; i++) {
    tmp = numero_de_arestas();
    for (int u = 0; u < tmp; u++) {
      int tmp_2 = ligacao_aleatoria();
      grafo.adicionarAresta(vertices[i], vertices[tmp_2], preso_aleatorio());
    }
  }
  ofstream fw("lista_adjacencias.txt");
  array<array<array<int, 2>, 3>, 20> adj = grafo.get_adj();
  for (int w = 0; w < 20; w++) {
    fw << vertices[w] << " ";
    for (int k = 0; k < adj[w].size(); k++) {
      fw << adj[w][k][0] << "d" << adj[w][k][1] << " ";
    }
    fw << "\n";
  }
  ofstream fc("lista_adjacencias.txt");
  dijkstra(grafo);
}
