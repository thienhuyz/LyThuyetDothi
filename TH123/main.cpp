#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <queue>
#include <algorithm>

using namespace std;

class Graph {
private:
    int n; 
    vector<vector<int>> adjMatrix; // Ma trận kề
    vector<vector<int>> adjList; // Danh sách kề
    vector<pair<int, int>> edgeList; // Danh sách cạnh
    bool flag;
    vector<bool> visited;
    vector<int> parent;
 
public:
    Graph(string file) {
        ifstream inFile(file);
        if (!inFile) {
            cerr << "Mo file that bai" << endl;
            exit(1);
        }

        inFile >> n;
        adjMatrix.resize(n + 1, vector<int>(n + 1, 0));
        adjList.resize(n + 1);
        visited.resize(n + 1, false);
        parent.resize(n + 1, -1);

        int x, y;
        string line;
        getline(inFile, line);

        while (getline(inFile, line)) {
            stringstream ss(line);
            ss >> x;
            while (ss >> y) {
                adjMatrix[x][y] = 1;
                adjList[x].push_back(y);
                if(!edgeExists(x,y))
                    edgeList.push_back({x, y});
            }
        }
        inFile.close();
        isGraphType();
    }

    // Kiểm tra trùng cạnh
    bool edgeExists(int x, int y) {
        for (auto edge : edgeList) {
            if ((edge.first == y && edge.second == x)) {
                return true;
            }
        }
        return false;
    }

    // Kiểu đồ thị
    void isGraphType() {
        flag = false;
        for (int x = 1; x <= n; x++) {
            for (int y = 1; y <= n; y++) {
                if (adjMatrix[x][y] != adjMatrix[y][x]) {
                    flag = true;
                    return; 
                }
            }
        }
    }

    void displayGraphType() {
        if (flag) {
            cout << "Do thi co huong" << endl;
        } else {
            cout << "Do thi vo huong" << endl;
        }
    }

    // Bậc đồ thị
    void displayDegs() {
        if (flag) {
            vector<int> inDeg(n + 1, 0);
            vector<int> outDeg(n + 1, 0);

            for (auto edge : edgeList) {
                int u = edge.first;
                int v = edge.second;
                outDeg[u]++;
                inDeg[v]++;
            }

            for (int i = 1; i <= n; i++) {
                cout << "Dinh " << i << ": Bac vao = " << inDeg[i] << ", Bac ra = " << outDeg[i] << endl;
            }
        } else {
            vector<int> deg(n + 1, 0);

            for (auto edge : edgeList) {
                int u = edge.first;
                int v = edge.second;
                deg[u]++;
                deg[v]++;
            }

            for (int i = 1; i <= n; i++) {
                cout << "Dinh " << i << ": Bac = " << deg[i] << endl;
            }
        }
    }

    void displayAdjMatrix() {
        cout << "Ma tran ke:" << endl;
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                cout << adjMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    void displayAdjList() {
        cout << "Danh sach ke:" << endl;
        for (int i = 1; i <= n; i++) {
            cout << i << ": ";
            for (int v : adjList[i]) {
                cout << v << " ";
            }
            cout << endl;
        }
    }

    void displayEdgeList() {
        cout << "Danh sach canh:" << endl;
        for (auto edge : edgeList) {
            cout << "(" << edge.first << ", " << edge.second << ")" << endl;
        }
    }

    // Duyệt đồ thị theo BFS
    void BFS(int u) {
        queue<int> queue; 
        queue.push(u); 
        visited[u] = true; 

        while (!queue.empty()) {
            int x = queue.front(); 
            queue.pop(); 
            // Duyệt các đỉnh kề 
            for (int y : adjList[x]) {
                if (!visited[y]) { 
                    queue.push(y);
                    visited[y] = true;
                }
            }
        }
    }

    // Duyệt đồ thị theo DFS
    void DFS(int u) {
        visited[u] = true; 

        // Duyệt danh sách kề
        for (int v : adjList[u]) {
            if (!visited[v]) {
                parent[v] = u;
                DFS(v);
            }
        }
    }

    // In đồ thị theo BFS
    void BFSprint(int u) {
        queue<int> queue; 
        queue.push(u); 
        visited[u] = true; 

        while (!queue.empty()) {
            int x = queue.front(); 
            queue.pop(); 
            cout << x << ' ';
            // Duyệt các đỉnh kề 
            for (int y : adjList[x]) {
                if (!visited[y]) { 
                    queue.push(y);
                    visited[y] = true;
                }
            }
        }
    }

    // In đồ thị theo DFS
    void DFSprint(int u) {
        cout << u << ' ';
        visited[u] = true; 

        // Duyệt danh sách kề
        for (int v : adjList[u]) {
            if (!visited[v]) {
                DFSprint(v);
            }
        }
    }
    // Cập nhật lại visited
    void resetVisited() {
        for (int i = 0; i < visited.size(); i++) {
            visited[i] = false;
        }
    }

    // Kiểm tra tính liên thông của đồ thị vô hướng
    bool isUndigraphConnected() {
        resetVisited();
        DFS(1);
        for (int i = 1; i <= n; i++) {
            if (!visited[i]) {
                return false;
            }
        }
        return true;
    }

      // Kiểm tra tính liên thông mạnh của đồ thị có hướng
    bool isStrongConnected() {
        for (int i = 1; i <= n; i++) {
            resetVisited();
            DFS(i);
            for (int j = 1; j <= n; j++) {
                if (!visited[j]) {
                    return false;
                }
            }
        }
        return true;
    }

    // Chuyển đổi đồ thị có hướng thành đồ thị vô hướng
     void digraphToUndigraph() {
        for (int x = 1; x <= n; x++) {
            for (int y : adjList[x]) {
                if (adjMatrix[y][x] == 0) {
                    adjMatrix[y][x] = 1;
                    adjList[y].push_back(x);
                }
            }
        }

        // Sắp xếp danh sách kề
        for (int i = 1; i <= n; i++) {
            sort(adjList[i].begin(), adjList[i].end());
        }

        edgeList.clear();
        for (int x = 1; x <= n; x++) {
            for (int y : adjList[x]) {
                if (x < y) {
                    edgeList.push_back({x, y});
                }
            }
        }
    }

    // Kiểm tra tính liên thông yếu của đồ thị có hướng
    bool isWeakConnected() {
        digraphToUndigraph();
        return isUndigraphConnected();
    }


    // Liệt kê các thành phần liên thông của đồ thị vô hướng
    void listConnectedComponents() {
        resetVisited();
        int cmt = 0;
        for (int i = 1; i <= n; i++) {
            if (!visited[i]) {
                cmt++;
                cout << "Thanh phan lien thong " << cmt << ": ";
                DFSprint(i);
                cout << endl;
            }
        }
    }

    void checkConnectivity() {
        if (flag) {
            // Đồ thị có hướng
            if (isStrongConnected()) {
                cout << "Do thi co huong la lien thong manh" << endl;
            } else if (isUndigraphConnected()) {
                cout << "Do thi co huong yeu" << endl;
            } else {
                cout << "Do thi co huong khong lien thong" << endl;
           
            }
        } else {
            // Đồ thị vô hướng
            if (isUndigraphConnected()) {
                cout << "Do thi vo huong lien thong" << endl;
            } else {
                cout << "Do thi vo huong khong lien thong" << endl;
                listConnectedComponents();
            }
        }
    }
 
    void path(int s, int t) {
        resetVisited();
        DFS(s);
        
        if (!visited[t]) {
            cout << "Khong ton tai duong di" << endl;
        } else {
            vector<int> ans;
            while (t != s) {
                ans.push_back(t);
                t = parent[t];
            }
            ans.push_back(s);
            reverse(ans.begin(), ans.end());
            for (int x : ans) cout << x << " ";
            cout << endl;
        }
    }

};



int main() {
    Graph graph("data3.txt");

    graph.displayGraphType();
    cout << endl;

    graph.displayDegs();
    cout << endl;

    graph.displayAdjMatrix();
    cout << endl;

    graph.displayAdjList();
    cout << endl;

    graph.displayEdgeList();
    cout << endl;

    graph.checkConnectivity();
    cout << endl;

    int s, t;
    cout << "Nhap dinh xuat phat s: ";
    cin >> s;
    cout << "Nhap dinh ket thuc t: ";
    cin >> t;

    cout << "Duong di tu " << s << " den " << t << ": ";
    graph.path(s, t);

    return 0;
}
