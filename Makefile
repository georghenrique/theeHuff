Huff:
	@echo
	@echo ======================================================
	@echo 				ARVORE DE COMPRESSÃO
	@echo ======================================================
	@echo Compilando o programa huff
	gcc --std=c99 -DLOG_USE_COLOR Huff.c -o huff
	@echo Tornando o arquivo executável
	chmod +x huff
	@echo Executando o programa huff
	@echo ======================================================
	@echo
	./huff	