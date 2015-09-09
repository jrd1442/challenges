/*
** Problem:     Count Fridays the 13th
** URL:         https://www.hackerrank.com/contests/w17/challenges/count-fridays-the-13-th
** Solution:
**     Simple if you can figure out what day of the week any given day is. Easy for Java
**     programmers. day_of_week() is a known algorithm for figuring it out, return value
**     of 5=friday. See
**     http://www.geeksforgeeks.org/find-day-of-the-week-for-a-given-date/
**
**     Iterate through every month until the final date has been passed. This works
**     because, given the constraints, the program will complete in less than 10k
**     iterations per test case.
*/

#include <iostream>
#include <cstdio>

using namespace std;

int day_of_week(int d, int m, int y)
{
    static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
}

bool is_lesser_equal(int d1, int m1, int y1, int d2, int m2, int y2)
{
    if (y1 < y2) return true;
    if (y1 > y2) return false;

    // y1 == y2

    if (m1 < m2) return true;
    if (m1 > m2) return false;

    // m1 == m2

    if (d1 <= d2) return true;

    return false;
}

void process()
{
    int d1, m1, y1, d2, m2, y2;
    cin >> d1 >> m1 >> y1 >> d2 >> m2 >> y2;

    // go up to the next 13th day of the month

    if (d1 > 13) 
    {
        // go to the next month
        ++m1;
        if (m1 > 12)
        {
            ++y1;
            m1 = 1;
        }
    }

    d1 = 13;
    int cnt = 0;

    while (is_lesser_equal(d1, m1, y1, d2, m2, y2))
    {
        if (day_of_week(d1, m1, y1) == 5) // friday
        {
            ++cnt;
        }

        ++m1;
        if (m1 > 12)
        {
            // loop back to January
            ++y1;
            m1 = 1;
        }
    }

    printf("%d", cnt);
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