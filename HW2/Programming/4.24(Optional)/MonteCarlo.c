#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h>

int i = 0;
int amtWanted = 0;
int totalPoints = 0;

void *count(void *X)
{
    /* initialize random numbers */
    for (i=0; i < amtWanted; i++)
    {
        double X = (double)rand() / RAND_MAX;
        double Y = (double)rand() / RAND_MAX;

        if (((X * X) + (Y * Y)) <= 1)
        {
            totalPoints++;
        }
    }
    return NULL;
}

int main()
{
    printf("\nMultiThreaded Monte Carlo\n");
    srand(time(NULL));
    pthread_t thread;

    do
        {
        printf("Please enter a positive number for the amount of points you would like to generate: ");
        scanf("%d", &amtWanted);
        }while (amtWanted <= 0);

    pthread_create(&thread, NULL, &count, NULL);
    pthread_join(thread, NULL);

    double points = 4.0 * totalPoints;
    double pi = points / amtWanted;
    printf("\nThe approximate value of pi for the desired amount of points (%d) is: %f \n\n", amtWanted, pi);
    return 0;
}
