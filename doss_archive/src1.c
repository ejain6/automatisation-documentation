/**
 * programme qui permet de jouer au jeu snake
*/

#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define CORPS_SNAKE '-' /**corps du snake*/
#define TETE_SNAKE 'B' /**tete du snake*/
#define DU_MANGER '*' /**pomme à manger pour le snake (score)*/
#define CASE_VIDE ' ' /**case vide*/


/**VARIABLES GLOBALES*/
char ** grille = NULL;/**grille*/
int nbLignes = 0;/**ligne de grille*/
int nbColonnes = 0;/**colonne de grille*/
int inutile = 0; /**mode lolilol*/


struct uneCellule {
  int ligne; /**coordonnées de ligne de la grille*/
  int colonne; /**coordonnées de colonne de la grille*/
  struct uneCellule * suiv; /**boucle*/
};
typedef struct uneCellule uneCellule;/**structure de grille du jeu*/


struct unSnake {
  uneCellule * teteSnake; /**tete du snake*/
  uneCellule * queueSnake; /**queue du snake*/
};
typedef struct unSnake unSnake;/**structure d'apparence du snake*/

struct uneDirection {
  int ligne; /**direction de ligne du snake*/
  int colonne; /**direction de colonne du snake*/
};
typedef struct uneDirection uneDirection; /**structure de direction du snake*/

/**initialisation du snake de base*/

/** FONCTIONS
 * \brief crée le snake
 * \detail donne une tete et une queue au snake et fait
 * en sorte que la queue suivent la tete du snake
 *
 * \return unSnake le snake
*/
unSnake creerSnake() {

  unSnake snake;

  uneCellule * teteSnake = malloc(sizeof(uneCellule));
  uneCellule * queueSnake = malloc(sizeof(uneCellule));
  uneCellule * milieuSnake = malloc(sizeof(uneCellule));

  teteSnake->ligne = 1;
  teteSnake->colonne = 4;
  teteSnake->suiv = NULL;

  milieuSnake->ligne = 1;
  milieuSnake->colonne = 3;
  milieuSnake->suiv = teteSnake;

  queueSnake->ligne = 1;
  queueSnake->colonne = 2;
  queueSnake->suiv = milieuSnake;

  snake.teteSnake = teteSnake;
  snake.queueSnake = queueSnake;

  return snake;
}

/** FONCTIONS
 * \brief permet au snake de bouger
 * \detail Redéfini la tête du snake aux coordonnées indiquées
 * gère les collisions avec les bords (sauf si mode lolilol) et le snake
 * vérifie si le snake mange quelque chose
 * 
 * \param unSnake le snake
 * \param int ligne de position du snake
 * \param int colonne de position du snake
 * \param int coordonner du manger
 * \param int état de défaite
 */
void ajouterEnTete (unSnake * snake, int ligne, int colonne,int * aMange, int * fail) {
  uneCellule * nouvelleTete = malloc (sizeof(uneCellule));

  nouvelleTete->ligne = ligne;
  nouvelleTete->colonne = colonne;
  nouvelleTete->suiv = NULL;

  snake->teteSnake->suiv = nouvelleTete;
  grille[(snake->teteSnake->ligne)][(snake->teteSnake->colonne)] = CORP_SNAKE;
  snake->teteSnake = snake->teteSnake->suiv;

  /* GESTION DES COLLISIONS */
  if (inutile) { 
    if( snake->teteSnake->ligne < 0) {
      snake->teteSnake->ligne = nbLignes - 1; 
    }

    else if( snake->teteSnake->ligne > nbLignes-1) {
      snake->teteSnake->ligne = 0; 
    }
    else if (snake->teteSnake->colonne < 0) {
      snake->teteSnake->colonne = nbColonnes - 1; 
    }
    else if ( snake->teteSnake->colonne > nbColonnes-1) {
      snake->teteSnake->colonne = 0; 
    }
    else if (grille[snake->teteSnake->ligne][snake->teteSnake->colonne] == CORP_SNAKE) {
      *fail = 1;
    }
  }
  else {
    if( snake->teteSnake->ligne < 0 ||
	snake->teteSnake->ligne > nbLignes-1 ||
	snake->teteSnake->colonne < 0 ||
	snake->teteSnake->colonne > nbColonnes-1)
      *fail = 1;
  }


  if(!*fail) {
    *aMange = (grille[snake->teteSnake->ligne][snake->teteSnake->colonne] == DU_MANGER) ? 1 : 0;
    grille[snake->teteSnake->ligne][snake->teteSnake->colonne] = TETE_SNAKE;
  }
}

/** FONCTIONS
 * \brief Supprime la queue du snake à sa mort
 * \detail Supprime la queue du snake à sa mort
 * 
 * \param unSnake le snake
*/
void supprimerQueue(unSnake * snake) {
  uneCellule * auxi;

  auxi = snake->queueSnake;
  grille[snake->queueSnake->ligne][snake->queueSnake->colonne] = CASE_VIDE;
  snake->queueSnake = snake->queueSnake->suiv;
  free(auxi);
}


/** FONCTIONS
 * \brief charge une grille vide
 * \detail charge une grille vide
*/
void initGrille() {
  int i , j = 0;
  for (i = 0; i<nbLignes;i++) {
    for (j=0;j<nbColonnes;j++) {
      grille[i][j] = CASE_VIDE;
    }
  }
}

/** FONCTIONS
 * \brief affiche la grille
 * \detail affiche la grille et le snake de base
 * 
 * \param unSnake le snake
*/
void afficherGrille(unSnake snake) {
  int i , j = 0;
  for (i = 0; i<nbLignes;i++) {
    for (j=0;j<nbColonnes;j++) {
      printw("%c",grille[i][j]);
    }
  }
}

/** FONCTIONS
 * \brief permet les controles de mouvement du snake
 * \detail change de direction au snake en fonction des touche appuier
 * 
 * \param unSnake le snake
 * \param int touche appuier
 * \param int état de défaite
 * \param uneDirection direction vers laquelle le snake vas
 * \param int cohordonnée du manger
*/
void gererEvenement(unSnake * snake, int touche, int * fail, uneDirection * direction,int * aMange) {
  if(direction->ligne == 0) { /**Pour ne pas 'aller en arrière'*/
    if (touche == KEY_UP){ 
      direction->ligne = -1;
      direction->colonne = 0; /**Pour ne pas aller en diagonale*/
    }
    if (touche == KEY_DOWN) {
      direction->ligne = 1;
      direction->colonne = 0;/**Pour ne pas aller en diagonale*/
    }
  }
  if (direction->colonne == 0) {/**Pour ne pas 'aller en arrière'*/
    if (touche == KEY_LEFT) {
      direction->colonne = -1;
      direction->ligne = 0;/**Pour ne pas aller en diagonale*/
    }
    if (touche == KEY_RIGHT) {
      direction->colonne = 1;
      direction->ligne = 0;/**Pour ne pas aller en diagonale*/
    }
  }

  ajouterEnTete(snake,snake->teteSnake->ligne + direction->ligne,snake->teteSnake->colonne + direction->colonne, aMange,fail);
  if(!*aMange)
    supprimerQueue(snake);
}

/** FONCTIONS
 * \brief genere une case contenent 'DU_MANGER' à des coordonnées aléatoires
 * \detail donne de nouvelle cohordonnée au manger quand le précedant ce fait manger
 * 
 * \param char grille de jeu
*/
void genererDuManger(char ** grille) {
  int ligne = 0;
  int colonne = 0;
  int done = 0;

  srand(time(NULL));
  while (!done){ 
    ligne = rand() % (nbLignes-1);
    colonne = rand() % (nbColonnes-1);
    if(grille[ligne][colonne] == CASE_VIDE) {
      grille[ligne][colonne] = DU_MANGER;
      done = 1;
    }
  }
}

/***/
/** FONCTIONS
 * \brief affiche 'FAIL' a l'écran
 * \detail affiche 'FAIL' a l'écran
*/
void printFail() {
  move(nbLignes/2 - 5/2,nbColonnes/2 - 37/2);
  printw("    _/_/_/_/    _/_/    _/_/_/  _/   \n");
  move(1 + nbLignes/2 - 5/2,nbColonnes/2 - 37/2);
  printw("   _/        _/    _/    _/    _/    \n");
  move(2 + nbLignes/2 - 5/2,nbColonnes/2 - 37/2);
  printw("  _/_/_/    _/_/_/_/    _/    _/     \n");
  move(3 + nbLignes/2 - 5/2,nbColonnes/2 - 37/2);
  printw(" _/        _/    _/    _/    _/      \n");  
  move(4 + nbLignes/2 - 5/2,nbColonnes/2 - 37/2);
  printw("_/        _/    _/  _/_/_/  _/_/_/_/ \n");
}

/** FONCTIONS
 * \brief fonction principale du jeu
 * \detail fonction principale du jeu
 * 
 * \param int je sais pas
 * \param char je sais pas
 * \return 0
*/
int main (int argc, char * argv []) {

  int i = 0;
  int touche = 0; /**touche pressée par le joueur*/
  int fail = 0; /**bool*/
  unSnake snake = creerSnake();
  uneDirection direction = {0,1};
  int aMange = 1; /**bool*/
  int nbCasesMangees = 0; /**score de la partie*/
  int delay = 0; /**vitesse de jeu*/

  /**Initialisations du mode de jeux et de la grille de jeu*/
  if (argc > 1) {
    inutile = 1; 
  }
  initscr();
  keypad(stdscr, TRUE);
  noecho();
  cbreak();

  getmaxyx(stdscr,nbLignes,nbColonnes);

  grille = malloc(nbLignes * sizeof(char *));
  for (i=0;i<nbLignes;i++) {
    grille[i] = malloc(nbColonnes*sizeof(char));
  }
  initGrille();

  /**Boucle du jeu qui tourne tant que le joueur n'a pas perdu*/
  while (!fail) {
    if (aMange){
      genererDuManger(grille);
      nbCasesMangees ++;
      delay = 101 - nbCasesMangees;
      delay = (delay < 60) ? 60 : delay;
      timeout(delay); /**On raffraichi toutes les 60 ms au max*/
    }
    afficherGrille(snake);
    touche = getch();
    gererEvenement(&snake,touche,&fail,&direction,&aMange); /**verifie si le snake est mort ou non*/
    erase();
  }

  /**Le joueur a perdu*/
  timeout(5000);
  erase();
  printFail();
  getch();

  endwin();
  return 0;
}