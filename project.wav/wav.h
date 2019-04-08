#include <stdbool.h>
#include <stdint.h>
typedef struct Fichier * wave_t;
wave_t wave_new(uint32_t f, uint16_t p, uint16_t c, uint32_t B);
bool endianness();
void wave_delete(wave_t w);
int64_t wave_get(wave_t w, uint32_t i, uint16_t j);
void wave_set(wave_t w, uint32_t i, uint16_t j, int64_t a);
void add_signal(wave_t w, double (*s)(double));
void enteteW(wave_t);
wave_t wave_load(const char* fname);
bool wave_save(const char* fname, wave_t w);
extern wave_t WAVE_COURANT;
void wave_canal(wave_t,int16_t);
void genererPiste(wave_t w);
