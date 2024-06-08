#include <iostream>

using namespace std;

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
                if(d >= (n/2))
                    dp = -1;

                int x = l, y = v, z = p;

                for(int i = 0; i < h; i++)
                {
                    for(int j = 0; j < w; j++)
                    {
                        for(int k = 0; k < d; k++)
                        {
                            if(x >= 0 && x < n && y >= 0 && y < n && z >= 0 && z < n)
                                result += tab[x][y][z];
                            x += dp;
                        }
                        y += dv;
                    }
                    z += dl;
                }
                cout<<result<<endl;
                result = 0;
                break;
            }
            case 'T':
            {
                /*for i in range(n):
                    for j in range(n-i):
                        for k in range(n-j-i):
                            print("*", end = '')*/
                int l = 0, v = 0, p = 0, e = 0, dl = 1, dv = 1, dp = 1;

                cin >> l >> v >> p >> e;

                if(l > (n/2))
                    dl = -1;
                if(v > (n/2))
                    dv = -1;
                if(p > (n/2))
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

                for(int i = 0; i < r; i++)
                {
                    for(int j = 0; j < r; j++)
                    {
                        for(int k = 0; k < r; k++)
                        {
                            if((x-l)*(x-l) + (y-v)*(y-v) + (z-p)*(z-p) <= r*r)
                                result += tab[x][y][z];
                            x += dp;
                        }
                        y += dv;
                    }
                    z += dl;
                }
                cout<<result<<endl;
                result = 0;
                break;
            }
            case 'D':
                break;
            default:
                break;
        }
    }
}