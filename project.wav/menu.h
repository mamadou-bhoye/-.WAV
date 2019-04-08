

typedef enum {MenuP = 0, Autre } Fonctions;
typedef struct menu_s menu_t;
typedef struct item_s item_t;
typedef struct action_s action_t;
typedef unsigned int uint;
menu_t* createMenu(const char*);
void purge(void);
void alerte(const char*);
void addMenuAction(menu_t* , const char* , void(*f)());
void addSubMenu(menu_t* m, menu_t* sm);
void libere(menu_t* );
void deleteMenu(menu_t*);
void affiche(menu_t* );
void selecte(menu_t* );
void launchMenu(menu_t* );
void f1();
void f2();
void quit(menu_t *);
void retour(menu_t*);
void ouvrir();
void enregister();
