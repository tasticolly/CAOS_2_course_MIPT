all: target 

SHELL := /bin/bash

target:
	sum=0; \
	result=; \
	echo '#include <stdio.h>' > "solution.c"; \
	echo 'int main() {' >> "solution.c"; \
	while read -r line; do \
		if [[ -z $$line ]] || [[ $$line =~ [#].* ]]; then \
			continue; \
		fi; \
		if [[ $$line =~ .*=[0-9]+ ]]; then \
			tmp=$$( echo $$line | sed -r "s/.*=([0-9]+)/\1/"); \
			sum=$$( echo $$sum + $$tmp | bc -l); \
		elif [[ $$line =~ .*=.* ]]; then \
			tmp=$$( echo $$line | sed -r "s/.*=(.*)/\1/"); \
			result+=$$tmp\\n; \
		fi; \
	done < "compile.conf"; \
	echo "printf(\"%s\",\"$$result\");" >> "solution.c"; \
	echo "printf(\"%d\",$$sum);}" >> "solution.c"; \
	gcc solution.c -o program.exe

clean:
	rm -rf program.exe