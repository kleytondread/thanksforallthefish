#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

//definindo a tecla Esc para finaliza��o do jogo
#define esc 27


int gameOver;
char jogador [50];
const int mapaLargura = 63;
const int mapaAltura = 18;
int score, caudaTamanho;
enum Direcional {STOP = 0, LEFT, RIGHT, UP, DOWN};
enum Direcional dir;
typedef struct coordenada
{
    int x;
    int y;
}coordenada;
coordenada head, fruta, cauda[100];

void info();
void hidecursor();
void showRanking();
void createRanking();
void aboutGame();
int initialization();
void Delay(long double k);
void setup ();
void desenho();
void input();
void logica();
void borda();
void bordaGrossa();
void snake();
void end();
void GotoXY(int x, int y);


int main()
{
    hidecursor();
    info();
    int i=1;
    setup();
    while (i!=0)
    {
        //tela de inicializa��o
        system ("cls");
        i = initialization();
    }
    system("cls");
    //game
    while (gameOver != 0)
    {
        borda();
        snake();
        input();
        logica();
        Sleep(100);     //deixa mais lento a cobra
    }
    //encerramento do jogo
    system("pause");
    fflush(stdin);
    system("cls");
    createRanking();
    showRanking();
    end();

    return 0;
}
void GotoXY(int x, int y)
{
    HANDLE a;
    COORD b;
    fflush(stdout);
    b.X = x;
    b.Y = y;
    a = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(a,b);
 }
void info()
{
    char end[100];
    //"Capa" do jogo
    FILE*fp = fopen("TELA INICIAL.txt", "r");
    if(!fp)
        printf("arquivo nao foi aberto");
    for (int i =0; i<18;i++)
    {
        fflush(stdin);
        GotoXY(3, i);
        fgets(end, 100, fp);
        for (int j=0;j<100;j++)
        {
            if (end[j]=='#')
                printf("%c", 219);
            else if(end[j]=='\0')
                continue;
            else if (end[j]==' ')
                printf(" ");
        }

    }
    GotoXY(19,12);
    printf("UM JOGO DESENVOLVIDO POR:");
    GotoXY(19,13);
    printf("  -KLEYTON CLEMENTINO");
    GotoXY(19,14);
    printf("  -MATHEUS VIEIRA");
    GotoXY(19,15);
    printf("  -MARIANA RUFINO");
    bordaGrossa();
    printf("\n\n\n");
    system("pause");
    system("cls");
    //dicas
    printf("\n\n\n\n   DICAS:\n\n   A Snake e movida pelas teclas W, S, D, A\n\n   Use a tecla 'ESC' para encerrar o jogo a qualquer momento");
    printf("\n\n   Para pausar o jogo, aperte qualquer outra tecla\n\n   Cada fruta que voce come, aumenta seu Score em 1 ponto\n\n\n");
    bordaGrossa();
    system ("pause");
}
void Delay(long double k)
{
    //suspende por 1,5 segundos
    Sleep(1500);
}
void createRanking()
{
    int pontos[10];
    char players[10][50];
    char aux[50];
    int auxNum;

    //input nome do jogador
    system("cls");
    printf("\n\n   Insira o nome do jogador:\n\n   ");
    fgets(jogador, sizeof(jogador), stdin);

    FILE *fp = fopen("ranking_jogadores.txt", "rb");
    if(!fp)
        printf("\narquivo 'ranking jogadores' nao foi aberto para leitura.\n");
    //armazenar nome de jogador no programa
    for (int i = 0; i<10; i++)
    {
        fflush(stdin);
        fscanf(fp, "%s %d", players[i], &pontos[i]);
    }
    //se o score for maior que o �ltimo jogador do ranking,
    //o algor�timo substitui este e reorganiza o ranking
    if (score>pontos[9])
    {
        pontos[9] = score;
        strcpy(players[9], jogador);
        for (int h=0;h<=10;h++)
        {
            for(int i =9; i>=0; i--)
            {
                if (pontos[i]>pontos[i-1])
                {
                    auxNum = pontos[i-1];
                    pontos[i-1] = pontos[i];
                    pontos[i] = auxNum;
                    strcpy(aux, players[i-1]);
                    strcpy(players[i-1], players[i]);
                    strcpy(players[i], aux);
                }
            }
        }
    }
    fclose(fp);
    fflush(stdin);

    //reescrever ranking na base no arquivo
    fp = fopen("ranking_jogadores.txt", "wb");
    if(!fp)
        printf("\narquivo 'ranking jogadores' nao foi aberto para escrita.\n");

    for(int i = 0; i<10; i++)
        fprintf(fp,"%s %d", players[i], pontos[i]);

    fclose(fp);
}
void showRanking()
{
    //abrir txt e armazenar nomes e pontua�oes dos jogadores depois printar an tela
    int pontuacao[10];
    char player[10][50];

    system("cls");

    FILE *fp = fopen("ranking_jogadores.txt", "rb");
    if(!fp)
        printf("\narquivo 'ranking jogadores' nao foi aberto para leitura.\n");

    printf("\n\n\t\t  As 10 melhores pontuacoes:\n\n");
    for (int i = 0; i<10; i++)
    {
        fscanf(fp, "%s %d", player[i], &pontuacao[i]);
        printf("\t\t   %d-\t%s\t%d\n", i+1, player[i], pontuacao[i]);
    }
    bordaGrossa();
    printf("\n\n");
    fclose(fp);
}
void aboutGame()
{
    //abrir txt e printar na tela os cr�ditos
    system ("cls");
    char creditos [150];
    FILE *snakeCreditos = fopen("Snake_Creditos_Jogo.txt", "r");
    if (!snakeCreditos)
    {
        printf("arquivo nao pode ser aberto");
        exit(1);
    }
    while(!feof(snakeCreditos))
    {
        fgets (creditos, sizeof(creditos), snakeCreditos);
        printf ("%s", creditos);
    }
    bordaGrossa();
    system ("pause");

}
int initialization()
{
    char sair;
    system("cls");

    //tela de menu
    GotoXY(30,3);
        printf("SNAKE");
    GotoXY(24,6);
        printf("DIGITE UMA OPCAO:");
    GotoXY(26,8);
        printf("1 - Novo Jogo ");
    GotoXY(26,9);
        printf("2 - Pontuacao ");
    GotoXY(26,10);
        printf("3 - Sobre o Jogo ");
    GotoXY(26,11);
        printf("4 - Sair ");

    bordaGrossa();
    printf ("\n\n\n\n\n\n\n\n");
    //navegar pelo menu
    if(!kbhit())
    {
        switch (getch())
        {
            case '1':
                return 0;
                break;
            case '2':
                showRanking();
                system("pause");
                return 1;
                break;
            case '3':
                aboutGame();
                return 1;
                break;
            case '4':
                system ("cls");
                printf("\n  Voce tem certeza que quer sair do jogo? Sim (Y) Nao (N)\n\n  ");
                fflush(stdin);
                scanf("%c", &sair); fflush(stdin);
                if (sair == 'N' || sair == 'n')
                    {return 1; break;}
                if (sair == 'Y' || sair == 'y')
                {
                    system ("cls");
                    printf("\n\n");
                    exit (0);
                }else
                    printf("comando invalido, seu jogo sera fechado.\n");
                break;
        }
    }
}
void setup ()
{
    //configura todas as vari�veis do jogo para inici�-lo
    srand(time(0));                      //gera um n�mero para a fun��o rand()
    gameOver = 1;
    dir = STOP;
    head.x = mapaLargura/2;
    head.y = mapaAltura/2;
    fruta.x = rand() % mapaLargura;
    if (fruta.x==0)                     //para o caso de a fruta ficar na posi��o x=0
        fruta.x+=1;
    fruta.y = rand()% mapaAltura;
    if (fruta.y==0)                     //para o caso de a fruta ficar na posi��o y=0
        fruta.y+=1;
    score = 0;
}
void input()
{
    //capturar a tecla apertar e associar com uma dire��o
    if(kbhit())
    {
        switch (_getch())
        {
        case 'w':
            dir = UP;
            break;
        case 's':
            dir = DOWN;
            break;
        case 'a':
            dir = LEFT;
            break;
        case 'd':
            dir = RIGHT;
            break;
        case 'W':                   //Caso o capslock esteja ligado
            dir = UP;
            break;
        case 'S':
            dir = DOWN;
            break;
        case 'A':
            dir = LEFT;
            break;
        case 'D':
            dir = RIGHT;
            break;
        case esc:                   //apertar Esc fecha o jogo
            system ("cls");
            exit(0);
            break;
        default:                    //qualquer outra tecla pausa o jogo
            dir = STOP;

        }
    }
}
void logica()
{
    coordenada prev, prev2;

    //passando a posi��o de uma parte da cauda para a parte seguinte da cauda
    prev.x = cauda[0].x;
    prev.y = cauda[0].y;
    cauda[0].x = head.x;
    cauda[0].y = head.y;
    for(int i = 1; i<=caudaTamanho; i++)
        {
            prev2.x = cauda[i].x;
            prev2.y = cauda[i].y;
            cauda[i].x = prev.x;
            cauda[i].y = prev.y;
            prev.x = prev2.x;
            prev.y = prev2.y;
        }
    //faz a cobra andar
    switch (dir)
    {
        case LEFT:
            head.x--;
            break;
        case RIGHT:
            head.x++;
            break;
        case UP:
            head.y--;
            break;
        case DOWN:
            head.y++;
            break;
        default:
            break;
    }
    //fazer a cobra passar de um lado da parede para o lado oposto
    if (head.x <=0)
    {
        head.x = mapaLargura-1;
    }if (head.x >= mapaLargura)
    {
        head.x = 1;
    }
    if (head.y <=0)
    {
        head.y = mapaAltura-1;
    }if (head.y >= mapaAltura)
    {
        head.y = 1;
    }
    for (int i=0; i<caudaTamanho; i++)
    {
        //finaliza��o quando a cabe�a da cobra atinge a cauda.
        if (head.x == cauda[i].x && head.y == cauda[i].y)
        {

            Delay(mapaAltura);
            gameOver = 0;

            system("cls");
            //mensagme de game over
            bordaGrossa();
            GotoXY(26,8);
            printf("GAME OVER!\n");
            GotoXY(mapaLargura,mapaAltura);
            printf("\n\n\n");
        }
    }
    //testa se a cobra comeu a fruta, se sim, randomiza a pr�xima posi��o x e y da fruta
    if (head.x == fruta.x && head.y == fruta.y)
    {
        caudaTamanho++;         //aumenta o tamanho da cauda
        score++;                //aumenta a pontua��o
        fruta.x = rand() % mapaLargura;
            if (fruta.x==0)     //testa se a fruta esta na posi��o 0 e muda pra a posi��o 1
            fruta.x+=1;
        fruta.y = rand()% mapaAltura;
            if (fruta.y==0)
            fruta.y+=1;
    }
}
void hidecursor()
{
    //retira o cursor da tela, porque estava aparecendo durante o jogo
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}
void borda()
{
    //usa a fun��o gotoxy para ir at� uma posi��o espec�fica da tela para printar a borda
    //system ("cls");
    GotoXY(fruta.x, fruta.y);
        printf("%c", 64);
    bordaGrossa();


    printf ("SCORE: %i\n", score);
    printf("\nuse w, s, a, d para mover a Snake\n");
}
void snake()
{
    //usa a fun��o gotoxy para ir at� uma posi��o da tela e printar a cobra e a cauda
    GotoXY(head.x, head.y);
    printf ("O");

    for (int k=0; k<=caudaTamanho;k++)
    {
        if (k==caudaTamanho)
        {
            //k == caudaTamanho cria um segmento a mais na cauda e o deixa com um espa�o em branco para apagar o rastro da cobra
            //solu��o para o system ("cls") - tela piscando
            GotoXY(cauda[k].x, cauda[k].y);
            printf(" ");
        }else{
        GotoXY(cauda[k].x, cauda[k].y);
        printf ("O");}
    }
}
void bordaGrossa()
{
    for (int i=0; i<= mapaLargura-1; i++)
    {
        GotoXY(i,0);
        printf("%c",219);
        GotoXY(i,mapaAltura);
        printf("%c",219);
    }
    for (int i=0; i<=mapaAltura; i++)
    {
        GotoXY(0, i);
        printf("%c",219);
        GotoXY(mapaLargura, i);
        printf("%c",219);
    }
    printf("\n");
}
void end()
{
    //vai at� o arquivo buscar a imagem final do jogo
    system("pause");
    system("cls");
    char end[1000];
    FILE*fp = fopen("so long and thanks for the fish.txt", "r");
    if(!fp)
        printf("arquivo nao foi aberto");

    for (int i =0; i<30;i++)
    {
        fgets(end, 1000, fp);
        printf("%s", end);
    }
    printf("\n\n               ");
    system("pause");
}
