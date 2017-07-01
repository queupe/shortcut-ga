/*
*  cruzamento.hpp
*
*  Módulo responsável pelas reotinas de cruzamento
*  Autor: Geraldo José Ferreira Chagas Junior - gjr.doc@gmail.com
*
*  PPGI - NCE - UFRJ
*  Data Criação: 19/05/2017
*  Datas de Modificações:
*
*/

#ifndef _CRUZAMENTO_H
#define	_CRUZAMENTO_H

#include "individuo.hpp"
#include "utils.hpp"
#include "tsp.hpp"
#include "arqlog.hpp"

class TCruzamento
{
   private:
      TMapaGenes *VP_Mapa;
      TArqLog *VP_ArqSaida;

      int VP_cruzamento;
   public:

      //Apenas leitura
      TMapaGenes *getMapa();
      TArqLog *getArqLog();


      TCruzamento (TMapaGenes *mapa, TArqLog *arqSaida, int tipoCruzamento);
      vector <TIndividuo *>processa (TIndividuo *parceiro1, TIndividuo *parceiro2);

   private:
      vector <TIndividuo *>GSTX (TIndividuo *parceiro1, TIndividuo *parceiro2);
      vector <TIndividuo *>PMX(TIndividuo *parceiro1, TIndividuo *parceiro2);
		vector <TIndividuo *>OX1(TIndividuo *parceiro1, TIndividuo *parceiro2);

};
#endif
