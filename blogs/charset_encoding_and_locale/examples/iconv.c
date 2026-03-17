#include <locale.h>
#include <stdio.h>
#include <libintl.h>
#include <stdlib.h>
#include <string.h>
#include <iconv.h>
void my_iconv_gb2312_utf8( char **inbuf, size_t *inbytesleft,
		char **outbuf, size_t *outbytesleft){

	int consuming_in_gb2312_char=0;
	int consuming_in_utf8_char=0;
	wchar_t tmp_wc;
	for (size_t i = 0; i < *inbytesleft; ++i){
		if (!*(*inbuf+consuming_in_gb2312_char)) break;
		setlocale(LC_CTYPE, "zh_CN.GB2312"); // char* 中存的 bytes 的格式
		consuming_in_gb2312_char+=mbtowc(&tmp_wc, *(inbuf)+consuming_in_gb2312_char, MB_CUR_MAX);
		//printf("%s\n",code_char+consuming_in_gb2312_char);
		setlocale(LC_CTYPE, "zh_CN.UTF-8"); // char* 中存的 bytes 的格式
		consuming_in_utf8_char+=wctomb(*(outbuf)+consuming_in_utf8_char, tmp_wc);
		//printf("%d %d\n", consuming_in_gb2312_char, consuming_in_utf8_char);
	}
}

int main(void)
{
	setlocale(LC_CTYPE, "zh_CN.GB2312");
	setlocale(LC_MESSAGES, "zh_CN.GB2312");
	bindtextdomain("ben_test", "./locale");
	textdomain("ben_test");

	char code_char[256]="你好世界！";
	wchar_t tmp_wc[256]={0};
	char utf8_char[256]={0};
	unsigned long inbytesleft=256;
	unsigned long outbytesleft=256;

	char *p_code_char=code_char;
	char *p_utf8_char=utf8_char;

	printf("p_utf8_char=%p\n", p_utf8_char);
#define MY_ICONV
#ifdef MY_ICONV
	my_iconv_gb2312_utf8(
			&p_code_char, &inbytesleft,
			&p_utf8_char, &outbytesleft);
#else
	iconv( iconv_open("UTF-8", "GB2312"),
			&p_code_char, &inbytesleft,
			&p_utf8_char, &outbytesleft);
#endif
	printf("p_utf8_char=%p\n", p_utf8_char);

	printf("%s\n", utf8_char);
	return 0;
}
