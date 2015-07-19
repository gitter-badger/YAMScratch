#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/*
              state 3
            {Alan alive}
               /     1-pB
           pA /    /--<--\
             /    /       \
     state 1 -----         --- state 2
    {Alan's turn} \        /     {Bob's turn}
                   \-->---         /
                     1-pA         /
                                 /
                                state 4
                                {Bob alive}

    |  0    1-pA  pA   0 |  
M = | 1-pB    0    0  pB |
    |  0      0    1   0 |
    |  0      0    0   1 |

we find the determinate of 
M - (lambda)I such that it is zero

    |  0    1-pA  pA   0 |  
det(| 1-pB    0    0  pB |) =0
    |  0      0    1   0 |
    |  0      0    0   1 |

from the characteristic polynomial we find 
l_1 = 1
l_2 = 1
l_3 = -sqrt(pA-1)sqrt(pB-1)
l_4 = sqrt(pA-1)sqrt(pB-1)

(these are column vectors)
    v_1 = {((-1 + x) y)/(-x - y + x y); -(y/(-x - y + x y)); 0; 1}
    v_2 = {-(x/(-x - y + x y)); (x (-1 + y))/(-x - y + x y); 1; 0}
    v_3 = {Sqrt[-1 + x]/Sqrt[-1 + y]; 1; 0; 0}
    v_4 = {-(Sqrt[-1 + x]/Sqrt[-1 + y]); 1; 0; 0}

we assemble these into full block matrix and find 

V = [v_1 v_2 v_3 v_4 ]

V = {{((-1 + x) y)/(-x - y + x y), -(x/(-x - y + x y)), Sqrt[-1 + x]/Sqrt[-1 + y], -(Sqrt[-1 + x]/Sqrt[-1 + y])},
     {-(y/(-x - y + x y)), (x (-1 + y))/(-x - y + x y), 1, 1},
     {0, 1, 0, 0},
     {1, 0, 0, 0}}

V^-1 = {{0, 0, 0, 1},
        {0, 0, 1, 0},
        {Sqrt[-1 + y]/(2 Sqrt[-1 + x]), 1/2, -(x (1 + Sqrt[-1 + y]/Sqrt[-1 + x] - y))/(2 (x + y - x y)), ((Sqrt[-1 + x] + Sqrt[-1 + y] - x Sqrt[-1 + y]) y)/(2 Sqrt[-1 + x] (x (-1 + y) - y))},
        {-Sqrt[-1 + y]/(2 Sqrt[-1 + x]), 1/2, -(x (1 - Sqrt[-1 + y]/Sqrt[-1 + x] - y))/(2 (x + y - x y)), ((Sqrt[-1 + x] - Sqrt[-1 + y] + x Sqrt[-1 + y]) y)/(2 Sqrt[-1 + x] (x (-1 + y) - y))}}

            | l_1  0   0    0|
P * V = V * |  0  l_2  0    0|
            |  0   0  l_3   0|
            |  0   0   0  l_4|

        | l_1  0   0    0|
P = V * |  0  l_2  0    0| * V^(-1)
        |  0   0  l_3   0|
        |  0   0   0  l_4|
 
and solving for the limit of P^N as N-> infinity,
we find steady state
which we can then multiply by the intial state vector to find final distribution

and we find that the chances of Alan surving, state 3
lim           pA *(((1-pA)(1-pB))^n - 1)
n->infinity   ---------------------------------
                (pA*pB -pA - pB)

where both 1-pA and 1-pB is less than one, so the expression simplifies

alan survives =  -pA / (pA*pB -pA - pB)

*/

int main(int argc, char const *argv[])
{
    /*compute state of markov process*/
    unsigned long N;
    int rc;
    errno = 0;
    rc = scanf(" %lu ", &N);
    if(rc != 1) {
        perror("failed to read input");
        exit(-1);
    }
    unsigned ii, pA, pB;
    double x, y;

    for(ii = 0; ii < N; ++ ii) {
        errno = 0;
        rc = scanf(" %u %u", &pA, &pB);
        if(rc != 2 || errno != 0) {
            fprintf(stderr, "input read failed on line %u\n", ii+2);
            fflush(stderr);
            perror("");
            exit(-1);
        }
        x = (double)pA / 100.0;
        y = (double)pB / 100.0;
        printf("%d ", (int)((100.0 * (-x /(x*y -x -y)))+ 0.5));
    }
    printf("\n");
    return 0;
}
