#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_CARDS 52
#define NUMBER_OF_COLORS 4

int random_from_interval(int a, int b)
{
    return (rand()%(b-a+1))+a;
}
void shuffle(int t[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        int k = random_from_interval(i, size-1);
        int swapper = t[i];
        t[i] = t[k];
        t[k] = swapper;
    }
}
void game(int n, int player_A[], int player_B[], int max_conflicts, int simplified_mode)
{
    int conflictCount = 0;
    int p1Index = 0, p1CardCount = n/2;
    int p2Index = 0, p2CardCount = n/2;

    while(conflictCount <= max_conflicts && p1CardCount > 0 && p2CardCount > 0)
    {
        //Zrównanie wartości figur o różnych kolorach
        int card1 = (int)(player_A[p1Index%n]/NUMBER_OF_COLORS);
        int card2 = (int)(player_B[p2Index%n]/NUMBER_OF_COLORS);
        if(card1 != card2)
        {
            if(card1 > card2)
            {
                player_A[(p1Index+p1CardCount)%n] = player_A[p1Index%n];
                player_A[(p1Index+p1CardCount+1)%n] = player_B[p2Index%n];
                p1CardCount++;
                p2CardCount--;
            }
            else
            {
                player_B[(p2Index+p2CardCount)%n] = player_B[p2Index%n];
                player_B[(p2Index+p2CardCount+1)%n] = player_A[p1Index%n];
                p2CardCount++;
                p1CardCount--;
            }
            conflictCount++;
            p1Index++;
            p2Index++;
        }
        else
        {
            if(simplified_mode==1)
            {
                //Przenosi karty gracza A i B na koniec swojej ręki
                player_A[(p1Index+p1CardCount)%n] = player_A[p1Index%n];
                p1Index++;
                player_B[(p2Index+p2CardCount)%n] = player_B[p2Index%n];
                p2Index++;
                conflictCount++;
            }
            else
            {
                int warCount = 0;
                while(card1 == card2)
                {
                    if(conflictCount > max_conflicts)
                    {
                        printf("0 %d %d\n", p1CardCount, p2CardCount);
                        return;
                    }
                    conflictCount++;
                    warCount++;
                    if(p1CardCount < 3 || p2CardCount < 3)
                    {
                        printf("1 %d %d\n", p1CardCount, p2CardCount);
                        return;
                    }
                    p1Index+=2;
                    p1CardCount-=2;
                    p2Index+=2;
                    p2CardCount-=2;
                    card1 = (int)(player_A[p1Index%n]/NUMBER_OF_COLORS);
                    card2 = (int)(player_B[p2Index%n]/NUMBER_OF_COLORS);
                }
                if(card1 > card2)
                {
                    //Cofamy się o ilość wojen * 2 karty, żeby ułożyć karty od pierwszej wyłożonej na stół
                    for (int i = warCount; i > 0; i--)
                    {
                        player_A[(p1Index+p1CardCount)%NUMBER_OF_CARDS] = player_A[(p1Index-(2*i))%NUMBER_OF_CARDS];
                        p1CardCount++;
                        player_A[(p1Index+p1CardCount)%NUMBER_OF_CARDS] = player_A[(p1Index-(2*i-1))%NUMBER_OF_CARDS];
                        p1CardCount++;
                    }
                    player_A[(p1Index+p1CardCount)%NUMBER_OF_CARDS] = player_A[p1Index%NUMBER_OF_CARDS];
                    p1Index++;
                    for (int i = warCount; i > 0; i--)
                    {
                        player_A[(p1Index+p1CardCount)%NUMBER_OF_CARDS] = player_B[(p2Index-(2*i))%NUMBER_OF_CARDS];
                        p1CardCount++;
                        player_A[(p1Index+p1CardCount)%NUMBER_OF_CARDS] = player_B[(p2Index-(2*i-1))%NUMBER_OF_CARDS];
                        p1CardCount++;
                    }
                    player_A[(p1Index+p1CardCount)%NUMBER_OF_CARDS] = player_B[p2Index%NUMBER_OF_CARDS];
                    p1CardCount++;
                    p2CardCount--;
                    p2Index++;
                    conflictCount++;
                }
                else
                {
                    //Cofamy się o ilość wojen * 2 karty, żeby ułożyć karty od pierwszej wyłożonej na stół
                    for (int i = warCount; i > 0; i--)
                    {
                        player_B[(p2Index+p2CardCount)%NUMBER_OF_CARDS] = player_B[(p2Index-(2*i))%NUMBER_OF_CARDS];
                        p2CardCount++;
                        player_B[(p2Index+p2CardCount)%NUMBER_OF_CARDS] = player_B[(p2Index-(2*i-1))%NUMBER_OF_CARDS];
                        p2CardCount++;
                    }
                    player_B[(p2Index+p2CardCount)%NUMBER_OF_CARDS] = player_B[p2Index%NUMBER_OF_CARDS];
                    p2Index++;
                    for (int i = warCount; i > 0; i--)
                    {
                        player_B[(p2Index+p2CardCount)%NUMBER_OF_CARDS] = player_A[(p1Index-(2*i))%NUMBER_OF_CARDS];
                        p2CardCount++;
                        player_B[(p2Index+p2CardCount)%NUMBER_OF_CARDS] = player_A[(p1Index-(2*i-1))%NUMBER_OF_CARDS];
                        p2CardCount++;
                    }
                    player_B[(p2Index+p2CardCount)%NUMBER_OF_CARDS] = player_A[p1Index%NUMBER_OF_CARDS];
                    p2CardCount++;
                    p1CardCount--;
                    p1Index++;
                    conflictCount++;
                }
            }
        }
    }
    if(conflictCount == max_conflicts)
    {
        printf("0 %d %d\n", p1CardCount, p2CardCount);
        return;
    }
    if(p2CardCount==0)
    {
        printf("2 %d\n", conflictCount);
        return;
    }
    else if(p1CardCount==0)
    {
        printf("3\n");
        for (int i = 0; i < p2CardCount; ++i)
            printf("%d ", player_B[(p2Index+i)%NUMBER_OF_CARDS]);
        printf("\n");
        return;
    }

}
int main(void) {
    int player_A[NUMBER_OF_CARDS], player_B[NUMBER_OF_CARDS];
    int deck[NUMBER_OF_CARDS];
    int max_conflicts;
    int simplified_mode;
    int seed;
    scanf("%d", &seed);
    scanf("%d", &simplified_mode);
    scanf("%d", &max_conflicts);
    for(int i = 0; i < NUMBER_OF_CARDS; i++) deck[i] = i;
    srand((unsigned) seed);
    shuffle(deck, NUMBER_OF_CARDS);
    for(int i = 0; i < NUMBER_OF_CARDS / 2; i++) {
        player_A[i] = deck[i];
        player_B[i] = deck[i + NUMBER_OF_CARDS / 2];
    }
    game(NUMBER_OF_CARDS, player_A, player_B, max_conflicts, simplified_mode);
    return 0;
}