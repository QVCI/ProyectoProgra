#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>

void LimpiarPantalla();
void DireccionArchivos();

int main()
{
    int opc;
    bool DirMemoriaLista = false;
    do
    {
        LimpiarPantalla();
        
        printf("Menu\n");
        if (opc == -1)
        {
            printf("Seleccione una opcion valida\n");
        }
        printf("0.- Modificar direccion de los archivos\n");
        printf("1.- Leer Archivo (Infijo) y guardar (Postfijo)\n");
        printf("2.- Evaluar archivo (Postfijo)\n");
        printf("3.- Salir\n");
        scanf("%d", &opc);
        switch(opc){
            case 0:
            break;
            case 1:
            break;
            case 2:
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