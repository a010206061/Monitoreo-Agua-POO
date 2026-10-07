#include <iostream>
#include <string>

using namespace std;

// clase global 
class SensorGlobal {
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

    // El "= 0" hace que tu clase sea abstracta y asi cada sensor decide como calcular y leer sus propios datos 
   
    virtual void leerDatos() = 0;
};

//clases hijas 

// Primer hijo: Sensor de PH
class SensorPH : public SensorGlobal {
public:
    // El hijo se conecta con la clase global 
    SensorPH(int idSensor, string ubiSensor) : SensorGlobal(idSensor, ubiSensor) {}

    // El hijo programa su propia forma de leer datos
    void leerDatos() override {
        cout << "Calculando el nivel de acidez (PH) en el agua..." << endl;
    }
};

// Segundo hijo: Sensor de Temperatura
class SensorTemperatura : public SensorGlobal {
public:
    // El hijo se conecta con la clase global
    SensorTemperatura(int idSensor, string ubiSensor) : SensorGlobal(idSensor, ubiSensor) {}

    // Este hijo tiene una forma distinta de leer datos
    void leerDatos() override {
        cout << "Midiendo los grados centigrados del agua..." << endl;
    }
};


int main() {
   
    // solo creamos los sensores porque la clase gobal es abstracta 
    SensorPH miSensorDeAcidez(1, "Rio Yaqui - Norte");
    SensorTemperatura miSensorDeCalor(2, "Lago de Chapala");

    // Usas la función heredada de la clase global
    miSensorDeAcidez.mostrarDatosGenerales();
    // Usas la función específica del hijo
    miSensorDeAcidez.leerDatos();

    miSensorDeCalor.mostrarDatosGenerales();
    miSensorDeCalor.leerDatos();

    return 0;
}