#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void printfunction(int a[], int frame_number);

int main()
{
    int page_string[12];
    srand(time(NULL));

    for(int i = 0; i < 12; i++)
    {
        page_string[i] = rand() % 10;    
    }

    printf("Page reference string is:\n");
    printfunction(page_string, 12);

    int page_frame;
    printf("Please enter page frame (1-7): ");
    scanf("%d", &page_frame);
    printf("\n");

    // Initialize frames
    int frame[page_frame];
    for(int i = 0; i < page_frame; i++)
    {
        frame[i] = -1;
    }

    // FIFO
    int fifo_fault = 0;
    int fifo_order = 0;
    int turn = 0;
    while(turn < 12)
    {
        int found = 0;
        for(int i = 0; i < page_frame; i++)
        {
            if(page_string[turn] == frame[i])
            {
                found = 1;
                break;
            }
        }
        if(!found)
        {
            fifo_fault++;
            frame[fifo_order] = page_string[turn];
            fifo_order = (fifo_order + 1) % page_frame;
        }
        printfunction(frame, page_frame);
        turn++;
    }

    printf("\nFIFO page fault = %d\n\n", fifo_fault);

    // LRU
    int frame_LRU[page_frame];
    for(int i = 0; i < page_frame; i++)
    {
        frame_LRU[i] = -1;
    }

    int LRU_fault = 0;
    int turn_LRU = 0;
    int write_position = 0;

    while(turn_LRU < 12)
    {
        int found = 0;
        for(int i = 0; i < page_frame; i++)
        {
            if(page_string[turn_LRU] == frame_LRU[i])
            {
                found = 1;
                for(int k = i; k < write_position - 1; k++)
                {
                    frame_LRU[k] = frame_LRU[k + 1];
                }
                frame_LRU[write_position - 1] = page_string[turn_LRU];
                break;
            }
        }
        if(!found)
        {
            LRU_fault++;
            if(write_position < page_frame)
            {
                frame_LRU[write_position] = page_string[turn_LRU];
                write_position++;
            }
            else
            {
                for(int k = 0; k < page_frame - 1; k++)
                {
                    frame_LRU[k] = frame_LRU[k + 1];
                }
                frame_LRU[page_frame - 1] = page_string[turn_LRU];
            }
        }
        printfunction(frame_LRU, page_frame);
        turn_LRU++;
    }

    printf("\nLRU page fault = %d\n\n", LRU_fault);

    // Optimal
    int frame_optimal[page_frame];
    for(int i = 0; i < page_frame; i++)
    {
        frame_optimal[i] = -1;
    }

    int optimal_fault = 0;
    for(int i = 0; i < 12; i++)
    {
        int found = 0;
        for(int j = 0; j < page_frame; j++)
        {
            if(page_string[i] == frame_optimal[j])
            {
                found = 1;
                break;
            }
        }
        if(!found)
        {
            optimal_fault++;
            if(i < page_frame)
            {
                frame_optimal[i] = page_string[i];
            }
            else
            {
                int farthest = i;
                int index = -1;
                for(int j = 0; j < page_frame; j++)
                {
                    int k;
                    for(k = i + 1; k < 12; k++)
                    {
                        if(frame_optimal[j] == page_string[k])
                        {
                            if(k > farthest)
                            {
                                farthest = k;
                                index = j;
                            }
                            break;
                        }
                    }
                    if(k == 12)
                    {
                        index = j;
                        break;
                    }
                }
                frame_optimal[index] = page_string[i];
            }
        }
        printfunction(frame_optimal, page_frame);
    }

    printf("\nOptimal page fault = %d\n", optimal_fault);

    return 0;
}

void printfunction(int a[], int frame_number)
{
    for(int t = 0; t < frame_number; t++)
    {
        printf("%d ", a[t]);
    }
    printf("\n");
}
