/*
** Problem:     Sereja and Two Lines
** URL:         https://www.codechef.com/problems/SEATL
** Solution:
**     Gather row information. Row information is a hash map that holds the number of times each unique value is found in a given row.
**     Also, keep track of the columns that contain the highest count for a given value in the table. Use this preprocessed data to 
**     calculate the answer. O(N * M * log( min(N,M) )
*/



#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#define ALL(x) begin(x), end(x)

using namespace std;


// used to keep track of a value's count per row/col
vector<int> cnt(1000001);

vector<vector<int>> transpose(const vector<vector<int>>& mat, int N, int M)
{
    vector<vector<int>> ret(M);
    for (int i = 0; i < M; ++i)
    {
        ret[i].resize(N);
        for (int j = 0; j < N; ++j)
        {
            ret[i][j] = mat[j][i];
        }
    }
    return ret;
}

// stores the list of best columns for a given value in the table
struct ValInfo
{
    //    <colIndex, count>  count is redundant, since the vec will only store the best
    vector<pair<int, int>> vec;

    // index into vec that is the best option; needed because we'll be modifying the elements
    // in the vec as we try to find the best answer
    int best = 0;
};


void process()
{
    int N; cin >> N;
    int M; cin >> M;

    vector<vector<int>> mat(N);
    for (auto& r : mat)
    {
        r.resize(M);
        for (auto& c : r)
        {
            cin >> c;
        }
    }

    // at the time, I felt the need to make sure M is small because most of the heavy lifting
    // will be done on M. Now i'm not so sure i needed to do it
    if (M > N)
    {
        mat = transpose(mat, N, M);
        swap(N, M);
    }

    //                <value, numTimesOccuring>
    vector<unordered_map<int, int>> rowInfo(N);

    for (int i = 0; i < N; ++i)
    {
        // first pass: find the count for each value
        for (int j = 0; j < M; ++j)
        {
            int key = mat[i][j];
            ++cnt[key];
        }

        // second pass: apply to rowInfo list
        for (int j = 0; j < M; ++j)
        {
            int key = mat[i][j];
            
            auto& val = rowInfo[i][key];
            val = cnt[key];
        }

        // third pass: reset counters
        for (int j = 0; j < M; ++j)
        {
            int key = mat[i][j];
            cnt[key] = 0;
        }
    }

    //         <value, bestCols>
    unordered_map<int, ValInfo> bestColInfo;

    for (int i = 0; i < M; ++i)
    {
        // first pass: find the count for each value
        for (int j = 0; j < N; ++j)
        {
            int key = mat[j][i];
            ++cnt[key];
        }

        // second pass: apply to bestColInfo. If we find a better column,
        // clear the list. We only need the best of the best.
        for (int j = 0; j < N; ++j)
        {
            int key = mat[j][i];
            int val = cnt[key];

            auto& valInfo = bestColInfo[key];

            if (valInfo.vec.empty())
            {
                valInfo.vec.push_back({ i, val });
            }
            else if (valInfo.vec.back().first != i)
            {
                if (valInfo.vec.back().second < val)
                {
                    valInfo.vec.clear();
                    valInfo.vec.push_back({ i, val });
                }
                else if (valInfo.vec.back().second == val)
                {
                    valInfo.vec.push_back({ i, val });
                }
            }

        }

        // third pass: reset counters
        for (int j = 0; j < N; ++j)
        {
            int key = mat[j][i];
            cnt[key] = 0;
        }
    }

    int best = -1;
    for (int i = 0; i < N; ++i)
    {
        // first pass: decrement best column count for entries in this row
        // we're essentially removing this row from the column counter information
        for (int j = 0; j < M; ++j)
        {
            int key = mat[i][j];
            auto& valInfo = bestColInfo[key];

            auto itr = lower_bound(ALL(valInfo.vec), j, [](const pair<int, int>& a, int val)
            {
                return a.first < val;
            });

            if (itr != end(valInfo.vec) && itr->first == j)
            {
                // if we find that this entry is at the intersection point of a best col, decrement that columns count
                --valInfo.vec[valInfo.best++].second;
            }
        }
        
        // second pass: find the best
        for (int j = 0; j < M; ++j)
        {
            int key = mat[i][j];
            int val = rowInfo[i][key];

            const auto& valInfo = bestColInfo[key];
            val += valInfo.best < valInfo.vec.size() ? valInfo.vec[valInfo.best].second : valInfo.vec.front().second;

            best = max(best, val);
        }

        // third pass, reset the best column counts
        for (int j = 0; j < M; ++j)
        {
            int key = mat[i][j];

            auto& valInfo = bestColInfo[key];
            auto itr = lower_bound(ALL(valInfo.vec), j, [](const pair<int, int>& a, int val)
            {
                return a.first < val;
            });

            if (itr != end(valInfo.vec) && itr->first == j)
            {
                ++valInfo.vec[--valInfo.best].second;
            }
        }
    }

    printf("%d", best);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int T; cin >> T;

    while (T--)
    {
        process();
        printf("\n");
        cout.flush();
    }

    return 0;
}