run: translation
	@echo "mo have been converted to UTF-8"
	@msgfmt -o locale/zh_CN.GB2312/LC_MESSAGES/ben_test.{mo,po}
	@msgfmt -o locale/zh_CN.UTF-8/LC_MESSAGES/ben_test.{mo,po}
	./translation
	@echo "Result: msgid matched"

	@echo
	@echo "keep mo's charset the same as po's"
	@msgfmt --no-convert -o locale/zh_CN.GB2312/LC_MESSAGES/ben_test.{mo,po}
	@msgfmt --no-convert -o locale/zh_CN.UTF-8/LC_MESSAGES/ben_test.{mo,po}
	./translation
	@echo "Result: msgid not matched"
clean:
	rm -f locale/zh_CN.GB2312/LC_MESSAGES/ben_test.mo
	rm -f locale/zh_CN.UTF-8/LC_MESSAGES/ben_test.mo
	rm -f ./translation



