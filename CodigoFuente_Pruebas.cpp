#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <unordered_map>
#include <set>
#include <cstring>

using namespace std;

struct Persona {
    char dni[9];
    char telefono[10];
    char email[50];
    char estadoCivil[20];
    char nombre[50];
    char apellido[50];
    char nacionalidad[50];
    char lugarNacimiento[50];
    char departamento[50];
    char provincia[50];
    char ciudad[50];
    char distrito[50];
};


struct Direccion {
    string departamento;
    string provincia;
    string ciudad;
    string distrito;
};



// FUNCIONES
// Generador de números de DNI
string generarDNI() {
    string dni;
    for (int i = 0; i < 8; ++i) {
        dni += to_string(rand() % 10);
    }
    return dni;
}



// Generador de números de Telefono
string generarTelefono() {
    // El primer dígito siempre es '9'
    string teléfono = "9"; 
    for (int i = 0; i < 8; ++i) {
	// Se añaden los siguientes 8 dígitos
        telefono += to_string(rand() % 10);
    }
    return telefono;
}

// Generador de números de emails
string generarEmail(const string& nombre) {
    vector<string> dominios = {"gmail.com", "yahoo.com", "outlook.com", "icloud.com"};
    string dominio = dominios[rand() % dominios.size()];
    return nombre + "@" + dominio;
}


// Generador de números del Estado Civil
string generarEstadoCivil() {
    vector<std::string> estadosCiviles = {"Soltero(a)", "Casado(a)", "Viudo(a)", "Divorciado(a)", "Separado(a)", "Conviviente"};
    return estadosCiviles[rand() % estadosCiviles.size()];
}

// Función para leer líneas de archivos de texto
vector<string> leerArchivo(const string& nombreArchivo) {
    vector<string> lineas;
    ifstream archivo(nombreArchivo);
    string linea;
    while (getline(archivo, linea)) {
        lineas.push_back(linea);
    }
    archivo.close();
    return lineas;
}



// Funcion que permite selccionar un registro de manera aleatoria

Direccion seleccionarDireccionAleatoriamente(const vector<string>& direcciones) {
    string direccion = direcciones[rand() % direcciones.size()];
    Direccion dir;
    size_t pos = 0;
    pos = direccion.find(',');
    dir.departamento = direccion.substr(0, pos);
    direccion.erase(0, pos + 1);

    pos = direccion.find(',');
    dir.provincia = direccion.substr(0, pos);
    direccion.erase(0, pos + 1);

    pos = direccion.find(',');
    dir.ciudad = direccion.substr(0, pos);
    direccion.erase(0, pos + 1);

    dir.distrito = direccion;

    return dir;
}

pair<string, string> seleccionarNacionalidadLugarAleatoriamente(const vector<string>& nacionalidadesLugares) {
    string nl = nacionalidadesLugares[rand() % nacionalidadesLugares.size()];
    size_t pos = nl.find(',');
    string nacionalidad = nl.substr(0, pos);
    string lugarNacimiento = nl.substr(pos + 1);
    return {nacionalidad, lugarNacimiento};
}


// Estructura de Cuckoo Hash
class CuckooHash {
    unordered_map<string, int> table;

public:
    void insert(const string& key, int position) {
        table[key] = position;
    }

    int search(const string& key) {
        if (table.find(key) != table.end()) {
            return table[key];
        }
        return -1; // No encontrado
    }

    void save(const string& filename) {
        ofstream out(filename, ios::binary);
        int size = table.size();
        out.write(reinterpret_cast<const char*>(&size), sizeof(size));
        for (const auto& [key, value] : table) {
            int keySize = key.size();
            out.write(reinterpret_cast<const char*>(&keySize), sizeof(keySize));
            out.write(key.data(), keySize);
            out.write(reinterpret_cast<const char*>(&value), sizeof(value));
        }
        out.close();
    }

    void load(const string& filename) {
        ifstream in(filename, ios::binary);
        int size;
        in.read(reinterpret_cast<char*>(&size), sizeof(size));
        for (int i = 0; i < size; ++i) {
            int keySize;
            in.read(reinterpret_cast<char*>(&keySize), sizeof(keySize));
            string key(keySize, '\0');
            in.read(&key[0], keySize);
            int value;
            in.read(reinterpret_cast<char*>(&value), sizeof(value));
            table[key] = value;
        }
        in.close();
    }
};




// Función para generar una persona aleatoria
Persona generarPersona(const vector<string>& nombres, const vector<string>& apellidos,
                      const vector<string>& nacionalidadesLugares, const vector<string>& direcciones) {
    Persona persona;
    string dni = generarDNI();
    string telefono = generarTelefono();
    string email = generarEmail(nombres[rand() % nombres.size()]);
    string estadoCivil = generarEstadoCivil();
    string nombre = nombres[rand() % nombres.size()];
    string apellido = apellidos[rand() % apellidos.size()];

    auto nl = seleccionarNacionalidadLugarAleatoriamente(nacionalidadesLugares);
    string nacionalidad = nl.first;
    string lugarNacimiento = nl.second;

    Direccion dir = seleccionarDireccionAleatoriamente(direcciones);
    string departamento = dir.departamento;
    string provincia = dir.provincia;
    string ciudad = dir.ciudad;
    string distrito = dir.distrito;

    strcpy(persona.dni, dni.c_str());
    strcpy(persona.telefono, telefono.c_str());
    strcpy(persona.email, email.c_str());
    strcpy(persona.estadoCivil, estadoCivil.c_str());
    strcpy(persona.nombre, nombre.c_str());
    strcpy(persona.apellido, apellido.c_str());
    strcpy(persona.nacionalidad, nacionalidad.c_str());
    strcpy(persona.lugarNacimiento, lugarNacimiento.c_str());
    strcpy(persona.departamento, departamento.c_str());
    strcpy(persona.provincia, provincia.c_str());
    strcpy(persona.ciudad, ciudad.c_str());
    strcpy(persona.distrito, distrito.c_str());

    return persona;
}

// Función para escribir la persona en un archivo binario
void escribirPersona(ofstream& archivo, const Persona& persona) {
    archivo.write(reinterpret_cast<const char*>(&persona), sizeof(persona));
}

// Función para leer una persona desde una posición específica en un archivo binario
Persona leerPersona(ifstream& archivo, int position) {
    archivo.seekg(position * sizeof(Persona));
    Persona persona;
    archivo.read(reinterpret_cast<char*>(&persona), sizeof(persona));
    return persona;
}



// Implementación de la estructura de Árbol B
class ArbolB {
private:
    static const int MAX_KEYS = 4; // Máximo número de claves en un nodo
    static const int MIN_KEYS = 2; // Mínimo número de claves en un nodo

    struct Nodo {
        int numKeys;
        string keys[MAX_KEYS];
        Nodo* hijos[MAX_KEYS + 1];
        bool esHoja;

        Nodo() : numKeys(0), esHoja(true) {
            for (int i = 0; i < MAX_KEYS + 1; ++i) {
                hijos[i] = nullptr;
            }
        }
    };

    Nodo* raiz;

    void dividirNodo(Nodo* padre, int indiceHijo, Nodo* hijo);
    void insertarNoLleno(Nodo* nodo, const string& clave);
    Nodo* buscarNodo(Nodo* nodo, const string& clave);

public:
    ArbolB() : raiz(nullptr) {}

    void insertar(const string& clave);
    bool buscar(const string& clave);
    void eliminar(const string& clave);

    ~ArbolB() {
        // Liberar la memoria del Árbol B (implementación opcional)
    }
};

void ArbolB::dividirNodo(Nodo* padre, int indiceHijo, Nodo* hijo) {
    Nodo* nuevoNodo = new Nodo();
    nuevoNodo->esHoja = hijo->esHoja;
    nuevoNodo->numKeys = MIN_KEYS;

    for (int j = 0; j < MIN_KEYS; j++) {
        nuevoNodo->keys[j] = hijo->keys[j + MIN_KEYS + 1];
    }

    if (!hijo->esHoja) {
        for (int j = 0; j < MIN_KEYS + 1; j++) {
            nuevoNodo->hijos[j] = hijo->hijos[j + MIN_KEYS + 1];
        }
    }

    hijo->numKeys = MIN_KEYS;

    for (int j = padre->numKeys; j >= indiceHijo + 1; j--) {
        padre->hijos[j + 1] = padre->hijos[j];
    }

    padre->hijos[indiceHijo + 1] = nuevoNodo;

    for (int j = padre->numKeys - 1; j >= indiceHijo; j--) {
        padre->keys[j + 1] = padre->keys[j];
    }

    padre->keys[indiceHijo] = hijo->keys[MIN_KEYS];
    padre->numKeys = padre->numKeys + 1;
}

void ArbolB::insertarNoLleno(Nodo* nodo, const string& clave) {
    int i = nodo->numKeys - 1;

    if (nodo->esHoja) {
        while (i >= 0 && clave < nodo->keys[i]) {
            nodo->keys[i + 1] = nodo->keys[i];
            i--;
        }

        nodo->keys[i + 1] = clave;
        nodo->numKeys = nodo->numKeys + 1;
    } else {
        while (i >= 0 && clave < nodo->keys[i]) {
            i--;
        }

        i++;
        if (nodo->hijos[i]->numKeys == MAX_KEYS) {
            dividirNodo(nodo, i, nodo->hijos[i]);

            if (clave > nodo->keys[i]) {
                i++;
            }
        }
        insertarNoLleno(nodo->hijos[i], clave);
    }
}

Nodo* ArbolB::buscarNodo(Nodo* nodo, const string& clave) {
    int i = 0;
    while (i < nodo->numKeys && clave > nodo->keys[i]) {
        i++;
    }

    if (i < nodo->numKeys && clave == nodo->keys[i]) {
        return nodo;
    }

    if (nodo->esHoja) {
        return nullptr;
    }

    return buscarNodo(nodo->hijos[i], clave);
}

void ArbolB::insertar(const string& clave) {
    if (raiz == nullptr) {
        raiz = new Nodo();
        raiz->keys[0] = clave;
        raiz->numKeys = 1;
    } else {
        if (raiz->numKeys == MAX_KEYS) {
            Nodo* nuevaRaiz = new Nodo();
            nuevaRaiz->esHoja = false;
            nuevaRaiz->hijos[0] = raiz;
            dividirNodo(nuevaRaiz, 0, raiz);
            insertarNoLleno(nuevaRaiz, clave);
            raiz = nuevaRaiz;
        } else {
            insertarNoLleno(raiz, clave);
        }
    }
}

bool ArbolB::buscar(const string& clave) {
    return buscarNodo(raiz, clave) != nullptr;
}

void ArbolB::eliminar(const string& clave) {
    // Implementación de la eliminación en un Árbol B
}



// Implementación de la estructura de Cuckoo Hash
class CuckooHash {
private:
    static const int TABLE_SIZE = 1000000; // Tamaño de la tabla de hash
    static const int NUM_HASH_FUNCTIONS = 2; // Número de funciones de hash

    struct Entry {
        string key;
        int value;
        bool occupied;

        Entry() : key(""), value(0), occupied(false) {}
    };

    Entry table1[TABLE_SIZE];
    Entry table2[TABLE_SIZE];

    int hashFunction1(const string& key);
    int hashFunction2(const string& key);

public:
    CuckooHash();

    void insert(const string& key, int value);
    int search(const string& key);
    void remove(const string& key);
    void save(const string& filename);
    void load(const string& filename);
};

int CuckooHash::hashFunction1(const string& key) {
    hash<string> hasher;
    return hasher(key) % TABLE_SIZE;
}

int CuckooHash::hashFunction2(const string& key) {
    hash<string> hasher;
    return (hasher(key) / TABLE_SIZE) % TABLE_SIZE;
}

CuckooHash::CuckooHash() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        table1[i] = Entry();
        table2[i] = Entry();
    }
}

void CuckooHash::insert(const string& key, int value) {
    int h1 = hashFunction1(key);
    int h2 = hashFunction2(key);

    if (table1[h1].occupied && table1[h1].key == key) {
        table1[h1].value = value;
        return;
    }

    if (table2[h2].occupied && table2[h2].key == key) {
        table2[h2].value = value;
        return;
    }

    if (!table1[h1].occupied) {
        table1[h1].key = key;
        table1[h1].value = value;
        table1[h1].occupied = true;
        return;
    }

    if (!table2[h2].occupied) {
        table2[h2].key = key;
        table2[h2].value = value;
        table2[h2].occupied = true;
        return;
    }

    string oldKey = table1[h1].key;
    int oldValue = table1[h1].value;

    table1[h1].key = key;
    table1[h1].value = value;

    insert(oldKey, oldValue);
}

int CuckooHash::search(const string& key) {
    int h1 = hashFunction1(key);
    int h2 = hashFunction2(key);

    if (table1[h1].occupied && table1[h1].key == key) {
        return table1[h1].value;
    }

    if (table2[h2].occupied && table2[h2].key == key) {
        return table2[h2].value;
    }

    return -1;
}

void CuckooHash::remove(const string& key) {
    int h1 = hashFunction1(key);
    int h2 = hashFunction2(key);

    if (table1[h1].occupied && table1[h1].key == key) {
        table1[h1] = Entry();
        return;
    }

    if (table2[h2].occupied && table2[h2].key == key) {
        table2[h2] = Entry();
        return;
    }
}

void CuckooHash::save(const string& filename) {
    ofstream file(filename, ios::binary);
    file.write(reinterpret_cast<const char*>(table1), sizeof(table1));
    file.write(reinterpret_cast<const char*>(table2), sizeof(table2));
    file.close();
}

void CuckooHash::load(const string& filename) {
    ifstream file(filename, ios::binary);
    file.read(reinterpret_cast<char*>(table1), sizeof(table1));
    file.read(reinterpret_cast<char*>(table2), sizeof(table2));
    file.close();
}




//*****************************************************************

int main() {
    srand(time(0)); // Inicializar la semilla para generar números aleatorios

    // Leer los archivos de texto una vez y almacenarlos en memoria
    vector<string> nombres = leerArchivo("nombres.txt");
    vector<string> apellidos = leerArchivo("apellidos.txt");
    vector<string> nacionalidadesLugares = leerArchivo("nacionalidadLugarNac.txt");
    vector<string> direcciones = leerArchivo("direcciones.txt");

    // Creando el objeto arbolB que pertenece a la clase Arbol B
	ArbolB arbolB;

    // Creando el objeto cuckooHash que pertenece a la clase Cuckoo Hash
    CuckooHash cuckooHash;

    // Cargar el índice si existe
    cuckooHash.load("indice.bin");


    // Abrir el archivo binario para escribir en modo de añadir
    ofstream personasArchivo("personas.bin", ios::binary | ios::app);

    // Generar y escribir varias personas
    for (int i = 0; i < 33000000; ++i) {
        Persona persona = generarPersona(nombres, apellidos, nacionalidadesLugares, direcciones);
        
	// Posición en el archivo binario
        int position = i; 
        escribirPersona(personasArchivo, persona);
        cuckooHash.insert(persona.dni, position);
    }

    // Guardar el índice
    cuckooHash.save("indice.bin");

    // ------->Buscar una persona por DNI
    string dniBuscado = "12345678"; 

    ifstream personasArchivoLectura("personas.bin", ios::binary);
    int position = cuckooHash.search(dniBuscado);
    if (position != -1) {
        Persona personaEncontrada = leerPersona(personasArchivoLectura, position);
        cout << "Persona encontrada: " << endl;
        cout << "DNI: " << personaEncontrada.dni << endl;
        cout << "Nombre: " << personaEncontrada.nombre << endl;
        cout << "Apellido: " << personaEncontrada.apellido << endl;
        cout << "Telefono: " << personaEncontrada.telefono << endl;
        cout << "Email: " << personaEncontrada.email << endl;
        cout << "Estado Civil: " << personaEncontrada.estadoCivil << endl;
        cout << "Nacionalidad: " << personaEncontrada.nacionalidad << endl;
        cout << "Lugar de Nacimiento: " << personaEncontrada.lugarNacimiento << endl;
        cout << "Departamento: " << personaEncontrada.departamento << endl;
        cout << "Provincia: " << personaEncontrada.provincia << endl;
        cout << "Ciudad: " << personaEncontrada.ciudad << endl;
        cout << "Distrito: " << personaEncontrada.distrito << endl;
    } else {
	cout << "----Registro inexistente----"<< endl;
        cout << "Persona con DNI " << dniBuscado << " no encontrada." << endl;
    }

    return 0;
}



