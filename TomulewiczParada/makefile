all:
	gcc logika.c odpowiedz.c mapa.c main.c bot.c -o projekt -lcurl cJSON/cjson.o

bot: all
	./projekt qwerty_25

pamiec: all
	valgrind ./projekt qwerty_25 bot

czyszczenie:
	rm -f projekt 
	rm -f *.o
	rm -f reply.cjson