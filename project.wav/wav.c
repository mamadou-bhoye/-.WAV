#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
typedef struct Fichier* wave_t;
struct WAV
{
  char RIFF[4];
  int32_t tailleS;
  char WAVE[4];
  char FMT[4];
  int32_t nbre16;
  int16_t format; // 1 toujours
  uint16_t nbreC; //c
  uint32_t frqceF;//f
  int32_t nbreR; //r
  uint16_t nbreOB;//b
  uint16_t echtP;//p
  char DATA[4];
  int32_t tailleD;//D
};
// int8_t, int32_t, int32_t, uint8_t, uint16_t, uint32_t

struct Fichier
{
  struct WAV* Header;
  int **Canal;
};

/* ************************************Construction, destruction, édition de base ***************************** */



wave_t wave_new(uint32_t f, uint16_t p, uint16_t c, uint32_t B)
{
  wave_t w;
  w = malloc(sizeof(struct Fichier));
  w -> Header = malloc(sizeof(struct WAV));
  w -> Header -> nbre16 = 16;
  w -> Header -> format = 1;
  w -> Header-> frqceF = f;
  w -> Header-> echtP = p;
  c = (c>=6)?6:c;
  w -> Header-> nbreC = c;
  w -> Header-> nbreOB = c * (p/8);
  w -> Header-> nbreR = f * (w ->  Header-> nbreOB);
  w -> Header-> tailleD = B * f * (w -> Header-> nbreOB);
  w -> Header-> tailleS = 36 + (w -> Header -> tailleD); //36 = 44 -8
  strncpy(w -> Header -> RIFF, "RIFF ", 4);
  strncpy(w -> Header -> FMT, "fmt  ", 4);
  strncpy(w -> Header -> DATA, "data ", 4);
  strncpy(w -> Header -> WAVE, "WAVE ", 4);
  w -> Canal = calloc(B,sizeof(int *));
  for (int i = 0; i < B; i++){
    w -> Canal[i] = malloc( c * sizeof(int));
  }
  return w;
}
/************************************************************************************/

void enteteW(wave_t w)
{
  printf("DEBUT\n");
  printf(" 1 -RIFF:%s\n", w -> Header -> RIFF);
  printf(" 2 -tailleS:%d\n", w -> Header -> tailleS);
  printf(" 3 -WAVE:%4s\n", w -> Header -> WAVE);
  printf(" 4 -FMT:%4s\n", w -> Header -> FMT);
  printf(" 5 -nbre16:%d \n", w -> Header -> nbre16);
  printf(" 6 -format:%d \n", w -> Header -> format);
  printf(" 7 -nbreC:%d \n", w -> Header -> nbreC);
  printf(" 8 -frqceF:%d \n", w -> Header -> frqceF);
  printf(" 9 -nbreR:%d \n", w -> Header -> nbreR);
  printf("10 -nbreOB:%d \n", w -> Header -> nbreOB);
  printf("11 -echtP:%d \n", w -> Header -> echtP);
  printf("12 -DATA:%4s \n", w -> Header -> DATA);
  printf("13 -tailleD:%d \n", w -> Header -> tailleD);
  printf("FIN\n");
}


/************************************************************************************/
void wave_delete(wave_t w)
{
  for (int i = 0; i < (w -> Header -> tailleD) / (w-> Header -> nbreR); i++)
  {
    free( w-> Canal[i]);
  }
  free(w -> Canal);
  free(w -> Header);
  free(w);
}
/************************************************************************************/
void decal(int64_t* nbre, int** tab, int n, int i, int j)
{
  for (int k = 0; k < n; k++)
  {
    *nbre = (*nbre | (tab[i][j] & 0x000000FF));
    *nbre <<= 8;
    tab[i][j] >>= 8;
  }
}

bool endianness()
{
  /* 1 --> Little endian *** 0 --> Big endian */
  unsigned test = 1;
  return (*(char *)&test == 1);
}

int64_t wave_get(wave_t w, uint32_t i, uint16_t j)
{
  int64_t tmp;
  if (w -> Canal[i][j] > 0)
  { // positif
    tmp = 0x0000000000000000L;
    if (endianness()) //Little
    {
      if (w -> Header -> echtP == 8) tmp = ( tmp | (w -> Canal[i][j] & 0x000000FF) ) << 56; // 56 = 64 - 8
      else if (w -> Header -> echtP == 16){ decal(&tmp, w -> Canal, 2, i, j);tmp <<= 48;}   // 48 = 64 - 8*2
      else if (w -> Header -> echtP == 24){ decal(&tmp, w -> Canal, 3, i, j);tmp <<= 40;}   // 40 = 64 - 8*3
    }else{// Big
      tmp |= w -> Canal[i][j];
      if (w -> Header -> echtP == 8) tmp <<= 56;
      else if (w -> Header -> echtP == 16) tmp <<= 48;
      else if (w -> Header -> echtP == 24) tmp <<= 40;
    }
  }else{//Negatif
    tmp = 0xFFFFFFFFFFFFFFFFL;
    if (endianness()) // Little
    {
      if (w -> Header -> echtP == 8) {tmp = (tmp & (w -> Canal[i][j] & 0x000000FF))<<56;tmp |= 0x00FFFFFFFFFFFFFFL;}
      else if (w -> Header -> echtP == 16){ decal(&tmp, w -> Canal, 2, i, j);tmp <<= 48;tmp |= 0x0000FFFFFFFFFFFFL;}
      else if (w -> Header -> echtP == 24){ decal(&tmp, w -> Canal, 3, i, j);tmp <<= 40;tmp |= 0x000000FFFFFFFFFFL;}
    }else{ // Big
      tmp |= w -> Canal[i][j];
    }
  }
  return tmp;
}
/************************************************************************************/

void wave_set(wave_t w, uint32_t i, uint16_t j, int64_t a)
{
  w -> Canal[i][j] = *(int *)(&a);
/*
  if (endianness())
  {
    if ( (a & 0x0000000000000080L) == 1)
    {
      if (w -> Header -> echtP == 8) {}
      else if (w -> Header -> echtP == 16 ) {}
      else if (w -> Header -> echtP == 24 ) {}
    }else {
      if (w -> Header -> echtP == 8) {}
      else if (w -> Header -> echtP == 16 ) {}
      else if (w -> Header -> echtP == 24 ) {}
    }
  }else{
    if ( (a & 0x8000000000000000L) == 1)
    {
      if (w -> Header -> echtP == 8) {}
      else if (w -> Header -> echtP == 16 ) {}
      else if (w -> Header -> echtP == 24 ) {}
    }else {
      if (w -> Header -> echtP == 8) {}
      else if (w -> Header -> echtP == 16 ) {}
      else if (w -> Header -> echtP == 24 ) {}
    }
  }
  */
}
/************************************************************************************/

void add_signal(wave_t w, double (*s)(double))
{

}
/************************************************************************************/
int ** CreeTabDy(int L, int C)
{
  int **tab =NULL;
  tab = malloc(L * sizeof(int * ));
  for (int i = 0; i < L; i++)
  {
    tab[i] = malloc(C * sizeof(int));
  }
  return tab;
}

wave_t wave_load(const char* fname)
 {
   FILE* file;
   wave_t ret;
   int i,j;
   ret = malloc(sizeof(wave_t));
   file = fopen(fname, "r+");
   if (ret == NULL || file == NULL) {return NULL;}
   printf("dfxcgvbndxf %p   %p\n", ret, file);
   fscanf(file, "%s ",ret -> Header -> RIFF);
   fscanf(file, "%d ",&ret -> Header -> tailleS);
   fscanf(file, "%s ",ret -> Header -> WAVE);
   fscanf(file, "%s ",ret -> Header -> FMT);
   fscanf(file, "%d ",&ret -> Header -> nbre16);
   fscanf(file, "%hd ",&ret -> Header -> format);
   fscanf(file, "%hd ",&ret -> Header -> nbreC);
   fscanf(file, "%u ",&ret -> Header -> frqceF);
   fscanf(file, "%d ",&ret -> Header -> nbreR);
   fscanf(file, "%hd ",&ret -> Header -> nbreOB);
   fscanf(file, "%hd ",&ret -> Header -> echtP);
   fscanf(file, "%s ",ret -> Header -> DATA);
   fscanf(file, "%d\n",&ret -> Header -> tailleD);
   int B = (ret -> Header -> tailleD)/(ret -> Header -> nbreR);
   ret -> Canal = calloc(B,sizeof(int *));
   for (i = 0; i < B; i++){
     ret -> Canal[i] = malloc( ret -> Header -> nbreC * sizeof(int));
   }

   for (i = 0; i < B && feof(file) != EOF; i++)
   {
     for (j = 0; j < (ret -> Header -> nbreC) && (feof(file) != EOF) ; j++)
     {
       fscanf(file, "%d",&(ret -> Canal[i][j]));fseek(file, 1, ftell(file));
     }
   }
   /**************************/
   fclose(file);
   return ret;
 }

/************************************************************************************/
void putS( FILE* fc, char *tab, char esp)
{
  for (int i = 0; i < 4; i++)fputc(tab[i], fc);
  fputc(esp, fc);
}
bool wave_save(const char* fname, wave_t w)
{
  FILE* file;
  if ((file = fopen(fname, "w")) == NULL){return false;}
  putS( file, w -> Header-> RIFF, ' ');
  fprintf(file,"%d ", w -> Header-> tailleS);
  putS( file, w -> Header-> WAVE,' ');
  putS( file, w -> Header-> FMT, ' ');
  fprintf(file,"%d ", w -> Header-> nbre16);
  fprintf(file,"%d ", w -> Header-> format);
  fprintf(file,"%d ", w -> Header-> nbreC);
  fprintf(file,"%d ", w -> Header-> frqceF);
  fprintf(file,"%d ", w -> Header-> nbreR);
  fprintf(file,"%d ", w -> Header-> nbreOB);
  fprintf(file,"%d ", w -> Header-> echtP);
  putS( file, w -> Header-> DATA,' ');
  fprintf(file,"%d\n", w -> Header-> tailleD);
  int B = (w -> Header -> tailleD)/(w -> Header-> nbreR );
  for (int i = 0; i < B; i++)
  {
    for (int j = 0; j < (w -> Header-> nbreC); j++) {fprintf(file,"%d ", w -> Canal[i][j]);}
    fputc('\n',file);
  }
  fclose(file);
  return true;
}

/* ********************************************** TRANSFORMATIONS ************************************************* */
void wave_canal(wave_t w, uint16_t c)
{
  c =(c>6)?6:c;
  int B = (w -> Header -> tailleD)/(w -> Header-> nbreR );
  if ( w -> Header -> nbreC < c)
  {
    for (int i = 0; i < B; i++)
    {
      for (int j = w -> Header -> nbreC - 1 ; j < c; j++) w -> Canal[i][j] = w -> Canal [0][0];
    }
  }else{
    int g =0;
    for (int i = 0; i < B; i++)
    {
      for (int j = w -> Header -> nbreC -1 ; j > c - 1; j --)
      {
        g = w -> Canal[i][j];
        for (int k = 0; k < c; k++){w -> Canal[i][k] += g;}
        w -> Canal[i][j] = 0;
      }
    }
  }
  w -> Header -> nbreC = c;
}
