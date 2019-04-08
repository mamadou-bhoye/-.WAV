#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define  MAX 9
typedef struct menu_s menu_t;
typedef struct item_s item_t;
typedef struct action_s action_t;
typedef unsigned int uint;
typedef enum {MenuP = 0, Autre } Fonctions;

extern menu_t *MENU_PRINCIPALE,*MENU_COURANT;
extern char* NOM_COURANT;

void alerte(char *mot)
{
  printf("\n\t::::::::: %s :::::::::\n", mot);
}

static void purge(void)
{
    int c;
    while ((c = fgetc(stdin)) != '\n' && c != EOF);
}

struct menu_s
{
  struct menu_s* parent;
  char * nomMenu;
  item_t *cpsntes;
  uint cptItem;
};
struct item_s
{
  enum {VIDE=0, ACT,MENU} TYPE;
  union
  {
    struct menu_s* ssMenu;
    action_t* Action;
  }contenu;
};

struct action_s
{
  char* nomAc;
  void (*fonction) ();
};

menu_t* createMenu(const char* text)
{
  int i;
  menu_t* p = NULL;
  if( (p = malloc(sizeof(menu_t)) ) == NULL) {alerte(" Erreur d'allocation ");}
  p -> nomMenu = malloc(strlen(text)+1);
  p -> nomMenu = strcpy(p -> nomMenu, text);
  p -> parent = NULL;
  p -> cpsntes = malloc(MAX * sizeof(item_t));
  for ( i= 0; i < MAX; i++){ p -> cpsntes[i] . TYPE = VIDE;}
  p -> cptItem = 0;
  return p;
}
void addMenuAction(menu_t* m, const char* text, void(*f)())
{
  if ( (m-> cptItem) < MAX -1)
  {
    m -> cpsntes[m-> cptItem] . TYPE = ACT;
    m -> cpsntes[m-> cptItem] . contenu . Action  = (action_t *)malloc(sizeof(action_t));
    m -> cpsntes[m-> cptItem] . contenu . Action -> nomAc = malloc(strlen(text)+1);
    strcpy(m -> cpsntes[m-> cptItem] . contenu . Action -> nomAc,text);
    m -> cpsntes[m-> cptItem] . contenu . Action -> fonction = f;
    m -> cptItem ++;
    return;
  }
}

void addSubMenu(menu_t* m, menu_t* sm)
{
  if ( (m -> cptItem) < MAX -1 )
  {
    m -> cpsntes [m -> cptItem] . TYPE = MENU;
    m -> cpsntes [m -> cptItem] . contenu . ssMenu = malloc(sizeof(struct menu_s*));
    m -> cpsntes[m -> cptItem] . contenu . ssMenu = sm;
    sm -> parent = m;
    m -> cptItem ++;
    return;
  }
  alerte("Nombre max de SOUS MENU atteint!");
}

void libere(menu_t *m)
{
  int i= 0;
  while(i < (m -> cptItem)){
    if( m -> cpsntes[i] . TYPE == MENU )
    {
      libere( m -> cpsntes[i] . contenu. ssMenu);
      free (m -> cpsntes[i] . contenu . ssMenu -> nomMenu);
      free (m -> cpsntes[i] . contenu. ssMenu);
    }else if (m -> cpsntes[i] . TYPE == ACT)
    {
      free (m -> cpsntes[i] . contenu . Action -> nomAc);
      free(m -> cpsntes[m-> cptItem] . contenu . Action);
    }
    i++;
  }
}

void deleteMenu(menu_t* m)
{
  if(m != NULL && m -> parent == NULL){libere(m);}
  else printf(" SOUS MENU !!!!!!!!!\n" );
}
void affiche(menu_t* m)
{
  int i;
  printf("\t::::::::::::::::::::::::::::::::::::::\n");
  printf("\t \t  $ %s $  \n", m -> nomMenu);
  printf("\t::::::::::::::::::::::::::::::::::::::\n");
  for ( i = 0; i < m -> cptItem;)
  {
      if ( m -> cpsntes[i] . TYPE == ACT)
      {
        printf("\t  %d -> %s\n", i+1, m -> cpsntes[i] . contenu . Action -> nomAc);
        i++;
      }else if (m -> cpsntes[i] . TYPE == MENU)
      {
        printf("\t  %d -> %s\n", i+1, m -> cpsntes[i] . contenu . ssMenu-> nomMenu);
        i++;
      }
  }
  printf("\t--------------------------------------\n");
}

void launchMenu(menu_t* m)
{
  system("clear");
  affiche(m);
  uint saisie = 10;
  while((saisie > (m -> cptItem)) || saisie <1)
  {
     printf("\tEntrez votre choix :");
     if (scanf("%u", &saisie) == 0)purge();
  }
  saisie -=1;
   if (m -> cpsntes[saisie] . TYPE == ACT)
   {
     MENU_COURANT = m;
     m -> cpsntes[saisie] . contenu. Action -> fonction();
   }else if( m -> cpsntes[saisie] . TYPE == MENU)
   {
     MENU_COURANT = m -> cpsntes[saisie] . contenu. ssMenu -> parent;
     launchMenu (m -> cpsntes[saisie] . contenu. ssMenu);
   }
}

void retour()
{
  if (MENU_COURANT -> parent != NULL) launchMenu(MENU_COURANT -> parent);
  else {
    alerte("Retour non autorisé!!\n");
    launchMenu(MENU_COURANT);
  }
}

/*void ouvrir()
{
  printf("\tEntrez le nom du fichier à ouvrir : ");
  scanf("%s",NOM_COURANT);purge();
  FILE* fic = NULL;
  if ( (fic = fopen(NOM_COURANT, "r")) == NULL) alerte("Erreur de chargement");
  if ( WAVE_COURANT != NULL)
  {
    wave_save(NOM_COURANT, WAVE_COURANT);
    WAVE_COURANT = wave_load(NOM_COURANT);
    enteteW(WAVE_COURANT);
  }else{
    WAVE_COURANT = wave_load(NOM_COURANT);
    printf("%s\n",NOM_COURANT);
    enteteW(WAVE_COURANT);
  }
}
*/
void f1() {
  alerte("Fonctionnalité indisponible!!");
  sleep(1);
  alerte("Retour au MENU PARENT");
  sleep(1);
  launchMenu(MENU_COURANT);

}

void quit()
{
  alerte("MERCI À BIENTÔT!!!!");
  deleteMenu(MENU_PRINCIPALE);
}

/*void enregister()
{
  uint choix =0;
  do {
    printf("\t  1 -> WAVE 8bits Mono 11.025kHz*\n");
    printf("\t  2 -> WAVE 16bits Stereo 44,1kHz*\n");
    printf("\t  3 -> WAVE 24bits 5.1 192kHz*\n");
    printf("\t  4 -> WAVE customisé*\n");
    printf("\t  5 -> Retour*\n");
    printf("\t--------------------------------------\n");
    printf("\tEntrez votre choix :");
    scanf("%u",&choix);purge();
  } while(choix==0 ||choix >5);
  if (choix !=5)
  {
    printf("\tEntrez le nom de votre fichier : ");
    scanf("%s",NOM_COURANT);purge();
  }
  switch (choix)
  {
    case 1:
      wave_canal(WAVE_COURANT, 1);
      wave_save(NOM_COURANT, WAVE_COURANT);
      sleep(1);
      alerte("VOTRE FICHIER A BIEN ÉTÉ SAUVEGARDÉ !!!");
      sleep(1);
      alerte("Retour au MENU PRINCIPALE ");
      sleep(1);
      launchMenu(MENU_PRINCIPALE);
      break;
    case 2:
      wave_canal(WAVE_COURANT, 2);
      wave_save(NOM_COURANT, WAVE_COURANT);
      sleep(1);
      alerte("VOTRE FICHIER A BIEN ÉTÉ SAUVEGARDÉ !!!");
      sleep(1);
      alerte("Retour au MENU PRINCIPALE ");
      sleep(1);
      launchMenu(MENU_PRINCIPALE);
      break;
    case 3:
      alerte("Fonctionnalité indisponible!!");
      sleep(1);
      launchMenu(MENU_COURANT);
    case 4:
      alerte("Fonctionnalité indisponible!!");
      sleep(1);
      launchMenu(MENU_COURANT);
    case 5:
      launchMenu(MENU_COURANT);
  }
}
*/