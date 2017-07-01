#include "utils.hpp"

double TUtils::fatorialStirling (int n)
{
   const double pi = 3.14;
   const double e  = 2.72;

   long result = 1;

   //A aproximação de Stirling funciona apenas para n>6
   if (n==2) result = 2;
   else if (n==3) result = 6;
   else if (n==4) result = 24;
   else if (n==5) result = 120;
   else if (n==6) result = 720;
   else if (n>6)  result = sqrt(2*pi*n)*pow(n/e,n);

   return result;
}

void TUtils::initRnd ()
{
   srand(time(NULL));	
}

int TUtils::rnd(unsigned low, unsigned high)
{
   return rand() % (high-low+1) + low;
}

