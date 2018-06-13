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

//========================== CABEÇALHOS ==================================
#include <string>
#include <vector>

//============================ MACROS ====================================
//Macros para manipulação dos atributos da classe. Funções get e set.
#define GetSet(type, name)\
private: type _##name; \
public: \
void Set##name(const type& var) \
{\
        _##name = var;\
}\
\
type Get##name() \
{\
        return _##name; \
}\

//=============== DECLARAÇÃO DA CLASSE SINTETICO =========================

class Sintetico
{
public:
    using vector = std::vector<float>;

    //======================= CONSTRUTOR =============================

    /**< Construtor. */
    Sintetico(){};

    /**< Destrutor */
    ~Sintetico(){};

    //========================= MÉTODOS ==============================
    /**< Gera uma wavelet. */
    void GeraWavelet(float wavelet[]);

    /**< Gera refletividade a partir da impedância. */
    void GeraRefletImped(float impedancia[], float refletividade[]);

    /**< Gera um exemplo de refletividade, se ruido for "true", a
             refletividade será gerada com ruído. */
    void GeraRefletMod(float refletividade[], bool ruido);

    /**< Realiza a convolução linear entre dois vetores. */
    void convolucao(float wavelet[], float refletividade[], float traco[]);

    /**< Ler arquivo de configuração. */
    void CarregarConfig();

    /**< Ler arquivo de impedância. */
    void CarregarImped(float impedancia[]);

    /**< Gravar o traço em um arquivo texto. */
    void GravarTraco(float traco[], int indice);

    //----------------------------------------------------------------

    /* IDENTIFICADORES - Usados para identificar os atributos no
        arquivo de configuração. Não altere a sequência abaixo. Se houver
        necessidade de inclusão de algum atributo, coloque seu identificador
        no fim da lista e atualize o arquivo "sismica.ini" e o
        método "CarregaConfig".*/

    enum
    {
        id_enderecoimp,
        id_numtraco,
        id_numamostra,
        id_amostragem,
        id_freqpico,
        id_polaridade,
        id_ruido,
        id_padrao
    };
    //----------------------------------------------------------------

private:

    //========================= ATRIBUTOS ============================
    // CONSTANTES
    /**< Valor de pi. */
    const double pi = 3.141592653589793238460;
    //----------------------------------------------------------------

    /*DECLARAÇÃO - Os comandos abaixo declaram os atributos e
        implementam suas funções get e set. */

    /**< Endereço do arquivo de impedância. */
    GetSet(std::string, enderecoImp);

    /**< Número de traços. */
    GetSet(long, numTraco);

    /**< Número de amostras. */
    GetSet(long, numAmostra);

    /**< Taxa de amostragem. */
    GetSet(float, amostragem);

    /**< Frequência de pico da wavelet. */
    GetSet(float, freqPico);

    /**< Tamanho da wavelet. */
    GetSet(int, tamanhoWavelet);

    /**< Polaridade da Wavelet. */
    GetSet(short, polaridade);

    /**< Presença ou não de ruído */
    GetSet(bool, ruido);

    /**< Usa ou não a refletividade padrao */
    GetSet(bool, padrao);
    //----------------------------------------------------------------
};
