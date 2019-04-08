
#include<string.h>
#include<stdbool.h>

typedef struct xelement_s xelement_t;
typedef struct xattribute_s xattribute_t;


xelement_t* create_xelement(const char* name);
xattribute_t* add_xattribute(xelement_t* e, const char* name,const char* value);
void add_sub_xelement(xelement_t* e,xelement_t* s);
void add_raw(xelement_t* e, const char* r);
void delete_xelement(xelement_t* e);
void save_xelement(FILE* fd , xelement_t* e);
void save_xml(const char* fname, xelement_t* e);
void print_xelement(xelement_t* e) ;
void check_next_char(FILE* fd,char c);
bool is_next_char(FILE* fd,char c, bool cons);
char* next_word(FILE*fd);
void check_next_word(FILE* fd,const char* w);
char* next_string(FILE* fd);
char* next_raw(FILE* fd);
xelement_t* load_xelement(FILE* fd, const char* end_tag);
void load_xelement_raw(FILE* fd, xelement_t* e);
void load_xelement_sub(FILE* fd, xelement_t* e);
void load_xelement_content(FILE* fd, xelement_t* e);
xelement_t* load_xml(const char* fname);