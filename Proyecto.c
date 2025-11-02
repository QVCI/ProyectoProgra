#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

void LimpiarPantalla();
void DireccionArchivos(char *Direccion);
void NombreArchivos(char *Nombre, const char *TipoArchivo);
bool AbrirArchivo(FILE **PtrArchivo, const char *DireccionInfijo, const char *DireccionPostfijo, char *PostFija);
int EvaluarPostfija(const char *exp);


int main() {
    FILE *PtrArchivo;
    int opc = 0;

    char Direccion[200] = "";
    char PostFija[200] = "";
    char NombreInfijo[40] = "";
    char NombrePostFijo[40] = "";
    bool DirMemoriaLista = false;
    bool ArchivoLeido = false;

    do {
        LimpiarPantalla();
        printf("Menu\n");
        printf("0.- Modificar direccion de los archivos\n");
        printf("1.- Leer Archivo (Infijo) y guardar (Postfijo)\n");
        printf("2.- Evaluar archivo (Postfijo)\n");
        printf("3.- Salir\n");

        printf("Ingrese una opcion: ");
        scanf("%d", &opc);
        getchar();

        switch (opc) {
            case 0:
                DireccionArchivos(Direccion);
                NombreArchivos(NombreInfijo, "Infijo");
                NombreArchivos(NombrePostFijo, "Postfijo");
                DirMemoriaLista = true;
                break;

            case 1:
                if (!DirMemoriaLista) {
                    printf("\nPrimero debe seleccionar la direccion (opcion 0)");
                    system("pause");
                    break;
                }

                {
                    char RutaInfijo[250], RutaPostfijo[250];
                    sprintf(RutaInfijo, "%s\\%s", Direccion, NombreInfijo);
                    sprintf(RutaPostfijo, "%s\\%s", Direccion, NombrePostFijo);
                    ArchivoLeido = AbrirArchivo(&PtrArchivo, RutaInfijo, RutaPostfijo, PostFija);
                }
                system("pause");
                break;

            case 2:
                if (!ArchivoLeido) {
                    printf("\nPrimero debe leer el archivo infijo y generar postfijo (opcion 1)");
                    system("pause");
                    break;
                }
                printf("\nResultado: : %d\n", EvaluarPostfija(PostFija));
                system("pause");
                break;

            case 3:
                printf("\nSaliendo...\n");
                break;

            default:
                printf("Opcion no valida\n");
                system("pause");
        }
    } while (opc != 3);

    return 0;
}


void LimpiarPantalla() {
    system("cls");
}

void DireccionArchivos(char Direccion[100]) {
    LimpiarPantalla();
    printf("Ingrese la direccion de memoria (sin nombre de archivo):\n");
    fgets(Direccion, 100, stdin);
    Direccion[strcspn(Direccion, "\n")] = '\0';
}

void NombreArchivos(char Direccion[40], const char TipoArchivo[20]) {
    LimpiarPantalla();
    printf("Ingrese el nombre del archivo (%s): ", TipoArchivo);
    fgets(Direccion, 40, stdin);
    Direccion[strcspn(Direccion, "\n")] = '\0';
}


struct NodoCaracter {
    char simbolo;
    struct NodoCaracter *siguiente;
};

struct PilaCaracteres {
    struct NodoCaracter *tope;
};

void inicializar(struct PilaCaracteres *pila) {
    pila->tope = NULL;
}

void MeterDato(struct PilaCaracteres *pila, char simbolo) {
    struct NodoCaracter *nuevo = malloc(sizeof(struct NodoCaracter));
    nuevo->simbolo = simbolo;
    nuevo->siguiente = pila->tope;
    pila->tope = nuevo;
}

int SacarDato(struct PilaCaracteres *pila, char *salida) {
    if (pila->tope == NULL) return 0;
    struct NodoCaracter *temp = pila->tope;
    *salida = temp->simbolo;
    pila->tope = temp->siguiente;
    free(temp);
    return 1;
}

char VerTope(struct PilaCaracteres *pila) {
    if (pila->tope == NULL) return '\0';
    return pila->tope->simbolo;
}

bool EstaVacia(struct PilaCaracteres *pila) {
    return pila->tope == NULL;
}

int precedencia(char op) {
    switch (op) {
        case '^': return 3;
        case '*': case '/': return 2;
        case '+': case '-': return 1;
        default: return 0;
    }
}


bool AbrirArchivo(FILE **PtrArchivo, const char DireccionInfijo[140], const char DireccionPostfijo[140], char *PostFija) {
    struct PilaCaracteres pila;
    char caracter;
    int pos = 0;
    inicializar(&pila);

    // Abrir archivo infijo
    *PtrArchivo = fopen(DireccionInfijo, "r");
    if (*PtrArchivo == NULL) {
        printf("Error: no se pudo abrir el archivo.\n");
        return false;
    }

    printf("Expresion leida: ");
    while ((caracter = fgetc(*PtrArchivo)) != EOF)
        printf("%c", caracter);
    rewind(*PtrArchivo);
    printf("\n");

    // Apilamos el paréntesis inicial
    MeterDato(&pila, '(');

    // Agregamos ')' al final para cerrar la expresión
    char buffer[200];
    int i = 0;
    while ((caracter = fgetc(*PtrArchivo)) != EOF)
        buffer[i++] = caracter;
    buffer[i++] = ')';
    buffer[i] = '\0';
    fclose(*PtrArchivo);

    // Recorremos el buffer en lugar del tmpfile
    for (int j = 0; buffer[j] != '\0'; j++) {
        caracter = buffer[j];
        if (isspace(caracter)) continue;

        if (isdigit(caracter) || isalpha(caracter)) {
            PostFija[pos++] = caracter;
        }
        else if (caracter == '(') {
            MeterDato(&pila, '(');
        }
        else if (caracter == '+' || caracter == '-' || caracter == '*' ||
                 caracter == '/' || caracter == '^') {
            char tope;
            while (!EstaVacia(&pila) && precedencia(VerTope(&pila)) >= precedencia(caracter)) {
                SacarDato(&pila, &tope);
                if (tope != '(')
                    PostFija[pos++] = tope;
            }
            MeterDato(&pila, caracter);
        }
        else if (caracter == ')') {
            char tope;
            while (SacarDato(&pila, &tope) && tope != '(')
                PostFija[pos++] = tope;
        }
    }

    // Vaciar la pila
    char tope;
    while (SacarDato(&pila, &tope)) {
        if (tope != '(')
            PostFija[pos++] = tope;
    }

    PostFija[pos] = '\0';

    // Guardar en archivo postfijo
    FILE *salida = fopen(DireccionPostfijo, "w");
    if (salida) {
        fprintf(salida, "%s", PostFija);
        fclose(salida);
    }

    printf("\nExpresion postfija: %s\n", PostFija);
    return true;
}


int EvaluarPostfija(const char *exp) {
    int pila[100];
    int tope = -1;
    for (int i = 0; exp[i] != '\0'; i++) {
        if (isdigit(exp[i])) {
            pila[++tope] = exp[i] - '0';
        } else {
            int b = pila[tope--];
            int a = pila[tope--];
            switch (exp[i]) {
                case '+': pila[++tope] = a + b; break;
                case '-': pila[++tope] = a - b; break;
                case '*': pila[++tope] = a * b; break;
                case '/': pila[++tope] = a / b; break;
                case '^': {
                    int res = 1;
                    for (int j = 0; j < b; j++) res *= a;
                    pila[++tope] = res;
                    break;
                }
            }
        }
    }
    return pila[tope];
}
