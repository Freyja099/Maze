#include "maze.h" // Inclusion du fichier d'en-tête contenant la déclaration de la classe Maze
#include <random> // Inclusion de la bibliothèque pour la génération de nombres aléatoires
#include <algorithm> // Inclusion de la bibliothèque pour les algorithmes standard comme std::shuffle

// Constructeur de la classe Maze qui initialise la taille du labyrinthe et génère le labyrinthe
Maze::Maze(int mazeSize) : size(mazeSize) {
    // Initialisation des murs du labyrinthe avec des valeurs true (mur présent)
    walls = std::vector<std::vector<bool>>(size, std::vector<bool>(size, true));
    generateMaze(1, 1); // Appel de la fonction pour générer le labyrinthe à partir de la position (1, 1)
    walls[1][1] = false; // Suppression du mur à la position de départ
    walls[size-2][size-2] = false; // Suppression du mur à la position d'arrivée
    ensurePath(); // Appel de la fonction pour s'assurer qu'il y a un chemin entre le début et la fin
}

// Fonction récursive pour générer le labyrinthe en utilisant l'algorithme de parcours en profondeur
void Maze::generateMaze(int x, int y) {
    walls[x][y] = false; // Suppression du mur à la position actuelle
    
    // Liste des directions possibles pour le déplacement (haut, bas, gauche, droite)
    std::vector<std::pair<int, int>> directions = {{0,2}, {2,0}, {0,-2}, {-2,0}};
    static std::random_device rd; // Dispositif de génération de nombres aléatoires
    static std::mt19937 gen(rd()); // Générateur de nombres aléatoires basé sur Mersenne Twister
    std::shuffle(directions.begin(), directions.end(), gen); // Mélange des directions pour un parcours aléatoire
    
    // Boucle sur chaque direction possible
    for (const auto& dir : directions) {
        int nextX = x + dir.first; // Calcul de la prochaine position en x
        int nextY = y + dir.second; // Calcul de la prochaine position en y
        
        // Vérification que la prochaine position est valide et que le mur est présent
        if (nextX > 0 && nextX < size-1 && nextY > 0 && nextY < size-1 && walls[nextX][nextY]) {
            walls[x + dir.first/2][y + dir.second/2] = false; // Suppression du mur intermédiaire
            generateMaze(nextX, nextY); // Appel récursif pour continuer la génération du labyrinthe
        }
    }
}

// Fonction pour s'assurer qu'il y a un chemin entre le début et la fin du labyrinthe
void Maze::ensurePath() {
    for (int i = 0; i < 3; i++) { // Boucle pour créer un chemin de 3 cellules
        walls[1][i+1] = false; // Suppression des murs pour le chemin de départ
        walls[size-2][size-2-i] = false; // Suppression des murs pour le chemin d'arrivée
    }
}

// Fonction pour vérifier si une position donnée est un mur
bool Maze::isWall(int x, int y) const {
    if (x < 0 || x >= size || y < 0 || y >= size) return true; // Retourne true si la position est en dehors des limites
    return walls[x][y]; // Retourne la valeur du mur à la position donnée
}

// Fonction pour dessiner le labyrinthe
void Maze::Draw() const {
    for (int i = 0; i < size; i++) { // Boucle sur chaque ligne
        for (int j = 0; j < size; j++) { // Boucle sur chaque colonne
            if (walls[i][j]) { // Si un mur est présent à la position (i, j)
                DrawRectangle(i * CELL_SIZE, j * CELL_SIZE, 
                                CELL_SIZE, CELL_SIZE, WALL_COLOR); // Dessiner le mur
            }
        }
    }
}

// Fonction pour obtenir la taille du labyrinthe
int Maze::GetSize() const {
    return size; // Retourne la taille du labyrinthe
}
