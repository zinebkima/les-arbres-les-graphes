#include <stdio.h>
#include <stdlib.h>



typedef struct Noeud {
    int data;
    struct Noeud* fg;
    struct Noeud* fd;
    int hauteur;
} Noeud;

typedef struct File {
    Noeud* noeud;
    struct File* suivant;
} File;


Noeud* creerNoeud(int val) {
    Noeud* NV = (Noeud*)malloc(sizeof(Noeud));
    NV->data = val;
    NV->fg = NULL;
    NV->fd = NULL;
    NV->hauteur = 1;
    return NV;
}

void insererNoeud(Noeud **R, int val) {
    Noeud* NV, *tmp;
    NV = creerNoeud(val);
    if(*R == NULL) {
        *R = NV;
        return;
    }
    tmp = *R;
    while(1) {
        if(NV->data > tmp->data) {
            if(tmp->fd == NULL) {
                tmp->fd = NV;
                break;
            } else tmp = tmp->fd;
        } else {
            if(tmp->fg == NULL) {
                tmp->fg = NV;
                break;
            } else tmp = tmp->fg;
        }
    }
}


File* enfiler(File* f, Noeud* n) {
    File* nouveau = (File*)malloc(sizeof(File));
    nouveau->noeud = n;
    nouveau->suivant = NULL;

    if(f == NULL) return nouveau;

    File* tmp = f;
    while(tmp->suivant != NULL) {
        tmp = tmp->suivant;
    }
    tmp->suivant = nouveau;
    return f;
}

Noeud* defiler(File** f) {
    if(*f == NULL) return NULL;
    File* tmp = *f;
    Noeud* n = tmp->noeud;
    *f = (*f)->suivant;
    free(tmp);
    return n;
}

// ============ PARCOURS ============
void Prefixe(Noeud* R) {
    if(R == NULL) return;
    printf(" %d ", R->data);
    Prefixe(R->fg);
    Prefixe(R->fd);
}

void Infixe(Noeud* R) {
    if(R == NULL) return;
    Infixe(R->fg);
    printf(" %d ", R->data);
    Infixe(R->fd);
}

void Postfixe(Noeud* R) {
    if(R == NULL) return;
    Postfixe(R->fg);
    Postfixe(R->fd);
    printf(" %d ", R->data);
}

void ParcoursLargeur(Noeud *R) {
    if(R == NULL) return;
    File *F = NULL;
    Noeud *NPrint = R;
    F = enfiler(F, NPrint);

    while(F != NULL) {
        NPrint = defiler(&F);
        printf(" %d ", NPrint->data);

        if(NPrint->fg != NULL) {
            F = enfiler(F, NPrint->fg);
        }
        if(NPrint->fd != NULL) {
            F = enfiler(F, NPrint->fd);
        }
    }
}


Noeud* search(Noeud* R, int val) {
    if(R == NULL) return NULL;
    if(val == R->data) return R;
    else if(val < R->data) return search(R->fg, val);
    else return search(R->fd, val);
}

Noeud* Min(Noeud* R) {
    if(R == NULL) return NULL;
    Noeud* tmp = R;
    while(tmp->fg != NULL) {
        tmp = tmp->fg;
    }
    return tmp;
}

Noeud* Max(Noeud* R) {
    if(R == NULL) return NULL;
    Noeud* tmp = R;
    while(tmp->fd != NULL) {
        tmp = tmp->fd;
    }
    return tmp;
}


Noeud* suppNoeud(Noeud* racine, int valeur) {
    if(racine == NULL) return NULL;

    if(valeur < racine->data) {
        racine->fg = suppNoeud(racine->fg, valeur);
    } else if(valeur > racine->data) {
        racine->fd = suppNoeud(racine->fd, valeur);
    } else {

        if(racine->fg == NULL) {
            Noeud* temp = racine->fd;
            free(racine);
            return temp;
        } else if(racine->fd == NULL) {
            Noeud* temp = racine->fg;
            free(racine);
            return temp;
        }

        Noeud* temp = Min(racine->fd);
        racine->data = temp->data;
        racine->fd = suppNoeud(racine->fd, temp->data);
    }
    return racine;
}


Noeud* construireABREquilibre(int* tab, int debut, int fin) {
    if(debut > fin) return NULL;
    int milieu = (debut + fin) / 2;
    Noeud* racine = creerNoeud(tab[milieu]);
    racine->fg = construireABREquilibre(tab, debut, milieu - 1);
    racine->fd = construireABREquilibre(tab, milieu + 1, fin);
    return racine;
}


int max(int a, int b) {
    return (a > b) ? a : b;
}

int hauteur(Noeud* n) {
    if(n == NULL) return 0;
    return n->hauteur;
}

int facteur_equilibre(Noeud* n) {
    if(n == NULL) return 0;
    return hauteur(n->fg) - hauteur(n->fd);
}

Noeud* rotationDroite(Noeud* y) {
    Noeud* x = y->fg;
    Noeud* T2 = x->fd;
    x->fd = y;
    y->fg = T2;
    y->hauteur = 1 + max(hauteur(y->fg), hauteur(y->fd));
    x->hauteur = 1 + max(hauteur(x->fg), hauteur(x->fd));
    return x;
}

Noeud* rotationGauche(Noeud* x) {
    Noeud* y = x->fd;
    Noeud* T2 = y->fg;
    y->fg = x;
    x->fd = T2;
    x->hauteur = 1 + max(hauteur(x->fg), hauteur(x->fd));
    y->hauteur = 1 + max(hauteur(y->fg), hauteur(y->fd));
    return y;
}

Noeud* insererAVL(Noeud* racine, int valeur) {
    if(racine == NULL) return creerNoeud(valeur);

    if(valeur < racine->data)
        racine->fg = insererAVL(racine->fg, valeur);
    else if(valeur > racine->data)
        racine->fd = insererAVL(racine->fd, valeur);
    else
        return racine;

    racine->hauteur = 1 + max(hauteur(racine->fg), hauteur(racine->fd));
    int balance = facteur_equilibre(racine);

    // Cas LL
    if(balance > 1 && valeur < racine->fg->data)
        return rotationDroite(racine);

    // Cas RR
    if(balance < -1 && valeur > racine->fd->data)
        return rotationGauche(racine);

    // Cas LR
    if(balance > 1 && valeur > racine->fg->data) {
        racine->fg = rotationGauche(racine->fg);
        return rotationDroite(racine);
    }

    // Cas RL
    if(balance < -1 && valeur < racine->fd->data) {
        racine->fd = rotationDroite(racine->fd);
        return rotationGauche(racine);
    }

    return racine;
}


int main() {
    printf("             TEST ABR SIMPLE           \n");
    Noeud* arbre = NULL;
    insererNoeud(&arbre, 50);
    insererNoeud(&arbre, 42);
    insererNoeud(&arbre, 66);
    insererNoeud(&arbre, 67);
    insererNoeud(&arbre, 30);
    insererNoeud(&arbre, 45);

    printf("Parcours Prefixe: ");
    Prefixe(arbre);
    printf("\n");

    printf("Parcours Infixe: ");
    Infixe(arbre);
    printf("\n");

    printf("Parcours Postfixe: ");
    Postfixe(arbre);
    printf("\n");

    printf("Parcours en Largeur: ");
    ParcoursLargeur(arbre);
    printf("\n\n");

    printf("              TEST RECHERCHE               \n");
    int valRecherche = 42;
    Noeud* trouve = search(arbre, valRecherche);
    if(trouve != NULL)
        printf("Valeur %d trouvee!\n", valRecherche);
    else
        printf("Valeur %d non trouvee.\n", valRecherche);

    Noeud* minimum = Min(arbre);
    Noeud* maximum = Max(arbre);
    printf("Minimum: %d\n", minimum->data);
    printf("Maximum: %d\n\n", maximum->data);

    printf("                     TEST SUPPRESSION             \n");
    printf("Avant suppression de 42: ");
    Infixe(arbre);
    printf("\n");
    arbre = suppNoeud(arbre, 42);
    printf("Apres suppression de 42: ");
    Infixe(arbre);
    printf("\n\n");

    printf("                TEST ABR ÉQUILIBRE               \n");
    int tab[] = {10, 20, 30, 40, 50, 60, 70};
    int n = sizeof(tab) / sizeof(tab[0]);
    Noeud* abrEquilibre = construireABREquilibre(tab, 0, n - 1);
    printf("ABR équilibre (Infixe): ");
    Infixe(abrEquilibre);
    printf("\n");
    printf("ABR équilibre (Prefixe): ");
    Prefixe(abrEquilibre);
    printf("\n\n");

    printf("              TEST AVL          \n");
    Noeud* avl = NULL;
    int valeurs[] = {30, 20, 40, 10, 25, 22};
    n = sizeof(valeurs) / sizeof(valeurs[0]);

    for(int i = 0; i < n; i++) {
        avl = insererAVL(avl, valeurs[i]);
        printf("Apres insertion de %d: ", valeurs[i]);
        Infixe(avl);
        printf(" (hauteur=%d, balance=%d)\n", hauteur(avl), facteur_equilibre(avl));
    }

    printf("\nParcours final AVL (Infixe): ");
    Infixe(avl);
    printf("\n");

    return 0;
}
