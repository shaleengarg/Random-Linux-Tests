#include <iostream>
#include <stdlib.h>

#include <immintrin.h>
#include <emmintrin.h>
#include <xmmintrin.h>
#include <ammintrin.h>

//using namespace std;

static inline void StoreVector(void * d, __m512i const & x) 
{
    _mm512_store_epi64((__m128i *)d, x);
}

struct my{
    int a[3];
    int b[4];
    int c;
};

void print512_num(__m512i var)
{
    int64_t *v64val = (int64_t*) &var;
    printf("%.16lld %.16lld\n", v64val[1], v64val[0]);
}

int main()
{
    //struct my *Hi = (struct my*)malloc(sizeof(struct my));// __attribute__ ((aligned(64)));
    //struct my *newh = (struct my*)malloc(sizeof(struct my));// __attribute__ ((aligned(64)));
    int a[8], b[8];

    a[0] = 0;
    a[1] = 1;
    a[2] = 2;

    b[0] = 3;
    b[1] = 4;
    b[2] = 5;
    b[3] = 6;

    //std::cout << Hi->a[2] << std::endl;


    __m512i A = _mm512_load_epi64((void const*)a); 


    StoreVector(b, A);

    //print512_num(A);

    std::cout << A[2] << std::endl;


    return 0;
}
