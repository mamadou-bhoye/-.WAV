#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct option_s option_t;

struct option_s {
  const char* motCle;
  enum { VOID = 0, INT, STRING, FLOAT } TYPE;
  union {
    void (*opt_void)();
    void (*opt_int)(int);
    void (*opt_str)(const char*);
    void (*opt_float)(float);
  } fonction;
  struct option_s* suiv;
};

//const option_t* NOOPTION = NULL;

option_t* opt_void(option_t* l, const char* kw, void (*f)())
{
  option_t *p, *tmp;
  p = malloc(sizeof(option_t)) ;
  tmp = l;
  p -> motCle = kw;
  p -> TYPE = VOID;
  p -> fonction . opt_void = f;
  while (1)
  {
    if (tmp -> suiv == NULL){tmp -> suiv = p; break;}
    tmp = tmp -> suiv;
  }
  return l;
}

option_t* opt_int(option_t* l, const char* kw, void (*f)(int))
{
  option_t *p, *tmp;
  p = malloc(sizeof(option_t)) ;
  tmp = l;
  p -> motCle = kw;
  p -> TYPE = INT;
  p -> fonction . opt_int = f;
  while (1)
  {
    if (tmp -> suiv == NULL){tmp -> suiv = p;break;}
    tmp = tmp -> suiv;
  }
  return l;
}

option_t* opt_float(option_t* l, const char* kw, void (*f)(float))
{
  option_t *p, *tmp;
  p = malloc(sizeof(option_t)) ;
  tmp = l;
  p -> motCle = kw;
  p -> TYPE = FLOAT;
  p -> fonction . opt_float = f;
  while (1)
  {
    if (tmp -> suiv == NULL){tmp -> suiv = p;break;}
    tmp = tmp -> suiv;
  }
  return l;
}

option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*))
{
  option_t *p, *tmp;
  p = malloc(sizeof(option_t)) ;
  tmp = l;
  p -> motCle = kw;
  p -> TYPE = STRING;
  p -> fonction . opt_str = f;
  while (1)
  {
    if (tmp -> suiv == NULL){tmp -> suiv = p;break;}
    tmp = tmp -> suiv;
  }
  return l;
}


void opt_delete(option_t* l)
{
  if ( l -> suiv !=NULL)
  {
    opt_delete(l -> suiv );
    free(l);
  }
  else free(l);
}

void process_arguments(option_t* l, int argc, char* *argv)
{
  option_t *tmp= l;
  int i = 0;
  while ( i < argc-1)
  {
    if (strcmp(argv[i], tmp -> motCle))
    {
      printf("sdtwxfhyg\n");
      switch (tmp -> TYPE)
      {
        case VOID:
          tmp -> fonction . opt_void();
          i++;
          break;
        case INT:
          tmp -> fonction . opt_int( (intptr_t)argv[i+1]);
          i+=2;
          break;
        case STRING:
          tmp -> fonction . opt_str((char*)argv[i+1]);
          i+=2;
          break;
        case FLOAT:
          //tmp -> fonction . opt_float((float)argv[i+1]);
          i+=2;
          break;
      }
      tmp = tmp -> suiv;
    }
  }
}
void F1(const char* str) {
  printf("F1: %s\n", str);
}

void F2(int i) {
  printf("F2: %d\n", i);
}

void F3() {
  printf("F3: no param\n");
}

void F4(float f) {
  printf("F4: %f\n", f);
}
/*
int main(int argc, char** argv) {
  int i;

  printf("Programme '%s' lancé avec %d option(s) :\n", argv[0], argc-1);
  for(i = 1; i < argc; i++)
    printf("Option %d : %s\n", i, argv[i]);

  return EXIT_SUCCESS;
}
*/
