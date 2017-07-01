
#include <random>
#include <chrono>

using namespace std;

#ifndef _UTIL_H
#define	_UTIL_H

class TUtils
{
   public:

      /************************************************************************************
      No lugar de utilizar uma função de n multiplcações em um algorítmo genético,
      será utilizado a aproximação de Stirling para agilizar o algoritmo, pois
      neste caso a aproximação será satisfatóra e utilizando pouco esforço computacional.
      ************************************************************************************/
      static double fatorialStirling (int n);

      static void initRnd ();
      static int rnd(unsigned low, unsigned high);
};

#endif	/* _UTIL_H */
