#include <iostream>
#include <vector>
#include <set>
#include <fstream>

using namespace std;

class Graph {
private:
    int n; 
    vector<set<int>> adj; // Danh sách kề 
    vector<bool> visited; 
    vector<int> path; 
    vector<vector<int>> hamilton; 

public:
    Graph(string file) {
        ifstream infile(file);
        if (!infile) {
            cout << "Mở file thất bại" << endl;
            exit(1);
        }

        infile >> n;
        adj.resize(n + 1);
        visited.resize(n + 1, false);
        path.resize(n, 0);

        int u, v;
        while (infile >> u >> v) {
            adj[u].insert(v);
        }
        infile.close();
    }

    // Kiểm tra chu trình Hamilton
    bool isHamilton(int pos) {
        if (pos == n) {
            if (adj[path[pos - 1]].count(path[0]) > 0) {
                vector<int> cycle(path.begin(), path.begin() + n);
                hamilton.push_back(cycle);
                return true;
            }
            return false;
        }

        for (int v = 1; v <= n; v++) {
            if (adj[path[pos - 1]].count(v) && !visited[v]) {
                visited[v] = true;
                path[pos] = v;

                isHamilton(pos + 1);

                visited[v] = false;
            }
        }
        return false;
    }

    // Duyệt chu trình Hamilton
    void findHamilton() {
        int cnt = 0;
        for (int i = 1; i <= n; i++) {
            fill(visited.begin(), visited.end(), false);
            visited[i] = true;
            path[0] = i;

            isHamilton(1);
        }

        if (hamilton.empty()) {
            cout << 0 << endl;
        } else {
            cout << hamilton.size() << endl;
            for (auto cycle : hamilton) {
                cout << "Chu trinh Hamilton "<< ++cnt <<": ";
                for (int vertex : cycle) {
                    cout << vertex << " ";
                }
                cout << cycle[0] << endl;
            }
        }
    }
};

int main() {
    Graph graph("data1.txt"); 
    graph.findHamilton();

    return 0;
}
