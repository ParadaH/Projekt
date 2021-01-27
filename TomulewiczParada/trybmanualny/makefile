all:
	gcc main_trybmanualny.c ../logika.c ../mapa.c ../odpowiedz.c -o testmanual -lcurl ../cJSON/cjson.o

trybmanualny: all
	./testmanual qwerty_25 eksploruj ruch eksploruj ruch eksploruj ruch eksploruj ruch eksploruj ruch eksploruj ruch eksploruj ruch eksploruj ruch eksploruj ruch eksploruj prawo ruch eksploruj ruch eksploruj ruch eksploruj prawo ruch eksploruj ruch eksploruj ruch eksploruj lewo ruch eksploruj ruch eksploruj lewo ruch eksploruj ruch eksploruj ruch eksploruj prawo ruch eksploruj prawo prawo ruch lewo ruch ruch lewo ruch eksploruj prawo ruch eksploruj ruch eksploruj ruch eksploruj prawo ruch eksploruj lewo ruch eksploruj ruch eksploruj ruch eksploruj ruch eksploruj lewo ruch eksploruj lewo ruch eksploruj lewo ruch prawo ruch ruch ruch ruch ruch lewo ruch ruch lewo ruch eksploruj ruch eksploruj ruch eksploruj ruch eksploruj ruch eksploruj ruch eksploruj prawo ruch eksploruj ruch eksploruj ruch eksploruj prawo

test-prawo: all
	./testmanual qwerty_25 prawo

test-lewo: all
	./testmanual qwerty_25 lewo
	 
test-ruch: all
	./testmanual qwerty_25 ruch
	
test-reset: all
	./testmanual qwerty_25 reset

test-info: all
	./testmanual qwerty_25 prawo

czyszczenie:
	rm -f testmanual 
	rm -f *.o
	rm -f reply.cjson
	
