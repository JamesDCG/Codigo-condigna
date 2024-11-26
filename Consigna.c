#include <stdio.h>

#define ZONAS 5
#define CONTAMINANTES 4 // CO2, SO2, NO2, PM2.5

// Prototipos de las funciones
void ingresarDatos(float niveles_actuales[ZONAS][CONTAMINANTES]);
void calcularPromedios(float historico[ZONAS][CONTAMINANTES][30], float promedios[ZONAS][CONTAMINANTES]);
void predecirContaminacion(float historico[ZONAS][CONTAMINANTES][30], float predicciones[ZONAS][CONTAMINANTES]);
void emitirAlertas(float predicciones[ZONAS][CONTAMINANTES], float limites[CONTAMINANTES]);

int main() {
    float niveles_actuales[ZONAS][CONTAMINANTES];
    float limites[CONTAMINANTES] = {50.0, 40.0, 30.0, 25.0}; // Limites aceptables
    float historico[ZONAS][CONTAMINANTES][30]; // Datos historicos predefinidos
    float predicciones[ZONAS][CONTAMINANTES];

    // Datos históricos ajustados para evitar predicciones excesivas
    for (int i = 0; i < ZONAS; i++) {
        for (int j = 0; j < CONTAMINANTES; j++) {
            for (int k = 0; k < 30; k++) {
                if (j == 3) {
                    historico[i][j][k] = 15 + (i + j + k) % 10; // PM2.5: valores entre 15 y 25
                } else {
                    historico[i][j][k] = 20 + (i + j + k) % 30; // Otros contaminantes: valores entre 20 y 50
                }
            }
        }
    }

    printf("--Limites de los contaminantes--\nCO2: 50.0\nSO2: 40.0\nNO2: 30.0\nPM2.5: 25\n");

    // Ingresar datos actuales
    ingresarDatos(niveles_actuales);

    // Calcular promedios historicos
    float promedios[ZONAS][CONTAMINANTES];
    calcularPromedios(historico, promedios);

    // Predecir contaminacion futura
    predecirContaminacion(historico, predicciones);

    // Emitir alertas solo si la predicción supera el límite
    emitirAlertas(predicciones, limites);

    return 0;
}

void ingresarDatos(float niveles_actuales[ZONAS][CONTAMINANTES]) {
    for (int i = 0; i < ZONAS; i++) {
        printf("--Zona %d--\n", i + 1);
        for (int j = 0; j < CONTAMINANTES; j++) {
            printf("Ingrese el nivel actual del contaminante %d: ", j + 1);
            scanf("%f", &niveles_actuales[i][j]);
        }
    }
}

void calcularPromedios(float historico[ZONAS][CONTAMINANTES][30], float promedios[ZONAS][CONTAMINANTES]) {
    for (int i = 0; i < ZONAS; i++) {
        for (int j = 0; j < CONTAMINANTES; j++) {
            float suma = 0;
            for (int k = 0; k < 30; k++) {
                suma += historico[i][j][k];
            }
            promedios[i][j] = suma / 30.0;
        }
    }
}

void predecirContaminacion(float historico[ZONAS][CONTAMINANTES][30], float predicciones[ZONAS][CONTAMINANTES]) {
    for (int i = 0; i < ZONAS; i++) {
        for (int j = 0; j < CONTAMINANTES; j++) {
            float suma_recientes = 0, suma_restantes = 0;
            for (int k = 0; k < 10; k++) suma_recientes += historico[i][j][29 - k];
            for (int k = 10; k < 30; k++) suma_restantes += historico[i][j][29 - k];
            predicciones[i][j] = (0.6 * suma_recientes / 10) + (0.4 * suma_restantes / 20); // Ajuste de ponderación
        }
    }
}

void emitirAlertas(float predicciones[ZONAS][CONTAMINANTES], float limites[CONTAMINANTES]) {
    int alertaImpreso = 0; // Variable para verificar si al menos una alerta fue impresa
    printf("\n--- alertas preventivas ---\n");
    for (int i = 0; i < ZONAS; i++) {
        for (int j = 0; j < CONTAMINANTES; j++) {
            if (predicciones[i][j] > limites[j]) {
                printf("ALERTA: zona %d, contaminante %d supera el limite. Prediccion: %.2f, Limite: %.2f\n",
                       i + 1, j + 1, predicciones[i][j], limites[j]);
                alertaImpreso = 1; // Marca que se imprimió al menos una alerta
            }
        }
    }
    if (!alertaImpreso) {
        printf("No se han superado los limites para los contaminantes.\n");
    }
}
