/*
** Problem:     Road Reform
** URL:         https://www.hackerrank.com/contests/w17/challenges/roads-building
** Solution:
**     Using Dijkstra's algorithm, find the distance (ie, cost) from every node to the first. Do
**     the same with the last node. You should now know the distance from the first to the last.
**     Sort the two distance lists.
**
**     For each node X in the first list, "create a path" to a node Y in the second list, provided
**     that the sum of the costs of X->Y, 1->X, and Y->last is smaller than the cost of 1->last.
**     There can be more than one cost of X->Y that fits that requirement. Naively, this can be
**     done in O(n^2). Rephrase the solution like this: for each path 1->X, find all Y->last paths
**     that fit the requirement. O(nlogn). See implementation details below.
*/

#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <numeric>

using namespace std;

struct City
{
    int rank;
    vector<pair<City*, long long>> neighbors;
};

void find_smallest_costs(City& a, vector<long long>& costs)  // Dijkstra's algo
{
    auto CitySorter = [](const pair<City*, long long>& a, const pair<City*, long long>& b)
    {
        return a.second > b.second;
    };

    // create a min-heap of City-cost pairs, sorted by cost
    // City-cost in this context represents cost from element x to the root a
    priority_queue<pair<City*, long long>, vector<pair<City*, long long>>, decltype(CitySorter)> heap(CitySorter);
    heap.push(make_pair(&a, 0ll));

    while (!heap.empty())
    {
        auto cost = heap.top().second;
        auto& next = *heap.top().first;
        heap.pop();

        if (costs[next.rank] != -1) continue; // skip if we've already computed the minimum cost of this node

        costs[next.rank] = cost; // this is the minimum cost of x to a

        for (auto& n : next.neighbors)
        {
            if (costs[n.first->rank] == -1) 
            {
                // push x's neighbors; neighbor nodes may already exist in the heap, but with different costs
                heap.push(make_pair(n.first, costs[next.rank] + n.second));
            }
        }
    }
}

int main()
{
    int N, M; cin >> N >> M;

    vector<City> cities(N + 1);
    vector<long long> c1(N + 1), cN(N + 1);

    // init costs to -1 for dijkstra's algo
    fill(begin(c1), end(c1), -1);
    fill(begin(cN), end(cN), -1);

    for (int i = 1; i <= N; ++i)
    {
        cities[i].rank = i;  // for accessing the cost arrays
    }

    for (int i = 0; i < M; ++i)
    {
        long long x, y, z; cin >> x >> y >> z;

        cities[x].neighbors.push_back(make_pair(&cities[y], z));
        cities[y].neighbors.push_back(make_pair(&cities[x], z));
    }

    find_smallest_costs(cities[1], c1);
    find_smallest_costs(cities[N], cN);

    auto smallest = c1.back(); // smallest distance from 1->N, could have also used cN[1]

    // remove the dummy first element
    swap(c1.front(), c1.back()); c1.pop_back();
    swap(cN.front(), cN.back()); cN.pop_back();

    sort(begin(c1), end(c1));
    sort(begin(cN), end(cN));

    // Create partial-sums for paths Y->N
    vector<long long> partSum(N);
    partial_sum(begin(cN), end(cN), begin(partSum));

    long long cnt = 0;

    for (auto& x : c1)
    {
        // some info: 1->X has to be smaller than 1->N because we need to give some cost for X->Y
        // since the 1->X list is sorted, exit the loop when we encounter the scenario
        if (x >= smallest)
        {
            break;
        }

        // if X->Y = 1, Y->N has to be at most this:
        auto d = smallest - x - 1;

        // find the index i in cN where cN[i] meets the constraint (use binary search)
        // yIdx points to the first term that DOESN'T meet requirements
        int yIdx = distance(begin(cN), upper_bound(begin(cN), end(cN), d));

        // this is tough to explain...consider the range of possible costs of X->Y as [1,Z].
        // that means that there are Z possible solutions, so you have to add Z to the count,
        // but how to calculate Z? Z = cost[1->N] - cost[1->X] - 1 - cost[1->Y]. Imagine that in
        // the naive implementation, we would be looping through every Y, so only the last term
        // needs to get evaluated. So the sum of Zs for a given X is
        // numYs*(cost[1->N] - cost[1->X] - 1) - sum_of_Y_costs
        // that last term was baked into the partialSum array. The first term is d*yIdx
        cnt += ((d)*(yIdx)) - partSum[yIdx - 1];
    }

    printf("%lld\n", cnt);

    return 0;
}