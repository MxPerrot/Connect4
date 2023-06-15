/**
 * \file main.c
 * \author Maxime Perrot (maxime.perrot10@proton.me)
 * \brief Jeu de Puissance 4 à deux joueurs en local pour l'association "Les têtes à clic"
 * \version 1.0
 * \date 2022-10-30
 *
 * Jeu de Puissance4 Réalisé dans le cadre de la SAE 1.01 : implémentation d'un besoin client
 * Commencé le 2022-10-30, terminé le 2022-11-23
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/*******************************
 **  CONSTANTES ET FONCTIONS  **
 *******************************/

/**
 * \def NBCOL
 *
 * \brief Nombre de colonnes de la grille
 */
#define NBCOL 7

/**
 * \def NBLIG
 *
 * \brief Nombre de lignes de la grille
 */
#define NBLIG 6

/**
 * \typedef Pion
 *
 * \brief Pion ○ ou ● ou VIDE
 */
typedef char Pion[4];

/**
 * \brief pion du joueur A
 *
 * Le caractère utilisé pour représenter le pion du premier joueur
 */
const Pion PION_A = "○"; // ○ \u9675

/**
 * \brief pion du joueur B
 *
 * Le caractère utilisé pour représenter le pion du second joueur
 */
const Pion PION_B = "●"; // ● \u9679

/**
 * \brief Caractère de case vide
 *
 * Le caractère affiché dans les cases vides de la grille
 */
const Pion VIDE = " ";

/**
 * \brief Le vainqueur jusqu'à ce qu'il soit déterminé par estVainqueur
 *
 * \see estVainqueur()
 */
const char INCONNU[] = "INCONNU";

/**
 * \brief La colonne dans laquelle apparait le pion
 */
const int COLONNE_DEBUT = 3;

/**
 * \brief Caractère de déplacement du pion à gauche
 */
const char GAUCHE = 'q';

/**
 * \brief Caractère de déplacement du pion à droite
 */
const char DROITE = 'd';

/**
 * \brief Caractère de placement du pion
 */
const char PLACER = 's';

/**
 * \brief Commande système pour l'effacement du terminal
 */
const char CLEAR[] = "clear";

/**
 * \brief Titre ASCII Art : "Puissance 4"
 */
const char TITRE[] = "\n      _____       _                                _  _\n     |  __ \\     (_)                              | || |\n     | |__) |   _ _ ___ ___  __ _ _ __   ___ ___  | || |_\n     |  ___/ | | | / __/ __|/ _` | '_ \\ / __/ _ \\ |__   _|\n     | |   | |_| | \\__ \\__ \\ (_| | | | | (_|  __/    | |\n     |_|    \\__,_|_|___/___/\\__,_|_| |_|\\___\\___|    |_|\n\n                                        Les têtes à clic\n\n\n";

/**
 * \brief Règles du jeu
 */
const char REGLES[] = "Règles du jeu :\n\n\tLe but du jeu est d'aligner une suite de 4 pions de même couleur sur une grille.\n\n\tChaque joueur dispose de 21 pions.\n\n\tTour à tour, les deux joueurs placent un pion dans la colonne de leur choix,\n\tle pion coulisse alors jusqu'à la position la plus basse possible.\n\n\tLe vainqueur est le joueur qui réalise le premier un alignement\n\t(horizontal, vertical ou diagonal) consécutif d'au moins quatre de ses pions. \n\n\tSi toutes les cases de la grille de jeu sont remplies et aucun des deux joueurs n'a gagné,\n\tla partie est déclarée nulle.\n\n\tDéplacer le jeton horizontalement avec Q et D, placer le jeton avec S.\n\tValider la commande avec la touche [Entrée].\n";

/**
 * \typedef Grille
 *
 * \brief Type tableau de 7 colonnes et 6 lignes
 */
typedef Pion Grille[NBLIG][NBCOL];

void initGrille(Grille);
void afficher(Grille, Pion, int);
void jouer(Grille, Pion, int *, int *, char[50]);
void finDePartie(char[50]);
bool grillePleine(Grille);
int choisirColonne(Grille, Pion, int, char[50]);
int trouverLigne(Grille, int);
bool estVainqueur(Grille, int, int);
void afficherTitre();
void demanderNoms();

/***************************
 **  PROGRAMME PRINCIPAL  **
 ***************************/

/**
 * \fn int main()
 * \brief Programme principal, fonction executée au démarrage
 *
 * \return int, code de sortie du programme (0 pour succès, 1 pour erreur)
 */
int main()
{
    // Initialisation
    int ligne, colonne;
    char vainqueur[50], joueur1[50], joueur2[50];
    Grille g;

    strcpy(vainqueur, INCONNU);
    initGrille(g);

    afficherTitre();
    demanderNoms(joueur1, joueur2);

    // Début de la partie
    afficher(g, PION_A, COLONNE_DEBUT);
    while ((strcmp(vainqueur, INCONNU) == 0) && (!grillePleine(g))) // Tant qu'il n'y a pas de vainqueur et que la grille n'est pas pleine
    {
        jouer(g, PION_A, &ligne, &colonne, joueur1);
        afficher(g, PION_B, COLONNE_DEBUT);
        if (estVainqueur(g, ligne, colonne)) // si le joueur 1 est vainqueur
        {
            strcpy(vainqueur, joueur1);
        }
        else if (!grillePleine(g)) // si la grille n'est pas pleine
        {
            jouer(g, PION_B, &ligne, &colonne, joueur2);
            afficher(g, PION_A, COLONNE_DEBUT);
            if (estVainqueur(g, ligne, colonne)) // si le joueur 2 est vainqueur
            {
                strcpy(vainqueur, joueur2);
            }
        }
    }
    finDePartie(vainqueur);
}

/*******************************
 **  FONCTIONS ET PROCEDURES  **
 *******************************/

/**
 * \fn void initGrille(Grille g)
 * \brief Initialise la grille en affectant la constante VIDE à chacun de ses éléments.
 *
 * \param g Grille, représente la grille de jeu
 */
void initGrille(Grille g)
{
    for (int i = 0; i < NBLIG; i++)
    {
        for (int j = 0; j < NBCOL; j++)
        {
            strcpy(g[i][j], VIDE);
        }
    }
}

/**
 * \fn void afficher(Grille g, char *pion, int col)
 * \brief Affiche à l'écran le titre et la grille
 *
 * \param g Grille, représente la grille de jeu
 * \param pion char, représente le pion à afficher au-dessus de la grille
 * \param col int, représente l’indice de la colonne au-dessus de laquelle le pion doit être affiché
 */
void afficher(Grille g, Pion pion, int col)
{
    system(CLEAR);
    afficherTitre();
    int i, x, y;
    int largeur = 4 * NBCOL + 1; // largeur de la grille en caractères

    // Ligne de démarquation
    for (i = 0; i < largeur; i++)
    {
        printf("─");
    }
    printf("\n");

    // Pion
    printf("  "); // 2 espaces pour arriver au millieu de la première colonne
    for (i = 0; i < col; i++)
    {
        printf("    "); // cycle de 4 caractères entre les milieux de colonnes
    }
    printf("%s", pion);
    printf("\n");

    // Grille
    // Ligne du haut
    printf("┌");
    for (i = 1; i < NBCOL; i++)
    {
        printf("───┬");
    }
    printf("───┐");
    printf("\n");

    // Corps de la grille
    for (y = 0; y < NBLIG; y++)
    {
        // Ligne contenant les pions
        printf("│");
        for (x = 0; x < NBCOL; x++)
        {
            printf(" %s │", g[y][x]);
        }
        printf("\n");
        if (y < NBLIG - 1) // Si on est pas à la dernière ligne
        {                  // Ligne intermédiaire
            printf("├");
            for (i = 1; i < NBCOL; i++)
            {
                printf("───┼");
            }
            printf("───┤");
        }
        else
        { // Ligne du bas
            printf("└");
            for (i = 1; i < NBCOL; i++)
            {
                printf("───┴");
            }
            printf("───┘");
        }
        printf("\n");
    }
}

/**
 * \fn bool grillePleine(Grille g)
 * \brief Teste si toutes les cases de la grille sont occupées ou non.
 *
 * \param g Grille, représente la grille de jeu
 *
 * \return true, si toutes les cases de la grille sont occupées par les pion
 * \return false, sinon
 */
bool grillePleine(Grille g)
{
    bool estPleine = true;

    for (int i = 0; i < NBLIG; i++)
    {
        for (int j = 0; j < NBCOL; j++)
        {
            if (strcmp(g[i][j], VIDE) == 0)
            {
                estPleine = false;
            }
        }
    }

    return estPleine;
}

/**
 * \fn void jouer(Grille g, char *pion, int *lig, int *col, char joueur[50])
 * \brief Permet à un joueur de jouer son pion.
 *
 * \param g Grille, représente la grille de jeu
 * \param pion char, correspond au pion à jouer
 * \param lig int, correspond à la ligne où est tombé le pion
 * \param col int, correspond à la colonne où est tombé le pion
 * \param joueur char[50], correspond au nom du joueur qui joue
 */
void jouer(Grille g, Pion pion, int *lig, int *col, char joueur[50])
{
    // Trouve la ligne
    *col = choisirColonne(g, pion, COLONNE_DEBUT, joueur);
    *lig = trouverLigne(g, *col);
    while (*lig == -1) // Tant que la colonne est pleine
    {
        afficher(g, pion, COLONNE_DEBUT);
        *col = choisirColonne(g, pion, COLONNE_DEBUT, joueur);
        *lig = trouverLigne(g, *col);
    }
    strcpy(g[*lig][*col], pion);
}
/**
 * \fn int choisirColonne(Grille g, char *pion, int col, char joueur[50])
 * \brief Permet à un joueur de choisir la colonne où faire tomber son pion.
 *
 * \param g Grille, représente la grille de jeu
 * \param pion char, représente le pion à tester
 * \param col int, colonne de départ (celle au-dessus de laquelle se trouve le pion initialement)
 * \param joueur char[50], correspond au nom du joueur qui joue
 *
 * \return int, indice de la colonne choisie par le joueur
 */
int choisirColonne(Grille g, Pion pion, int col, char joueur[50])
{
    char action;
    bool colonneEstTrouvee = false;
    while (!colonneEstTrouvee)
    {
        // Vérification de la validité de l'action
        printf(" A %s de jouer : ", joueur);
        scanf(" %c", &action); // ERREUR : accepte plusieurs caractères a la suite, solution : %[^\n]c, nouvelle erreur : potentielle erreur de segmentation si deux commandes valides à la suite
        while ((action != GAUCHE) && (action != DROITE) && (action != PLACER))
        {
            afficher(g, pion, col);
            printf(" A %s de jouer : ", joueur);
            scanf(" %c", &action);
        }
        // Action
        if (action == GAUCHE)
        {
            if (col > 0)
            {
                col--;
            }
            afficher(g, pion, col);
        }
        else if (action == DROITE)
        {
            if (col < NBCOL - 1)
            {
                col++;
            }
            afficher(g, pion, col);
        }
        else if (action == PLACER)
        {
            colonneEstTrouvee = true;
        }
    }
    return col;
}

/**
 * \fn int trouverLigne(Grille g, int col)
 * \brief Consiste à trouver la première case non occupée de la colonne.
 *
 * \param g Grille, représente la grille de jeu
 * \param col int, indice de la colonne dans laquele le pion doit tomber
 *
 * \return int, indice de la ligne où le pion devra être ajouté ou -1 si la colonne est pleine
 */
int trouverLigne(Grille g, int col)
{
    int resultat = -1;
    for (int i = 0; i <= NBLIG; i++)
    {
        if (strcmp(g[i][col], VIDE) != 0)
        {
            resultat = i - 1;
            break;
        }
    }
    return resultat;
}

/**
 * \fn bool estVainqueur(Grille g, int lig, int col)
 * \brief Indique si le pion situé dans la case repérée par les paramètres ligne et colonne a gagné la partie.
 *
 * \param g Grille, représente la grille de jeu
 * \param lig int, représentant la ligne à partir de laquelle rechercher une série de 4 pions successifs identiques
 * \param col int, représentant la colonne à partir de laquelle rechercher une série de 4 pions successifs identiques
 *
 * \return true, s’il y a une telle ligne de 4 à partir de la case indiqué
 * \return false, sinon
 */
bool estVainqueur(Grille g, int lig, int col)
{
    int i, case_x, case_y;
    int compteur = 0;
    Pion pion;
    strcpy(pion, g[lig][col]);

    // Horizontale
    for (i = -3; i <= 3; i++)
    {
        case_x = col + i;
        if ((case_x >= 0) && (case_x <= NBCOL)) // si la case existe
        {
            if (strcmp(g[lig][case_x], pion) == 0) // si la case est un pion
            {
                compteur++;
            }
            else
            {
                compteur = 0;
            }
            if (compteur == 4)
            {
                return true;
            }
        }
    }

    // Verticale
    if ((lig <= NBLIG - 4)) // Si la case est à au moins 4 lignes de haut
    {
        if ((strcmp(g[lig][col], g[lig + 1][col]) == 0) && (strcmp(g[lig][col], g[lig + 2][col]) == 0) && (strcmp(g[lig][col], g[lig + 3][col]) == 0)) // Si la case est identique au trois d'en dessous
        {
            return true;
        }
    }

    // Diagonale Nord-Ouest / Sud-Est
    compteur = 0;
    for (i = -3; i <= 3; i++)
    {
        case_x = col + i;
        case_y = lig + i;
        if (((case_y >= 0) && (case_y <= NBLIG)) && ((case_x >= 0) && (case_x <= NBCOL))) // si la case existe
        {
            if (strcmp(g[case_y][case_x], pion) == 0) // si la case est un pion
            {
                compteur++;
            }
            else
            {
                compteur = 0;
            }
            if (compteur == 4)
            {
                return true;
            }
        }
    }

    // Diagonale Nord-Est / Sud-Ouest
    compteur = 0;
    for (i = -3; i <= 3; i++)
    {
        case_x = col + i;
        case_y = lig - i;
        if (((case_y >= 0) && (case_y <= NBLIG)) && ((case_x >= 0) && (case_x <= NBCOL))) // si la case existe
        {
            if (strcmp(g[case_y][case_x], pion) == 0) // si la case est un pion
            {
                compteur++;
            }
            else
            {
                compteur = 0;
            }
            if (compteur == 4)
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * \fn void finDePartie(char vainqueur[50])
 * \brief Affiche le résultat d'une partie lorsqu'elle est terminée.
 *
 * \param vainqueur char[50], représente le joueur gagnant (joueur1 ou joueur2) ou bien INCONNU si match nul
 */
void finDePartie(char vainqueur[50])
{
    if (strcmp(vainqueur, INCONNU) == 0)
    {
        printf("Partie nulle !");
    }
    else
    {
        printf("%s a gagné !", vainqueur);
    }
}

/**
 * \fn void afficherTitre()
 * \brief Affiche le titre et les règles du jeu
 */
void afficherTitre()
{
    printf("%s\n", TITRE);
    printf("%s\n", REGLES);
}

/**
 * \fn void demanderNoms(char joueur1[50], char joueur2[50])
 * \brief Demande le nom des deux joueurs
 *
 * \param joueur1 char[50], le nom du premier joueur
 * \param joueur2 char[50], le nom du second joueur
 */
void demanderNoms(char joueur1[50], char joueur2[50])
{
    printf("\nEntrer un nom pour le joueur 1 (%s) : ", PION_A);
    scanf(" %[^\n]", joueur1);
    printf("\nEntrer un nom pour le joueur 2 (%s) : ", PION_B);
    scanf(" %[^\n]", joueur2);
    printf("\n");
}
