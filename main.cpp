// Servidor.cpp
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime>
#include <vector>
#include <queue>
#include <algorithm>
#include <gtk/gtk.h>
#include <sstream>
#include <functional>

using namespace std;

GtkWidget *window;
GtkTextView *myTextView;

const int matrixSize = 10;

struct Node {
    int x, y;
    int f, g, h;
    Node* parent;

    Node(int x, int y, int g, int h, Node* parent)
            : x(x), y(y), g(g), h(h), parent(parent) {
        f = g + h;
    }
};

// Funcion que calcula la distancia de manhattan para dos nodos
int calculateManhattanDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Funcion que encuentra el camino del samurai a la meta
vector<pair<int, int>> findPath(int matrix[matrixSize][matrixSize], int startX, int startY, int targetX, int targetY) {
    vector<pair<int, int>> path;
    priority_queue<Node*, vector<Node*>, function<bool(Node*, Node*)>> openSet(
            [](Node* a, Node* b) { return a->f > b->f; });

    Node* startNode = new Node(startX, startY, 0, calculateManhattanDistance(startX, startY, targetX, targetY), nullptr);
    openSet.push(startNode);

    while (!openSet.empty()) {
        Node* currentNode = openSet.top();
        openSet.pop();

        if (currentNode->x == targetX && currentNode->y == targetY) {
            // Reconstruye el camino
            while (currentNode != nullptr) {
                path.emplace_back(currentNode->x, currentNode->y);
                currentNode = currentNode->parent;
            }
            reverse(path.begin(), path.end());
            break;
        }

        // Genera nodos vecinos
        const int dx[4] = {1, -1, 0, 0};
        const int dy[4] = {0, 0, 1, -1};
        for (int i = 0; i < 4; ++i) {
            int nextX = currentNode->x + dx[i];
            int nextY = currentNode->y + dy[i];

            if (nextX >= 0 && nextX < matrixSize && nextY >= 0 && nextY < matrixSize && matrix[nextX][nextY] == 0) {
                int g = currentNode->g + 1;
                int h = calculateManhattanDistance(nextX, nextY, targetX, targetY);
                Node* neighbor = new Node(nextX, nextY, g, h, currentNode);
                openSet.push(neighbor);
            }
        }
    }

    // Limpia la memoria
    while (!openSet.empty()) {
        delete openSet.top();
        openSet.pop();
    }

    return path;
}


struct Samurai {
    int uniqueId;
    int age;
    double survivalProbability;
    int expectedGenerations;
    int emotionalIntelligence;
    int physicalCondition;
    int upperBodyStrength;
    int lowerBodyStrength;
    double resistance;

    Samurai(int generation) {
        // Generar un identificador único aleatorio de 7 dígitos en cada generación
        uniqueId = rand() % 10000000 + 1000000;

        if (generation == 1) {
            // Inicializar los atributos en la primera generación
            age = 18;
            survivalProbability = 20.0;
            expectedGenerations = 4;
            emotionalIntelligence = 4;
            physicalCondition = 4;
            upperBodyStrength = 4;
            lowerBodyStrength = 4;
            resistance = calculateResistance(); // Calcular la resistencia inicial
        }
        else {
            evolve(); // Evolucionar los atributos en generaciones posteriores
        }
    }

    double calculateResistance() const {
        return age + emotionalIntelligence + physicalCondition + upperBodyStrength + lowerBodyStrength;
    }

    // Función para aplicar el incremento del 50% en cada atributo en cada generación
    void evolve() {
        age = static_cast<int>(age * 1.2);
        survivalProbability *= 1.5;
        expectedGenerations = static_cast<int>(expectedGenerations - 1);
        emotionalIntelligence = static_cast<int>(emotionalIntelligence * 1.5);
        physicalCondition = static_cast<int>(physicalCondition * 1.5);
        upperBodyStrength = static_cast<int>(upperBodyStrength * 1.5);
        lowerBodyStrength = static_cast<int>(lowerBodyStrength * 1.5);
        resistance = calculateResistance();
    }

    void printAttributes() const {
        cout << "Samurái " << uniqueId << " - Atributos:\n";
        cout << "Edad: " << age << "\n";
        cout << "Probabilidad de supervivencia: " << survivalProbability << "%\n";
        cout << "Generaciones esperadas de supervivencia: " << expectedGenerations << "\n";
        cout << "Inteligencia emocional: " << emotionalIntelligence << "\n";
        cout << "Condición física: " << physicalCondition << "\n";
        cout << "Fuerza en tronco superior: " << upperBodyStrength << "\n";
        cout << "Fuerza en tronco inferior: " << lowerBodyStrength << "\n";
        cout << "Resistencia: " << resistance << "\n";
    }

};

int main(int argc, char *argv[]) {
    // Crear un socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        cerr << "Error al crear el socket del servidor" << std::endl;
        return 1;
    }

    // Configurar la dirección del servidor
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080); // Puerto de escucha
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Vincular el socket a la dirección
    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        cerr << "Error al vincular el socket a la dirección" << endl;
        close(serverSocket);
        return 1;
    }

    // Escuchar conexiones entrantes
    if (listen(serverSocket, 5) == -1) {
        cerr << "Error al escuchar conexiones entrantes" << endl;
        close(serverSocket);
        return 1;
    }

    cout << "Servidor esperando conexiones entrantes..." << endl;

    int matrix[10][10]; // Declara la matrix 10x10

    srand(time(0));

    // Inicia la matriz con ceros
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            matrix[i][j] = 0;
        }
    }

    // Print de la matriz
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            cout << matrix[i][j] << " ";
        }
        //cout << endl;
    }

    // Defina el punto de inicio y de llegada de los samurai
    int startX = 0;
    int startY = 0;
    int targetX = 9;
    int targetY = 9;

    vector<pair<int, int>> path = findPath(matrix, startX, startY, targetX, targetY);

    // Enseña el camino del samurai
    cout << "Camino del samurai: ";
    for (const auto &point: path) {
        cout << "(" << point.first << ", " << point.second << ") ";
    }
    cout << endl;

    srand(static_cast<unsigned>(std::time(0)));

    const int numGenerations = 5;

    Samurai bestSamurai(1);  // El primer samurái es el mejor en la primera generación

    GtkBuilder *builder;
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "/home/aleprominecraft/Documents/github/Server-ProyectoII/samuraiStats.glade",
                              NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "MyWindow"));
    myTextView = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "MyTextView"));

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(myTextView);

    stringstream generationAtributes;  // Crea un stringstream que guarda la informacion a mostrar

    for (int generation = 1; generation <= numGenerations; generation++) {
        bestSamurai = Samurai(generation); // Crea un nuevo samurai con un ID unico

        // Crea un string que guarda los atributos de cada generacion
        stringstream attributesStream;
        attributesStream << "Generación " << generation << ":\n";
        attributesStream << "Samurái " << bestSamurai.uniqueId << " - Atributos: ";
        attributesStream << "Edad: " << bestSamurai.age << "  ";
        attributesStream << "Probabilidad de supervivencia: " << bestSamurai.survivalProbability << "%  ";
        attributesStream << "Generaciones esperadas de supervivencia: " << bestSamurai.expectedGenerations << "  ";
        attributesStream << "Inteligencia emocional: " << bestSamurai.emotionalIntelligence << "  ";
        attributesStream << "Condición física: " << bestSamurai.physicalCondition << "  ";
        attributesStream << "Fuerza en tronco superior: " << bestSamurai.upperBodyStrength << "  ";
        attributesStream << "Fuerza en tronco inferior: " << bestSamurai.lowerBodyStrength << "  ";
        attributesStream << "Resistencia: " << bestSamurai.resistance << "  ";

        // Añade la informacion de la generacion
        generationAtributes << attributesStream.str();
        generationAtributes << "\n";
    }

    gtk_text_buffer_set_text(buffer, generationAtributes.str().c_str(), -1);

    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(builder);

    gtk_widget_show_all(window);
    gtk_main();


    while (true) {
        // Aceptar una conexión entrante
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            cerr << "Error al aceptar la conexión" << endl;
            continue; // Continuar esperando conexiones
        }

        cout << "Cliente conectado" << endl;

        while (true) {
            // Recibir la palabra clave del cliente
            char word[5] = {0};
            int bytesRead = recv(clientSocket, word, sizeof(word), 0);
            if (bytesRead != 4 || strcmp(word, "suma") != 0) {
                cerr << "Palabra clave incorrecta o error al recibir" << endl;
                break;
            }

            // Recibir el primer número del cliente
            char num1[1024] = {0};
            bytesRead = recv(clientSocket, num1, sizeof(num1), 0);
            if (bytesRead <= 0) {
                cerr << "Error al recibir el primer número del cliente" << endl;
                break;
            }

            // Recibir el segundo número del cliente
            char num2[1024] = {0};
            bytesRead = recv(clientSocket, num2, sizeof(num2), 0);
            if (bytesRead <= 0) {
                cerr << "Error al recibir el segundo número del cliente" << endl;
                break;
            }


            // Cerrar el socket del cliente
            close(clientSocket);
        }


    }


}
