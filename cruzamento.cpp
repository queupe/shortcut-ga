/*
*  cruzamento.cpp
*
*  Módulo responsável pelas reotinas de cruzamento
*  Autor: Geraldo José Ferreira Chagas Junior - gjr.doc@gmail.com
*
*  PPGI - NCE - UFRJ
*  Data Criação: 19/05/2017
*  Datas de Modificações:
*
*/

#include "cruzamento.hpp"
#include <iostream>

//Apenas letura
TMapaGenes *TCruzamento::getMapa() { return VP_Mapa; }
TArqLog *TCruzamento::getArqLog()  { return VP_ArqSaida; }

TCruzamento::TCruzamento (TMapaGenes *mapa, TArqLog *arqSaida, int tipoCruzamento)
{
   VP_Mapa = mapa;
   VP_ArqSaida = arqSaida;
   VP_cruzamento = tipoCruzamento;
}

vector <TIndividuo *>TCruzamento::processa (TIndividuo *parceiro1, TIndividuo *parceiro2)
{
   switch (VP_cruzamento)
   {
      case 0:
      {
         return GSTX (parceiro1, parceiro2);
         break;
      }
      case 1:
      {
         return PMX(parceiro1, parceiro2);
         break;
      }
      case 2:
      {
			return OX1(parceiro1, parceiro2);
         break;
      }
      case 3:
      {
         break;
      }
      default:
      {
         break;
      }
   }

   vector <TIndividuo *> filhos;
   filhos.push_back(parceiro1->clona());
   filhos.push_back(parceiro2->clona());
   return filhos;
}

/**
 *
 * Edge Recombination Crossover (ER)
 *
 * Nguyen, Yoshihara, Yamamori & Yasunaga (2002). Greedy genetic algorithms
 * for symmetric and asymmetric TSPs. IPSJ Trans. Mathematical Modeling and
 * Its Applications, 43, 165-175.
 *
 * Sengoku & Yoshihara (1998). A fast TSP solver using GA on JAVA. In Third
 * International Symposium on Artificial Life, and Robotics (AROB III’98)
 * (pp. 283-288).
 *
 **/
vector <TIndividuo *>TCruzamento::GSTX(TIndividuo *parceiro1, TIndividuo *parceiro2)
{
   TGene *gPar1;
   TGene *gPar2;

   vector<TGene *> temp;
   vector<bool> controle (parceiro1->get_qtdeGenes(), true);

   TIndividuo *filho = new TIndividuo(VP_Mapa, VP_ArqSaida);

   int pivo =  TUtils::rnd(1, parceiro1->get_qtdeGenes()-1);
   gPar1 = parceiro1->get_por_indice(pivo);
   gPar2 = parceiro2->get_por_id(gPar1->id);

   temp.push_back (gPar1);
   controle[gPar1->id] = false;

   bool dir = true;
   bool esq = true;
   int i=1;

   while (i<parceiro1->get_qtdeGenes())
   {
      if (esq)
      {
         if (gPar1->id==0) esq = false;
         else
         {
            gPar1 = parceiro1->ant(gPar1);
            if (controle[gPar1->id])
            {
               temp.insert(temp.begin(),gPar1);
               controle[gPar1->id] = false;
               i++;
            }
         }
      }

      if (dir)
      {
         if (gPar2->prox->id==0) dir = false;
         else
         {
            gPar2 = parceiro2->prox(gPar2);
            if (controle[gPar2->id])
            {
               temp.push_back (gPar2);
               controle[gPar2->id] = false;
               i++;
            }
         }
      }

      if ((!esq)&&(esq==dir))
      {
         gPar1 = parceiro1->ant(gPar1);
         if (controle[gPar1->id])
         {
            temp.push_back (gPar1);
            controle[gPar1->id] = false;
            i++;
         }

         gPar2 = parceiro2->prox(gPar2->id);
         if (controle[gPar2->id])
         {
            temp.push_back (gPar2);
            controle[gPar2->id] = false;
            i++;
         }
      }
   }

   filho->novo(temp);
   controle.clear();
   temp.clear();

   vector <TIndividuo *> filhos;
   filhos.push_back(filho);
   return filhos;
}

/**
 *
 * Partially-Mapped Crossover (PMX)
 *
 * Goldberg and Lingle (1985). Alleles, Loci and the TSP. In Grefenstette,
 * J. J. (ed.) Proceedings of the First International Conference on Genetic
 * Algorithms and Their Applications, 154–159. Hillsdale, New Jersey:
 * Lawrence Erlbaum.
 *
 **/
vector <TIndividuo *>TCruzamento::PMX(TIndividuo *parceiro1, TIndividuo *parceiro2)
{
   int k;
   int uInicio, uFin;
   TGene *aux1;
   TGene *aux2;
   TIndividuo *filho1 = parceiro1->clona();
   TIndividuo *filho2 = parceiro2->clona();

   uInicio = TUtils::rnd(1, filho1->get_qtdeGenes()-2);
   uFin = TUtils::rnd(uInicio+1, filho1->get_qtdeGenes()-1);
  
   //O algorítimo está diferente do contreras, porém, ele foi
   //entendi e modificado de forma a ficar mais simples
   for(k=uInicio; k<=uFin; k++)
   {
      aux1 = filho1->get_por_indice(k);
      aux2 = filho2->get_por_indice(k);
      
      filho1->troca(aux1->id, aux2->id);
      filho2->troca(aux2->id, aux1->id);
   }
   vector <TIndividuo *> filhos;
   filhos.push_back(filho1);
   filhos.push_back(filho2);
   return filhos;
}

/**
 *
 * Order Crossover (OX1)
 *
 * Davis (1985). Applying Adaptive Algorithms to Epistatic Domains.
 * Proceedings of the International Joint Conference on Artiﬁcial Intelligence, 162–164
 *
 **/
vector <TIndividuo *>TCruzamento::OX1(TIndividuo *parceiro1, TIndividuo *parceiro2)
{
	vector <TIndividuo *> filhos;
	TIndividuo *filho1 = parceiro1->clona();
   TIndividuo *filho2 = parceiro2->clona();

   //Para executar esse cruzamento a quantdade de genes tem 
	// que ser maior que 3.
	if (parceiro1->get_qtdeGenes()<=3)
	{
		filhos.push_back(filho1);
		filhos.push_back(filho2);
		return filhos;
	}
	
	int uInicio, uFin;
	uInicio = TUtils::rnd(1, filho1->get_qtdeGenes()-3);
	uFin = TUtils::rnd(uInicio+1, filho1->get_qtdeGenes()-2);

	//  vector <unsigned> aux = child1;
	TGene *it1;
	TGene *it2;
	int j = uFin;
	int fin_i = filho1->get_qtdeGenes();
	int fin_j = filho2->get_qtdeGenes();
	
	//Filho1
	for(int i = uFin+1; i < fin_i; i++)
	{
		while(j < fin_j)
		{
			j++;
         if(j >= parceiro2->get_qtdeGenes())
         {
				j = 1;
				fin_j = uFin;
         }
			
			it2 = parceiro2->get_por_indice(j);
			it1 = filho1->get_por_id(it2->id);
			if(it1->i<uInicio || it1->i>uFin) //Não está entre uInicio e uFin
         {
				filho1->troca_indice(i, it1->i);
				break;
         }
		}

      if(i >= filho1->get_qtdeGenes()-1)
      {
          i = 0;
          fin_i = uInicio;
      }
	}

	//Filho2
	j = uFin;
	fin_i = filho2->get_qtdeGenes();
	fin_j = filho1->get_qtdeGenes();
	for(int i = uFin+1; i < fin_i; i++)
	{
		while(j < fin_j)
		{
			j++;
         if(j >= parceiro1->get_qtdeGenes())
         {
				j = 1;
				fin_j = uFin;
         }
			
			it1 = parceiro1->get_por_indice(j);
			it2 = filho2->get_por_id(it1->id);
			if(it2->i<uInicio || it2->i>uFin) //Não está entre uInicio e uFin
         {
				filho2->troca_indice(i, it2->i);
				break;
         }
		}

      if(i >= filho2->get_qtdeGenes()-1)
      {
          i = 0;
          fin_i = uInicio;
      }
	}
	
   filhos.push_back(filho1);
   filhos.push_back(filho2);
	
	return filhos;
}

/**
 *
 * Order Based Crossover (OX2)
 *
 * Syswerda (1991). Schedule Optimization Using Genetic Algorithms. In Davis, L. (ed.)
 * Handbook of Genetic Algorithms, 332–349. New York: Van Nostrand Reinhold.
 *
 **/
bool Crossover::OX2(vector <unsigned> &child1, vector <unsigned> child2)
{
  vector<unsigned> child1Aux = child1;
  vector<unsigned> child2Aux = child2;

  // child1.swap(child2);
  int j = 1, k = child1.size() * 0.4;


  //(ciudad, posición)
  map<int, int> position;
  map<int, int> positionInv;
  int found = rnd(0, sizeChrm - 1);
  position.insert(pair<int, int>(child1[found], found));
  positionInv.insert(pair<int, int>(found, child1[found]));
  do {
    found = rnd(0, sizeChrm - 1);
    if(position.find(child1[found]) == position.end())
      {
        position.insert(pair<int, int>(child1[found], found));
        positionInv.insert(pair<int, int>(found, child1[found]));
        j++;
      }
  }
  while(j < k);

  map<int,int>::iterator itC1 = positionInv.begin();
  for(j = 0; j < child2.size(); j++)
    if((position.find(child2Aux[j])) != position.end())
      {
        child1[itC1->first] = child2Aux[j];
        child2[j] = child1Aux[itC1->first];
        itC1++;
      }

  return true;
}


/**
 *
 * Modified Order Based Crossover (MOX)
 *
 * New Operators of Genetic Algorithms for Traveling Salesman Problem
 *
 **/
bool Crossover::MOX(vector <unsigned> &child1, vector <unsigned> child2)
{


  vector<unsigned> child1Aux = child1;
  vector<unsigned> child2Aux = child2;
  int point = rnd(1, sizeChrm - 2)+1;

  vector<unsigned> position1(child1Aux.begin(), child1Aux.begin() + point);

  vector<unsigned> position2(child2Aux.begin(), child2Aux.begin() + point);

  for(int j = 0, k = point; j < child1.size(); j++)
    if(find(position2.begin(), position2.end(), child1Aux[j]) == position2.end())
      {
        child1[j] = child2Aux[k];
        k++;
      }

  for(int j = 0, k = point; j < child2.size(); j++)
    if(find(position1.begin(), position1.end(), child2Aux[j]) == position1.end())
      {

        child2[j] = child1Aux[k];
        k++;
      }


  return true;
}


/**
 *
 * Position Based Crossover (POS)
 *
 * Syswerda (1991). Schedule Optimization Using Genetic Algorithms. In Davis, L. (ed.)
 * Handbook of Genetic Algorithms, 332–349. New York: Van Nostrand Reinhold.
 *
 **/
bool Crossover::POS(vector <unsigned> &child1, vector <unsigned> child2)
{
  vector<unsigned> child1Aux = child1;
  vector<unsigned> child2Aux = child2;

  int j = 1, k = child1.size() * 0.4;

  //(ciudad, posición)
  map<int, int> position;
  map<int, int> positionInv;
  int found = rnd(0, sizeChrm - 1);
  position.insert(pair<int, int>(child1[found], found));
  positionInv.insert(pair<int, int>(found, child1[found]));
  do {
    found = rnd(0, sizeChrm - 1);
    if(position.find(child1[found]) == position.end())
      {
        position.insert(pair<int, int>(child1[found], found));
        positionInv.insert(pair<int, int>(found, child1[found]));
        j++;
      }
  }
  while(j < k);

  vector<int> cambio;

  for(map<int,int>::iterator ita=positionInv.begin(); ita!=positionInv.end(); ++ita)
    {
      child1[ita->first] = child2Aux[ita->first];
      child2[ita->first] = child1Aux[ita->first];
      cambio.push_back(child1[ita->first]);
    }

  //para que no se caíga con los itC1++
  positionInv.insert(pair<int, int>(sizeChrm+10, sizeChrm+10));
  map<int,int>::iterator itC1 = positionInv.begin();
  int i = 0;
  for(j = 0; j < child1.size(); j++)
    if(find(cambio.begin(), cambio.end(), child1Aux[j]) == cambio.end())
      {
        if(i == itC1->first)
          {
            i++;
            itC1++;
          }
        while(i == itC1->first)
          {
            i++;
            itC1++;
          }
        child1[i] = child1Aux[j];
        i++;
      }

  itC1 = positionInv.begin();
  i = 0;

  for(j = 0; j < child1.size(); j++)
    if(position.find(child2Aux[j]) == position.end())
      {

        if(i == itC1->first)
          {
            i++;
            itC1++;
          }
        while(i == itC1->first)
          {
            i++;
            itC1++;
          }

        child2[i] = child2Aux[j];
        i++;
      }

  return true;
}

/**
 *
 * Cycle Crossover (CX)
 *
 * Oliver, Smith and Holland (1987). A Study of Permutation Crossover
 * Operators on the TSP. In Grefenstette, J. J. (ed.) Genetic Algorithms and
 * Their Applications: Proceedings of the Second International Conference,
 * 224–230. Hillsdale, New Jersey: Lawrence Erlbaum.
 *
 **/
bool Crossover::CX(vector <unsigned> &child1, vector <unsigned> &child2)
{
  int i,j, aux;
  vector <unsigned> ciclo;
  vector <unsigned> position;
  vector <unsigned> child(sizeChrm, child1.size());
  vector<unsigned>::iterator it;
  i = 0;
  position.push_back(0);

  while(true)
    {
      j = find(child1.begin(), child1.end(), child2[i]) - child1.begin();
      position.push_back(j);
      if(child1[0] == child2[j])
        {
          break;
        }
      i = j;
    }

  for(int k = 0; k < position.size(); k++)
    {
      aux = child1[position[k]];
      child1[position[k]] = child2[position[k]];
      child2[position[k]] = aux;
    }
  return true;
}

/**
 *
 * Distance Preserving Crossover (DPX)
 *
 * Reisleben, Freisleben & Merz (1996). A genetic local search algorithm for
 * solving symmetric and asymmetric traveling salesman problems. Proceedings
 * of IEEE International Conference on Evolutionary Computation,
 * 1996. (pp. 616–621).
 *
 **/
bool Crossover::DPX(vector <unsigned> &child1, vector <unsigned> child2, TSP *tsp1)
{
  vector<vector<unsigned> > fragmento;

  // for(int i = 0; i < child1.size(); i++)
  //   cout << child1[i] << " ";
  // cout << endl;

  // for(int i = 0; i < child2.size(); i++)
  //   cout << child2[i] << " ";
  // cout << endl;
  for(int i = 0; i < child1.size(); i++)
    {
      unsigned found = find(child2.begin(), child2.end(), child1[i]) - child2.begin();
      // cout << found << endl;
      vector<unsigned> v;
      for(int j = found, k = 0; j < child2.size() && i < child1.size(); j++, i++)
      	{
      	  if(child2[j] == child1[i])
      	    {
              // cout << "\t" << child2[j] << endl;
      	      v.push_back(child2[j]);
      	      k++;
      	    }
          else
            break;
        }
      // cout << "\t\tla"<< endl;
      i--;
      fragmento.push_back(v);
    }

  if(fragmento[0].size() == child1.size())
    return true;

  // for(int i = 0; i < fragmento.size(); i++)
  //   {
  //     for(int j = 0; j < fragmento[i].size(); j++)
  // 	cout << fragmento[i][j] << " ";
  //     cout << " - ";
  //   }
  // cout << endl;

  unsigned frag = rnd(0, fragmento.size() - 1);
  unsigned begin;
  if(fragmento[frag].size() == 1)
    begin =  0;
  else
    begin =  rnd(0, 1) ? fragmento[frag].size() - 1 : 0;


  vector<unsigned> childAux;

  if(begin == 0)
    for(int i = 0; i < fragmento[frag].size(); i++)
      childAux.push_back(fragmento[frag][i]);
  else
    for(int i = begin; i >= 0; i--)
      childAux.push_back(fragmento[frag][i]);

  int k = fragmento[frag].size() - 1;

  // cout << fragmento[frag][k] << endl;
  fragmento[frag].clear();
  fragmento.erase(fragmento.begin() + frag);

  while(fragmento.size() > 0)
    {
      int min = Infinite, costo;

      // cout << "\t " << k << endl;
      for(int i = 0; i < fragmento.size(); i++)
        {
          int posicion = 0;
          if(fragmento[i].size() == 1)
            {
              // cout << "k = " << k << endl;
              // cout << "ChA.size = " << childAux.size() << endl;
              // cout << "ChA = " << childAux[k] << endl;
              costo = tsp1->distancia2Ciudades(childAux[k], fragmento[i][0]);
            }
          else if(fragmento[i].size() > 1)
            {
              int last = fragmento[i].size() - 1;
              int costo1 = tsp1->distancia2Ciudades(childAux[k], fragmento[i][0]);
              int costo2 = tsp1->distancia2Ciudades(childAux[k], fragmento[i][last]);
              if(costo1 < costo2)
                costo = costo1;
              else
                {
                  costo = costo2;
                  posicion = fragmento[i].size() - 1;
                }
            }
          else
            exit(0);
          if(costo < min)
            {
              min = costo;
              frag = i;
              begin = posicion;
            }
        }
      // cout << "\t\t"<< begin << " "<< fragmento[frag].size() << " "<< fragmento[frag][begin] << endl;
      // cout <<"\t\t"<< fragmento[frag].size() << endl;
      if(begin == 0)
        for(int i = 0; i < fragmento[frag].size(); i++)
          childAux.push_back(fragmento[frag][i]);
      else
        for(int i = begin; i >= 0; i--)
          {
            childAux.push_back(fragmento[frag][i]);
            // cout << fragmento[frag][i] << endl;
          }
      k += fragmento[frag].size();

      fragmento[frag].clear();
      fragmento.erase(fragmento.begin() + frag);
    }

  // cout << endl;
  // for(int i = 0; i < childAux.size(); i++)
  //   cout << childAux[i] << " ";
  // cout << endl;
  // exit(0);
  // child1.clear();
  child1 = childAux;

  return true;
}

/**
 *
 * Alternating-position Crossover (AP)
 *
 * Larrañaga, Kuijpers, Poza & Murga (1997). Decomposing Bayesian networks:
 * triangulation of the moral graph with genetic algorithms. Statistics and
 * Computing 7, 1 (January 1997), 19-34.
 *
 **/
bool Crossover::AP(vector <unsigned> &child1, vector <unsigned> &child2)
{
  if(child1 == child2)
    return true;

  vector<unsigned> child1Aux = child1;
  vector<unsigned> child2Aux = child2;

  child1.clear();
  child2.clear();
  //child1
  child1.push_back(child1Aux[0]);
  for(int i = 1, j = 0, k = 1; k < 2*child1Aux.size(); k++)
    {
      if(k%2 == 0)
        {
          int found = child1Aux[i];
          if(find(child1.begin(), child1.end(), found) == child1.end())
            child1.push_back(found);
          i++;
        }
      else
        {
          int found = child2Aux[j];
          if(find(child1.begin(), child1.end(), found) == child1.end())
            child1.push_back(found);
          j++;
        }
    }
  //child2
  child2.push_back(child2Aux[0]);
  for(int i = 1, j = 0, k = 1; k < 2*child2Aux.size(); k++)
    {
      if(k%2 == 0)
        {
          int found = child2Aux[i];
          if(find(child2.begin(), child2.end(), found) == child2.end())
            child2.push_back(found);
          i++;
        }
      else
        {
          int found = child1Aux[j];
          if(find(child2.begin(), child2.end(), found) == child2.end())
            child2.push_back(found);
          j++;
        }
    }

  return true;
}

/**
 *
 * Maximal preservative crossover (MPX)
 *
 * Mühlenbein, Gorges-Schleuter & Krämer (1988). Evolution algorithms in
 * combinatorial optimization. Parallel Computing, 7(1), 65–85.
 *
 * Mühlenbein (1991). Parallel genetic algorithms, population genetics and
 * combinatorial optimization. In J. D. Becker, I. Eisele, & F. W. Mündemann
 * (Eds.), Parallelism, Learning, Evolution (Vol. 565, pp. 398–406). Springer
 * Berlin Heidelberg.
 *
 **/
bool Crossover::MPX(vector <unsigned> &child1, vector <unsigned> child2)
{
  int begin, end;
  int Lmin, Lmax;

  if(child1.size() <= 30)
    {
      Lmax = child1.size()/2;
      Lmin = rnd(1, Lmax-1);
    }
  else
    {
      Lmax = child1.size()/2;
      Lmin = 10;
    }

  do {
    begin = rnd(0, sizeChrm - 2);
    end = rnd(begin + 1, sizeChrm - 1);
  }
  while((end - begin < Lmin) || (end - begin > Lmax));


  vector<unsigned> child1Aux = child1;
  vector<unsigned> child2Aux = child2;

  // Primer Hijo
  child1.swap(child2);

  for(int i = 0; i < child1.size(); i++)
    {
      if(find(child1Aux.begin() + begin, child1Aux.begin() + end, child1[i]) != (child1Aux.begin() + end))
        {
          child1.erase(child1.begin() + i);
          i--;
        }
    }
  child1.insert(child1.begin(), child1Aux.begin() + begin, child1Aux.begin() + end);


  //Segundo Hijo
  child2.swap(child1Aux);
  for(int i = 0; i < child2.size(); i++)
    {
      if(find(child2Aux.begin() + begin, child2Aux.begin() + end, child2[i]) != (child2Aux.begin() + end))
        {
          child2.erase(child2.begin() + i);
          i--;
        }
    }
  child2.insert(child2.begin(), child2Aux.begin() + begin, child2Aux.begin() + end);

  return true;

}

/**
 *
 * Heuristic crossover (HX)
 *
 * Grefenstette, Gopal, Rosmaita & Van Gucht (1985). Genetic Algorithms for
 * the TSP. In Grefenstette, J. J. (ed.) Proceedings of the First
 * International Conference on Genetic Algorithms and Their Applications,
 * 160–165. Hillsdale, New Jersey: Lawrence Erlbaum.
 *
 **/
bool Crossover::HX(vector <unsigned> &child1, vector <unsigned> child2, TSP *tsp1)
{
  int i = rnd(0, sizeChrm - 1);
  int found = child1[i];
  int Imas1, Imenos1, Jmas1, Jmenos1, j;
  vector<unsigned> child1Aux = child1;
  vector<unsigned> child2Aux = child2;
  vector<int> d(4);

  set <int> city(child1.begin(), child1.end());
  child1.clear();
  child1.push_back(child1Aux[i]);
  city.erase(city.find(found));

  while(child1.size() != child1Aux.size() - 1)
    {
      d[0] = d[1] = d[2] = d[3] = Infinite;
      Imas1 = (i == child1Aux.size() - 1) ? 0 : i + 1;
      Imenos1 = (i == 0) ? child1Aux.size() - 1: i - 1;

      if(child1.size() < (child1Aux.size() / 2))
        {
          if(child1.size() == 1 || find(child1.begin(), child1.end(), child1Aux[Imas1]) == child1.end())
            d[0] = tsp1->distancia2Ciudades(found, child1Aux[Imas1]);

          if(child1.size() == 1 || find(child1.begin(), child1.end(), child1Aux[Imenos1]) == child1.end())
            d[1] = tsp1->distancia2Ciudades(found, child1Aux[Imenos1]);

          j = find(child2Aux.begin(), child2Aux.end(), found) - child2Aux.begin();
          Jmas1 = (j == child2Aux.size() - 1) ? 0 : j + 1;
          Jmenos1 = (j == 0) ? child2Aux.size() - 1: j - 1;

          if(find(child1.begin(), child1.end(), child2Aux[Jmas1]) == child1.end())
            if(child2Aux[Jmas1] != child1Aux[Imas1] && child2Aux[Jmas1] != child1Aux[Imenos1])
              d[2] = tsp1->distancia2Ciudades(child2Aux[j], child2Aux[Jmas1]);

          if(find(child1.begin(), child1.end(), child2Aux[Jmenos1]) == child1.end())
            if(child2Aux[Jmenos1] != child1Aux[Imas1] && child2Aux[Jmenos1] != child1Aux[Imenos1])
              d[3] = tsp1->distancia2Ciudades(child2Aux[j], child2Aux[Jmenos1]);
        }
      else
        {
          if(child1.size() == 1 || city.find(child1Aux[Imas1]) != city.end())
            d[0] = tsp1->distancia2Ciudades(found, child1Aux[Imas1]);

          if(child1.size() == 1 || city.find(child1Aux[Imenos1]) != city.end())
            d[1] = tsp1->distancia2Ciudades(found, child1Aux[Imenos1]);

          j = find(child2Aux.begin(), child2Aux.end(), found) - child2Aux.begin();
          Jmas1 = (j == child2Aux.size() - 1) ? 0 : j + 1;
          Jmenos1 = (j == 0) ? child2Aux.size() - 1: j - 1;

          if(city.find(child2Aux[Jmas1]) != city.end())
            if(child2Aux[Jmas1] != child1Aux[Imas1] && child2Aux[Jmas1] != child1Aux[Imenos1])
              d[2] = tsp1->distancia2Ciudades(child2Aux[j], child2Aux[Jmas1]);

          if(city.find(child2Aux[Jmenos1]) != city.end())
            if(child2Aux[Jmenos1] != child1Aux[Imas1] && child2Aux[Jmenos1] != child1Aux[Imenos1])
              d[3] = tsp1->distancia2Ciudades(child2Aux[j], child2Aux[Jmenos1]);
        }
      int indice = 0, ind;
      int min = d[0];
      for(int k = 1; k < 4; k++)
        {
          if(min > d[k])
            {
              min = d[k];
              indice = k;
            }
        }

      if(indice == 0)
        ind = Imas1;
      else if(indice == 1)
        ind = Imenos1;
      else if(indice == 2)
        ind = Jmas1;
      else
        ind = Jmenos1;

      if(min == Infinite)
      	{
          int rand = rnd(1, city.size());
          int value;
          set<int>::iterator it = city.begin();
          for(int k = 0; k < rand; k++, ++it)
            value = *it;

          child1.push_back(value);
          city.erase(city.find(value));
        }
      else if(indice == 0 || indice == 1)
        {
          child1.push_back(child1Aux[ind]);
          city.erase(city.find(child1Aux[ind]));
        }
      else
        {
          child1.push_back(child2Aux[ind]);
          city.erase(city.find(child2Aux[ind]));
        }

      found = child1[child1.size() - 1];
      i = find(child1Aux.begin(), child1Aux.end(), found) - child1Aux.begin();
    }

  child1.push_back(*city.begin());

  return true;
}

/**
 *
 * Inver-over operator (IO)
 *
 * Tao & Michalewicz (1998). Inver-over operator for the TSP. In A. Eiben,
 * T. Bäck, M. Schoenauer, & H.-P. Schwefel (Eds.), Parallel Problem Solving
 * from Nature — PPSN V (Vol. 1498, pp. 803–812). Springer Berlin
 * Heidelberg.
 *
 **/
bool Crossover::IO(vector <unsigned> &child1, individuos *pop, TSP *tsp1)
{
  int c_prim, c, k, i, j; // c[i] & c_prim[j]
  float Prd = 0.02;
  i = rnd(0, sizeChrm - 1);
  c = child1[i];

  vector<unsigned> aux = child1;
  int actual = tsp1->evaluar(child1);
  while(true)
    {
      if(flip(Prd))
        {
          do {
            j = rnd(0, sizeChrm - 1);
            c_prim = child1[j];
          }
          while(c_prim == c);
        }
      else
        {
          do {
            j = rnd(0, sizePop - 1);
            k = find(pop[j].chrm.begin(), pop[j].chrm.end(), c) - pop[j].chrm.begin();

            c_prim = (k == sizeChrm - 1) ? pop[j].chrm[0] : pop[j].chrm[k + 1];
            j = find(child1.begin(), child1.end(), c_prim) - child1.begin();
          }
          while(c_prim == c);
        }

      int Imas1 = (i == sizeChrm - 1) ? 0 : i + 1;
      int Imenos1 = (i == 0) ? sizeChrm - 1 : i - 1;
      if(child1[Imas1] == c_prim || child1[Imenos1] == c_prim)
        break;
      else
        {
          if(i > j)
            {
              reverse(child1.begin() + j, child1.begin() + i);
              j = i - 1;
            }
          else
            {
              reverse(child1.begin() + i + 1, child1.begin() + j + 1);
              j = i + 1;
            }
        }
      c = c_prim;
      i = j;
    }

  if(actual < tsp1->evaluar(child1))
    child1 = aux;

  return true;
}


/**
 *
 * Modified Inver-over operator (MIO)
 *
 * Wang, Sun, Li & Gao (2012). A Modified Inver-over Operator for the
 * Traveling Salesman Problem. In D.-S. Huang, Y. Gan, P. Gupta, &
 * M. M. Gromiha (Eds.), Advanced Intelligent Computing Theories and
 * Applications. With Aspects of Artificial Intelligence (Vol. 6839,
 * pp. 17–23). Springer Berlin Heidelberg.
 *
 **/
bool Crossover::MIO(vector <unsigned> &child1, individuos *pop, TSP *tsp1, int maxgen, int gen)
{
  int c_prim, c, k, i, j, temp; // c[i] & c_prim[j]
  float Prd = 0.02, PUCmax = 0.5, PUCmin = 0.2, Puc;
  int cantVecinos = 5;
  i = rnd(0, sizeChrm - 1);
  c = child1[i];
  vector<unsigned> aux = child1;
  int actual = tsp1->evaluar(child1);

  while(true)
    {
      if(flip(Prd))
        {
          vector<pair <int, int> > vecino;
          for(int ii = 0; ii < sizeChrm; ii++)
            if(c != ii)
              {
                pair <int, int> city (ii, tsp1->distancia2Ciudades(c, ii));
                vecino.push_back(city);
              }
          sort (vecino.begin(), vecino.end(), compare_pair_second<std::less>());

          do {
            j = rnd(0, cantVecinos - 1);
            c_prim = vecino[j].first;
          }
          while(c_prim == c);
        }
      else
        {
          do {
            j = rnd(0, sizePop - 1);
            k = find(pop[j].chrm.begin(), pop[j].chrm.end(), c) - pop[j].chrm.begin();
            c_prim = (k == sizeChrm - 1) ? pop[j].chrm[0] : pop[j].chrm[k + 1];
            j = find(child1.begin(), child1.end(), c_prim) - child1.begin();
          }
          while(c_prim == c);
        }

      int Imas1 = (i == sizeChrm - 1) ? 0 : i + 1;
      int Imenos1 = (i == 0) ? sizeChrm - 1: i - 1;
      if(child1[Imas1] == c_prim || child1[Imenos1] == c_prim)
        break;
      else
        {
          if(i > j)
            {
              int largo = child1.size() - i + j, kkk, jjj;
              for(int kk = i + 1, r = 0, jj = j; r < (largo/2); r++, jj--, kk++)
                {
                  if(jj == -1)
                    jj = sizeChrm - 1;
                  kkk = kk % sizeChrm;
                  jjj = jj % sizeChrm;
                  temp = child1[kkk];
                  child1[kkk] = child1[jjj];
                  child1[jjj] = temp;
                }
            }
          else
            reverse(child1.begin() + i + 1, child1.begin() + j + 1);
        }

      Puc = PUCmax * exp((log(PUCmin / PUCmax)/maxgen)*gen);
      if(flip(Puc))
      	{
      	  c = c_prim;
      	  i++;
      	  i = i % sizeChrm;
      	}
    }

  if(actual < tsp1->evaluar(child1))
    child1 = aux;

  return true;
}

/**
 *
 * Voting Recombination Crossover (VR)
 *
 * Mühlenbein (1989). Parallel Genetic Algorithms, Population Genetics and
 * Combinatorial Optimization. In Schaffer, J. (ed.) Proceedings on the Third
 * International Conference on Genetic Algorithms, 416–421. Los Altos, CA:
 * Morgan Kaufmann Publishers.
 *
 **/
bool Crossover::VR(vector <unsigned> &child1, vector <unsigned> child2, individuos *pop)
{
  unsigned ch3, ch4, cont = 0, last = sizeChrm + 10;
  Selection *s = new Selection(sizePop, seed);
  ch3 = s->TorneoK(pop, 5);
  ch4 = s->TorneoK(pop, 5);
  delete s;
  vector<unsigned> child = child1;
  vector<unsigned> child3 = pop[ch3].chrm;
  vector<unsigned> child4 = pop[ch4].chrm;
  vector<unsigned> random = child1;

  for(int i = 0; i < child.size(); i++)
    {
      if(child[i] == child2[i])
        {
          if(child[i] != child3[i])
            {
              if(child[i] == child4[i])
                {
                  random[i] = last;
                  cont++;
                }
              else
                child1[i] = last;
            }
          else
            {
              random[i] = last;
              cont++;
            }
        }
      else if(child[i] == child3[i] && child[i] == child4[i])
        {
          random[i] = last;
          cont++;
        }
      else
        {
          if(child2[i] == child3[i] && child2[i] == child4[i])
            {
              child1[i] = child2[i];
              int j = find(random.begin(), random.end(), child1[i]) - random.begin();
              random[j] = last;
              cont++;
            }
          else
            child1[i] = last;
        }
    }

  if(child1 == child && cont == 0)
    {
      int i, j;
      do {
        i = rnd(0, sizeChrm - 1);
        j = rnd(0, sizeChrm - 1);
      }
      while(i == j);

      int aux = child1[i];
      child[i] = child[j];
      child[j] = aux;
      return true;
    }

  shuffle(random.begin(), random.end(), generator);

  for(int i = 0, j = 0; i < child1.size() && j < random.size();)
    {
      if(child1[i] == last)
        {
          if(random[j] != last)
            {
              child1[i] = random[j];
              i++;
              j++;
            }
          else
            j++;
        }
      else
        i++;
    }

  return true;
}

/**
 *
 * Edge Recombination Crossover (ER)
 *
 * Whitley, Starkweather & Fuquay (1989). Scheduling Problems and Travelling
 * Salesman: The Genetic Edge Recombination Operator. In Schaffer, J. (ed.)
 * Proceedings on the Third International Conference on Genetic Algorithms,
 * 133–140. Los Altos, CA: Morgan Kaufmann Publishers.
 *
 * Whitley, Starkweather & Shaner (1991). The Traveling Salesman and Sequence
 * Scheduling: Quality Solutions Using Genetic Edge Recombination. In Davis,
 * L. (ed.) Handbook of Genetic Algorithms, 350–372. New York: Van Nostrand
 * Reinhold.
 *
 **/
bool Crossover::ER(vector <unsigned> &child11, vector <unsigned> child2)
{
  vector< vector<int> > edge(child11.size());
  int j, Jmas1, Jmenos1, j2, J2mas1, J2menos1, actual;

  vector<unsigned> child1 = child11;
  child11.clear();

  for(int i = 0; i < edge.size(); i++)
    {
      j = find(child1.begin(), child1.end(), i) - child1.begin();
      Jmas1 = (j == sizeChrm - 1) ? 0 : j + 1;
      Jmenos1 = (j == 0) ? sizeChrm - 1: j - 1;
      vector<int> v;
      v.push_back(child1[Jmas1]);
      v.push_back(child1[Jmenos1]);

      j2 = find(child2.begin(), child2.end(), i) - child2.begin();
      J2mas1 = (j2 == sizeChrm - 1) ? 0 : j2 + 1;
      J2menos1 = (j2 == 0) ? sizeChrm - 1: j2 - 1;

      if(child2[J2mas1] != child1[Jmas1] && child2[J2mas1] != child1[Jmenos1])
        v.push_back(child2[J2mas1]);
      if(child2[J2menos1] != child1[Jmas1] && child2[J2menos1] != child1[Jmenos1])
        v.push_back(child2[J2menos1]);

      edge[i] = v;
    }


  actual = (rnd(0, 1)) ? child1[0] : child2[0];
  child11.push_back(actual);

  vector<int>::iterator it;
  int flag = false;
  for(int k = 0; child11.size() < child1.size(); k++)
    {
      for(int i = 0; i < edge.size(); i++)
        if((it = find(edge[i].begin(), edge[i].end(), actual)) != edge[i].end())
          edge[i].erase(it);

      if(edge[actual].size() == 0)
        {
          vector<int> elegir;
          for(int i = 0; i < edge.size(); i++)
            if(edge[i].size() != 0)
              elegir.push_back(edge[i][0]);

          if(elegir.size() == 0)
            break;

          shuffle(elegir.begin(), elegir.end(), generator);
          actual = elegir[0];
          child11.push_back(actual);
        }
      else
        {
          int min = 10, tam, randomMax = 0, random, revisar = actual, cand;
          flag = false;
          for(int i = 0; i < edge[revisar].size(); i++)
            {
              tam = edge[edge[revisar][i]].size();
              if(tam != 0)
                {
                  random = rnd(1, 7);
                  if((tam < min) || (tam == min && randomMax < random))
                    {
                      min = tam;
                      random  = randomMax;
                      cand = edge[revisar][i];
                      flag = true;
                    }
                }
            }
          if(flag == true)
            {
              actual = cand;
              child11.push_back(actual);
            }
          else
            {
              actual = edge[revisar][0];
              child11.push_back(actual);
            }
        }
    }
  return true;
}



/**
 *
 * Edge Recombination Crossover (ER)
 *
 * Nguyen, Yoshihara, Yamamori & Yasunaga (2002). Greedy genetic algorithms
 * for symmetric and asymmetric TSPs. IPSJ Trans. Mathematical Modeling and
 * Its Applications, 43, 165-175.
 *
 * Sengoku & Yoshihara (1998). A fast TSP solver using GA on JAVA. In Third
 * International Symposium on Artificial Life, and Robotics (AROB III’98)
 * (pp. 283-288).
 *
 **/
bool Crossover::GSTX(vector <unsigned> &child11, vector <unsigned> child2)
{
  vector<unsigned> child1 = child11;
  vector<unsigned> random = child11;
  child11.clear();
  // x de child1 (a) e y de child2 (b)
  bool Fa = true, Fb = true;
  int t = rnd(0, sizeChrm - 1);

  int x = find(child1.begin(), child1.end(), t) - child1.begin();
  int y = find(child2.begin(), child2.end(), t) - child2.begin();

  child11.push_back(t);
  random.erase(find(random.begin(), random.end(), child1[x]));
  // for(int i = 0; i < child1.size(); i++)
  //   cout << child1[i] << " ";
  // cout << endl;

  // for(int i = 0; i < child2.size(); i++)
  //   cout << child2[i] << " ";
  // cout << endl;

  do {
    x = (x - 1) % sizeChrm;
    y = (y + 1) % sizeChrm;

    if(Fa)
      {
        if(find(child11.begin(), child11.end(), child1[x]) == child11.end())
          {
            child11.insert(child11.begin(), child1[x]);
            random.erase(find(random.begin(), random.end(), child1[x]));
          }
        else
          Fa = false;
      }
    if(Fb)
      {
        if(find(child11.begin(), child11.end(), child2[y]) == child11.end())
          {
            child11.push_back(child2[y]);
            random.erase(find(random.begin(), random.end(), child2[y]));
          }
        else
          Fb = false;
      }
    // cout << " actual ";
    // for(int j = 0; j < child11.size(); j++)
    //   cout << child11[j] << " ";
    // cout << endl;
  }
  while(Fa || Fb);

  // for(int i = 0; i < child11.size(); i++)
  //   cout << child11[i] << " ";
  // cout << endl;

  if(child11.size() != child1.size())
    {
      shuffle(random.begin(), random.end(), generator);
      for(int i = 0; i < random.size(); i++)
        {
          if(rnd(0, 1))
            child11.insert(child11.begin(), random[i]);
          else
            child11.push_back(random[i]);
        }
    }
  // for(int i = 0; i < child11.size(); i++)
  //   cout << child11[i] << " ";
  // cout << endl;

  // exit(0);

  return true;
}
