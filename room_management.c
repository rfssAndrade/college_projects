/*
 * File:  room_manager.c
 * Author:  Ricardo Andrade
 * Description: A program of room reservation.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* Valores maximos para cada parametro. */
#define MAX_SALA 10
#define MAX_EVENTO 100
#define EVENTOS 1000
#define NOME 64
#define DATA 9
#define INICIO 5
#define MAX_IN 342
#define DATA_INICIAL 1061805600     /* 01012019 em minutos */
#define PARTICIPANTES 3


/* Tipo evento */
typedef struct evento {
    char descricao[NOME];
    char data[DATA];         /* em ddmmaaaa */
    int data_m;              /* em minutos */
    char inicio[INICIO];     /* em hhmm */
    int inicio_m;            /* em minutos */
    int duracao;             /* em minutos */
    int sala;
    char responsavel[NOME];
    char participantes[3][NOME];
} Evento;


/* Prototipos */
void le_linha(char s[]);

Evento cria_evento();

void mostra_evento(Evento a);

int inicio_para_min(int n);

int data_para_mins(int data);

void selection_sort();

void swap(Evento *xp, Evento *yp);

void adiciona_evento();

int binary_search(int esq, int dir, int x);

int verifica_evento(char descricao[]);

int verifica_sala(Evento a, Evento b);

int verifica_participantes(Evento temp, Evento b, char comando);

int verifica_sobreposicao(Evento temp, char comando);

void listar_todos();

void listar_sala();

void mudar_sala();

void altera_hora(char comando);

void apaga_evento();

void adiciona_participante();

void remove_participante();


/* Variaveis globais */
Evento tab_eventos[EVENTOS];    /* Array que armazena os eventos */
int n_eventos = 0;      /* Variavel que controla o numero de eventos existentes no array */


/* Executa os comandos fornecidos pelo utilizador */
int main() {
    char c;

    while ((c = getchar())) {
        getchar();      /* ignora o espaco apos o comando */
        switch (c) {
            case 'a':
                adiciona_evento();
                break;

            case 'l':
                listar_todos();
                break;

            case 's':
                listar_sala();
                break;

            case 'r':
                apaga_evento();
                break;
            
            case 'i':
                altera_hora('i');
                break;

            case 't':
                altera_hora('t');
                break;
            
            case 'm':
                mudar_sala();
                break;

            case 'A':
                adiciona_participante();
                break;

            case 'R':
                remove_participante();
                break;

            case 'x':
                return 0;
        }
    }
    return 0;
}


/* Le uma linha do standard input ate encontrar \n */
void le_linha(char s[]) {
    char c;
    int i = 0;

    while ((c = getchar()) != '\n') {
        s[i++] = c;
    }
    s[i] = '\0';    /* Coloca '\0' no final da string */
}


/* Cria um evento atribuindo valores lidos do standard input as variaveis */
Evento cria_evento() {
    char s[MAX_IN];
    char limite[] = ":";
    char* token;
    Evento novo_evento;
    
    le_linha(s);

    /* Contrucao da estrutur a partir do input fornecido */
    strcpy(novo_evento.descricao, strtok(s, limite));
    strcpy(novo_evento.data, strtok(NULL, limite));
    novo_evento.data_m = data_para_mins(atoi(novo_evento.data));
    strcpy(novo_evento.inicio, strtok(NULL, limite));
    novo_evento.inicio_m = inicio_para_min(atoi(novo_evento.inicio));
    novo_evento.duracao = atoi(strtok(NULL, limite));
    novo_evento.sala = atoi(strtok(NULL, limite));
    strcpy(novo_evento.responsavel, strtok(NULL, limite));
    strcpy(novo_evento.participantes[0], strtok(NULL, limite));

    /* Verifica se o evento tem mais que um participante */
    if ((token = strtok(NULL, limite)) != NULL) 
        strcpy(novo_evento.participantes[1], token);
    else 
        strcpy(novo_evento.participantes[1], "");

    if ((token = strtok(NULL, limite)) != NULL) 
        strcpy(novo_evento.participantes[2], token);
    else 
        strcpy(novo_evento.participantes[2], "");

    return novo_evento;
}


/* Imprime um evento para o terminal*/
void mostra_evento(Evento a) {
    int i;

    printf("%s %s %s %d Sala%d %s\n*", a.descricao, a.data, a.inicio, a.duracao, a.sala, a.responsavel);

    for (i = 0; i < PARTICIPANTES; i++) {
        if (strcmp(a.participantes[i], "") != 0)
            printf(" %s", a.participantes[i]);
    }
    printf("\n");
}


/* Converte o inicio no formato hhmm para minutos */
int inicio_para_min(int n) {
    int horas;
    int mins;

    horas = 60 * (n / 100);     /* Obtem os dois primeiros digitos e transforma em minutos */
    mins = n % 100;     /* Obtem os dois ultimos digitos */

    return horas + mins;
}


/* Converte a data no formato ddmmaaaa em minutos, considerando 01/01/2019 como minuto 0 */
int data_para_mins(int data) {
    int ano, mes, dia;

    ano = data % 10000;     /* Obtem os 4 digitos do ano */
    mes = (data / 10000) % 100;     /* Obtem os digitos correspondentes ao mes */
    dia = data / 1000000;       /* Obtem os digitos correspondentes ao dia */

    mes = (mes + 9) % 12;       /* Converte os meses do ano de 0 (Marco) a 11 (Fevereiro) */
    ano = ano - mes / 10;       /* Se o mes e Janeiro ou Fevereiro subtrai um ao ano pois considera-se que pertencem ao ano transato */

    /* Formula que converte a data em minutos, tendo em conta os anos bissextos. No fim subtrai-se a data inicial do programa (01/01/2019)*/
    return ((365 * ano + ano / 4 - ano / 100 + ano / 400 + (mes * 306 + 5) / 10 + ( dia - 1 )) * 24 *60) - DATA_INICIAL;
}


/* Algoritmo de ordenacao selection sort */
void selection_sort() { 
    int i, j, min_idx; 
  
    /* Um por um move os limites do array desordenado */
    for (i = 0; i < n_eventos; i++) { 
        /* Encontra o evento que comeca mais cedo no array desordenado */
        min_idx = i; 
        for (j = i+1; j < n_eventos; j++) 
            if (tab_eventos[j].data_m + tab_eventos[j].inicio_m < tab_eventos[min_idx].data_m + tab_eventos[min_idx].inicio_m ) {
                min_idx = j;
          }
            else if (tab_eventos[j].data_m + tab_eventos[j].inicio_m == tab_eventos[min_idx].data_m + tab_eventos[min_idx].inicio_m && tab_eventos[j].sala < tab_eventos[min_idx].sala) {
                min_idx = j;
            }
  
        /* Troca o evento que comeca primeiro com o que esta no primeiro lugar do array */
        swap(&tab_eventos[min_idx], &tab_eventos[i]); 
    } 
}


/* Troca dois elementos do array */
void swap(Evento *xp, Evento *yp) { 
    Evento temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 


/* Agenda um evento */
void adiciona_evento() {
    Evento temp = cria_evento();

    if (verifica_sobreposicao(temp, 'a')) {
        return;
    }
    else {
        tab_eventos[n_eventos] = temp;
        n_eventos++;
        selection_sort();
    }
}


/* Algoritmo de pesquisa binaria que retorna a primeira ocurrencia de um elemento no array */
int binary_search(int esq, int dir, int x) {
    if (dir >= esq) {
        int meio = esq + (dir - esq) / 2;

        /* Procura a primeira vez que ocorre uma determinada data no array */
        if ((meio == 0 || x > tab_eventos[meio - 1].data_m) && tab_eventos[meio].data_m == x) 
            return meio;

        else if (tab_eventos[meio].data_m < x) 
            return binary_search(meio + 1, dir, x);

        else
            return binary_search(esq, meio - 1, x);
    }
    return -1;
}


/* Verifica se um evento existe procurando pela sua descricao e retorna o indice do array onde encontra */
int verifica_evento(char descricao[]) {
    int i;

    for (i = 0; i < n_eventos; i++) {
        if (strcmp(tab_eventos[i].descricao, descricao) == 0) 
            return i;
    }
    printf("Evento %s inexistente.\n", descricao);
    return -1;
}


/* Verifica se dois eventos estao marcados na mesma sala */
int verifica_sala(Evento temp, Evento b) {
    if (temp.sala == b.sala) {
        printf("Impossivel agendar evento %s. Sala%d ocupada.\n", temp.descricao, temp.sala);
        return 1;
    }
    return 0;
}


/* Verifica se os participantes de um evento tambem participam noutro evento */
int verifica_participantes(Evento temp, Evento b, char comando) {
    int i, j, flag = 0;
    if (strcmp(temp.responsavel, b.responsavel) == 0) {
        printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", 
            temp.descricao, temp.responsavel);
        flag++;
    }
    for (i = 0; i < PARTICIPANTES; i++) {
        if (strcmp(temp.responsavel, b.participantes[i]) == 0) {
            if (comando == 'A') {
                printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n", 
                    temp.responsavel);
                return 1;
            }
            else {
                printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", 
                    temp.descricao, temp.responsavel);
                flag++;
            }
        }
        else if (strcmp(temp.participantes[i], b.responsavel) == 0) {
            if (comando == 'A') {
                printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n", 
                    temp.participantes[i]);
                return 1;
            }
            else {
                printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", 
                    temp.descricao, temp.participantes[i]);
                flag++;
            }
        }
        for (j = 0; j < PARTICIPANTES; j++) {
            if (strcmp(b.participantes[j], "") != 0) {
                if (strcmp(temp.participantes[i], b.participantes[j]) == 0) {
                    if (comando == 'A') {
                        printf("Impossivel adicionar participante. Participante %s tem um evento sobreposto.\n", 
                            temp.participantes[i]);
                        return 1;
                    }
                    else {
                        printf("Impossivel agendar evento %s. Participante %s tem um evento sobreposto.\n", 
                            temp.descricao, temp.participantes[i]);
                        flag++;
                    }
                }
            }
        }
    }
    return flag;
}


/* Verifica se existe conflito entre eventos */
int verifica_sobreposicao(Evento temp, char comando) {
    int i, counter = 0, flag = 0, j;

    if ((i = binary_search(0, n_eventos - 1, temp.data_m)) != -1) {
        int indices[EVENTOS];

        for (; tab_eventos[i].data_m == temp.data_m; i++) {
            if (strcmp(tab_eventos[i].descricao, temp.descricao) != 0) {
                indices[counter++] = i;
            }
        }
        
        for (j = 0; j < counter; j++) {

            if (tab_eventos[indices[j]].inicio_m + tab_eventos[indices[j]].duracao > temp.inicio_m 
            && temp.inicio_m + temp.duracao > tab_eventos[indices[j]].inicio_m) {

                switch (comando) {
                    case 's':
                        if (verifica_sala(temp, tab_eventos[indices[j]])) 
                            return 1;

                        break;

                    case 'A':
                        if (verifica_participantes(temp, tab_eventos[indices[j]], comando)) 
                            return 1;

                        break;

                    case 'a':
                        if (verifica_sala(temp, tab_eventos[indices[j]])) 
                            return 1;

                        break;
                }
            }
        }

        if (comando == 'a') {
            for (j = 0; j < counter; j++) {
                if (tab_eventos[indices[j]].inicio_m + tab_eventos[indices[j]].duracao > temp.inicio_m 
                && temp.inicio_m + temp.duracao > tab_eventos[indices[j]].inicio_m) {

                    if (verifica_participantes(temp, tab_eventos[indices[j]], comando)){
                        flag++;
                    }
                }
            }
        }   
    }
    return flag;
}


/* Imprime todos os eventos agendados */
void listar_todos() {
    int i;

    for (i = 0; i < n_eventos; i++) {
        mostra_evento(tab_eventos[i]);
    }
}


/* Imprime todos os eventos de uma sala escolhida pelo utilizador */
void listar_sala() {
    int i;
    char s[2];
    int sala;

    le_linha(s);
    sala = atoi(s);
    
    for (i = 0; i < n_eventos; i++) {
        if (tab_eventos[i].sala == sala) 
            mostra_evento(tab_eventos[i]);
    }
}


/* Muda a sala de um evento */
void mudar_sala() {
    char s[NOME + 2], descricao[NOME];
    int sala, i;
    le_linha(s);

    strcpy(descricao, strtok(s, ":"));
    sala = atoi(strtok(NULL, ":"));

    if ((i = verifica_evento(descricao)) != -1) {
        Evento temp = tab_eventos[i];
        temp.sala = sala;

        if (!(verifica_sobreposicao(temp, 's'))) {
            tab_eventos[i].sala = sala;
        }
    }
}


/* Muda a hora de inicio ou a duracao de um evento, mediante o comando que recebe */
void altera_hora(char comando) {
    char s[NOME + INICIO], descricao[NOME], tempo[INICIO];
    int tempo_m, i;

    le_linha(s);
    strcpy(descricao, strtok(s, ":"));
    strcpy(tempo, strtok(NULL, ":"));
    tempo_m = atoi(tempo);

    if ((i = verifica_evento(descricao)) != -1) {
        Evento temp = tab_eventos[i];
        
        if (comando == 'i') {
            temp.inicio_m = inicio_para_min(tempo_m);
        }
        else {
            temp.duracao = tempo_m;
        }

        if (!(verifica_sobreposicao(temp, 'a'))) {
            if (comando == 'i') {
                tab_eventos[i].inicio_m = inicio_para_min(tempo_m);
                strcpy(tab_eventos[i].inicio, tempo);
            }
            else {
                tab_eventos[i].duracao = tempo_m;
            }
            selection_sort();
        }
    }
}


/* Elimina um evento do array */
void apaga_evento() {
    char descricao[NOME];
    int i;

    le_linha(descricao);

    if ((i = verifica_evento(descricao)) != -1) {
        int j = i + 1;

        while (i < n_eventos && j < 1000) {
            tab_eventos[i++] = tab_eventos[j++];
        }
        n_eventos--;
    }
}


/* Adiciona um participante a um evento */
void adiciona_participante() {
    char s[NOME*2], descricao[NOME], participante[NOME];
    int i, j, n_participantes = 0;
    le_linha(s);

    strcpy(descricao, strtok(s, ":"));
    strcpy(participante, strtok(NULL, ":"));

    if ((i = verifica_evento(descricao)) != -1) {
        for (j = 0; j < PARTICIPANTES; j++) {
            if (strcmp(tab_eventos[i].participantes[j], "") != 0) {
                n_participantes++;
                }
            }

            if (n_participantes < 3) {
                Evento temp = tab_eventos[i];
                for (j = 0; j < PARTICIPANTES; j++) {
                    if (strcmp(tab_eventos[i].participantes[j], participante) == 0) 
                        return;
                    
                    else if (strcmp(temp.participantes[j], "") == 0) {
                        strcpy(temp.participantes[j], participante);
                        break;
                    }
                }
                if (!(verifica_sobreposicao(temp, 'A'))) {
                    strcpy(tab_eventos[i].participantes[j], participante);
                }
            }
            else printf("Impossivel adicionar participante. Evento %s ja tem 3 participantes.\n", descricao);
    }

}


/* Remove um participante de um evento */
void remove_participante() {
    char s[NOME*2], descricao[NOME], participante[NOME];
    int i, j, n_participantes = 0, existe = 0;
    le_linha(s);

    strcpy(descricao, strtok(s, ":"));
    strcpy(participante, strtok(NULL, ":"));

    if ((i = verifica_evento(descricao)) != -1) {
        for (j = 0; j < PARTICIPANTES; j++) {
            if (strcmp(tab_eventos[i].participantes[j], "") != 0) {
                n_participantes++;
                if (strcmp(tab_eventos[i].participantes[j], participante) == 0)
                    existe = 1;
            }
        }
        if (n_participantes > 1) {
            for (j = 0; j < PARTICIPANTES; j++) {
                if (strcmp(tab_eventos[i].participantes[j], participante) == 0 && existe) {
                    strcpy(tab_eventos[i].participantes[j], "");
                    return;
                }
            }
        }
        else if (!existe) 
            return;

        else {
            printf("Impossivel remover participante. Participante %s e o unico participante no evento %s.\n", 
                participante, descricao);
        }
    }
} 
