#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Point {
    int x;
    int y;
};

// 工具函式：印出點群
void printPoints(const vector<Point>& pts, const string& title) {
    cout << title << "\n";
    for (const auto& p : pts) {
        cout << "(" << p.x << "," << p.y << ") ";
    }
    cout << "\n\n";
}

// 排序輔助：先比 x，再比 y
bool cmpX(const Point& a, const Point& b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

// 判斷 p1 是否被 p2 凌駕 (Dominated)
bool isDominated(const Point& p1, const Point& p2) {
    if (p2.x >= p1.x && p2.y >= p1.y) {
        if (p2.x > p1.x || p2.y > p1.y) {
            return true;
        }
    }
    return false;
}

// 暴力法：處理基本型 (n <= 3)
vector<Point> bruteForceMaximal(const vector<Point>& S) {
    vector<Point> result;
    for (size_t i = 0; i < S.size(); i++) {
        bool dominated = false;
        for (size_t j = 0; j < S.size(); j++) {
            if (i != j && isDominated(S[i], S[j])) {
                dominated = true;
                break;
            }
        }
        if (!dominated) {
            result.push_back(S[i]);
        }
    }
    return result;
}

// 取得 x 座標的中位數
int getMedianX(const vector<Point>& S) {
    vector<int> xs;
    for (const auto& p : S) {
        xs.push_back(p.x);
    }
    sort(xs.begin(), xs.end());
    return xs[xs.size() / 2];
}

// 2D_Maximal 主遞迴函式
vector<Point> maximalPoints(const vector<Point>& S) {
    int n = S.size();

    if (n <= 3) {
        return bruteForceMaximal(S);
    }

    // Step 1: 找出中位數
    int medianX = getMedianX(S);

    // Step 2: 分割為 SL 與 SR
    vector<Point> SL, SR;
    for (const auto& p : S) {
        if (p.x <= medianX) {
            SL.push_back(p);
        } else {
            SR.push_back(p);
        }
    }

    // 避免極端情況（例如所有點 x 都相同）造成死迴圈
    if (SL.size() == S.size() || SR.size() == S.size()) {
        return bruteForceMaximal(S);
    }

    // Step 3: 遞迴求解
    vector<Point> leftMaximal = maximalPoints(SL);
    vector<Point> rightMaximal = maximalPoints(SR);

    // Step 4: 找出右半邊的最大 y 值
    int ymax = -1;
    if (!rightMaximal.empty()) {
        ymax = rightMaximal[0].y;
        for (const auto& p : rightMaximal) {
            if (p.y > ymax) {
                ymax = p.y;
            }
        }
    }

    // Step 5: 淘汰左半邊 y < ymax 的點
    vector<Point> filteredLeft;
    if (rightMaximal.empty()) {
        filteredLeft = leftMaximal;
    } else {
        for (const auto& p : leftMaximal) {
            if (p.y >= ymax) {
                filteredLeft.push_back(p);
            }
        }
    }

    // Step 6: 合併結果
    vector<Point> result = filteredLeft;
    result.insert(result.end(), rightMaximal.begin(), rightMaximal.end());

    return result;
}

int main() {
    // 題目第 4 題的測資 1
    vector<Point> case1 = {
        {5,12}, {12,10}, {2,9}, {5,7}, {10,6},
        {9,4}, {6,3}, {11,4}, {1,3}, {6,1}
    };

    // 題目第 4 題的測資 2
    vector<Point> case2 = {
        {6,0}, {11,3}, {10,9}, {8,5}, {9,7}, {6,10},
        {4,7}, {5,4}, {3,8}, {2,3}, {2,9}, {9,4}
    };

    cout << "=== Part A: 2D_Maximal 執行結果 ===\n\n";

    printPoints(case1, "Input Case 1:");
    vector<Point> ans1 = maximalPoints(case1);
    sort(ans1.begin(), ans1.end(), cmpX);
    printPoints(ans1, "Maximal Points of Case 1:");

    printPoints(case2, "Input Case 2:");
    vector<Point> ans2 = maximalPoints(case2);
    sort(ans2.begin(), ans2.end(), cmpX);
    printPoints(ans2, "Maximal Points of Case 2:");

    return 0;
}