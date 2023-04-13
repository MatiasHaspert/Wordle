#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //IMPORTADA PARA STRLEN, STRCASECMP Y STRCPY
#include <unistd.h> // LA IMPORTO PARA GETPID
#include "header.h"

int main()
{

    char palabraAdivinar[MAX_CARACTERES]; // VARIABLE DONDE SE GUARDA LA PALABRA A ADIVINAR
    int pos[MAX_PARTIDAS];                // VARIABLE PARA GUARDAR NÚMEROS ALEATORIOS Y ESOS NÚMEROS SON LA POSICION DE LA PALABRA EN EL ARCHIVO
    int cant_partidas;                    // VARIABLE PARA GUARDAR LA CANTIDAD DE PARTIDAS QUE DESEE JUGAR EL USUARIO
    int i, j;
    int cant_intentos;                       // VARIABLE PARA IR CONTANDO LOS INTENTOS
    char palabra_usuario[MAX_CARACTERES];    // VARIABLE PARA GUARDAR LA PALABRA INGRESADA POR EL USUARIO
    char palabra_modificada[MAX_CARACTERES]; // VARIABLE PARA IR MOSTRANDO LA PALABRA CON SUS RESPECTIVOS SIGNOS
    int puntaje;
    int puntajes[MAX_PARTIDAS]; // VARIABLE PARA IR GUARDANDO EL PUNTAJE DE CADA PARTIDA

    presentacion_juego();

    /* LE PIDO AL USUARIO LA CANTIDAD DE PARTIDAS QUE DESEE JUGAR */
    printf("Ingrese la cantidad de partidas que desee jugar (max.8):\n");
    do
    {
        printf("> ");
        scanf("%d", &cant_partidas);

        if (cant_partidas == 0)
        {
            printf("¿Para qué corriste el programa?\n");
            return EXIT_SUCCESS;
        }
        else if (cant_partidas > MAX_PARTIDAS)
        {
            printf("Podes jugar máximo 8 partidas.\n");
            printf("Ingrese la cantidad de partidas:\n");
        }

    } while (cant_partidas > MAX_PARTIDAS);

    /* BUCLE DEL JUEGO */
    for (i = 0; i < cant_partidas; i++)
    {

        numAleatorios(pos);
        getWordInLine("palabras.txt", pos[i], palabraAdivinar);

        printf("\n-----PARTIDA NRO %d DE %d-----\n\n", i + 1, cant_partidas);

        cant_intentos = 0; // DESPUES DE CADA PARTIDA SE VUELVE A INICIALIZAR EN 0
        puntaje = PUNTOS;  /// DESPUES DE CADA PARTIDA SE VUELVE A INICIALIZAR EN 5000

        for (j = 0; j < INTENTOS; j++)
        {

            printf("INTENTO #%d:", j + 1);

            /*USUARIO INGRESA SU PALABRA*/
            do
            {
                scanf("%s", palabra_usuario);

                if ((strlen(palabra_usuario)) != MAX_LETRAS)
                {
                    printf("Ingresa una palabra de 5 letras: ");
                }

            } while ((strlen(palabra_usuario)) != MAX_LETRAS);

            if ((strcasecmp(palabraAdivinar, palabra_usuario)) != 0)
            {
                for (int i = 0; i < MAX_CARACTERES - 1; i++)
                {
                    if (palabra_usuario[i] == palabraAdivinar[i]) // SI LA LETRA ESTÁ EN LA PALABRA Y POSICIÓN CORRECTA.
                    {
                        palabra_modificada[i] = '*';
                        puntaje += 100;
                    }
                    else
                    {
                        palabra_modificada[i] = '-'; // SI NO COMPLETO CON '-'
                    }
                }
                for (int i = 0; i < MAX_CARACTERES - 1; i++)
                {
                    if (palabra_modificada[i] == '-') // AYUDA A QUE NO SUME SIEMPRE UNA LETRA MAL UBICADA MÁS DE UNA VEZ
                    {
                        for (int j = 0; j < MAX_CARACTERES - 1; j++)
                        {
                            if (palabra_usuario[i] == palabraAdivinar[j] && palabra_usuario[i] != palabraAdivinar[i]) // //LA LETRA ESTÁ EN LA PALABRA PERO NO EN EL LUGAR CORRECTO.
                            {
                                palabra_modificada[i] = '+';
                                puntaje += 50;
                            }
                        }
                    }
                }

                printf("\t   %s\n", palabra_modificada);
                puntaje = puntaje - NO_ACIERTA; // SE DESCUENTA 500 AL PUNTAJE POR PASAR DE FILA
                cant_intentos++;                // CONTABILIZO LOS INTENTOS

                if ((usuario_gana(palabra_modificada)) == TRUE)
                {
                    printf("\nACERTASTE!!\n");

                    if (cant_intentos == 1) // SI ADIVINA LA PALABRA A LA PRIMERA
                    {
                        puntaje = PUNTOS + PUNTOS;
                    }
                    else
                    {
                        puntaje += ACIERTA;
                    }

                    puntajes[i] += puntaje;

                    break;
                }

                if (cant_intentos == INTENTOS) // SI EL USUARIO SUPERA LOS INTENTOS
                {
                    printf("\nPERDISTE!!\n");
                    printf("La palabra era %s.\n", palabraAdivinar);
                    puntaje = 0;

                    puntajes[i] += puntaje;
                }
            }
        }

        printf("\nPuntaje de la partida %d.\n", puntaje);

        /*SE LE PREGUNTA AL USUARIO SI DESEA SEGUIR JUGANDO*/
        if ((i + 1) != cant_partidas)
        {
            if (finalizar_partida() == TRUE)
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }

    printf("\n>>>>>ESTADÍSTICAS<<<<<\n");
    promedio(puntajes, cant_partidas);
    estadisticas(puntajes, cant_partidas);

    return EXIT_SUCCESS;
}

void presentacion_juego()
{
    printf("\n>>>BIENVENIDOS A WORDLE<<<\n");
    printf("El juego consiste en adivinar una la palabra de cinco letras.\n");
    printf("Para adivinarla tendrás 6 intentos.\n\n");
    printf("Por cada palabra ingresada se mostrará:\n");
    printf("'*', indica que la letra forma parte de la palabra y está bien ubicada.\n");
    printf("'+', indica que la letra forma parte de la palabra pero no está bien ubicada.\n");
    printf("'-', no forma parte de la palabra misteriosa.\n\n");
    printf("RECUERDA JUGAR CON MAYÚSCULA ACTIVADA.\n");
    printf("QUE COMIENZE EL JUEGO!\n");
    printf("BUENA SUERTE!\n\n");
}

/* FUNCIÓN QUE SI ENCUENTRA TODOS '*' EN LA PALABRA MODIFICADA SIGNIFICA QUE EL USUARIO ADIVINO Y RETORNO 1 */
int usuario_gana(char palabra_modificada[])
{
    int i;
    int contador = 0;

    for (i = 0; i < MAX_CARACTERES - 1; i++)
    {
        if (palabra_modificada[i] == '*')
        {
            contador++;
        }
    }

    if (contador >= 5)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*FUNCIÓN PARA CONSULTARLE AL USUARIO SI DESEA SEGUIR JUGANDO*/
int finalizar_partida()
{
    char respuesta;

    printf("\nDesea finalizar la partida?\n");
    printf("Ingrese 'N' para continuar.\n");
    printf("Si desea finalizar la partida ingrese 'S'.\n");
    printf("> ");

    do
    {

        scanf("%c", &respuesta);

    } while (respuesta != 'S' && respuesta != 'N');

    if (respuesta == 'N')
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void estadisticas(int puntajes[], int cant_partidas)
{
    int i;
    int mayorPuntaje = puntajes[0]; // inicializo en la primera posicion el valor mayor
    int menorPuntaje = puntajes[0]; // inicializo en la primera posicion el valor manor
    int juego = 1;
    int juego2 = 1;

    for (i = 1; i < cant_partidas; i++)
    {
        if (puntajes[i] > mayorPuntaje)
        {
            mayorPuntaje = puntajes[i];

            juego2 = i + 1;
        }
        if (puntajes[i] < menorPuntaje)
        {
            menorPuntaje = puntajes[i];
            juego = i + 1;
        }
    }

    printf("El puntaje mayor fue %d en la partida %d.\n", mayorPuntaje, juego2);
    printf("El puntaje menor fue %d en la partida %d.\n", menorPuntaje, juego);
}

void promedio(int puntajes[], int cant_partidas)
{
    float suma = 0.0;

    for (int i = 0; i < cant_partidas; i++)
    {
        suma += puntajes[i];
    }

    printf("El promedio de las partidas(%d) es %f.\n", cant_partidas, suma / cant_partidas);
}

/* FUNCION QUE GENERA NUMEROS ALEATORIOS NO REPETIDOS PARA LA POSICION DE LAS PALABRAS EN ARCHIVO */
void numAleatorios(int A[])
{
    int i, j;
    srand(getpid()); // GENERA UN NUMERO EN LA SEMILLA

    for (i = 0; i < MAX_PARTIDAS; i++)
    {
        A[i] = 1 + rand() % 30; // GENERA NUMERO ENTRE 1 Y 30

        for (j = i; j >= 0; j--)
        {
            while ((A[i] == A[j]) && (i != j))
            {
                A[i] = 1 + rand() % 30; // GENERA OTRO NUMERO SIN REPETIRSE
            }
        }
    }
}

/*FUNCIÓN PROVISTA POR EL PROFESOR PARA RECUPERAR UNA PALABRA DE UN ARCHIVO*/
void getWordInLine(char *fileName, int lineNumber, char *p)
{
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(fileName, "r"); // FOPEN ABRE Y LEE EL ARCHIVO
    if (fp == NULL)            // SI EL ARCHIVO NO EXISTE TIRA ERROR
        exit(EXIT_FAILURE);

    int i = 1;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        if (i == lineNumber)
        {
            strcpy(p, line); // STRCPY COPIA LA PALABRA EN LA VARIABLE P MEDIANTE LA LÍNEA QUE BUSCO EN EL ARCHIVO
            return;
        }
        i++;
    }
}
