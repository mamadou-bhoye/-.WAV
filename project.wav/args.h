
struct option_s;
typedef struct option_s option_t;
option_t* opt_void(option_t* l, const char* kw, void (*f)());
option_t* opt_int(option_t* l, const char* kw, void (*f)(int));
option_t* opt_float(option_t* l, const char* kw, void (*f)(float));
option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*));
void opt_delete(option_t* l);
void process_arguments(option_t* l, int argc, char* *argv);
void F1(const char* str);
void F2(int i) ;
void F3() ;
void F4(float f);
enum { VOID = 0, INT, STRING, FLOAT } TYPE;
