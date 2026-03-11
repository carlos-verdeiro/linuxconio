#include <stdio.h>
#include <conio2.h>

int main()
{
    char texto[100];
    
    clrscr();
    
    // 1. Testando kbhit silencioso e getche
    printf("1. Teste do kbhit() e getche()\n");
    printf("Pressione qualquer tecla para continuar...");
    while(!kbhit()); 
    char input = getche(); 
    printf("\nVoce pressionou: %c\n\n", input);
    
    // 2. Testando cores
    printf("2. Teste de Cores\n");
    textbackground(GREEN);
    textcolor(BLACK);
    printf(" Texto com fundo verde e letra preta! ");

    textbackground(BLACK);
    textcolor(LIGHTGRAY);
    printf("\n\nPressione qualquer tecla para limpar a tela e ir para o proximo teste...");
    getch();
    
    clrscr();

    // 3. Testando o gets()
    printf("4. Teste do gets() editavel\n");
    printf("Digite uma frase (use as setas para a esquerda/direita e o backspace/delete):\n> ");
    gets(texto);
    printf("Frase capturada: %s\n", texto);

    printf("\nTeste finalizado! Pressione qualquer tecla para sair..."); 
    getch();
    clrscr();
 
    return 0;
}