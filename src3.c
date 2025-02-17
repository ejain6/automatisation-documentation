#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* DEFINES */

#define N 6

/*VARIABLES GLOBALES*/
const int MAX = 30; /*C'est juste le max qui sert à rien.*/

typedef char chaine40[40];
typedef char chaine20[20];
typedef char chaine60[60];

/* STRUCTURES */
struct hello{
    char message[20]; /* Un message venu de l'autre monde !*/
};
typedef struct hello hello;

/*FONCTIONS
* \brief Une fonction qui demande et affiche un message.
* \detail Des messages venus des Enfers peuvent être envoyés ici.
*
* \param hello C'est la structure qui contient le message :O
*/
void sayHello(hello hello){
    printf("Votre message : \n");   /*Besoin d'expliquer ?*/
    scanf("%s",hello.message);  /* L'utilisateur rentre son message venu des Enfers !*/
    printf("%s",hello.message); /*Et on affiche son message (parce-que la vie privée n'existe pas ici ;) )*/
}

int main(){
    hello Bonjour;
    sayHello(Bonjour);
    return EXIT_SUCCESS;
}