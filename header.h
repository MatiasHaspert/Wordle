/* DECLARACÍON DE CONSTANTES */
#define INTENTOS 6
#define MAX_CARACTERES 6    // CINCO LETRAS MÁS EL CARACTER NULL
#define MAX_LETRAS  5
#define PUNTOS 5000
#define ACIERTA 2000
#define NO_ACIERTA 500
#define MAX_PARTIDAS 8
#define TRUE 1
#define FALSE 0

/* PROTOTIPOS DE FUNCIONES */
void presentacion_juego();
int usuario_gana (char []);
int finalizar_partida();
void estadisticas (int [],int);
void promedio (int [],int );
void numAleatorios (int []);
void getWordInLine (char *, int, char *);