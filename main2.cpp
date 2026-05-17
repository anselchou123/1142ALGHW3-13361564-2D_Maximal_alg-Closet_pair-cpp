#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iomanip>

using namespace std;

struct Point {
    int x;
    int y;
};

// 計算兩點歐幾里得距離
double distancePoints(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

bool cmpX(const Point& a, const Point& b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

bool cmpY(const Point& a, const Point& b) {
    if (a.y != b.y) return a.y < b.y;
    return a.x < b.x;
}

// 暴力法：處理小規模點群 (n <= 3)
double bruteForceClosest(const vector<Point>& S) {
    if (S.size() < 2) {
        return numeric_limits<double>::infinity();
    }
    double minDist = numeric_limits<double>::infinity();
    for (size_t i = 0; i < S.size(); i++) {
        for (size_t j = i + 1; j < S.size(); j++) {
            double d = distancePoints(S[i], S[j]);
            if (d < minDist) {
                minDist = d;
            }
        }
    }
    return minDist;
}

// 2D Closest Pair 分治核心遞迴
double closestPairRecursive(vector<Point> Px, vector<Point> Py) {
    int n = Px.size();

    if (n <= 3) {
        return bruteForceClosest(Px);
    }

    // Step 1: 劃分中線
    int mid = n / 2;
    Point midPoint = Px[mid];
    int L = midPoint.x;

    // Step 2: 切分 Px
    vector<Point> PxL(Px.begin(), Px.begin() + mid);
    vector<Point> PxR(Px.begin() + mid, Px.end());

    // 精確分流 Py 到 PyL 與 PyR (處理潛在的重複點座標)
    vector<Point> PyL, PyR;
    for (const auto& p : Py) {
        if (p.x < L || (p.x == L && p.y < midPoint.y)) {
            PyL.push_back(p);
        } else if (p.x == L && p.y == midPoint.y) {
            if (PyL.size() < PxL.size()) {
                PyL.push_back(p);
            } else {
                PyR.push_back(p);
            }
        } else {
            PyR.push_back(p);
        }
    }

    // 避免切分異常造成的無效遞迴
    if (PxL.empty() || PxR.empty() || PyL.size() != PxL.size() || PyR.size() != PxR.size()) {
        return bruteForceClosest(Px);
    }

    // Step 3: 遞迴找出左右半邊的最小距離
    double dL = closestPairRecursive(PxL, PyL);
    double dR = closestPairRecursive(PxR, PyR);
    double delta = min(dL, dR);

    // Step 4: 收集位於中線帶狀區域 (Strip) 內的點
    vector<Point> strip;
    for (const auto& p : Py) {
        if (abs(p.x - L) < delta) {
            strip.push_back(p);
        }
    }

    // Step 5: 檢查帶狀區域內跨越中線的可能更短點對 (至多往前檢查 7 個點)
    int m = strip.size();
    for (int i = 0; i < m; i++) {
        for (int j = i + 1; j < m && (strip[j].y - strip[i].y) < delta; j++) {
            double d = distancePoints(strip[i], strip[j]);
            if (d < delta) {
                delta = d;
            }
        }
    }

    return delta;
}

// 主封裝函式
double closestPair(vector<Point> S) {
    if (S.size() < 2) return 0.0;
    
    vector<Point> Px = S;
    vector<Point> Py = S;
    
    sort(Px.begin(), Px.end(), cmpX);
    sort(Py.begin(), Py.end(), cmpY);

    return closestPairRecursive(Px, Py);
}

int main() {
    // 題目第 5 題的測資 1
    vector<Point> case1 = {
        {5,12}, {12,10}, {2,9}, {5,7}, {10,6},
        {9,4}, {6,3}, {11,4}, {1,3}, {6,1}
    };

    // 題目第 5 題的測資 2
    vector<Point> case2 = {
        {6,0}, {11,3}, {10,9}, {8,5}, {9,7}, {6,10},
        {4,7}, {5,4}, {3,8}, {2,3}, {2,9}, {9,4}
    };

    cout << "=== Part B: 2D_Closest_Pair 執行結果 ===\n\n";
    
    // 設定輸出浮點數格式
    cout << fixed << setprecision(5);

    cout << "Closest Pair Distance of Case 1: " << closestPair(case1) << "\n";
    cout << "Closest Pair Distance of Case 2: " << closestPair(case2) << "\n";

    return 0;
}