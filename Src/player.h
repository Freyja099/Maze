#pragma once // Empêche les inclusions multiples de ce fichier d'en-tête
#include "maze.h" // Inclut la définition de la classe Maze
#include "raylib.h" // Inclut la bibliothèque raylib pour la gestion des graphiques

class Player { // Déclaration de la classe Player
private: // Début de la section privée, accessible uniquement à la classe elle-même
    Vector2 position; // Position actuelle du joueur
    float radius; // Rayon du joueur (utilisé pour les collisions)
    float speed; // Vitesse de déplacement du joueur
    bool isMoving; // Indique si le joueur est en mouvement

public: // Début de la section publique, accessible depuis l'extérieur de la classe
    Player(float x, float y); // Constructeur de la classe Player initialisant la position du joueur
    bool Update(const Maze& maze); // Met à jour l'état du joueur en fonction du labyrinthe
    void Draw() const; // Dessine le joueur à l'écran
    // Retourne la position actuelle du joueur
    Vector2 GetPosition() const;
};
