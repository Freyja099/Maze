#include "player.h" // Inclut le fichier d'en-tête pour la classe Player
#include <cmath> // Inclut la bibliothèque cmath pour les fonctions mathématiques
#include <raymath.h> // Inclut la bibliothèque raymath pour les opérations vectorielles

// Constructeur de la classe Player, initialise la position , le rayon, la vitesse, et l'état de mouvement du joueur
Player::Player(float x, float y) 
    : position{x, y}, radius{CELL_SIZE * 0.4f}, 
      speed{CELL_SIZE}, isMoving{false} {}

// Méthode pour mettre à jour la position du joueur en fonction des entrées et du labyrinthe
bool Player::Update(const Maze& maze) {
    bool moved = false; // Variable pour suivre si le joueur a bougé
    
    // Accepter une nouvelle entrée de mouvement seulement si le joueur ne bouge pas actuellement
    if (!isMoving) {
        Vector2 newPosition = position; // Nouvelle position potentielle du joueur
        bool shouldMove = false; // Indicateur pour savoir si le joueur doit bouger

        // Vérifie si la touche droite est pressée
        if (IsKeyPressed(KEY_RIGHT)) {
            newPosition.x += CELL_SIZE; // Déplace la position vers la droite
            shouldMove = true; // Indique que le joueur doit bouger
        }
        // Vérifie si la touche gauche est pressée
        else if (IsKeyPressed(KEY_LEFT)) {
            newPosition.x -= CELL_SIZE; // Déplace la position vers la gauche
            shouldMove = true; // Indique que le joueur doit bouger
        }
        // Vérifie si la touche bas est pressée
        else if (IsKeyPressed(KEY_DOWN)) {
            newPosition.y += CELL_SIZE; // Déplace la position vers le bas
            shouldMove = true; // Indique que le joueur doit bouger
        }
        // Vérifie si la touche haut est pressée
        else if (IsKeyPressed(KEY_UP)) {
            newPosition.y -= CELL_SIZE; // Déplace la position vers le haut
            shouldMove = true; // Indique que le joueur doit bouger
        }

        // Vérifie si la nouvelle position est valide (pas dans un mur)
        if (shouldMove) {
            int newCellX = static_cast<int>(newPosition.x / CELL_SIZE); // Calcule la nouvelle cellule en X
            int newCellY = static_cast<int>(newPosition.y / CELL_SIZE); // Calcule la nouvelle cellule en Y
            
            // Vérifie si la nouvelle cellule n'est pas un mur
            if (!maze.isWall(newCellX, newCellY)) {
                position = newPosition; // Déplace instantanément à la nouvelle position
                moved = true; // Indique que le joueur a bougé
            }
        }
    }
    
    return moved; // Retourne si le joueur a bougé ou non
}

// Méthode pour dessiner le joueur à l'écran
void Player::Draw() const {
    DrawCircleV(position, radius, PLAYER_COLOR); // Dessine le corps du joueur

    // Dessine les yeux
    float eyeSize = radius / 4; // Taille des yeux
    Vector2 leftEye = {position.x - radius / 3, position.y - radius / 3}; // Position de l'œil gauche
    Vector2 rightEye = {position.x + radius / 3, position.y - radius / 3}; // Position de l'œil droit
    DrawCircleV(leftEye, eyeSize, WHITE); // Dessine l'œil gauche
    DrawCircleV(rightEye, eyeSize, WHITE); // Dessine l'œil droit

    // Dessine le sourire
    Vector2 smileStart = {position.x - radius / 2, position.y + radius / 3}; // Début du sourire
    Vector2 smileEnd = {position.x + radius / 2, position.y + radius / 3}; // Fin du sourire
    DrawLineEx(smileStart, smileEnd, 2.0f, WHITE); // Dessine le sourire
}

// Méthode pour obtenir la position actuelle du joueur
Vector2 Player::GetPosition() const {
    return position; // Retourne la position du joueur
}
