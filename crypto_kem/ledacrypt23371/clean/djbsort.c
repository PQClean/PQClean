#include <stdint.h>
#define int32 int32_t


#define int32_MINMAX(a,b) \
do { \
  int32 ab = b ^ a; \
  int32 c = b - a; \
  c ^= ab & (c ^ b); \
  c >>= 31; \
  c &= ab; \
  a ^= c; \
  b ^= c; \
} while(0)

void PQCLEAN_LEDACRYPT23371_CLEAN_int32_sort(int32 *x, long long n);

void PQCLEAN_LEDACRYPT23371_CLEAN_int32_sort(int32 *x, long long n)
{
   long long top,p,q,r,i,j;

   if (n < 2) return;
   top = 1;
   while (top < n - top) top += top;

   for (p = top; p >= 1; p >>= 1) {
      i = 0;
      while (i + 2 * p <= n) {
         for (j = i; j < i + p; ++j)
            int32_MINMAX(x[j],x[j+p]);
         i += 2 * p;
      }
      for (j = i; j < n - p; ++j)
         int32_MINMAX(x[j],x[j+p]);

      i = 0;
      j = 0;
      for (q = top; q > p; q >>= 1) {
         if (j != i) for (;;) {
               if (j == n - q) goto done;
               int32 a = x[j + p];
               for (r = q; r > p; r >>= 1)
                  int32_MINMAX(a,x[j + r]);
               x[j + p] = a;
               ++j;
               if (j == i + p) {
                  i += 2 * p;
                  break;
               }
            }
         while (i + p <= n - q) {
            for (j = i; j < i + p; ++j) {
               int32 a = x[j + p];
               for (r = q; r > p; r >>= 1)
                  int32_MINMAX(a,x[j+r]);
               x[j + p] = a;
            }
            i += 2 * p;
         }
         /* now i + p > n - q */
         j = i;
         while (j < n - q) {
            int32 a = x[j + p];
            for (r = q; r > p; r >>= 1)
               int32_MINMAX(a,x[j+r]);
            x[j + p] = a;
            ++j;
         }

done:
         ;
      }
   }
}

