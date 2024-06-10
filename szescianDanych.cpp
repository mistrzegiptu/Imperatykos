#include <iostream>

using namespace std;
int det(int matrix[32][32], int size)
{
    int determinant = 0;
    if(size == 1)
        return matrix[0][0];
    else if(size == 2)
        return matrix[0][0]*matrix[1][1] - matrix[0][1]*matrix[1][0];
    else
    {
        int temp[32][32];
        for(int index = 0; index < size; index++)
        {
            int x = 0;
            for (int i = 1; i < size; i++)
            {
                int y = 0;
                for (int j = 0; j < size; j++)
                {
                    if(j == index)
                        continue;
                    temp[x][y] = matrix[i][j];
                    y += 1;
                }
                x += 1;
            }
            if(index%2 == 0)
                determinant += matrix[0][index] * det(temp, size-1);
            else
                determinant += matrix[0][index] * det(temp, size-1) * (-1);
        }
    }
    return determinant;
}
int main()
{
    int n = 0, result = 0;
    cin>>n;
    int tab[n][n][n];

    for(int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                cin>>tab[i][j][k];

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
                            if((x-l)*(x-l) + (y-v)*(y-v) + (z-p)*(z-p) <= r*r)
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
            case 'D':
                char k = 0;
                int index = 0;
                cin>>k>>index;
                int matrix[32][32];
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