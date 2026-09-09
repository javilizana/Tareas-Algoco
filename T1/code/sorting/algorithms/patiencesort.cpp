//algoritmo Patience Sort basado en los textos guia 
//"Introduction to Algorithms" (Cormen) y "Algorithms Illuminated"
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

void PatienceSort(vector<int>& A){
    int n = A.size();
    if (n <= 1) return;

    vector<int> temp_A = A; //copiamos los datos originales
    vector<int>  head;
    vector<int> next_node(n, -1);
    vector<int> pila_top_vals;

    //formamos las pilas
    for(int i = 0; i < n; i++){
        int val = temp_A[i];
        auto it = lower_bound(pila_top_vals.begin(), pila_top_vals.end(), val);
        int pile_idx = distance(pila_top_vals.begin(), it);

        if(pile_idx == (int)pila_top_vals.size()){
            pila_top_vals.push_back(val);
            head.push_back(i);
        } else {
            pila_top_vals[pile_idx] = val;
            next_node[i] = head[pile_idx]; //enlazamos con el nodo anterior
            head[pile_idx] = i; //actualizamos la cima de la pila
        }
    }

    //fusionamos
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    for(size_t p = 0; p < head.size(); p++){
        pq.push({temp_A[head[p]], head[p]});
    }

    int idx = 0;
    while(!pq.empty()){
        auto p = pq.top();
        pq.pop();

        A[idx++] = p.first; //sobreescribimos el arreglo original in-place

        int nxt = next_node[p.second];
        if (nxt != -1){
            pq.push({temp_A[nxt], nxt});
        }
    }
}

//fusion envoltorio
void sortArray(vector<int>& arr){
    if(arr.empty()){
        return;
    }
    PatienceSort(arr);
}