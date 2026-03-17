#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#define BUF_SIZE 512

#define GUARD 0
#ifdef GUARD
#include <langinfo.h>
void guard(){
	if (strcmp(nl_langinfo(CODESET), "UTF-8") != 0) {
	    fprintf(stderr, "UTF-8 locale required\n");
	    exit(1);
	}
}
#endif

void print_bytes(const unsigned char* p, int size){
	printf("0x");
	for (int i = 0; i < size; ++i){
		if (*(p+i) == 0) break;
		printf("%x", *(p+i));
	}
}
int from_unicode(){
	//setlocale(LC_CTYPE, "zh_CN.GB2312");
	//setlocale(LC_CTYPE, "zh_CN.UTF-8");
	/*
	 * LANG=zh_CN.GB2312 时，输出的字符就是 GB2312 的 bytes，正确显示需要 Terminal Emulator 支持，gnome-terminal preference-Compatiblilty-Encoding
	 */
	setlocale(LC_CTYPE, "");

	wchar_t unicode = 0x4f60; // unicode of 你

	char locale_char[BUF_SIZE]={0};
	wctomb(locale_char, unicode);
	printf("unicode:          0x%x (fixed)\n", unicode);
	printf("local char:       '%s'\n", locale_char);
	printf("local char bytes: "); print_bytes((unsigned char*)locale_char, BUF_SIZE); printf("\n");
	return 0;
}
int from_hardcode(){
	/*
	 * "你" 其实就是一堆 bytes，而我们的源码是用 UTF-8 存的，所以 "你" 的
	 * bytes 就是 UTF-8 的 bytes
	 *
	 * 而这个时候我设置了 LC_CTYPE 为 GB2312 的之后，就会把这三个 bytes 当
	 * 作 GB2312 的编码，导致 mbtowc 算 unicode 时误把 UTF-8 的前两个 bytes
	 * 当作 GB2312 去解析了。所以出现 unicode 也错的情况。
	 *
	 * 对于显示
	 * * 我们用和 LANG 匹配的 Terminal Emulator 打开。
	 * 	+ UTF-8  的 terminal 显示都是对的
	 * 	+ GB2312 的 terminal 中 code_char 相较于 locale_char 会多个反问
	 * 	  号，这就是第三个 byte 解析失败，虽然前两个 byte 也显示得不正
	 * 	  确，但好歹是解出来了
	 *
	 * * 我们用和 LANG 不匹配的 Terminal Emulator 打开，也就是在 UTF-8 的
	 *   Terminal Emulator 中指定 LANG 为 GB2312
	 * 	+ code_char 是输出和显示都是对的，因为它是固定的
	 * 	+ unicode 的值和 GB2312 Terminal Emulator 中解析出来的是一致的，
	 * 	  因为就是拿 GB2312 去解的。
	 * 	+ local_char 的值和 GB2312 Terminal Emulator 中解析出来的是一致
	 * 	  的。显示反问号，大概是因为这个值落在需要的 bytes 数大于 2 的
	 * 	  的 UTF-8 表中。
	 *
	 * 当然，如果这份文件使用 GB2312 存的话，也是一样的
	 */
	setlocale(LC_CTYPE, "");

	char code_char[BUF_SIZE]="你";
	wchar_t unicode;
	char locale_char[BUF_SIZE]={0};

	mbtowc(&unicode, code_char, BUF_SIZE);
	wctomb(locale_char, unicode);

	printf("code char:        '%s'\n", code_char);
	printf("code char bytes:  "); print_bytes((unsigned char*)code_char, BUF_SIZE); printf(" (fixed)\n");
	printf("-----\n");
	printf("unicode:          0x%x\n", unicode);
	printf("-----\n");
	printf("local char:       '%s'\n", locale_char);
	printf("local char bytes: "); print_bytes((unsigned char*)locale_char, BUF_SIZE); printf("\n");
	printf("-----\n");
	return 0;
}
int from_hardcode_fixup(){
	char code_char[BUF_SIZE]="你";
	wchar_t unicode;
	char locale_char[BUF_SIZE]={0};

	// use source code encoding to parse the code_char
	setlocale(LC_CTYPE, "zh_CN.UTF-8"); // keep the same as source code
	mbtowc(&unicode, code_char, BUF_SIZE);
	/* do other source code decoding */

	// use locale encoding to encoding the unicode
	setlocale(LC_CTYPE, "");
	wctomb(locale_char, unicode);

	printf("code char:        '%s'\n", code_char);
	printf("code char bytes:  "); print_bytes((unsigned char*)code_char, BUF_SIZE); printf(" (fixed)\n");
	printf("-----\n");
	printf("unicode:          0x%x\n", unicode);
	printf("-----\n");
	printf("local char:       '%s'\n", locale_char);
	printf("local char bytes: "); print_bytes((unsigned char*)locale_char, BUF_SIZE); printf("\n");
	printf("-----\n");
	return 0;
}

int from_hardcode_fixup_2(){
	wchar_t unicode = L"你"[0];
	return 0;
}


int main(int argc, char *argv[]){
	from_unicode();
	printf("==============\n");
	from_hardcode();
	printf("=====fixup=========\n");
	from_hardcode_fixup();
	
	return 0;
}
