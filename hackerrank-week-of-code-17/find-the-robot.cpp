/*
** Problem:     Find the Robot
** URL:         https://www.hackerrank.com/contests/w17/challenges/find-the-robot
** Solution:
**     Looking at the x-axis, the robot changes position every 2 moves. Same as the y-axis.
**
**     Sequence of x coords at N=1,3,5,7,9,11:  1, -2, 3, -4, 5, -6
**     Sequence of y coords at N=2,4,6,8,10,12: 2, -2, 4, -4, 6, -6
**
**     x = (N + 1) / 2, and negative if even
**     y = (N / 2), +1 if odd, negative if even
*/

#include <iostream>
#include <cstdio>

using namespace std;

void process()
{
    int N; cin >> N;

    int x, y;

    x = (N + 1) >> 1;
    if ((x & 1) == 0)
    {
        x = -x;
    }

    y = N >> 1;
    if (y & 1)
    {
        y += 1;
    }
    else
    {
        y = -y;
    }

    printf("%d %d", x, y);
}

int main()
{
    int T; cin >> T;

    while (T--)
    {
        process();
        printf("\n");
    }

    return 0;
}