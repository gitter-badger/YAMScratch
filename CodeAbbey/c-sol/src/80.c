#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/*
              state C
            {alan alive}
1 - pA - pB    /
 +-----+      / pA
 |     |     /
 +-> state A
    {both alive}
            \
             \ pB
              \
              state B
              {bob alive}

    | (1-pA-pB) pB  pA |  
M = |    0      1    0 |
    |    0      0    1 |

we find the determinate of 
M - (lambda)I such that it is zero

    | (1-pA-pB) - l   0    0  |
det(|      pB        1-l   0  |) = 0
    |      pA         0   1-l |

from the characteristic polynomial we find 
l_1 = 1
l_2 = 1
l_3 = 1-pB-pA

we then find the right eigenvectors and find
      | pB/(pA+pB)|        | pA/(pA+pB)|        | 1 |
v_1 = |     0     |  v_2 = |     1     |  v_3 = | 0 |
      |     1     |        |     0     |        | 0 |

we assemble these into full block matrix and find 

V = [v_1 v_2 v_3 ]

            | l_1  0   0  |
P * V = V * |  0  l_2  0  |
            |  0   0  l_3 |

        | l_1  0   0  |
P = V * |  0  l_2  0  | * V^(-1)
        |  0   0  l_3 |

and solving for the limit of P^N as N-> infinity,
we find steady state is
    | 0  pB/(pA+pB) pA/(pA+pB) |
E = | 0     1           0      |
    | 0     0           1      |

which we can then multiply by the intial state vector to find final distribution

| 0  pB/(pA+pB) pA/(pA+pB) |   | 1 |
| 0     1           0      | * | 0 |
| 0     0           1      |   | 0 |

and we find that the chances of Alan surving, state 3, is given by E_13, or pA/(pA+pB)

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

    for(ii = 0; ii < N; ++ ii) {
        errno = 0;
        rc = scanf(" %u %u", &pA, &pB);
        if(rc != 2 || errno != 0) {
            fprintf(stderr, "input read failed on line %u\n", ii+2);
            fflush(stderr);
            perror("");
            exit(-1);
        }
        printf("%d ", (int)( 100.0 * ((double)pA / (double)(pA+pB)) + 0.5));
    }
    printf("\n");
    return 0;
}
