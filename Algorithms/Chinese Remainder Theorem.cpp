#include <iostream>
using namespace std;
typedef long long int lli;

lli power_mod(lli x, lli y, lli k){
    if(y == 0){
        return 1;
    }
    else{
        lli temp = power_mod(x, y/2, k);

        return ((temp * temp) % k * (y & 1 ? x : 1)) % k;
    }
}

lli totient(lli x){
    lli res = x;
    for(lli i = 2; i * i <= x; i++){
        if(x % i == 0){
            res = res/i * (i - 1);

            while(x % i == 0){
                x /= i;
            }
        }
    }
    if(x != 1){
        res = res/x * (x - 1);
    }

    return res;
}

lli mod_inv(lli x, lli k){
    return power_mod(x, totient(k) - 1, k) % k;
}

lli GaussCRT(lli a[], lli p[], lli k) {
   lli P = 1;
   for (lli i = 0; i < k; i++)
      P *= p[i];

   lli res = 0;
   for (lli i = 0; i < k; i++) {
      lli yi = P/p[i];
      res += a[i] * mod_inv(yi, p[i]) * yi;
   }
   return res % P;
}

lli GarnerCRT(lli a[], lli p[], lli k) {
   lli res = 0, cumulative_product = 1;
   lli x[k];

   for(lli i = 0; i < k; i++){
      x[i] = a[i];

      for(lli j = 0; j < i; j++){
         x[i] = mod_inv(p[j], p[i]) * (x[i] - x[j]);

         x[i] = x[i] % p[i];
         if(x[i] < 0)
            x[i] += p[i];
      }

      res += x[i] * cumulative_product;
      cumulative_product *= p[i];
   }

   return res % cumulative_product;
}

int main(){
    lli N;
    lli a[1000];
    lli p[1000];

    cin >> N;
    for(lli i = 0; i < N; i++){
        cin >> a[i] >> p[i];
    }

    lli gauss = GaussCRT(a, p, N);
    lli garner = GarnerCRT(a, p, N);

    cout << gauss << ' ' << garner << '\n';
    return 0;
}
