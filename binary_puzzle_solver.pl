% Ricardo Andrade 93750.

:- use_module(library(clpfd)).
:-[codigo_comum].

%-----------------------------------------------------------------------------
% FUNCOES AUXILIARES:
% count_var(Lista, Num), count_num(Lista, Num Elem).
%-----------------------------------------------------------------------------

% Num e' o numero de variaveis na lista.
count_var([], 0).
count_var([H | T], Num) :-
    var(H),
    !,
    count_var(T, Num1),
    Num is Num1 + 1.

% Se o elemento nao e uma variavel
count_var([_ | T], Num) :- 
    count_var(T, Num).

%-----------------------------------------------------------------------------

% Num e' o numero de elementos da lista iguais a Elem
count_num([], 0, _).
count_num([H | T], Num, Elem) :-
    H == Elem,
    !,
    count_num(T, Num1, Elem),
    Num is Num1 + 1.

count_num([_| T], Num, Elem) :-
    count_num(T, Num, Elem).

%-----------------------------------------------------------------------------
% aplica_R1_triplo(Triplo, N_Triplo):
% N_Triplo e' o resultado de aplicar a regra 1 a Triplo.
%-----------------------------------------------------------------------------

% Triplo e' Triplo se 2 ou mais elementos forem variaveis.
aplica_R1_triplo(Triplo, Triplo) :-
    count_var(Triplo, Num),
    Num >= 2,
    !.

% Triplo e' Triplo se os seus elementos nao forem todos iguais.
aplica_R1_triplo(Triplo, Triplo) :-
    Triplo \= [A,A,A],
    !.

% Devolve o Triplo alterando Z para 0 ou 1.
aplica_R1_triplo([A, A, X], [A, A, Z]) :-
    var(X),
    Z is abs(A - 1), 
    !.

% Devolve o Triplo alterando Z para 0 ou 1.
aplica_R1_triplo([A, X, A], [A, Z, A]) :-
    var(X), 
    Z is abs(A - 1),
    !.

% Devolve o Triplo alterando Z para 0 ou 1.
aplica_R1_triplo([X, A, A], [Z, A, A]) :-
    var(X), 
    Z is abs(A - 1),
    !.


%-----------------------------------------------------------------------------
% aplica_R1_fila_aux(Fila, N_Fila):
% N_Fila e' a fila resultante de aplicar a regra 1 a Fila uma vez.
%-----------------------------------------------------------------------------
aplica_R1_fila_aux([X, Y], [X, Y]).
aplica_R1_fila_aux([X, Y, Z | Tail], [Xs | N_Fila]) :-
    aplica_R1_triplo([X, Y, Z], [Xs, Ys, Zs]),
    aplica_R1_fila_aux([Ys, Zs | Tail], N_Fila),
    !.


%-----------------------------------------------------------------------------
% aplica_R1_fila(Fila, N_Fila):
% N_Fila e' a fila resultante de aplicar a regra 1 a Fila.
%-----------------------------------------------------------------------------
aplica_R1_fila(Fila, N_Fila) :-
    aplica_R1_fila_aux(Fila, N_Fila1),
    Fila \== N_Fila1,
    !,
    aplica_R1_fila(N_Fila1, N_Fila).

aplica_R1_fila(Fila, N_Fila) :-
    aplica_R1_fila_aux(Fila, N_Fila).


%-----------------------------------------------------------------------------
% aplica_R2_fila(Fila, N_Fila):
% N_Fila e' a fila resultante de aplicar a regra 2 a Fila.
%-----------------------------------------------------------------------------
aplica_R2_fila(Fila, N_Fila) :-
    length(Fila, Len),
    count_num(Fila, Num, 0),
    Num =:= Len / 2,
    aplica_R2_fila_aux(Fila, N_Fila, 1),
    !.

aplica_R2_fila(Fila, N_Fila) :-
    length(Fila, Len),
    count_num(Fila, Num, 1),
    Num =:= Len / 2,
    aplica_R2_fila_aux(Fila, N_Fila, 0),
    !.

aplica_R2_fila(Fila, Fila) :-
    length(Fila, Len), 
    count_num(Fila, Num0, 0),
    Num0 < (Len / 2),     % Numero de 0 e' menor que metade do numero de elementos da fila
    count_num(Fila, Num1, 1),
    Num1 < (Len / 2).     % Numero de 1 e' menor que metade do numero de elementos da fila


% Substitui variaveis por Num.
aplica_R2_fila_aux([], [], _).
aplica_R2_fila_aux([H | T], [Num | N_Fila], Num) :-
    var(H),
    aplica_R2_fila_aux(T, N_Fila, Num),
    !.

aplica_R2_fila_aux([H | T], [H | N_Fila], Num) :-
    aplica_R2_fila_aux(T, N_Fila, Num).


%-----------------------------------------------------------------------------
% aplica_R1_R2_fila(Fila, N_Fila):
% N_Fila e' a fila resultante de aplicar as regra 1 e 2 a Fila.
%-----------------------------------------------------------------------------
aplica_R1_R2_fila(Fila, N_Fila) :-
    aplica_R1_fila(Fila, N_Fila1),
    aplica_R2_fila(N_Fila1, N_Fila).


%-----------------------------------------------------------------------------
% aplica_R1_R2_puzzle(Puz, N_Puz):
% N_Puz e' o puzzle resultante de aplicar o predicado aplica_R1_R2_fila
% 'as linhas e 'as colunas de Puz.
%-----------------------------------------------------------------------------
aplica_R1_R2_puzzle(Puz, N_Puz) :-
    aplica_R1_R2_puzzle_fila(Puz, N_Puz1),
    aplica_R1_R2_puzzle_colunas(N_Puz1, N_Puz).


% Aplica o predicado_R1_R2_fila a todos as filas do puzzle.
aplica_R1_R2_puzzle_fila([], []).
aplica_R1_R2_puzzle_fila([H | T], [N_Fila | N_Puz]) :-
    aplica_R1_R2_fila(H, N_Fila),
    aplica_R1_R2_puzzle_fila(T, N_Puz).

% Aplica o predicado_R1_R2_fila a todos as colunas do puzzle.
aplica_R1_R2_puzzle_colunas([], []).
aplica_R1_R2_puzzle_colunas(Puz, N_Puz) :-
    transpose(Puz, T_Puz),
    aplica_R1_R2_puzzle_fila(T_Puz, N_Puz1),
    transpose(N_Puz1, N_Puz).


%-----------------------------------------------------------------------------
% inicializa(Puz, N_Puz):
% N_Fila e' o puzzle resultante de inicializar Puz.
%-----------------------------------------------------------------------------
inicializa(Puz, N_Puz) :-
    aplica_R1_R2_puzzle(Puz, N_Puz1),
    Puz \== N_Puz1,
    !,
    inicializa(N_Puz1, N_Puz).

inicializa(Puz, N_Puz) :-
    aplica_R1_R2_puzzle(Puz, N_Puz).


%-----------------------------------------------------------------------------
% verifica_R3(Puz):
% No puzzle Puz todas as linhas sao diferentes entre si e
% todas as colunas sao diferente entre si.
%-----------------------------------------------------------------------------
verifica_R3(Puz) :-
    verifica_R3_aux(Puz),
    transpose(Puz, T_Puz),
    verifica_R3_aux(T_Puz).


% Compara Fila com todas as filas do puzzle.
verifica_R3_linhas(_, []).
verifica_R3_linhas(Fila, [H | T]) :-
    Fila \== H,
    verifica_R3_linhas(Fila, T).


% Itera pelo puzzle para comparar todas as linhas/colunas.
verifica_R3_aux([]).
verifica_R3_aux([H | T]) :-
    verifica_R3_linhas(H, T),
    verifica_R3_aux(T).


%-----------------------------------------------------------------------------
% propaga_posicoes(Lista, Puz, N_Puz):
% N_Puz e' o resultado de propagar recursivamente, 
% (as mudancas de) as posicoes de Posicoes.
%-----------------------------------------------------------------------------
propaga_posicoes([], Puz, Puz).
propaga_posicoes([(F, C) | T], Puz, N_Puz) :-
    nth1(F, Puz, Fila),
    aplica_R1_R2_fila(Fila, N_Fila),
    mat_elementos_coluna(Puz, C, Coluna),
    aplica_R1_R2_fila(Coluna, N_Coluna),
    verfica_alteracoes(Fila, N_Fila, 1, Lista),
    verfica_alteracoes(Coluna, N_Coluna, 1, Lista1),
    cria_tuplo_fila(F, Lista, Coord1),
    cria_tuplo_coluna(C, Lista1, Coord2),
    mat_muda_linha(Puz, F, N_Fila, N_Puz1),
    mat_muda_coluna(N_Puz1, C, N_Coluna, N_Puz2),
    verifica_R3(N_Puz2),
    append(Coord1, Coord2, Coord3),
    append(Coord3, T, Coord),
    !,
    propaga_posicoes(Coord, N_Puz2, N_Puz).    


% Lista e' uma lista de tuplos com posicoes onde houve alteracoes.
verfica_alteracoes([], [], _, []).
verfica_alteracoes([H | T], [New_H | New_T], Count, Lista) :-
    H =@= New_H,
    Count1 is Count + 1,
    !,
    verfica_alteracoes(T, New_T, Count1, Lista).

verfica_alteracoes([H | T], [New_H | New_T], Count, [Count | Lista]) :-
    H \=@= New_H,
    Count1 is Count + 1,
    !,
    verfica_alteracoes(T, New_T, Count1, Lista).


cria_tuplo_fila(_, [], []).
cria_tuplo_fila(F, [C | T], [(F, C) | Lista]) :-
    !,
    cria_tuplo_fila(F, T, Lista).

cria_tuplo_coluna(_, [], []).
cria_tuplo_coluna(C, [F | T], [(F, C) | Lista]) :-
    !,
    cria_tuplo_coluna(F, T, Lista).


%-----------------------------------------------------------------------------
% resolve(Puz, Sol):
% Sol e' uma solucao do puzzle Puz.
%-----------------------------------------------------------------------------
resolve(Puz, N_Puz) :-
    aplica_R1_R2_puzzle(Puz, N_Puz),
    flatten(N_Puz, Puz_F),
    count_var(Puz_F, Num),
    Num == 0,
    !.
    
resolve(Puz, Sol) :-
    aplica_R1_R2_puzzle(Puz, N_Puz),
    tenta_numero(N_Puz, N_Puz1),
    !,
    resolve(N_Puz1, Sol).


% Substitui uma variavel por 0.
tenta_numero(Puz, N_Puz) :-
    encontra_var(Puz, (1,1), (F, C)),
    mat_muda_posicao(Puz, (F, C), 0, N_Puz1),
    propaga_posicoes([(F, C)], N_Puz1, N_Puz).

% Substitui uma variavel por 1.
tenta_numero(Puz, N_Puz) :-
    encontra_var(Puz, (1,1), (F, C)),
    mat_muda_posicao(Puz, (F, C), 1, N_Puz1),
    propaga_posicoes([(F, C)], N_Puz1, N_Puz).


% (F, C) e' o ponto na matriz onde esta a primeira variavel.
encontra_var(Puz, (F, C), (F, C)) :-
    mat_ref(Puz, (F, C), Cont),
    var(Cont),
    !.

encontra_var(Puz, (F, C), (FF, CC)) :-
    mat_ref(Puz, (F, C), Cont),
    nonvar(Cont),
    length(Puz, Len),
    C < Len,
    C1 is C + 1,
    !,
    encontra_var(Puz, (F, C1), (FF, CC)).

encontra_var(Puz, (F, C), (FF, CC)) :-
    mat_ref(Puz, (F, C), Cont),
    nonvar(Cont),
    length(Puz, Len),
    C >= Len,
    F1 is F + 1,
    C1 is 1,
    !,
    encontra_var(Puz, (F1, C1), (FF, CC)).
