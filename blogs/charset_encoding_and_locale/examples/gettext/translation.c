#include <locale.h>
#include <stdio.h>
#include <libintl.h>
int main(void)
{
	/* 由于我们没有对 gettext 的参数做本地化转化，所以该参数目前只会影响输出*/
	setlocale(LC_CTYPE, "zh_CN.UTF-8");

	/* 选 mo LC_MESSAGES 的父目录，即用哪个翻译文件 */
	setlocale(LC_MESSAGES, "zh_CN.GB2312");


	/* 设置在哪找这个 text domain */
	bindtextdomain("ben_test", "./locale");
	/* set default text domain */
	textdomain("ben_test");

	/* 设置 gettext 输出的 bytes 的 codeset。如果不设置，就会用 LC_CTYPE 的 bytes 来编码。 */
	//bind_textdomain_codeset("ben_test", "GB2312");

	/* gettext：
	 * 1. 根据 LC_MESSAGES(zh_CN.UTF-8)，找到 <text_domain>.mo
	 * 2. 根据 gettext 的参数 byte by byte 比较 msg_id，找到 msg_str 的 bytes
	 * 	* msgfmt --no-convert 会保留原始 mo 文件的编码格式，而不是统一转换成 UTF-8，原文件不是 UTF-8，并对 gettext 的参数没有转换时有用
	 * 3. 找到 mo 中的 charset（UTF-8/GB2312）作为 iconv 的 from-enconding
	 * 4. 根据 LC_CTYPE/$OUTPUT_CHARSET/bind_textdomain_codeset 作为 iconv 的 to-encoding
	 * 5. iconv 将 mo 中的 bytes 转成输出的 bytes，return
	 */
	char *char_translated = gettext("世");
	//char *char_translated = gettext("Hello world!");

	printf("char_translated=%s\n", char_translated);
	return 0;
}
