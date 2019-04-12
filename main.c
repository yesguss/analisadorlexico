
/*

BEC - Bacharelado em Engenharia da Computação - 7º Semestre

ANALISADOR LÉXICO - TEORIA DE COMPUTADORES E COMPILADORES

TRABALHO ELABORADO POR:
	Diego Capassi
	Gustavo Fontes Araujo
	
*/

#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include <String.h>

#define NOVALINHA 13
#define BRANCO 32
#define TAMANHO 1000

typedef enum
{

    ERRO,
    IDENTIFICADOR,
    NUM_INT,
    NUM_REAL,
    EOS,
    ALGORITMO,
    ATE,
    CADEIA,
    CARACTER,
    ENQUANTO,
    ENTAO,
    FACA,
    FIM,
    FUNCAO,
    INICIO,
    INTEIRO,
    PARA,
    PASSO,
    PROCEDIMENTO, 
    REAL,
    REF,
    RETORNE,
    SE,
    SENAO,
    VARIAVEIS,
	OP_LOGICO,
	OP_RELACIONAL,
	FRASE,
	COMENTARIO,
	DIVISAO,
	PALAVRARESERVADA,
	ADD,
	SUB,
	MULT,
	REST,
	OPEN,
	CLOSE,
	DOT,
	DOTT

}Atomo;


char *msgAtomo[] = { "ERRO",
                     "IDENTIFICADOR",
                     "NUMERO INTEIRO",
                     "NUMERO REAL",
                     "FIM DO ARQUIVO",
                     "ALGORITMO",
                     "ATE",
                     "CADEIA",
                     "CARACTER",
                     "ENQUANTO",
                     "ENTAO",
                     "FACA",
                     "FIM",
                     "FUNCAO",
                     "INCIO",
                     "INTEIRO",
                     "PARA",
                     "PASSO",
                     "PROCEDIMENTO",
                     "REAL",
                     "REF",
                     "RETORNE",
                     "SE",
                     "SENAO",
                     "VARIAVEIS",
					 "OPERADOR LOGICO",
					 "OPERADOR RELACIONAL",
					 "FRASE",
					 "COMENTARIO",
					 "DIVISAO",
					 "PALAVRA RESERVADA",
					 "ADICAO",
					 "SUBTRACAO",
					 "MULTIPLICACAO",
					 "RESTO",
					 "ABRE PARANTESE",
					 "FECHA PARENTESE",
					 "PONTO",
					 "PONTO E VIRGULA"
				 
               
                        };

Atomo reconhece_NUM();
Atomo reconhece_IDENTIFICADOR(char);
Atomo proximo_token();
Atomo reconhece_OPERADOR(char);
Atomo reconheco_FRASE(char);
Atomo reconhece_COMENTARIO(char);
Atomo buscaAtomo(char* );
Atomo reconhe_NAO_ATRIBUIDO(char);

void retract_chat();
char proximo_char(void);


// variáveis globais
char *buffer;
int nlinha = 1;
int cont = 30;
char *Atm = NULL;

void main(int args, char *argv[])
{
    Atomo atomo;
// abre o arquivo fonte
    FILE *arq;
    char *result, c;
    int i=0;
    

    arq = fopen(argv[1], "r");

    if(arq == NULL)
    {
        printf("Houve algum problema na abertura do arquivo, verifique \n");
        return;
    }
// lê as palavras

    while (fgetc(arq) != EOF)
    {
        i++;
    }

    printf("%d\n", i);
// armazena no buffer

    result = (char *)malloc( (i+1) * sizeof(char) );
    result[i] = '\0';

    fseek(arq, 0, SEEK_SET);

    for(int a = 0; a < i; a++)
    {
        result[a] = fgetc(arq);
        if (result[a] == '\n')
            printf("result[%d] = <ENTER>\n", a);
        else
            printf("result[%d] = %c\n", a, result[a]);
    }
     buffer = result;
    do
    {
        atomo = proximo_token();
        printf("Linha %d o lexema e %s e o atomo %s\n", nlinha, Atm,msgAtomo[atomo] );

		Atm = NULL;
		free(Atm);
    }
    while (atomo != EOS && atomo != ERRO);

    free(result);
    fclose(arq);

}


char proximo_char(void)
{
    return *buffer++;
}

Atomo proximo_token()
{
    char c;
    char aux;
    Atomo atomo = ERRO;

// ignora espaço em branco e trata nova linha

    while(*buffer == ' '  || *buffer == '\n')
    {
        if (*buffer == '\n')
		{
            nlinha++;
		}

        buffer++;
    }
        if (*buffer == '\0')
		{
			Atm = "EOS";
            return EOS;
		}
    c = proximo_char();
    
    if (isalpha(c))
    {
        atomo = reconhece_IDENTIFICADOR(c);
    }
    if (isdigit(c))
    {
        atomo = reconhece_NUM();
    }
	if(isascii(c) && !isalnum(c))
	{
		atomo = reconhece_OPERADOR(c);
	}
	if(c == '"')
	{
		atomo = reconheco_FRASE(c);
	}
	if(c == '/')
	{
		atomo = reconhece_COMENTARIO(c);
	}
	if (((int)c == 43)||((int)c == 45)||((int)c == 42)||((int)c == 37)||((int)c == 40)||((int)c == 41)||((int)c == 46)||((int)c == 59))
	{
	 atomo = reconhe_NAO_ATRIBUIDO(c);
	}
	
    return atomo;
}

Atomo reconhece_RESERVADA(char *palavra)
{
	for(int num = 0; num < cont; num++)
	{
	if(( _stricmp(palavra, msgAtomo[num])) == 0){
  
               Atm = palavra;
               return PALAVRARESERVADA;
      }
	}
		Atm = palavra;
		return IDENTIFICADOR;
}
Atomo reconhece_IDENTIFICADOR(char id)
{
    char c;
    char palavra[50] = {};
    int n = 0;

    Atomo atomo;

    palavra[n++] = id;


    c = proximo_char();
	
    if ( c != '\n') 
	{

        palavra[n++] = c;

    }

    while(isalpha(c) || isdigit(c))
    {
        c = proximo_char();
        if ( c != '\n') 
	{

        palavra[n++] = c;

    }

    }
	
	   
    atomo = buscaAtomo(palavra);
	
    retract_chat();

    return atomo;
}

Atomo buscaAtomo(char *palavra) {
	
   for ( int i = 0; i < cont; i++ )
   {
      if(( _stricmp(palavra, msgAtomo[i])) == 0){
              Atm = (char *)malloc( strlen(palavra) * sizeof(char) );
               Atm = palavra;
               return i;
      }
      
   }
   
   Atm = (char *)malloc( strlen(palavra) * sizeof(char) );
   Atm = palavra;
   return IDENTIFICADOR;            
     
}


void retract_chat()
{
    buffer--;
}

Atomo reconhece_NUM()
{
    char c;
    c = proximo_char();
    int estado = 2;
    while(1)
    {
        switch(estado)
        {
        case 2:
            c = proximo_char();
            if (c == '.')
                estado = 3;
            else if (isdigit(c))
                estado = 2;
            else if (!isalpha(c))
                return NUM_INT;
            else
                return ERRO;
            break;
        case 3:
            c = proximo_char();
            if(isdigit(c))
                estado = 4;
            else
                return ERRO;
            break;
        case 4:
            c = proximo_char();
            if(isdigit(c))
                estado = 4;
            else if (!isalpha(c))
                return NUM_REAL;
            else
                return ERRO;
            break;
        }
    }
}

Atomo reconhece_OPERADOR(char ope){
   char op;
   char temp;
   
   retract_chat();
   op = proximo_char();
   
   temp = op;
   
   if (op == '!'){
	   Atm = (char*)malloc((1)*sizeof(char));
	   Atm = "!";
	   return OP_LOGICO;
	   
   }
   if (op == '$'){
	   Atm = (char*)malloc((1)*sizeof(char));
	   Atm = "$";
	   return OP_LOGICO;
	   
   }
   if (op == '&'){
	   Atm = (char*)malloc((1)*sizeof(char));
	   Atm = "&";
	   return OP_LOGICO;
	   
   }
   
   if ((op == '>' || op == '<') && (proximo_char() == '='))
   {
	   retract_chat();
	   Atm = (char*)malloc((2)*sizeof(char));
	 // printf("Estou aqui");
	   Atm = "<=";
	   return OP_RELACIONAL;
   }
   else if ((op == '<') && (proximo_char() == '>'))
   {
	   retract_chat();
	   Atm = (char*)malloc((2)*sizeof(char));
	   Atm = "<>";
	   return OP_RELACIONAL;
   }
   else if (op == '=')
   {
	   Atm = (char*)malloc((2)*sizeof(char));
	   Atm = "=";
	   return OP_RELACIONAL;
   }  
   else if (op == '>')
   {
	 Atm = (char*)malloc((2)*sizeof(char));
	   Atm = ">";
	   return OP_RELACIONAL; 
	   
   }
   else if (op == '<')
   {
	 Atm = (char*)malloc((2)*sizeof(char));
	   Atm = "<";
	   return OP_RELACIONAL; 
	   
   }
   else
   {
	   return ERRO;
   }
}

Atomo reconheco_FRASE(char c)
{
	char frase;
	retract_chat();
	
	frase = proximo_char();
	int estado = 2; 
	
	while(1){
		switch(estado)
		{
			case 2:
			frase = proximo_char();
			if (isascii (frase)|| isalnum(frase))
			{
				estado = 3;
			}
			else if (((int)frase == 92)&&(proximo_char() == '"'))
			{
				estado = 3;
			}
			else
			{
				return ERRO;
			}
			break;
			
			case 3:
			frase = proximo_char();
			if (((int)frase == 92) &&(proximo_char() == '"'))
			{
				estado = 3;
			}
			else if (frase == '"')
			{
				return FRASE;
			}
			else if (isascii (frase)|| isalnum(frase))
			{
				estado = 3;
			}
			else
			{
				return ERRO;
			}
			break;
		}
	}
} 

Atomo reconhece_COMENTARIO(char com)
{
	int i = 0;
	char c,d;
	char *numero = (char *)calloc( 1000 , sizeof(char) );
	numero[i++] = com;
	retract_chat();
	c = proximo_char();
	d = proximo_char();
	retract_chat();
	retract_chat();
	
	int estado = 2;
	
	if(c == '/')
	{
		while(1)
		{
			switch(estado)
			{
				case 2:
				c = proximo_char();
				if(c == '/' && d == '*')
				{
					estado = 3;
				}
				else if(c == '/' && d != '*')
				{
					Atm = numero;
					return DIVISAO;
				}
				else
				{
					Atm = "ERRO";
					return ERRO;
				}
				break;
				case 3:
				c = proximo_char();
				if(c == '*')
				{
					numero[i++] = c;
					estado = 4;
				}
				else if(c != '*')
				{
					numero[i++] = c;
					estado = 3;
				}
				else
				{
					Atm = "ERRO";
					return ERRO;
				}
				break;
				case 4:
				c = proximo_char();
				if(c == '/')
				{
					numero[i++] = c;
					Atm = numero;
					return COMENTARIO;
				}
				else if(c != '/')
				{
					numero[i++] = c;
					estado = 3;
				}
				else
				{
					Atm = "ERRO";
					return ERRO;
				}
			}
		}
	}
	else
	{
		Atm = "ERRO";
		return ERRO;
	}
	
}

Atomo reconhe_NAO_ATRIBUIDO(char natrib)
{
	char atrib, temp;
	retract_chat();
	
	atrib =  proximo_char();
	
	temp = atrib;
	
	if ((int)atrib == 43)
	{
	   Atm = (char*)malloc((1)*sizeof(char));
	   Atm = "+";
	   return ADD;
	   
	}
	else if ((int)atrib == 45)
	{
	   Atm = (char*)malloc((1)*sizeof(char));
	   Atm = "-";
	   return SUB;
	   
	}
	else if ((int)atrib == 42)
	{
	   Atm = (char*)malloc((1)*sizeof(char));
	   Atm = "*";
	   return MULT;
	   
	}
   	else if ((int)atrib == 37)
	{
	   Atm = (char*)malloc((1)*sizeof(char));
	   Atm = "%";
	   return REST;
	   
	}
	else if ((int)atrib == 40)
	{
	   Atm = (char*)malloc((1)*sizeof(char));
	   Atm = "(";
	   return OPEN;
	   
	}
	else if ((int)atrib == 41)
	{
	   Atm = (char*)malloc((1)*sizeof(char));
	   Atm = ")";
	   return CLOSE;
	   
	}
	else if ((int)atrib == 46)
	{
	   Atm = (char*)malloc((1)*sizeof(char));
	   Atm = ".";
	   return DOT;
	   
	}
	else if ((int)atrib == 59)
	{
	   Atm = (char*)malloc((1)*sizeof(char));
	   Atm = ";";
	   return DOTT;
	   
	}
	else
	{
		return ERRO;
	}
 
   
}