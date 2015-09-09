/*
** Problem:     Cross the River
** URL:         https://www.hackerrank.com/contests/w17/challenges/cross-the-river
** Solution:
**     Use DP for rock scores (bottom-up by going backwards), group rocks by x coordinates with
**     each group being a max-heap, RMQ on x coordinates (using segtrees). O(nlogn)
*/

#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <memory>
#include <numeric>

using namespace std;

static const long long MINVAL = numeric_limits<long long>::min();

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

struct Rock
{
    int x, y;
    long long z;
};


auto RockHeapSorter = [](const Rock* a, const Rock* b)
{
    return a->z < b->z;
};

struct XRock  // rock group (wrapper struct for max-heap)
{
    using HeapType = priority_queue<Rock*, vector<Rock*>, decltype(RockHeapSorter)>;

    int x;  // the x coord that all rocks in this group share
    unique_ptr<HeapType> q;
};

// typical segtree stuff
struct RMQNode
{
    long long maxVal;

    unique_ptr<RMQNode> pLeft, pRight;
};

// typical segtree stuff
unique_ptr<RMQNode> build_rmq(int lo, int hi)
{
    auto ret = make_unique<RMQNode>();

    ret->maxVal = MINVAL;

    if (lo < hi)
    {
        int mid = (lo + hi) >> 1;

        ret->pLeft = build_rmq(lo, mid);
        ret->pRight = build_rmq(mid + 1, hi);
    }

    return ret;
}

// typical segtree stuff
long long update_rmq(RMQNode& node, int nLo, int nHi, long long val, int x)
{
    if (x == nLo && x == nHi) // inside
    {
        node.maxVal = val;
    }
    else // partially inside
    {
        int mid = (nLo + nHi) >> 1;

        if (x <= mid)
        {
            update_rmq(*node.pLeft, nLo, mid, val, x);
        }
        else
        {
            update_rmq(*node.pRight, mid + 1, nHi, val, x);
        }

        node.maxVal = max(node.pLeft->maxVal, node.pRight->maxVal);
    }

    return node.maxVal;
}

// typical segtree stuff
long long query_rmq(RMQNode& node, int nLo, int nHi, int qLo, int qHi)
{
    if (nLo >= qLo && nHi <= qHi)
    {
        return node.maxVal;
    }

    if (nHi < qLo || nLo > qHi)
    {
        return MINVAL;
    }

    int mid = (nLo + nHi) >> 1;

    long long a = query_rmq(*node.pLeft, nLo, mid, qLo, qHi);
    long long b = query_rmq(*node.pRight, mid + 1, nHi, qLo, qHi);

    return max(a, b);
}

int main()
{
    int N, H, dH, dW; cin >> N >> H >> dH >> dW;

    vector<Rock> rocks(N);
    unordered_set<int> xs; // this is to only generate the x-coord groupings that we need

    for (auto& r : rocks)
    {
        cin >> r.y >> r.x >> r.z;
        xs.insert(r.x);
    }

    // representing all rocks by groups of x-coords
    // initially empty, will populate as we iterate through each rock
    vector<XRock> alives(xs.size());
    auto xsItr = begin(xs);
    for (auto& a : alives)
    {
        a.x = *xsItr;
        ++xsItr;

        a.q = make_unique<XRock::HeapType>(RockHeapSorter);
    }

    // sort by x, not really necessary for the algorithm; it's an artifact of a previous implementation
    // that did not work. But it may potentially enhance cache performance because of the way the rocks
    // are sorted...(I'm not going to validate this claim)
    sort(begin(alives), end(alives), [](const XRock& a, const XRock& b)
    {
        return a.x < b.x;
    });

    // this is why the above sort is unecessary
    // using this table to help me find the correct x-grouping in O(1)
    int xMapperTable[100001];
    for (int i = 0; i < alives.size(); ++i)
    {
        xMapperTable[alives[i].x] = i;
    }

    // sort by largest y, then by smallest x
    // the DP algorithm is going to look at the rocks closest to the finish and choose it's best neighbor
    sort(begin(rocks), end(rocks), [](const Rock& a, const Rock& b)
    {
        return a.y > b.y || (a.y == b.y && a.x < b.x);
    });

    int rmqWidth = alives.back().x;
    auto rmq = build_rmq(0, rmqWidth);

    // keep track of rocks that are in the staging buffer (ie all the same y-coords as the current y)
    // and rocks that are no longer in the y range.
    int stage = 0, dead = 0;
    for (int i = 0; i < N; ++i)
    {
        if (rocks[i].y < rocks[stage].y)
        {
            // current y coord has changed, push all staged rocks into their respective groups
            while (stage < i)
            {
                auto& heap = *alives[xMapperTable[rocks[stage].x]].q;
                heap.push(&rocks[stage]);

                // update segtree because best score in this x-group may have changed
                update_rmq(*rmq, 0, rmqWidth, heap.top()->z, rocks[stage].x);

                ++stage;
            }
        }

        while (dead < stage && rocks[i].y + dH < rocks[dead].y)
        {
            auto& heap = *alives[xMapperTable[rocks[dead].x]].q;

            // there are rocks that are no longer in range
            // remove the bad rocks from the heap
            while (!heap.empty() && heap.top()->y > rocks[i].y + dH)
            {
                heap.pop();

                // update segtree because best score in this x-group may have changed
                update_rmq(*rmq, 0, rmqWidth, heap.empty() ? MINVAL : heap.top()->z, rocks[dead].x);
            }

            ++dead;
        }

        // find the x-group within range that has the best score
        auto best = query_rmq(*rmq, 0, rmqWidth, max(0, rocks[i].x - dW), min(rmqWidth, rocks[i].x + dW));

        // it's possible to reach the shore from a rock, so that could be the rock's best score unless
        // you can get more points by jumping to another rock
        if (rocks[i].y + dH < H || best > 0)
        {
            if (best == MINVAL)
            {
                rocks[i].z = MINVAL; // can't jump to shore and there are no good rocks to reach the shore
            }
            else
            {
                rocks[i].z += best; // there's a good rock to jump to from this rock
            }
        }
    }

    // one last iteration which represents jumping from the starting shore
    while (stage < N)
    {
        // flush everything that is staged

        auto& heap = *alives[xMapperTable[rocks[stage].x]].q;
        heap.push(&rocks[stage]);

        update_rmq(*rmq, 0, rmqWidth, heap.top()->z, rocks[stage].x);

        ++stage;
    }

    while (dead < N && dH < rocks[dead].y)
    {
        // kill off out of y-range rocks

        auto& heap = *alives[xMapperTable[rocks[dead].x]].q;

        while (!heap.empty() && heap.top()->y > dH)
        {
            heap.pop();

            update_rmq(*rmq, 0, rmqWidth, heap.empty() ? MINVAL : heap.top()->z, rocks[dead].x);
        }

        ++dead;
    }

    // the best score is already located in the root node of the segtree, representing range [0, maxX]
    printf("%lld\n", rmq->maxVal);

    return 0;
}