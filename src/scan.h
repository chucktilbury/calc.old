
#ifndef __SCAN_H__
#define __SCAN_H__

extern int yylex(void);
extern int yyparse(void);
extern void yyerror(const char *s);
extern void* yy_scan_string(const char* str);
extern void yy_delete_buffer(const char* str);

#endif
