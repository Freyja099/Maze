#pragma once // Assure que le fichier est inclus une seule fois lors de la compilation
#include "raylib.h" // Inclut la bibliothèque raylib pour les graphiques
#include <vector> // Inclut la bibliothèque vector pour utiliser std::vector

const int CELL_SIZE = 40; // Définit la taille de chaque cellule dans le labyrinthe
const Color WALL_COLOR = WHITE; // Définit la couleur des murs
const Color PATH_COLOR = {236, 240, 241, 255}; // Définit la couleur du chemin
const Color PLAYER_COLOR = {231, 76, 60, 255}; // Définit la couleur du joueur
const Color EXIT_COLOR = {46, 204, 113, 255}; // Définit la couleur de la sortie
const Color HOUSE_COLOR = {155, 89, 182, 255}; // Définit la couleur de la maison

class Maze { // Définit la classe Maze (Labyrinthe)
private: // Section privée pour les données et fonctions internes
    std::vector<std::vector<bool>> walls; // Vecteur 2D pour stocker les murs du labyrinthe
    int size; // Taille du labyrinthe
    
    void generateMaze(int x, int y); // Fonction pour générer le labyrinthe
    void ensurePath(); // Fonction pour s'assurer qu'il y a un chemin dans le labyrinthe

public: // Section publique pour les fonctions d'interface
    Maze(int mazeSize); // Constructeur pour initialiser le labyrinthe avec une taille donnée
    bool isWall(int x, int y) const; // Fonction pour vérifier si une cellule est un mur
    void Draw() const; // Fonction pour dessiner le labyrinthe
    int GetSize() const; // Fonction pour obtenir la taille du labyrinthe
};