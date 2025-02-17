/**
 * Un programme pour jouer au jeu du Puissance 4.
 * Le code comprend les fonctions nécessaires pour initialiser
 * le tableau de jeu, permettre aux joueurs de placer leurs jetons,
 * afficher le tableau à l'écran, vérifier s'il y a un gagnant,
 * et afficher le résultat de la partie.
 */


#include <stdio.h>
#include <stdlib.h>
 

#define HAUTEUR 6  /** Hauteur du damier */
#define LARGEUR 7  /** Largeur du damier */

/**VARIABLES GLOBALES*/
char damier[LARGEUR][HAUTEUR]; /** Tableau représentant le damier */
int joueur = 1; /** Variable pour suivre le joueur actuel */
int nbtokens = HAUTEUR * LARGEUR; /** Nombre de jetons restants */
char symbols[2]; /** Symboles utilisés pour les joueurs */

/** symbols[0] = 'X' */
/** symbols[1] = 'O' */
 


/**
 * \brief Initialise le damier avec des espaces vides.
 * \detail Cette procédure initialise le tableau du damier en remplissant chaque case avec un espace vide (' ').
 * Elle parcourt toutes les lignes et colonnes du damier pour les remplir d'espaces vides, 
 * permettant ainsi de démarrer le jeu avec un damier vide.
 */
void initdamier()
{
	int i;
	int j;
	for (i = 0; i < HAUTEUR; i++)
	{
		for (j = 0; j < LARGEUR; j++)
		{
			damier[i][j] = ' ';
		}
	}
}
 


/**
 * \brief Ajoute un jeton dans le damier pour un joueur donné.
 * \detail Cette fonction permet à un joueur de placer son jeton dans une colonne spécifique du damier.
 * Elle vérifie si la colonne est valide et place le jeton dans la première case vide de cette colonne.
 * \param joueur Le numéro du joueur qui souhaite placer le jeton (1 ou 2).
 * \return int col, La colonne choisie par le joueur pour placer son jeton.
 */
int askcolonne(int joueur)
{
	int col;
	do
	{
		printf("Joueur %i, à vous de jouer !\n", joueur);
		printf("Entrez un numéro de colonne entre 0 et 6\n");	
		scanf("%d", &col); 
	}
	while (col<0 || col>6);
	printf("Le numéro de colonne choisi est %i\n", col);
	return col; 
}	


/**
 * \brief Affiche le damier actuel du jeu.
 * \detail Cette procédure affiche le damier actuel du Puissance 4 dans la console en utilisant des caractères.
 * Chaque case est représentée par un espace vide, 'X' ou 'O' selon les jetons placés.
 */
void displaydamier()
{
	int i;
	int j;
	printf("Tableau Puissance 4 :\n");
	printf("---------------------\n");
	for (i = 0; i < HAUTEUR; i++)
	{
		printf("+---+---+---+---+---+---+---+\n");
		printf("|");
		for (j = 0; j < LARGEUR; j++)
		{
			printf(" %c |", damier[i][j]);
		}
		printf("\n");
		}
	printf("+---+---+---+---+---+---+---+\n");
	printf("  0   1   2   3   4   5   6 \n");
 
}
 


/**
 * \brief Affiche le damier actuel dans la console.
 * \detail Cette procédure parcourt le damier et affiche chaque case du tableau dans la console.
 * Chaque case est représentée par son contenu ('X', 'O' ou ' ') avec les indices de ligne et de colonne correspondants.
 * La sortie est affichée ligne par ligne pour représenter visuellement le damier.
 */ 
void affichedamier()
{
	int m;
	int n;
	for (m = 0; m < 6; m++)
	{
		for (n = 0; n < 7; n++)
		{
			printf("Damier[%i][%i]=%c",m,n,damier[m][n]);
		}
		putchar('\n');
	}
 
}
 
/**
 * \brief Calcule la position où placer le jeton dans une colonne spécifique.
 * \detail Cette fonction parcourt une colonne spécifique pour trouver la première case vide (caractère espace ' ').
 * Elle retourne la ligne où le jeton peut être placé dans la colonne choisie.
 *
 * @param col La colonne choisie par le joueur.
 * @return int La ligne où le jeton doit être placé.
 */
int calcule_position(int col)
{
 
	int i = 6;
	int lig;
	do
	{
		i--;
	}
	while (damier[i][col] != ' ');
	lig = i;
 
	return lig;

}
 
/**
 * \brief Recherche une combinaison gagnante dans le damier.
 * \detail Cette fonction examine le damier pour rechercher une combinaison gagnante horizontale, verticale ou diagonale.
 * Si une combinaison gagnante est trouvée, elle renvoie 1 ; sinon, elle renvoie 0.
 * 
 * \param col La colonne du dernier jeton placé.
 * \param ligne La ligne du dernier jeton placé.
 * \return int 1 si une combinaison gagnante est trouvée, sinon int 0.
 */
int searchp4(int col,int ligne)
{
	return 0; 
}

/**
 * Fonction principale du jeu Puissance 4.
 * 
 * \detail Cette fonction gère le déroulement du jeu en appelant les différentes fonctions nécessaires.
 * Elle permet aux joueurs de placer leurs jetons sur le damier jusqu'à ce qu'un joueur gagne ou que le jeu se termine par un match nul.
 * 
 * \param argc Nombre d'arguments de la ligne de commande.
 * \param argv Tableau d'arguments de la ligne de commande.
 * \return char Statut de fin de programme.
 */ 
int main (int argc, char ** argv)
{
	int winner = 0;
	char symbol;
	initdamier();
	displaydamier();
	do {
		joueur = (joueur + 1)%2;
		int colonne = askcolonne(joueur);
		printf("Colonne vaut %i\n", colonne);
		int ligne = calcule_position(colonne);
		printf("Ligne vaut %i\n", ligne);
 
		if (joueur == 0)
		{
			symbol = 'X';
		}	
		else
		{
			symbol = 'O';
		}
		damier[ligne][colonne] = symbol;
		printf("symbol = %c\n", symbol);
 
		winner = searchp4(colonne,ligne);
		nbtokens--;
		displaydamier();
		printf("Winner vaut %i\n", winner);
	}
	while ( winner!=1 || nbtokens );
 
}

typedef struct {
    int ligne; /** Numéro de ligne sur le damier */
    int colonne; /** Numéro de colonne sur le damier */
} Coordonnees; /** Structure de coordonnées*/


typedef struct Joueur {
    char nom[50]; /** Nom du joueur */
    char symbole; /** Symbole utilisé par le joueur (X ou O) */
} Joueur; /** Structure de joueur*/