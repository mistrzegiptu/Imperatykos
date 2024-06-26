#include <iostream>

using namespace std;
long long det(long long matrix[32][32], int size)
{
    long long sign = 1;
    if(size <= 0 || size > 32 || size % 2 == 1)
        return 1;

    for(int i = 0; i < size-1; i++)
    {
        if(matrix[i][i] == 0)
        {
            int j = 0;
            for(j = i + 1; j < size; j++)
            {
                if(matrix[j][i] != 0)
                {
                    for (int k = 0; k < size; k++)
                    {
                        long long swapper = matrix[j][k];
                        matrix[j][k] = matrix[i][k];
                        matrix[i][k] = swapper;
                    }
                    sign = -sign;
                    break;
                }
            }
            if(j == size)
                return 0;
        }
        for(int j = i + 1; j < size; j++)
        {
            for(int k = i + 1; k < size; k++)
            {
                matrix[j][k] = matrix[j][k] * matrix[i][i] - matrix[j][i] * matrix[i][k];
                if(i != 0)
                    matrix[j][k] /= matrix[i-1][i-1];
            }
        }
    }
    return sign * matrix[size-1][size-1];
}
int main()
{
    int n = 0, result = 0;
    cin>>n;
    int tab[n][n][n];

    for(int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                cin>>tab[j][k][i];

    char c = 0;
    cin>>c;

    while(c != 'E')
    {
        switch(c)
        {
            case 'C':
            {
                int l = 0, v = 0, p = 0, h = 0, w = 0, d = 0, dl = 1, dv = 1, dp = 1;
                cin >> l >> v >> p >> h >> w >> d;

                if(l >= (n/2))
                    dl = -1;
                if(v >= (n/2))
                    dv = -1;
                if(p >= (n/2))
                    dp = -1;

                int x = l, y = v, z = p;

                for(int i = 0; i <= h; i++)
                {
                    y = v;
                    for(int j = 0; j <= w; j++)
                    {
                        z = p;
                        for(int k = 0; k <= d; k++)
                        {
                            if(x >= 0 && x < n && y >= 0 && y < n && z >= 0 && z < n)
                            {
                                result += tab[x][y][z];
                            }
                            z += dp;
                        }
                        y += dv;
                    }
                    x += dl;
                }
                cout<<result<<endl;
                result = 0;
                break;
            }
            case 'T':
            {
                int l = 0, v = 0, p = 0, e = 0, dl = 1, dv = 1, dp = 1;

                cin >> l >> v >> p >> e;

                if(l >= (n/2))
                    dl = -1;
                if(v >= (n/2))
                    dv = -1;
                if(p >= (n/2))
                    dp = -1;

                int x = l, y = v, z = p;

                for(int i = 0; i <= e; i++)
                {
                    y = v;
                    for(int j = 0; j <= e - i; j++)
                    {
                        z = p;
                        for(int k = 0; k <= e - i - j; k++)
                        {
                            if(x >= 0 && x < n && y >= 0 && y < n && z >= 0 && z < n)
                                result += tab[x][y][z];
                            z += dp;
                        }
                        y += dv;
                    }
                    x += dl;
                }
                cout<<result<<endl;
                result = 0;
                break;
            }
            case 'O':
            {
                int l = 0, v = 0, p = 0, r = 0, dl = 1, dv = 1, dp = 1;
                cin >> l >> v >> p >> r;

                if(l >= (n/2))
                    dl = -1;
                if(v >= (n/2))
                    dv = -1;
                if(p >= (n/2))
                    dp = -1;

                int x = l, y = v, z = p;

                for(int i = 0; i <= r; i++)
                {
                    y = v;
                    for(int j = 0; j <= r; j++)
                    {
                        z = p;
                        for(int k = 0; k <= r; k++)
                        {
                            if(x >= 0 && x < n && y >= 0 && y < n && z >= 0 && z < n)
                            {
                                if ((x - l) * (x - l) + (y - v) * (y - v) + (z - p) * (z - p) <= r * r)
                                    result += tab[x][y][z];
                            }
                            z += dp;
                        }
                        y += dv;
                    }
                    x += dl;
                }
                cout<<result<<endl;
                result = 0;
                break;
            }
            case 'D':
                char k = 0;
                int index = 0;
                cin>>k>>index;
                long long matrix[32][32];
                for(int i = 0; i < n; i++)
                {
                    for(int j = 0; j < n; j++)
                    {
                        switch(k)
                        {
                            case 'l':
                                matrix[i][j] = tab[index][i][j];
                                break;
                            case 'v':
                                matrix[i][j] = tab[i][index][j];
                                break;
                            case 'p':
                                matrix[i][j] = tab[i][j][index];
                                break;
                        }
                    }
                }
                cout<<det(matrix, n)<<endl;
                break;
        }
        cin>>c;
    }
}