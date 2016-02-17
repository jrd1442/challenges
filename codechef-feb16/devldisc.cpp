/*
** Problem:     Devu and a light discussion
** URL:         https://www.codechef.com/problems/DEVLDISC
** Solution:
**        (1)
**        / \
**      (2) (6)--(7)
**       |   |
**      (3) (5)
**        \ /
**        (4)
**
**     A valid solution needs at least 7 nodes as pictured above. The true diameter is the distance
**     from (3) to (7), which is 4. However, if Churu's algorithm starts on (1), it will return the
**     distance from (1) to (4), or from (7) to (4), which is 3. To create a solution using more
**     than 7 nodes, just create edges from the new nodes to (6). (This is why they didn't list the
**     sample answer in the sample output; it's trivial to create a new solution from it)
**     O(N)
*/


#include <iostream>
#include <vector>

using namespace std;

using int32 = int;
#define int long long

void process()
{
    int N; cin >> N;
    if (N < 7)
    {
        // not enough nodes to break Churu's algorithm
        printf("-1");
        return;
    }

    printf("%lld\n", N);

    // build the hexagon
    printf("1 2\n2 3\n3 4\n4 5\n5 6\n1 6");

    for (int i = 7; i <= N; ++i)
    {
        // create edges for each new node to (6)
        printf("\n6 %lld", i);
    }

    printf("\n1");
}

int32 main()
{
    int T; cin >> T;

    while (T--)
    {
        process();
        printf("\n");
    }

    return 0;
}