# 93750 - Ricardo Andrade


def cria_celula(valor):
    """
    cria_celula: {1, 0, -1} --> celula
    Cria a celula com o valor de estado de uma celula do qubit
    Representacao interna: lista
    """
    if valor not in range(-1, 2):
        raise ValueError('cria_celula: argumento invalido.')
    return [valor]


def obter_valor(celula):
    """
    obter_valor: celula --> {1, 0, -1}
    Devolve o valor de estado de uma celula do qubit
    """
    return celula[0]


def inverte_estado(celula):
    """
    inverte_estado: celula --> celula
    Inverte o estado da celula
    Se a celula estiver ativa(1) torna-se inativa(0)
    Se a celula estiver inativa(0) torna-se ativa(0)
    Se a celula estiver em estado incerto(-1) mantem-se nesse estado
    """
    if obter_valor(celula) == 1:
        celula[0] = 0
    elif obter_valor(celula) == 0:
        celula[0] = 1
    return celula


def eh_celula(universal):
    """
    eh_celula: universal --> logico
    Verifica se o argumento e do tipo celula
    """
    return isinstance(universal, list) and len(universal) == 1 and \
           obter_valor(universal) in range(-1, 2)


def celulas_iguais(celula1, celula2):
    """
    celulas_iguais: celula x celula --> logico
    Verifica se as celulas estao no mesmo estado
    """
    return eh_celula(celula1) and eh_celula(celula2) and celula1 == celula2


def celula_para_str(celula):
    """
    celula_para_str: celula --> cad. caracteres
    Devolve a cadeia de caracteres que representa o estado da celula
    Se o estado da celula for incerto(-1) devolve x
    """
    return str(obter_valor(celula)).replace('-1', 'x')


def cria_coordenada(linha, coluna):
    """
    cria_coordenada: numero x numero --> coordenada
    Cria a coordenada correspondente a linha e a coluna se os seus
    valores pertencerem a {0, 1, 2}
    Representacao interna: tuplo
    """
    if linha not in range(0, 3) or coluna not in range(0, 3):
        raise ValueError('cria_coordenada: argumentos invalidos.')
    return linha, coluna


def coordenada_linha(coordenada):
    """
    coordenada_linha: coordenada --> numero
    Devolve a linha da coordenada
    """
    return coordenada[0]


def coordenada_coluna(coordenada):
    """
    coordenada_coluna: coordenada --> numero
    Devolve a coluna da coordenada
    """
    return coordenada[1]


def eh_coordenada(universal):
    """
    eh_coordenada: universal --> logico
    Verifica se o argumento e do tipo coordenada
    """
    return isinstance(universal, tuple) and len(universal) == 2 and \
           coordenada_coluna(universal) in range(0, 3) and \
           coordenada_linha(universal) in range(0, 3)


def coordenadas_iguais(coordenada1, coordenada2):
    """
    coordenadas_iguais: coordenada x coordenada --> logico
    Verifica se as coordenadas sao iguais ou sejam representam a mesma posicao
    """
    return eh_coordenada(coordenada1) and eh_coordenada(coordenada2) and \
           coordenada1 == coordenada2


def coordenada_para_str(coordenada):
    """
    coordenada_para_str: coordenada x coordenada --> logico
    Devolve a cadeia de caracteres que representa a coordenada
    """
    return str((coordenada_linha(coordenada), coordenada_coluna(coordenada)))


def cria_tabuleiro(valores):
    """
    cria_tabuleiro: lista --> tabuleiro
    Recebe os valores a associar as coordenadas de um tabuleiro e
    devolve o respetivo tabuleiro
    Representacao interna: dicionario
    """
    chaves = [cria_coordenada(i, j) for i in range(0, 3) for j in range(0, 3)]  # cria lista com as chaves do tabuleiro
    chaves.remove(cria_coordenada(2, 0))  # remove a coordenada(2,0) pois esta nao existe no tabuleiro
    celula = [cria_celula(valor) for valor in valores]  # cria lista com os valores a associar as chaves do tabuleiro
    return dict(zip(chaves, celula))


def tabuleiro_inicial():
    """
    tabuleiro_inicial: {} --> tabuleiro
    Devolve o tabuleiro inicial do jogo
    """
    valores = [-1, -1, -1, 0, 0, -1, 0, -1]  # valores das celulas do tabuleiro inicial
    return cria_tabuleiro(valores)


def str_para_tabuleiro(caracteres):
    """
    str_para_tabuleiro: cad. caracteres --> tabuleiro
    Devolve o tabuleiro correspondente a cadeia de caracteres recebida
    """
    def verifica_tuplos(tab):
        # nesta funcao fazemos a verificacao do tipo e dos valores do input
        return isinstance(tab, tuple) and len(tab) == 3 and \
               all([isinstance(tab[i], tuple) for i in range(len(tab))]) and \
               len(tab[0]) == 3 and len(tab[1]) == 3 and len(tab[2]) == 2

    if not isinstance(caracteres, str) or not verifica_tuplos(eval(caracteres)):
        raise ValueError('str_para_tabuleiro: argumento invalido.')
    tabuleiro = eval(caracteres)
    valores = []
    for tuplo in range(len(tabuleiro)):
        for elemento in tabuleiro[tuplo]:
            if elemento not in range(-1, 2):
                raise ValueError('str_para_tabuleiro: argumento invalido.')
            valores += [elemento]
            # caso todos os elementos do input tenham valores validos criamos uma lista com valores para criar a representacao interna do tabuleiro
    return cria_tabuleiro(valores)


def tabuleiro_dimensao(tabuleiro):
    """
    tabuleiro_dimensao: tabuleiro --> numero
    Devolve 3
    """
    return 3


def tabuleiro_celula(tabuleiro, coordenada):
    """
    tabuleiro_celula: tabuleiro x coordenada --> celula
    Devolve a celula localizada na coordenada do tabuleiro recebido
    """
    return tabuleiro[coordenada]


def eh_tabuleiro(universal):
    """
    eh_tabuleiro: universal --> logico
    Verifica se o argumento e do tipo tabuleiro
    """
    def verifica_valores(valores):
        # verificar se todos os valores do dicionario sao celulas
        return all([eh_celula(e) for e in list(valores)])

    def verifica_chaves(chaves_tab):
        # verificar se todos os valores do dicionario sao coordenadas
        return all([eh_coordenada(e) for e in list(chaves_tab)])

    return isinstance(universal, dict) and verifica_chaves(universal.keys()) and verifica_valores(universal.values())


def tabuleiro_substitui_celula(tabuleiro, celula, coordenada):
    """
    tabuleiro_substitui_celula: tabuleiro x celula x coordenada --> tabuleiro
    Substitui a celula localizada na coordenada do tabuleiro recebido
    pela nova celula e devolve o tabuleiro modificado
    """
    if not eh_celula(celula) or not eh_coordenada(coordenada) or not eh_tabuleiro(tabuleiro):
        raise ValueError('tabuleiro_substitui_celula: argumentos invalidos.')
    tabuleiro[coordenada] = celula
    return tabuleiro


def tabuleiro_inverte_estado(tabuleiro, coordenada):
    """
    tabuleiro_inverte_estado: tabuleiro x coordenada --> tabuleiro
    Inverte o estado da celula localizada na coordenada do tabuleiro recebido
    pela nova celula e devolve o tabuleiro modificado
    """
    if not eh_coordenada(coordenada) or not eh_tabuleiro(tabuleiro):
        raise ValueError('tabuleiro_inverte_estado: argumentos invalidos.')
    tabuleiro[coordenada] = inverte_estado(tabuleiro[coordenada])
    return tabuleiro


def tabuleiros_iguais(tabuleiro1, tabuleiro2):
    """
    tabuleiro_iguais: tabuleiro x tabuleiro -- > logico
    Verifica se os tabuleiros sao iguais ou seja se possuem as mesmas celulas
    """
    return eh_tabuleiro(tabuleiro1) and eh_tabuleiro(tabuleiro2) and tabuleiro1 == tabuleiro2


def tabuleiro_para_str(tabuleiro):
    """
    tabuleiro_para_str: tabuleiro --> cad. caracteres
    Devolve a cadeia de caracteres que representa o tabuleiro
    """
    linha = '+-------+\n'
    linha += '|...{}...|\n'.format(celula_para_str(tabuleiro[cria_coordenada(0, 2)]))
    linha += '|..{}.{}..|\n'.format(celula_para_str(tabuleiro[cria_coordenada(0, 1)]), celula_para_str(tabuleiro[cria_coordenada(1, 2)]))
    linha += '|.{}.{}.{}.|\n'.format(celula_para_str(tabuleiro[cria_coordenada(0, 0)]), celula_para_str(tabuleiro[cria_coordenada(1, 1)]), celula_para_str(tabuleiro[cria_coordenada(2, 2)]))
    linha += '|..{}.{}..|\n'.format(celula_para_str(tabuleiro[cria_coordenada(1, 0)]), celula_para_str(tabuleiro[cria_coordenada(2, 1)]))
    linha += '+-------+'
    return linha


def portas_x_z(porta, tabuleiro, lado):
    """
    portas_x_z: porta x tabuleiro x lado --> tabuleiro
    Devolve o tabuleiro resultante da aplicacao da porta escolhida
    """
    if lado == 'E':
        i = 1 if porta == 'X' else 0
        for j in range(0, 3):
            tabuleiro_inverte_estado(tabuleiro, cria_coordenada(i, j))
        return tabuleiro
    elif lado == 'D':
        j = 1 if porta == 'X' else 2
        for i in range(0, 3):
            tabuleiro_inverte_estado(tabuleiro, cria_coordenada(i, j))
        return tabuleiro


def porta_x(tabuleiro, lado):
    """
    porta_x: tabuleiro x {'E', 'D'} --> tabuleiro
    Devolve o tabuleiro resultante de aplicar a porta X ao qubit esquerdo ou direito
    """
    if not eh_tabuleiro(tabuleiro) or lado not in ('D', 'E'):
        raise ValueError('porta_x: argumentos invalidos.')
    return portas_x_z('X', tabuleiro, lado)


def porta_z(tabuleiro, lado):
    """
    porta_z: tabuleiro x {'E', 'D'} --> tabuleiro
    Devolve o tabuleiro resultante de aplicar a porta Z ao qubit esquerdo ou direito
    """
    if not eh_tabuleiro(tabuleiro) or lado not in ('D', 'E'):
        raise ValueError('porta_z: argumentos invalidos.')
    return portas_x_z('Z', tabuleiro, lado)


def porta_h(tabuleiro, lado):
    """
    porta_h: tabuleiro x {'E', 'D'} --> tabuleiro
    Devolve o tabuleiro resultante de aplicar a porta H ao qubit esquerdo ou direito
    """
    if not eh_tabuleiro(tabuleiro) or lado not in ('D', 'E'):
        raise ValueError('porta_h: argumentos invalidos.')
    elif lado == 'E':
        for i in range(0, 3):
            tabuleiro[cria_coordenada(0, i)], tabuleiro[cria_coordenada(1, i)] = \
                tabuleiro[cria_coordenada(1, i)], tabuleiro[cria_coordenada(0, i)]
    elif lado == 'D':
        for i in range(0, 3):
            tabuleiro[cria_coordenada(i, 1)], tabuleiro[cria_coordenada(i, 2)] = \
                tabuleiro[cria_coordenada(i, 2)], tabuleiro[cria_coordenada(i, 1)]
    return tabuleiro


def hello_quantum(caracteres):
    """
    hello_quantum: cad.caracteres --> logico
    Recebe a cadeia de caracteres que contem o tabuleiro objetivo
    e o numero de jogadas para o alcancar
    Devolve True se o jogador chegar ao tabuleiro objetivo com
    numero igual ou inferior ao numero limite de jogadas e
    devolve False caso contrario
    """
    def escolhe_porta(tipo_porta, tabuleiro, lado_escolhido):
        # esta funcao aplica as portas escolhidas pelo jogador
        if tipo_porta == 'X':
            return porta_x(tabuleiro, lado_escolhido)
        elif tipo_porta == 'Z':
            return porta_z(tabuleiro, lado_escolhido)
        elif tipo_porta == 'H':
            return porta_h(tabuleiro, lado_escolhido)

    # inicializacao das variaveis para o jogo
    caracteres = caracteres.split(':')
    tabuleiro_objetivo = str_para_tabuleiro(caracteres[0])
    num_max_jogadas = eval(caracteres[1])
    num_jogadas = 0
    tabuleiro_de_jogo = tabuleiro_inicial()

    print('Bem-vindo ao Hello Quantum!\nO seu objetivo e chegar ao tabuleiro:\n'
          + tabuleiro_para_str(tabuleiro_objetivo) +
          '\nComecando com o tabuleiro que se segue:\n'
          + tabuleiro_para_str(tabuleiro_de_jogo))

    while num_jogadas < num_max_jogadas and not tabuleiros_iguais(tabuleiro_de_jogo, tabuleiro_objetivo):
    # e neste ciclo que o jogo e jogado, enquanto o limite de jogadas nao for alcancado
        porta = input('Escolha uma porta para aplicar (X, Z ou H): ')
        lado = input('Escolha um qubit para analisar (E ou D): ')
        escolhe_porta(porta, tabuleiro_de_jogo, lado)
        print(tabuleiro_para_str(tabuleiro_de_jogo))
        num_jogadas += 1  # contador das jogadas do jogador

    if tabuleiros_iguais(tabuleiro_de_jogo, tabuleiro_objetivo):
        print('Parabens, conseguiu converter o tabuleiro em {} jogadas!'.format(num_jogadas))

    return tabuleiros_iguais(tabuleiro_de_jogo, tabuleiro_objetivo)
