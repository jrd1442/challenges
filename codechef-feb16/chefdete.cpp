/*
** Problem:     Chef-Detective
** URL:         https://www.codechef.com/problems/CHEFDETE
** Solution:
**     Super simple. If someone is receiving a report, that person is not a minor criminal. Keep
**     an array of reportees, then spit out whoever is not a reportee. O(N)
*/


#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int N; cin >> N;

    vector<bool> reportee(N+1);

    for (int i = 0; i < N; ++i)
    {
        int t; cin >> t;
        reportee[t] = true;
    }

    bool firstOutput = true;

    // the Don is not a lowly criminal, so start from i=1
    for (int i = 1; i < reportee.size(); ++i)
    {
        if (!reportee[i])
        {
            if (firstOutput)
            {
                printf("%d", i);
                firstOutput = false;
            }
            else
            {
                printf(" %d", i);
            }
        }
    }

    return 0;
}