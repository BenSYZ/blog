#include <stdio.h>
#include <locale.h>
#include <string.h>
int main(int argc, char *argv[]){
	setlocale(LC_CTYPE, "zh_CN.GB2312");
	setlocale(LC_COLLATE, "zh_CN.GB2312");
	char s1[256]="ƒ„∫√ ¿ΩÁ"; //c4e3 bac3 cac0 bde7
	char s2[256]="„∫√ "; //e3ba c3ca
	printf("%s\n", s1);
	printf("%s\n", s2);
	printf("%s\n", strstr(s1, s2)==0? "Not found": "Founded");
	return 0;
}
