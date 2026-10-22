#include <iostream>
#include <string>
using namespace std;

// --- Excepcion para las alertas ---
class AlertaAguaException {
public:
    string mensaje;
    AlertaAguaException(string msj) {
        mensaje = msj;
    }
};

// --- INTERFACES (Del diagrama UML) ---
class Lectura {
public:
    virtual float leerDatos() = 0;
};

class Alerta {
public:
    virtual void generarAlerta() = 0;
};

class Reportable {
public:
    virtual void generarReporte() = 0;
};

// --- CLASE GLOBAL ---
class SensorGlobal : public Lectura {
protected:
    // todos los datos que los sensores comparten
    int id;
    string ubicacion;
public:
    // se guardan datos en el momento que se crea el sensor 
    SensorGlobal(int idSensor, string ubiSensor) {
        id = idSensor;
        ubicacion = ubiSensor;
    }

    // imprime la informacion general de los sensores 
    void mostrarDatosGenerales() {
        cout << "ID: " << id << " | Ubicacion: " << ubicacion << endl;
    }

    // Hereda de Lectura, pero se deja abstracta para que los hijos decidan
    virtual float leerDatos() = 0;
};

// --- CLASES HIJAS ---
// El hijo se conecta con la clase global y usa las interfaces faltantes
class SensorPH : public SensorGlobal, public Alerta, public Reportable {
private:
    float nivelPH; // Encapsulacion
public:
    SensorPH(int idSensor, string ubiSensor) : SensorGlobal(idSensor, ubiSensor) {
        nivelPH = 9.5; // Dato falso alto para provocar la alerta
    }

    // El hijo programa su propia forma de leer datos
    float leerDatos() override {
        cout << "Calculando el nivel de acidez (PH) en el agua..." << endl;
        return nivelPH;
    }

    void generarAlerta() override {
        if (nivelPH > 8.5) {
            throw AlertaAguaException("ALERTA: Nivel de PH muy alto.");
        }
    }

    void generarReporte() override {
        cout << "Reporte: El PH actual es " << nivelPH << endl;
    }
};

class SensorTemperatura : public SensorGlobal, public Alerta, public Reportable {
private:
    float grados;
public:
    SensorTemperatura(int idSensor, string ubiSensor) : SensorGlobal(idSensor, ubiSensor) {
        grados = 24.0; // Dato normal
    }

    float leerDatos() override {
        cout << "Midiendo los grados centigrados del agua..." << endl;
        return grados;
    }

    void generarAlerta() override {
        if (grados > 35.0) {
            throw AlertaAguaException("ALERTA: Temperatura demasiado alta.");
        }
    }

    void generarReporte() override {
        cout << "Reporte: La temperatura es de " << grados << " grados." << endl;
    }
};

// --- ESTACION DE MONITOREO ---
class EstacionMonitoreo {
private:
    string nombreEstacion;
    SensorGlobal* listaSensores[10]; // Arreglo normal para guardar sensores
    int cantidadSensores;
public:
    EstacionMonitoreo(string nombre) {
        nombreEstacion = nombre;
        cantidadSensores = 0;
    }

    // Guardamos los sensores en el arreglo
    void agregarSensor(SensorGlobal* nuevoSensor) {
        if (cantidadSensores < 10) {
            listaSensores[cantidadSensores] = nuevoSensor;
            cantidadSensores++;
        }
    }

    // Polimorfismo: recorremos todos los sensores juntos
    void realizarLecturas() {
        cout << "\n--- Iniciando Monitoreo en: " << nombreEstacion << " ---" << endl;
        for (int i = 0; i < cantidadSensores; i++) {
            listaSensores[i]->mostrarDatosGenerales();
            listaSensores[i]->leerDatos();

            // Casteo basico para poder usar las interfaces de Alerta y Reporte
            Alerta* sensorAlerta = dynamic_cast<Alerta*>(listaSensores[i]);
            Reportable* sensorReporte = dynamic_cast<Reportable*>(listaSensores[i]);

            if (sensorReporte != nullptr) {
                sensorReporte->generarReporte();
            }

            // Usamos try-catch para atrapar las alertas de agua contaminada
            if (sensorAlerta != nullptr) {
                try {
                    sensorAlerta->generarAlerta();
                    cout << "[OK] Parametros normales." << endl;
                }
                catch (AlertaAguaException e) {
                    cout << ">> " << e.mensaje << " <<" << endl;
                }
            }
            cout << "----------------------------------" << endl;
        }
    }
};

int main() {
    // 1. Creamos la estacion
    EstacionMonitoreo miEstacion("Central ODS 6");

    // 2. Creamos los sensores
    SensorPH miSensorDeAcidez(1, "Rio Yaqui - Norte");
    SensorTemperatura miSensorDeCalor(2, "Lago de Chapala");

    // 3. Los agregamos a la estacion
    miEstacion.agregarSensor(&miSensorDeAcidez);
    miEstacion.agregarSensor(&miSensorDeCalor);

    // 4. Ejecutamos el sistema con polimorfismo
    miEstacion.realizarLecturas();

    return 0;
}