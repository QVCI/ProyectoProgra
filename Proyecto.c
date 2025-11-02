#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>

void LimpiarPantalla();
void DireccionArchivos(char *Direccion);
void NombreArchivos(char *Direccion, const char *TipoArchivo);


int main()
{
    int opc;
    char Direccion [200];
    char NombreInfijo[40];
    char NombrePostFijo[40];
    bool DirMemoriaLista = false;
    bool ArchvioLeido = false;
    do
    {
        LimpiarPantalla();
        
        printf("Menu\n");
        
        printf("0.- Modificar direccion de los archivos\n");
        printf("1.- Leer Archivo (Infijo) y guardar (Postfijo)\n");
        printf("2.- Evaluar archivo (Postfijo)\n");
        printf("3.- Salir\n");
        if (opc == -1)
        {
            printf("Seleccione una opcion valida\n");
        }
        if (opc == -2)
        {
            printf("Primero Seleccione la direccion de los archivos.txt (Opcion 0)\n");
            
        }
        if (opc == -3)
        {
            printf("Primero se debe leer el archivo y guardar (Opcion 1)\n");
            
        }
        printf("Ingrese una opcion: ");
        scanf("%d", &opc);
        switch(opc){
            case 0:
                DireccionArchivos(Direccion);
                NombreArchivos(NombreInfijo, "Infijo");
                NombreArchivos(NombrePostFijo, "Postfijo");
                opc = -4;
                DirMemoriaLista = true;
            break;
            case 1:
                if(!DirMemoriaLista)
                {
                    opc = -2;
                    break;
                }
                ArchvioLeido = true;

            break;
            case 2:
                if(!DirMemoriaLista)
                {
                    opc = -2;
                    break;
                }
                if(!ArchvioLeido)
                {
                    opc = -3;
                    break;
                }
            break;
            case 3:
            break;
            default:
            opc = -1;
            break;

        }
    }while(opc != 3);

    return 0;
}
void LimpiarPantalla()
{
    system("CLS");
}

void DireccionArchivos(char Direccion[100]) {
    LimpiarPantalla();
    
    char DireccionTemp[100] = "";
    strcpy(Direccion, ""); 

    while (1) {
        printf("Ingrese la direccion de memoria o presione ESC para salir:\n");
        printf("Ruta actual: %s\n", Direccion);

    
        while (1) {
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                while (GetAsyncKeyState(VK_ESCAPE) & 0x8000) Sleep(50);
                return;
            }

            if (_kbhit()) break; 
            Sleep(50);
        }

        if (fgets(DireccionTemp, sizeof(DireccionTemp), stdin) == NULL)
            continue;


        DireccionTemp[strcspn(DireccionTemp, "\n")] = '\0';

        if (strlen(DireccionTemp) == 0)
            continue;

        if (strlen(Direccion) == 0) {
            strcpy(Direccion, DireccionTemp);
        } else {
            strcat(Direccion, "//");
            strcat(Direccion, DireccionTemp);
        }

        LimpiarPantalla();
    }
}

void NombreArchivos(char Direccion[40], const char TipoArchivo[20]) {
    LimpiarPantalla();

    char DireccionTemp[40] = "";

    printf("Ingrese el nombre del archivo (%s) o presione ESC para salir:\n", TipoArchivo);

    while (1) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            while (GetAsyncKeyState(VK_ESCAPE) & 0x8000) Sleep(50);
            Direccion[0] = '\0'; 
            return;
        }

        if (_kbhit()) break; 
        Sleep(50);
    }

  
    if (fgets(DireccionTemp, sizeof(DireccionTemp), stdin) == NULL) {
        Direccion[0] = '\0';
        return;
    }

    DireccionTemp[strcspn(DireccionTemp, "\n")] = '\0';

    if (strlen(DireccionTemp) == 0) {
        Direccion[0] = '\0';
        return;
    }

    strcpy(Direccion, DireccionTemp);

    LimpiarPantalla();
}
