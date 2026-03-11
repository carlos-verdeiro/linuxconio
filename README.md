# Linux <conio2.h>

## _Implementação do "conio2.h" para Linux_

Esta é uma adaptação do diretório ```/casualsnek/linuxconio```, onde tem por objetivo utilizar o conio2.h para escrever códigos que funcionem tanto no linux quanto no windows.

Por que usar "conio2" no linux em vez de "curses" ou outras bibliotecas?
- Porque a faculdade exige o uso do conio2 para o desenvolvimento dos trabalhos.
- É preciso que o código que escrevo rode nativamente no computador do professor.



### Recursos
- Não utiliza a biblioteca "curses".
- Funciona no linux com GCC.
- Resolve o bug dos caracteres de borda do Windows no terminal Linux.
- Ressuscita funções nativas do C para facilitar a criação de interfaces.

## Suporte a Caracteres de Borda (Tabelas e Janelas)

No Windows, os caracteres usados para desenhar interfaces, painéis e editores de texto no terminal (como `┌`, `└`, `│`, `─`) usam uma numeração antiga (códigos numéricos de 176 a 254). Se você tentar imprimir isso direto no Linux, a tela fica toda bugada porque o Linux usa um padrão de texto mais moderno (UTF-8).

**Solução usada**

Esta biblioteca tem um "tradutor" automático embutido (`get_mapped_char`). Você pode programar sua interface gráfica usando os mesmos números que usaria no Windows. A biblioteca converte esses números para os caracteres corretos no Linux, fazendo com que o visual do programa não quebre. A utilização dele é feita automaticamente ao usar o printf.

## Melhorias e Correções Embutidas

Além das bordas, a partir do C11, foi descontinuado o gets por problemas de overflow de string. Mas como em sala utilzamos ele, reescrevi ele nessa biblioteca para não ter problemas com projetos feitos usando os compiladores da faculdade.

* Este `gets` permite usar as **setas (esquerda/direita)**, o **Backspace** e o **Delete** para editar a frase antes de dar Enter, funcionando como um campo de digitação.

* **Leitura de Setas no `getch`:** Lê as teclas de setas do teclado (no linux é usado outro prefixo) e as converte para o padrão numérico do Windows (o prefixo `224`). Isso permite programar a navegação de menus com a mesma lógica nos dois sistemas.

---

## Instalação

A instalação é feita copiando o arquivo "conio2.h" para o local onde o compilador procura por arquivos header ou mantendo o "conio2.h" no mesmo diretório do seu código e usando ``` #include "conio2.h"``` em vez de ```#include <conio2.h> ```.

Na maioria dos sistemas, ele pode ser instalado executando os seguintes comandos no emulador de terminal:

```sh

git clone https://github.com/carlos-verdeiro/linux-conio

cd linux-conio

sudo cp conio2.h /usr/include/conio2.h

```

Reinicie sua IDE se você estiver usando uma e utilize-a como faria normalmente.

Você pode verificar se está funcionando corretamente após a instalação compilando e executando o "testconio.c" através dos seguintes comandos:

```sh

gcc testconio.c -o testconio

./testconio

```
## Funções implementadas
- showcursor / nocursor
- gotoxy
- clrscr / clreol / delline
- textcolor / textbackground
- kbhit
- getch / getche
- putch / cputs / cprintf
- gets / printf (adaptados)

## Como Contribuir

Sinta-se à vontade para contribuir com o projeto! Como a ideia é manter a compatibilidade com os trabalhos da faculdade, se notar alguma função da `conio2.h` original que está faltando, encontrar algum bug ou tiver ideias para melhorar o código, toda ajuda é bem-vinda.

1. Faça um **Fork** do projeto
2. Crie uma *branch* para sua modificação
3. Faça o *commit* das suas alterações
4. Dê um *push* para a *branch*
5. Abra um **Pull Request**
