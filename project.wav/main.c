#ifndef BIBLIOTHEQUES
  #include <stdlib.h>
  #include <stdio.h>
  //#include "args.h"
  #include "menu.h"

#endif

/**********:::: VARIABLES GLOBALES && CERTAINS PROTOTYPES ::::::*******/
menu_t *MENU_COURANT, *MENU_PRINCIPALE;
char NOM_COURANT[29];

/**********::::::::::::::::::::: LA FONCTION PRINCIPALE :::::::::::::::::::::::::::*******/
int main(int argc, char** argv)
{
 
  menu_t *Menu, *sm,*ssm;
  Menu = createMenu("MENU PRINCIPALE");
  MENU_PRINCIPALE= Menu;
  sm = createMenu("Fichier");
  addSubMenu(Menu,sm);
  addMenuAction(sm,"Ouvrir",f1);
   ssm=createMenu("enregister sous");
  addSubMenu(sm,ssm);
  addMenuAction(ssm,"svg", f1);
  addMenuAction(ssm,"Pdf", f1);
  addMenuAction(ssm,"Retour",f1);
  addMenuAction(sm,"Information",f1);
  addMenuAction(sm,"affichage",f1);
  addMenuAction(sm,"Quitter",quit);
  sm = createMenu("Figure");
  addSubMenu(Menu,sm);
  addMenuAction(sm,"type de figure",f1);
  addMenuAction(sm,"changer de couleur",f1);
  addMenuAction(sm,"Redimensionner*",f1);
  addMenuAction(sm,"Retour",f1);
   addMenuAction(sm,"Quitter",quit);
  sm = createMenu("Groupe");
  addSubMenu(Menu,sm);
  addMenuAction(sm,"grouper les images",f1);
  addMenuAction(sm,"Quitter",quit);
  launchMenu(Menu);
  return EXIT_SUCCESS;
}
/**********:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*******/


/*
option_t noop; //= malloc(sizeof(option_t));
strcpy(noop . motCle,"amd");
noop -> TYPE = VOID;
const option_t* NOOPTION = &noop;
option_t* opt =(option_t*)NOOPTION;
opt = opt_string(opt, "-a", F1);
opt = opt_int(opt, "-b", F2);
opt = opt_void(opt, "-c", F3);
opt = opt_float(opt, "-d", F4);
process_arguments(opt, argc, argv);
opt_delete(opt);
*/
