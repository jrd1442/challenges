/*
** Problem:     Chef and Strange Operations
** URL:         https://www.codechef.com/problems/STROPR
** Solution:
**     After M operations,
**       A[x] = A[x] + A[x-1]*M + A[x-2]*M*(M+1)/2! + A[x-3]*M*(M+1)*(M+2)/3! + ...
**     Factorials are too expensive to compute given the constraints, but notice that the
**     coefficient can be computed iteratively by multiplying by M+i and dividing by i+1.
**     O(N+x)
*/


#include <iostream>
#include <vector>

using namespace std;

using int64 = long long;

const int64 MOD = 1000000007;

int64 fast_pow(int64 n, int64 p)
{
    if (p == 0)
    {
        return 1;
    }

    n = n % MOD;

    if (p == 1)
    {
        return n;
    }

    if (p == 2)
    {
        return (n*n) % MOD;
    }

    auto a2 = fast_pow(n, p >> 1);
    a2 = (a2*a2) % MOD;

    if (p & 1)
    {
        a2 *= n;
        a2 %= MOD;
    }

    return a2;
}

int64 mod_inverse(int64 n)
{
    return fast_pow(n, MOD - 2);
}


void process()
{
    int64 N, x, M;
    cin >> N >> x >> M;

    vector<int64> A(N+1);
    for (int i = 1; i <= N; ++i)
    {
        cin >> A[i];
        A[i] %= MOD; // we need to mod anyway so we might as well do it here
    }

    int64 sum = A[x];

    // ugly for loop, i know
    for (int64 i = x - 1, j = 1, coeff = 1, m = M % MOD;
         i > 0;
         --i, ++j, m = (m+1) % MOD)
    {
        coeff *= m;
        coeff %= MOD;
        auto mj = mod_inverse(j);
        coeff *= mj;
        coeff %= MOD;

        sum += (A[i] * coeff) % MOD;
        sum %= MOD;
    }

    printf("%lld", sum);
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