/*
*  ag.hpp
*
*  Módulo responsável pela execução do algoritmo genético
*  Autor: Geraldo José Ferreira Chagas Junior - gjr.doc@gmail.com
*
*  PPGI - NCE - UFRJ
*  Data Criação: 06/05/2017
*  Datas de Modificações:
*
*/

#ifndef _AG_H
#define	_AG_H

#include "mutacao.hpp"
#include "cruzamento.hpp"
#include "tsp.hpp"
#include "arqlog.hpp"
#include "individuo.hpp"
#include "populacao.hpp"
#include "utils.hpp"
#include "tabconversao.hpp"


class TAlgGenetico
{
   private:
      time_t sysTime1;
      time_t sysTime2;
      int VP_profundidade;
      
      int VP_tamPopulacao;
      int VP_maxGeracao;
      int VP_percentMutacao;
      int VP_percentManipulacao;
      int VP_mutacao;
      int VP_cruzamento;
      int VP_printParcial;
      int VP_percentElitismo;
      int VP_roleta;
      int VP_selIndMutacao;
      int VP_percentReducao;
      int VP_profundidadeMaxima;
      int VP_ativaRecursivo;


      double VP_melhor_dist;
	  int VP_melhor_gera;

      TMapaGenes *VP_Mapa;
      TArqLog *VP_ArqSaida;

   public:      
      TMutacao *mutacao;
      TCruzamento *cruzamento;

      //Apenas leitura
      TMapaGenes *getMapa();
      TArqLog *getArqLog();
      double get_melhor_dst();
      int get_melhor_gera();
	  int get_profundidade();

      //propriedades
      void setTamPopulacao (int val);
      void setMaxGeracao (int val);
      void setPrintParcial (int val);
      void setPercentMutacao (int val);
      void setPercentManipulacao (int val);
      void setMutacao (int val);
      void setCruzamento (int val);
      void setPercentElitismo (int val);
      void setRoleta (int val);
      void setSelIndMutacao (int val);
      void setPercentReducao (int val);
	  void setProfundidadeMaxima(int val);
      void setAtivaRecursivo (int val);

      int getPercentReducao ();
	  int getProfundidadeMaxima();
      int getAtivaRecursivo ();
      int getTamPopulacao ();
      int getMaxGeracao ();
      int getPrintParcial ();
      int getPercentMutacao ();
      int getPercentManipulacao ();
      int getMutacao ();
      int getCruzamento ();
      int getPercentElitismo ();
      int getRoleta ();
      int getSelIndMutacao ();

      void setTime (time_t sTime);
      //Métodos
      TAlgGenetico (TMapaGenes *mapa, TArqLog *arqSaida, int profundidade);
	  TAlgGenetico (TMapaGenes *mapa, TArqLog *arqSaida);
      ~TAlgGenetico ();
	  
	  void exec(TIndividuo *indBase, TTabConversao *tabConversao, int extraPadrao);
      void exec ();

      TPopulacao *iteracao (TPopulacao *populacao, int geracao);
      void elitismo(TPopulacao *novaPop, TPopulacao *populacao);
      void geraDescendentes(TPopulacao *novaPop, TPopulacao *populacao);
      void fazMutacao(TPopulacao *populacao);
      int sorteiaPais(TPopulacao *populacao, int exceto);
      int sorteiaPais(TPopulacao *populacao);
      void mutacaoAGRecursivo(TPopulacao *populacao, int indice);
};

#endif
