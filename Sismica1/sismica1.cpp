//////////////////////////////////////////////////////////////////////////
// PROGRAMA: sismica1.h                                                  /
//                                                                       /
//   Descrição: Calcula as amostras de um traço sísmico sintético usando /
//              convolução.                                              /
//                                                                       /
// Autor: Carlos da Silva Claudino                                       /
// Data:07/05/2018                                                       /
//                                                                       /
//////////////////////////////////////////////////////////////////////////

//=========================== CABEÇALHO ==================================
#include "./sismica1.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <locale.h>
#include "math.h"

using std::cout;
using std::ifstream;
using std::string;

//========================= FUNÇÃO MAIN ==================================
int main()
{
    setlocale(LC_ALL, "Portuguese");
    // Instanciando a classe Sintetico
    Sintetico a = Sintetico();

    // Carregando o arquivo de configuração
    a.CarregarConfig();

    // Definindo os vetores de dados
    Sintetico::vector impedancia(a.GetnumAmostra());
    Sintetico::vector wavelet(a.GettamanhoWavelet());
    Sintetico::vector refletividade(a.GetnumAmostra());
    Sintetico::vector traco(a.GetnumAmostra());

    // Gerando a wavelet
    a.GeraWavelet(wavelet.data());

    // Gerando refletividade padrão
    a.GeraRefletMod(refletividade.data(), a.Getruido());

    // Gerando o traço padrão
    a.convolucao(wavelet.data(), refletividade.data(), traco.data());

    if(a.Getpadrao())
    {
        // Exportar o traço padrão
        for(int i = 0; i < a.GetnumTraco(); i++)
        {
            a.GravarTraco(traco.data(), i);
        }
    }else
    {
        for(int i = 0; i < a.GetnumTraco(); i++)
        {
            // Atualizando a refletividade
            a.GeraRefletImped(impedancia.data(), refletividade.data());

            // Atualizando o(s) traço(s) sintético(s)
            a.convolucao(wavelet.data(), refletividade.data(), traco.data());

            // Exportar o traço atualizado
            a.GravarTraco(traco.data(), i);
        }
    }
    system("PAUSE");
    return 0;
}

//============ IMPLEMENTAÇÃO DOS MÉTODOS DA CLASSE SINTETICO =============
void Sintetico::GeraWavelet(float wavelet[])
{
    // Fator da função Ricker.
    float a = -1.*pow(pi*GetfreqPico(),2);

    // Período da frequência de pico.
    float periodoPico = 1./GetfreqPico();

    // Calculando o vetor wavelet.
    for (int i = 0; i < GettamanhoWavelet(); i++)
    {
        wavelet[i] = (float) Getpolaridade()*(((1 + 2*a*pow(
                    ((Getamostragem()*i)-periodoPico),2))*exp(a*pow(
                    ((Getamostragem()*i)-periodoPico),2))));
    }
}
//------------------------------------------------------------------------

void Sintetico::GeraRefletImped(float impedancia[], float refletividade[])
{
    // Calculando a refletividade, exceto para a última amostra.
    for (int i = 0; i < GetnumAmostra() - 1; i++)
    {
        refletividade[i] = (impedancia[i+1] - impedancia[i])/
                           (impedancia[i+1] + impedancia[i]);
    }

    // Ùltima amostra.
    refletividade[GetnumAmostra()-1] = refletividade[GetnumAmostra()-2];
}
//------------------------------------------------------------------------

void Sintetico::GeraRefletMod(float refletividade[], bool ruido)
{
    // Zerando as amostras.
    for(int n = 0; n < GetnumAmostra(); n++)
    {
        refletividade[n] = 0.;
    }

    // Incluindo ruido de, no máximo, 0.05.
    if(ruido)
    {
        int aux = GetnumAmostra();
        for(int m = 0; m < (GetnumAmostra()/4); m++)
        {
            refletividade[rand()%aux] = pow(-1,rand()%2)*(1.0*
                                        (rand()%6)/100.);
        }
    }

    // Calculando 5 refletividades aleatórias, com mesmo espaçamento e com
    // valor absoluto maior ou igual 0.3 e menor que 1.
    float passo = (1.0*GetnumAmostra())/6.;
    int indice = 0;
    for(int p = 1; p < 6; p++)
    {
        indice = passo*p;
        refletividade[indice] = pow(-1,rand()%2)*((0.699*
                                (rand()%11)/10.)+0.3);
    }
}
//------------------------------------------------------------------------

void Sintetico::convolucao(float wavelet[], float refletividade[], float traco[])
{
    // Vetor auxiliar
    vector aux(GetnumAmostra() + GettamanhoWavelet() - 1);

    // Convolução linear completa
    for(int i = 0; i < GetnumAmostra(); i++)
    {
        for(int j = 0; j < GettamanhoWavelet(); j++)
        {
            aux[i+j] = aux[i+j] + wavelet[j]*refletividade[i];
        }
    }

    // Tamanho a ser removido do início e do fim do traço
    int tamanho =  GettamanhoWavelet()/2;

    // Removendo trechos do traço
    for(int j = tamanho; j < tamanho + GetnumAmostra(); j++)
    {
        traco[j-tamanho] = aux[j];
    }
}
//------------------------------------------------------------------------

void Sintetico::CarregarConfig()
{
    // Controle de tela
    cout << "CARREGANDO ARQUIVO DE CONFIGURAÇÃO. \n\n\n";

    // Endereço do arquivo de configuração - mesmo local do executável.
    string enderecoConfig = "./sismica1.ini";

    // Verifica se o arquivo foi aberto com sucesso.
    ifstream ifpConfig(enderecoConfig);

    // Contador para os atributos
    int contParametro = 0;

    string linha;
    if (ifpConfig)
    {
        // Controle de tela
        cout << "=========================================================================" << "\n";
        cout << "                 ARQUIVO DE CONFIGURAÇÃO ENCONTRADO                      " << "\n";
        cout << "=========================================================================" << "\n";


        // Enquanto não chegar o fim do arquivo faça:
        while (!ifpConfig.eof())
        {
            // Leia a linha atual do arquivo
            getline(ifpConfig,linha);

            // Se a linha não for vazia ou um comentário faça:
            if (!(linha.length() == 0 || linha[0] == '/'))
            {
                // Carrega o endereço do arquivo de impedância
                if(contParametro == id_enderecoimp)
                {
                    SetenderecoImp(linha);

                }

                // Carrega o número de traços
                if(contParametro == id_numtraco)
                {
                    // atoi converte de string para int
                    if(atoi(linha.c_str())>0)
                        SetnumTraco(atoi(linha.c_str()));
                    else
                        SetnumTraco(1);
                }

                // Carrega o número de amostras
                if(contParametro == id_numamostra)
                {
                    if(atoi(linha.c_str()) >= 0)
                        SetnumAmostra(atoi(linha.c_str()));
                    else
                        SetnumAmostra(1000);
                }

                // Carrega a taxa de amostragem
                if(contParametro == id_amostragem)
                {
                    // atof converte de string para double
                    if (atof(linha.c_str()) >= 0)
                        Setamostragem(atof(linha.c_str()));
                    else
                        Setamostragem(0.004);
                }

                // Carrega a frequência de pico
                if(contParametro == id_freqpico)
                {
                    if (atof(linha.c_str()) >= 0)
                        SetfreqPico(atof(linha.c_str()));
                    else
                        SetfreqPico(20);
                }

                // Carrega a polaridade da wavelet
                if(contParametro == id_polaridade)
                {
                    if ((atof(linha.c_str()) >= 0)&&(atof(linha.c_str()) <= 1))
                        Setpolaridade((short) atof(linha.c_str()));
                    else
                        Setpolaridade(1);
                }

                // Carrega a presença de ruído
                if(contParametro == id_ruido)
                {
                    if ((atof(linha.c_str()) >= 0)&&(atof(linha.c_str()) <= 1))
                        Setruido((bool) atof(linha.c_str()));
                    else
                        Setruido(0);
                }

                // Define se usa o modelo padrão de refletividade
                if(contParametro == id_padrao)
                {
                    if ((atof(linha.c_str()) >= 0)&&(atof(linha.c_str()) <= 1))
                        Setpadrao((bool) atof(linha.c_str()));
                    else
                        Setpadrao(1);
                }
                contParametro++;

            }

        }
    }else
    {
        // Controle de tela
        cout << "=========================================================================" << "\n";
        cout << "                 ARQUIVO DE CONFIGURAÇÃO NÃO ENCONTRADO                  " << "\n";
        cout << "=========================================================================" << "\n";
        cout << "Usando a configuração padrão, conforme abaixo:" << "\n";

        SetenderecoImp(" ");
        SetnumTraco(1);
        SetnumAmostra(1000);
        Setamostragem(0.004);
        SetfreqPico(20);
        Setpolaridade(1);
        Setruido(0);
        Setpadrao(1);
    }

    // Definindo tamanho da wavelet
    SettamanhoWavelet((int)(2./(GetfreqPico()*Getamostragem()))+2);

    // Controle de tela
    cout << "\n";
    cout << "Arquivo de Impedância                                             " << GetenderecoImp() << "\n";
    cout << "Número de traços                                                  " << GetnumTraco() << "\n";
    cout << "Número de amostras                                                " << GetnumAmostra() << "\n";
    cout << "Taxa de amostragem                                                " << Getamostragem() << "\n";
    cout << "Frequência de pico da wavelet                                     " << GetfreqPico() << "\n";
    cout << "Polaridade                                                        " << Getpolaridade() << "\n";
    cout << "Usa ruído?                                                        " << (Getruido() == 1 ? "SIM\n" : "NÃO\n");
    cout << "Usa modelo padrão de refletividade?                               " << (Getpadrao() == 1 ? "SIM\n" : "NÃO\n");
    cout << "Tamanho da wavelet                                                " <<  GettamanhoWavelet() << "\n";
    cout << "\n";

    // Fechando o arquivo de configuração
    ifpConfig.close();
}
//------------------------------------------------------------------------

// NÃO IMPLEMENTADO. É NECESSÁRIO CONHECER O ARQUIVO DE IMPEDÂNCIA
void Sintetico::CarregarImped(float impedancia[])
{

}
//------------------------------------------------------------------------

void Sintetico::GravarTraco(float traco[], int indice)
{
    // Abrindo os arquivos de entrada e temporário de saída
    ifstream entrada("./Tracos.txt");
    FILE *saida = fopen ("./Tracos_tmp.txt", "w+");

    // Linha do arquivo
    string linha;

    // Contador para as amostras do traço
    int cont = 0;

    // Se não for o primeiro traço, recupera os dados do arquivo
    if(indice)
    {
        while (!entrada.eof())
        {
            // Lê a linha atual do arquivo entrada
            getline(entrada,linha);

            if (!(linha.length() == 0 || linha[0] == '/'))
            {
                // Concatena as informações do arquivo com o traço atual
                fprintf(saida,"%s",linha.c_str());
                fprintf(saida,"%3.2f",traco[cont]);
                fprintf(saida,"\t\n");
                cont++;
            }
        }

        // Se for o primeiro traço, descarta o arquivo
    }else
    {
        for (int i = 0; i < GetnumAmostra(); i++)
        {
            fprintf(saida,"%3.2f",traco[cont]);
            fprintf(saida,"\t\n");
            cont++;
        }
    }

    // Fechando os arquivos
    fclose(saida);
    entrada.close();

    // Excluindo o arquivo antigo
    remove("./Tracos.txt");
    rename("./Tracos_tmp.txt","./Tracos.txt");
}
