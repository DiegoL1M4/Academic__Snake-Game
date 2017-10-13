#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>

// --------------------- Valores Constantes ----------------------
#define tam1 10         //tamanho da arena altura
#define tam2 15         //tamanho da arena comprimento
#define cobra 100       //tamanho da snake
#define inicio 3        //coordenada de inicio da snake
#define duracaoDoce (tam1+tam2)/2  //tempo de duração do doce

#define cabeca 'x'      //caractere designado a ser a cabeça da snake
#define corpo 'o'       //caractere designado a ser o corpo da snake
#define calda '+'       //caractere designado a ser a calda da snake
#define alimento '*'    //caractere designado a ser o alimento da snake
#define aliEspecial '%' //caractere designado a ser o alimento especial
#define refeicao '#'    //caractere designado a ser o alimento na barriga
#define bocaCheia 'X'   //caractere designado a ser o alimento na boca
// ---------------------------------------------------------------



// ---------------------- PACK DE ESTRUTURAS ---------------------
struct posicao{
    int i;
    int j;
};

struct movimentacao{
    int i;
    int j;
    char orientacao;
};
// ---------------------------------------------------------------



// ---------------------- Variáveis Globais ---------------------
char matriz[tam1][tam2];  //matriz da arena do jogo
struct posicao *snake;  //guarda as posições da snake
struct posicao doce;    //guarda a posição do doce na matriz
struct posicao doceEsp; //guarda a posição do doce na matriz
// ---------------------------------------------------------------



// ----------------------- PACK DE FUNÇÕES -----------------------
void iniciarConfiguracoes(int *snakeTam,char crescimento[][tam2],struct movimentacao mov[]){
    int i , j , k;

    for(i=0; i<tam1; i++) // Preenchimento da Matriz da Arena e da Matriz Crescimento
        for(j=0; j<tam2; j++){
            matriz[i][j] = ' ';
            crescimento[i][j] = 'n';}

    for(k=0,i=0,j=0; k<tam1*tam2; k++,j++){ // Preenchimento do Vetor de Movimentação
        if(j>=tam2){
            j=0;
            i++;}
        mov[k].i = i;
        mov[k].j = j;
        mov[k].orientacao = '0';}

    for(i=0; i<*snakeTam; i++){ // Preenchimento das Posições no Array (Snake)
        snake[i].i = inicio;
        snake[i].j = inicio+i;}

    // Retirar posição (0,0) do doce da matriz
    doce.i=tam1; doce.j=tam2;
    doceEsp.i=tam1; doceEsp.j=tam2;
}

void exibirMatriz(int snakeTam,char crescimento[][tam2],int pontos){
    int i , j;
    printf("\n   --SNAKE GAME--\n");
    printf("   Tamanho: %d\n",snakeTam);
    printf("   Pontuação: %d\n\n",pontos);
    for(i=0; i<tam1; i++){
        printf("   |");
        for(j=0; j<tam2; j++)
            if(crescimento[i][j] == 's' && i==snake[0].i && j==snake[0].j)
                printf("%c",bocaCheia); //quando a boca da snake fica em cima do doce
            else if(crescimento[i][j] == 's')
                printf("%c",refeicao); //quando a snake passa por cima de um doce
            else
                printf("%c",matriz[i][j]); //demais posições
        printf("|\n");}
}

int ponteiroNulo(struct posicao aloc[]){
    if(aloc==NULL)
        return 1;
    return 0;
}

void alteraPosicao(char *P, char aux, struct movimentacao mov[], int snakeTam, int *doceEspTempo, int doceEspExiste){
    int i=0 , block=0;
    while(i<snakeTam && block!=1){
        char comando = orienta(mov,snake[i].i,snake[i].j); //adquire o comando de orientação da matriz
        matriz[snake[i].i][snake[i].j] = ' '; //deleta antiga posição

        int colisao = verificacao(3,snake[0].i,snake[0].j,snakeTam-1); //define se houve encontro com a propria snake

        switch(comando){
        case 'w':
            if(colisao==1) *P = 'q';
            if(snake[i].i==0)
                snake[i].i=tam1-1;
            else
                snake[i].i-=1;
            if(snake[i].i==snake[i+1].i){
                snake[i].i+=1;
                block=1;}
            break;
        case 's':
            if(colisao==1) *P = 'q';
            if(snake[i].i==tam1-1)
                snake[i].i=0;
            else
                snake[i].i+=1;
            if(snake[i].i==snake[i+1].i){
                snake[i].i-=1;
                block=1;}
            break;
        case 'a':
            if(snake[i].j==0 || colisao==1) *P = 'q';
            snake[i].j-=1;
            if(snake[i].j==snake[i+1].j){
                snake[i].j+=1;
                block=1;}
            break;
        case 'd':
            if(snake[i].j==tam2-1 || colisao==1) *P = 'q';
            snake[i].j+=1;
            if(snake[i].j==snake[i+1].j){
                snake[i].j-=1;
                block=1;}
            break;
        default:
            if(snake[i].j!=0) //movimento padrão para a esquerda
                snake[i].j-=1;
        }
        matriz[snake[i].i][snake[i].j] = corpo; //nova posição
        i++; //incrementação do ciclo while
    }
    if(block!=1 && doceEspExiste==1) (*doceEspTempo)++; //contagem das movimentações da cobra para a vida do doce
    if(block==1) *P = aux;
    matriz[snake[0].i][snake[0].j] = cabeca; //exceção da cabeça
    matriz[snake[snakeTam-1].i][snake[snakeTam-1].j] = calda; //exceção da calda
}

int orienta(struct movimentacao mov[],int Pi,int Pj){ //resgata qual a orientação da posição
    int i;
    for(i=0; i<tam1*tam2; i++){
        if(Pi==mov[i].i && Pj==mov[i].j){
            char res = mov[i].orientacao;
            return res;}
    }
}

int verificacao(int k,int Pi,int Pj,int N){ //verifica as posições da snake
    for(k; k<N; k++)
        if(Pi==snake[k].i && Pj==snake[k].j)
            return 1;
    return 0;
}

void gerarDoce(int *doceExiste,int snakeTam){
    doce.i = rand() % tam1;
    doce.j = rand() % tam2;
    while(verificacao(0,doce.i,doce.j,snakeTam) || verificacao(0,doceEsp.i,doceEsp.j,1)){ //refaz se o doce ficar em cima da snake
        doce.i = rand() % tam1;
        doce.j = rand() % tam2;
    }
    matriz[doce.i][doce.j] = alimento;
    *doceExiste = 1;
}

void gerarDoceEsp(int *doceEspExiste,int snakeTam){
    doceEsp.i = rand() % tam1;
    doceEsp.j = rand() % tam2;
    while(verificacao(0,doceEsp.i,doceEsp.j,snakeTam) || verificacao(0,doceEsp.i,doceEsp.j,1)){ //refaz se o doce ficar em cima da snake
        doceEsp.i = rand() % tam1;
        doceEsp.j = rand() % tam2;
    }
    matriz[doceEsp.i][doceEsp.j] = aliEspecial;
    *doceEspExiste = 1;
}

void apagaDoce(){
    matriz[doceEsp.i][doceEsp.j] = ' ';
    doceEsp.i=tam1;
    doceEsp.j=tam2;
}
// ---------------------------------------------------------------



// ----------------------- MÉTODO PRINCIPAL ----------------------
int main(){
    char opcao; //escolha do menu
    int k , l; //variáveis de assistencia
    setlocale(LC_ALL,""); //permissão para adicionar acentuação gráfica

    do{
        // Declaração de Variáveis e Vetores
        char P = 'a', aux;     //variável que guarda a orientação da snake
        char dificuldade;      //nivel de dificuldade (aumento de velocidade)
        int pontos = 0;        //registro de pontos
        int snakeTam = 3;      //variável que define o tamanho da snake
        int acrescimo = 0;     //variável que ativa o crescimento da snake
        int velocidade = 450;  //variável que define a velocidade
        int doceExiste = 0;    //variável que controla a criação de um novo doce
        int doceEspExiste = 0; //variável que controla a criação de um novo doce especial
        int doceEspTempo = 0;  //variável que conta a vida do doce
        char crescimento[tam1][tam2]; //matriz que define se em uma posição haverá o crescimento da snake
        struct movimentacao *mov = malloc(tam1*tam2 * sizeof(struct movimentacao)); //posições da movimentação e sua orientação
        int ponteiro = ponteiroNulo(mov); //variável que verifica a alocação dinâmica
        if(!ponteiro){ //evita não dar certo no mov e dar certo no snake.
            snake = malloc(snakeTam * sizeof(struct posicao)); //posições da snake
            ponteiro = ponteiroNulo(snake);}
        struct posicao ultimo;

        do{
            // Menu de Opções
            system("cls");
            printf("\n   --SNAKE GAME--\n\n");
            printf("   Selecione uma opção:\n");
            printf("   (1) - Novo Jogo\n");
            printf("   (2) - Continuar Jogo\n");
            printf("   (3) - Pontuações\n");
            printf("   (0) - Sair\n");
            opcao = getch();

            if(opcao=='1'){
                system("cls");
                printf("\n   --SNAKE GAME--\n\n");
                printf("   Dificuldade:\n");
                printf("   (1) - Normal\n");
                printf("   (2) - Difícil\n");
                dificuldade = getch();
            }
            if(opcao=='3'){
                system("cls");
                printf("\n   --SNAKE GAME--\n\n");
                printf("   Melhores Pontuações:\n");
                FILE *arquivo;
                arquivo = fopen("pontuacao.txt", "r");
                for(k=1; k<=5; k++){
                    char nome[20]; int num;
                    fscanf(arquivo,"%s\n",&nome);
                    fscanf(arquivo,"%d\n",&num);
                    printf("   %i - %s: %i pontos\n",k,nome,num);}
                close(arquivo);
                printf("\n");
                system("pause");
            }

        }while(opcao!='1' && opcao!='2' && opcao!='0');

        // Configurações
        if(!ponteiro) //execução somente com alocações corretas
            iniciarConfiguracoes(&snakeTam,crescimento,mov);

        if(!ponteiro && opcao=='2'){
            FILE *arquivo;
            arquivo = fopen("saveGame.txt", "r");
            fscanf(arquivo,"%c\n",&P);
            fscanf(arquivo,"%c\n",&dificuldade);
            fscanf(arquivo,"%d\n",&pontos);
            fscanf(arquivo,"%d\n",&snakeTam);
            fscanf(arquivo,"%d\n",&acrescimo);
            fscanf(arquivo,"%d\n",&velocidade);
            fscanf(arquivo,"%d\n",&doceExiste);
            fscanf(arquivo,"%d\n",&doceEspExiste);
            fscanf(arquivo,"%d\n",&doceEspTempo);
            fscanf(arquivo,"%d %d\n",&doce.i,&doce.j);
            fscanf(arquivo,"%d %d\n",&doceEsp.i,&doceEsp.j);
            fscanf(arquivo,"%d %d\n",&ultimo.i,&ultimo.j);
            matriz[doce.i][doce.j] = alimento;
            if(doceEspExiste==1)
                matriz[doceEsp.i][doceEsp.j] = aliEspecial;

            snake = malloc(snakeTam * sizeof(struct posicao)); // criação do vetor com tamanho recuperado
            ponteiro = ponteiroNulo(snake);
            if(!ponteiro)
                for(k=0; k<snakeTam; k++)
                    fscanf(arquivo,"%d %d - ",&snake[k].i,&snake[k].j);
            for(k=0; k<100; k++)
                fscanf(arquivo,"%c - ",&mov[k].orientacao);
            for(k=0; k<10; k++)
                for(l=0; l<10; l++)
                    fscanf(arquivo,"%c - ",&crescimento[k][l]);
            fclose(arquivo);
        }

        // Preenchimento da Matriz (Posições Snake)
        if(!ponteiro){
            for(k=0; k<snakeTam; k++)
                matriz[snake[k].i][snake[k].j] = corpo;
            matriz[snake[0].i][snake[0].j] = cabeca;
            matriz[snake[snakeTam-1].i][snake[snakeTam-1].j] = calda;}

        system("cls"); // limpar tela do menu

        // Ciclo de Repetição
        if(!ponteiro && opcao!='0') //executa somente com as alocações corretas
        do{
                // Exibição da Matriz
                exibirMatriz(snakeTam,crescimento,pontos);

                // Ciclo de Entrada Incorreta
                do{
                    aux = P;
                    Sleep(velocidade);
                    if(kbhit())
                        P = getch(); //entrada da orientação

                    //Aumento da Dificuldade (Velocidade)
                    if(snakeTam%15==0 && dificuldade=='2')
                        velocidade -= 7;

                    if(doceEspTempo==duracaoDoce) //se a contagem do doce se igualar a duração, o doce é apagado
                        apagaDoce(); //define que o doce não existe e zera a contagem

                    // Registro da orientação do posição onde a snake está
                    for(k=0; k<tam1*tam2; k++)
                        if(snake[0].i==mov[k].i && snake[0].j==mov[k].j){
                            mov[k].orientacao = P; //registra a orientação onde a cabeça da cobra está
                            break;}                       //todo elemento da snake que passar ali vai seguir esta orientação

                    if(P=='a' || P=='d' || P=='w' || P=='s' || P=='q'){ //restrição para entrada correta dos dados
                        alteraPosicao(&P,aux,mov,snakeTam,&doceEspTempo,doceEspExiste); //movimenta a snake
                        crescimento[snake[snakeTam-1].i][snake[snakeTam-1].j]='n'; //remove da matriz que nesta posição haverá crescimento
                        if(acrescimo==1){
                            acrescimo = 0;
                            snake[snakeTam-1].i = ultimo.i; //registro real da posição na ultima posição da snake
                            snake[snakeTam-1].j = ultimo.j;
                            matriz[snake[snakeTam-1].i][snake[snakeTam-1].j] = calda;
                            matriz[snake[snakeTam-2].i][snake[snakeTam-2].j] = corpo;}
                        if((doce.i==snake[0].i && doce.j==snake[0].j) || (doceEsp.i==snake[0].i && doceEsp.j==snake[0].j)){ //marca quando a sake comeu o doce
                            if(doce.i==snake[0].i && doce.j==snake[0].j){
                                doceExiste = 0; //o doce não existe mais
                                pontos += 2;
                                doceEspTempo = 0;
                            }else{
                                doceEspTempo = 1;  //a contagem é zerada
                                pontos += 5;
                            }
                            doceEspExiste = 0; //o doce especial não existe mais
                            if(snakeTam<cobra){ //limita o crescimento da snake de acordo com a constante de limite
                                acrescimo = 1;
                                if(doce.i==snake[0].i && doce.j==snake[0].j)
                                    crescimento[doce.i][doce.j]='s'; //define que naquela posição haverá um crescimento
                                else{
                                    crescimento[doceEsp.i][doceEsp.j]='s';
                                    doceEsp.i=tam1;
                                    doceEsp.j=tam2;}
                                ultimo.i = snake[snakeTam-1].i; //guarda a ultima posição
                                ultimo.j = snake[snakeTam-1].j;
                                snakeTam++;
                                snake = realloc(snake,snakeTam * sizeof(struct posicao));
                                ponteiro = ponteiroNulo(snake);
                                if(!ponteiro){ //execução somente com alocações corretas
                                    snake[snakeTam-1].i = ultimo.i; //registra na ultima posição da snake
                                    snake[snakeTam-1].j = ultimo.j;} //somente para evitar erros, pois há numeros aleatório
                                }
                        }
                        if(doceExiste==0)
                            gerarDoce(&doceExiste,snakeTam); //gera um doce em posição aleatória
                        if(snakeTam%7==0 && doceEspExiste==0)
                            gerarDoceEsp(&doceEspExiste,snakeTam); //gera um doce especial em posição aleatória
                    }else if(P=='r'){
                        P = aux;
                        FILE *arquivo;
                        arquivo = fopen("saveGame.txt", "w");
                        fprintf(arquivo,"%c\n",P);
                        fprintf(arquivo,"%c\n",dificuldade);
                        fprintf(arquivo,"%d\n",pontos);
                        fprintf(arquivo,"%d\n",snakeTam);
                        fprintf(arquivo,"%d\n",acrescimo);
                        fprintf(arquivo,"%d\n",velocidade);
                        fprintf(arquivo,"%d\n",doceExiste);
                        fprintf(arquivo,"%d\n",doceEspExiste);
                        fprintf(arquivo,"%d\n",doceEspTempo);
                        fprintf(arquivo,"%d %d\n",doce.i,doce.j);
                        fprintf(arquivo,"%d %d\n",doceEsp.i,doceEsp.j);
                        fprintf(arquivo,"%d %d\n",ultimo.i,ultimo.j);
                        for(k=0; k<snakeTam; k++)
                            fprintf(arquivo,"%d %d - ",snake[k].i,snake[k].j);
                        for(k=0; k<100; k++)
                            fprintf(arquivo,"%c - ",mov[k].orientacao);
                        for(k=0; k<10; k++)
                            for(l=0; l<10; l++)
                                fprintf(arquivo,"%c - ",crescimento[k][l]);
                        fclose(arquivo);

                        printf("O jogo foi salvo!\n");
                        system("pause");
                    }else{
                        printf("\nO botão pressionado é invalido! Pressione:\n");
                        printf("Movimentação: (a)-Esquerda, (d)-Direita,\n\t\t(w)-Cima, (s)-Baixo\n");
                        printf("Comandos: (r)-Salvar, (q)-aSair\n\n");
                        system("pause");
                        P = aux;}


                }while(P!='a' && P!='d' && P!='w' && P!='s' && P!='1' && P!='2' && P!='r' && P!='R' && P!='q' && ponteiro!=1);

            system("cls"); //limpa a tela

        }while(P!='q' && ponteiro!=1);

        if(opcao!='0')
            if(ponteiro)
                printf("\nFalha na alocacao!\n");
            else{
                printf("\n   Game Over!\n");
                printf("   Tamanho: %d\n\n",snakeTam);
                printf("   Pontuação: %d\n\n",pontos);
                free(snake);
                free(mov);

                FILE *arquivo;
                char nom[5][20];
                int pont[5];
                char nome[20];
                int num;
                int colocacao;
                l = 0;

                arquivo = fopen("pontuacao.txt", "r");
                for(k=0; k<5; k++){
                    fgets(nom[k],20,arquivo);
                    fscanf(arquivo,"%d\n",&num);
                    pont[k] = num;}
                fclose(arquivo);

                for(k=0; k<5; k++){
                    if(pontos>pont[k]){
                        colocacao = k;
                        break;}
                }

                arquivo = fopen("pontuacao.txt", "w");
                if(colocacao<5){
                    printf("   Novo Recorde!\n");
                    printf("   Informe seu nome: ");
                    scanf("%s",nome);
                }else
                    system("pause");
                for(k=0; k<5; k++){
                    if(k==colocacao){
                        fprintf(arquivo,"%s\n",nome);
                        fprintf(arquivo,"%d\n",pontos);
                    }else{
                        fprintf(arquivo,"%s",nom[l]);
                        fprintf(arquivo,"%d\n",pont[l]);
                        l++;
                    }
                }
                fclose(arquivo);
                system("cls");
            }
    }while(opcao!='0');

    printf("\n   O Jogo Foi Finalizado!\n");

    return 0;
}
// ---------------------------------------------------------------
