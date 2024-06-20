#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <fstream>

using namespace std;

class Graph {
private:
    int n; 
    vector<set<int>> adj; // Danh sách kề
    vector<int> deg; // Bậc của mỗi đỉnh

public:
    Graph(string file) {
        ifstream infile(file);
        if (!infile) {
            cout << "Mở file thất bại" << endl;
            exit(1);
        }

        infile >> n;
        adj.resize(n + 1);
        deg.resize(n + 1, 0);

        int u, v;
        while (infile >> u >> v) {
            addEdge(u, v);
        }
        infile.close();
    }

    void addEdge(int u, int v) {
        adj[u].insert(v);
        adj[v].insert(u);
        deg[u]++;
        deg[v]++;
    }

    // Kiểm tra chu trình Euler
    bool isEuler() {
        for (int i = 1; i <= n; i++) {
            if (deg[i] % 2 != 0 ||  deg[i] == 0) {
                return false;
            }
        }
        return true;
    }

    // Duyệt chu trình Euler
    vector<int> findEuler(int start) {
        vector<int> EC;
        stack<int> st;
        st.push(start);

        while (!st.empty()) {
            int v = st.top();
            if (!adj[v].empty()) {
                int u = *adj[v].begin();
                st.push(u);
                adj[v].erase(u);
                adj[u].erase(v);
            } else {
                st.pop();
                EC.push_back(v);
            }
        }

        return EC;
    }

    void printEuler() {
        if (!isEuler()) {
            cout << 0 << endl;
        } else {
            vector<int> euler = findEuler(1);
            cout << 1 << endl << "Chu trinh Euler: ";
            for (int vertex : euler) {
                cout << vertex << " ";
            }
        }
    }
};

int main() {
    Graph graph("data1.txt"); 
    graph.printEuler();

    return 0;
}
