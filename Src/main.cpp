#include "raylib.h" // Inclusion de la bibliothèque raylib pour le développement de jeux
#include "maze.h" // Inclusion du fichier d'en-tête pour la classe Maze
#include "player.h" // Inclusion du fichier d'en-tête pour la classe Player
#include <memory> // Inclusion pour utiliser des pointeurs intelligents
#include <vector> // Inclusion pour utiliser des vecteurs
#include <cmath> // Inclusion pour les fonctions mathématiques
#include <iostream> // Inclusion pour les entrées/sorties standard

// Constantes pour la configuration de la fenêtre du jeu
const int SCREEN_WIDTH = 1280; // Largeur de la fenêtre du jeu
const int SCREEN_HEIGHT = 886; // Hauteur de la fenêtre du jeu
const Color MENU_COLOR = BLACK; // Couleur de fond du menu
const Color MENU_TEXT_COLOR = WHITE; // Couleur du texte du menu
const float TEXT_FADE_SPEED = 0.5f; // Vitesse de l'apparition progressive du texte

// Enums pour définir des constantes nommées pour une meilleure lisibilité et maintenabilité du code.
enum class GameState { MENU, PLAYING, FINISHED }; // Représente les différents états du jeu.
enum class Difficulty { EASY, MEDIUM, HARD }; // Représente les différents niveaux de difficulté du jeu.
enum class MenuState { STORY, LEVEL_SELECT }; // Représente les différents états du menu.

// Structure pour le texte de l'histoire
struct StoryText {
    const char* text; // Le texte de l'histoire
    float alpha; // Transparence du texte
    float y; // Position verticale du texte
    bool isVisible; // Indique si le texte est visible
};

int main() {
    // Initialisation de la fenêtre et du dispositif audio
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Game"); // Crée une fenêtre de jeu
    InitAudioDevice(); // Initialise le dispositif audio
    SetTargetFPS(60); // Définit le nombre d'images par seconde

    // Chargement de la texture de fond
    Texture2D backgroundTex = LoadTexture("resources/dd.png"); // Charge l'image de fond
    if (backgroundTex.id == 0) { // Vérifie si le chargement a échoué
        CloseWindow(); // Ferme la fenêtre
        printf("Erreur : Impossible de charger l'image 'dd.png'.\n"); // Affiche un message d'erreur
        return -1; // Quitte le programme avec une erreur
    }

    // Chargement des sons
    Sound menuMusic = LoadSound("resources/AA.mp3"); // Charge la musique du menu
    Sound stepSound = LoadSound("resources/step.wav"); // Charge le son des pas
    Sound victorySound = LoadSound("resources/victory.wav"); // Charge le son de victoire
    
    // Réglage des volumes des sons
    SetSoundVolume(menuMusic, 0.5f); // Définit le volume de la musique du menu
    SetSoundVolume(stepSound, 0.7f); // Définit le volume du son des pas
    SetSoundVolume(victorySound, 1.0f); // Définit le volume du son de victoire

    // Initialisation des variables d'état du jeu
    GameState gameState = GameState::MENU; // État initial du jeu
    MenuState menuState = MenuState::STORY; // État initial du menu
    Difficulty selectedDifficulty = Difficulty::EASY; // Difficulté sélectionnée par défaut
    float gameStartTime = 0; // Temps de début du jeu
    float gameEndTime = 0; // Temps de fin du jeu
    float menuTimer = 0; // Timer pour le menu
    bool soundOn = true; // Variable to track sound state
    
    // Story texts initialization
    std::vector<StoryText> storyTexts = {
        {"In a world of twisting paths and endless corridors...", 0.0f, SCREEN_HEIGHT/2 - 100, true},
        {"A little adventurer finds themselves lost and far from home...", 0.0f, SCREEN_HEIGHT/2, false},
        {"Can you help guide them back to their cozy house?", 0.0f, SCREEN_HEIGHT/2 + 100, false},
        {"Press SPACE to begin your journey...", 0.0f, SCREEN_HEIGHT/2 + 200, false}
    };
    int currentStoryText = 0;// Index du texte d'histoire actuel
    
    float victoryTimer = 0;// Timer pour l'affichage de la victoire
    Vector2 characterHomePos = {0, 0};// Position de la maison du personnage


    // Unique pointers for maze and player
std::unique_ptr<Maze> maze; // Pointeur unique pour le labyrinthe
std::unique_ptr<Player> player; // Pointeur unique pour le joueur

// Play menu music
PlaySound(menuMusic); // Joue la musique du menu

// Boucle principale du jeu
while (!WindowShouldClose()) { // Continue tant que la fenêtre n'est pas fermée
    BeginDrawing(); // Commence le dessin
    ClearBackground(RAYWHITE); // Efface l'arrière-plan

    // ClearBackground(RAYWHITE); // Efface l'arrière-plan (commenté)

    // Dessine la texture de fond
    DrawTexture(backgroundTex, 0, 0, WHITE); // Dessine l'image de fond à la position (0, 0)

    // Boucle de la musique du menu
    if (!IsSoundPlaying(menuMusic) && gameState == GameState::MENU) { // Si la musique du menu n'est pas en cours et que l'état du jeu est MENU
        PlaySound(menuMusic); // Joue la musique du menu
    }

    switch(gameState) { // Vérifie l'état du jeu
        case GameState::MENU: { // Si l'état est MENU
            ClearBackground(MENU_COLOR); // Efface l'arrière-plan avec la couleur du menu
            
            if (menuState == MenuState::STORY) { // Si l'état du menu est l'histoire
                menuTimer += GetFrameTime(); // Met à jour le timer du menu
                
                // Met à jour et dessine les textes de l'histoire
                for (size_t i = 0; i < storyTexts.size(); i++) { // Pour chaque texte d'histoire
                    if (storyTexts[i].isVisible) { // Si le texte est visible
                        // Fait apparaître le texte progressivement
                        storyTexts[i].alpha = fmin(1.0f, storyTexts[i].alpha + TEXT_FADE_SPEED * GetFrameTime());
                        
                        // Dessine le texte avec l'alpha actuel
                        Color textColor = MENU_TEXT_COLOR; // Définit la couleur du texte
                        textColor.a = (unsigned char)(255 * storyTexts[i].alpha); // Définit la transparence
                        int textWidth = MeasureText(storyTexts[i].text, 30); // Mesure la largeur du texte
                        DrawText(storyTexts[i].text, SCREEN_WIDTH/2 - textWidth/2, (int)storyTexts[i].y, 35, textColor); // Dessine le texte centré
                    }
                }

                // Affiche le texte suivant toutes les 2 secondes
                if (menuTimer >= 2.0f && static_cast<std::vector<StoryText>::size_type>(currentStoryText) < storyTexts.size() - 1) {
                    currentStoryText++; // Passe au texte suivant
                    storyTexts[currentStoryText].isVisible = true; // Rendre le texte suivant visible
                    menuTimer = 0; // Réinitialise le timer
                }

                // Si la touche ESPACE est pressée, passe à l'état de sélection de niveau
                if (IsKeyPressed(KEY_SPACE)) {
                    menuState = MenuState::LEVEL_SELECT; // Change l'état du menu
                    menuTimer = 0; // Réinitialise le timer
                }
            }
            else { // Si l'état du menu est LEVEL_SELECT
                const char* title = "Choose Your Path"; // Titre de la sélection de niveau
                DrawText(title, SCREEN_WIDTH/2 - MeasureText(title, 45)/2, 200, 70, MENU_TEXT_COLOR); // Dessine le titre // Centre le titre

                // Dessine les options de niveau
                DrawText("1 - A Gentle Start ",
                        SCREEN_WIDTH/2 - 280, 400, 50, MENU_TEXT_COLOR);
                DrawText("2 - The Winding Path ",
                        SCREEN_WIDTH/2 - 260, 500, 50, MENU_TEXT_COLOR);
                DrawText("3 - The Grand Maze ",
                        SCREEN_WIDTH/2 - 240, 600, 50, MENU_TEXT_COLOR);

                // Vérifie si une touche de niveau est pressée
                if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_THREE)) {
                    // Définit la difficulté en fonction de la touche pressée
                    selectedDifficulty = IsKeyPressed(KEY_ONE) ? Difficulty::EASY : 
                                        IsKeyPressed(KEY_TWO) ? Difficulty::MEDIUM : Difficulty::HARD;
                    // Définit la taille du labyrinthe en fonction de la difficulté
                    int mazeSize = (selectedDifficulty == Difficulty::EASY) ? 13 : 
                                    (selectedDifficulty == Difficulty::MEDIUM) ? 15 : 20;
                    
                    // Crée une nouvelle instance de Maze et Player avec des tailles appropriées
                    maze = std::make_unique<Maze>(mazeSize); // Crée un labyrinthe de la taille choisie
                    player = std::make_unique<Player>(CELL_SIZE * 1.5f, CELL_SIZE * 1.5f); // Crée un joueur à la position initiale
                    gameState = GameState::PLAYING; // Change l'état du jeu à JOUER
                    gameStartTime = GetTime(); // Enregistre le temps de début du jeu
                }
            }
            

            break; // Fin du cas MENU
        }
        
        case GameState::PLAYING: { // Si l'état est JOUER
            bool moved = player->Update(*maze); // Met à jour la position du joueur et vérifie s'il a bougé
            if (moved) {
                PlaySound(stepSound); // Joue le son de pas si le joueur a bougé
            }
            
            int mazeSize = maze->GetSize(); // Obtient la taille du labyrinthe
            int mazePixelSize = mazeSize * CELL_SIZE; // Calcule la taille en pixels du labyrinthe
            int offsetX = (SCREEN_WIDTH - mazePixelSize) / 2; // Calcule le décalage horizontal pour centrer le labyrinthe
            int offsetY = (SCREEN_HEIGHT - mazePixelSize) / 2; // Calcule le décalage vertical pour centrer le labyrinthe
            
            BeginMode2D((Camera2D){ // Commence le mode 2D pour dessiner le labyrinthe et le joueur
                .offset = {(float)offsetX, (float)offsetY}, // Définit le décalage de la caméra
                .target = {0, 0}, // Définit le point cible de la caméra
                .rotation = 0, // Rotation de la caméra
                .zoom = 1.0f // Zoom de la caméra
            });
            
            maze->Draw(); // Dessine le labyrinthe
            
            // Dessine la maison à la sortie
            int exitX = (mazeSize-2) * CELL_SIZE; // Position X de la sortie
            int exitY = (mazeSize-2) * CELL_SIZE; // Position Y de la sortie
            
            // Dessine le corps de la maison
            DrawRectangle(exitX, exitY + CELL_SIZE/3, CELL_SIZE, 2*CELL_SIZE/3, HOUSE_COLOR); // Dessine le rectangle représentant la maison
            
            // Dessine le toit (triangle)
            Vector2 roofTop = {(float)(exitX + CELL_SIZE/2.0f), (float)exitY}; // Sommet du toit
            Vector2 roofLeft = {(float)exitX, (float)(exitY + CELL_SIZE/3.0f)}; // Coin gauche du toit
            Vector2 roofRight = {(float)(exitX + CELL_SIZE), (float)(exitY + CELL_SIZE/3.0f)}; // Coin droit du toit
            DrawTriangle(roofTop, roofLeft, roofRight, RED); // Dessine le toit
            
            // Dessine la porte
            DrawRectangle(exitX + CELL_SIZE/3, exitY + CELL_SIZE/2,CELL_SIZE/3, CELL_SIZE/2, DARKBROWN); // Dessine la porte
            
            // Dessine la poignée de la porte
            DrawCircle(exitX + 2*CELL_SIZE/3 - 5, exitY + 3*CELL_SIZE/4, 3, GOLD); // Dessine la poignée
            
            player->Draw(); // Dessine le joueur
            
            EndMode2D(); // Termine le mode 2D
            
            float currentTime = GetTime() - gameStartTime; // Calcule le temps écoulé depuis le début du jeu
            DrawText(TextFormat("Time: %.1f", currentTime), 10, 10, 50, WHITE); // Affiche le temps écoulé
            
            Vector2 pos = player->GetPosition(); // Obtient la position actuelle du joueur
            // Vérifie si le joueur a atteint la sortie
            if (pos.x > (mazeSize-2) * CELL_SIZE && pos.y > (mazeSize-2) * CELL_SIZE) {
                gameState = GameState::FINISHED; // Change l'état du jeu à TERMINÉ
                gameEndTime = currentTime; // Enregistre le temps de fin
                PlaySound(victorySound); // Joue le son de victoire
                victoryTimer = 0; // Réinitialise le timer de victoire
                characterHomePos = pos; // Enregistre la position du joueur à la sortie
            }
            break; // Fin du cas PLAYING
        }
        
        case GameState::FINISHED: { // Si l'état est TERMINÉ
            victoryTimer += GetFrameTime(); // Met à jour le timer de victoire
            
            const char* congrats = "Home Sweet Home!"; // Message de félicitations
            float scale = 1.0f + 0.1f * sinf(victoryTimer * 5.0f); // Calcule l'échelle pour l'effet de pulsation
            float rotation = sinf(victoryTimer * 3.0f) * 5.0f; // Calcule la rotation pour l'effet de rotation
            
            Vector2 textPos = { // Position du texte de félicitations
                SCREEN_WIDTH/2.0f,
                SCREEN_HEIGHT/2.0f - 60
            };
            
            // Dessine le texte de félicitations avec des effets de scale et de rotation
            DrawTextPro(GetFontDefault(), congrats, textPos, {MeasureText(congrats, 60) * scale/2.0f, 40}, rotation, 100, 6, VIOLET);
            
            // Affiche le temps écoulé depuis la victoire
            DrawText(TextFormat("Your Score: %.1f seconds", gameEndTime),SCREEN_WIDTH/2 - 100,SCREEN_HEIGHT/2 + 50,50,YELLOW);
            
            // Affiche un message pour recommencer le jeu
            if (victoryTimer > 1.0f) { // Si le timer de victoire est supérieur à 1 seconde
                float alpha = (sinf(victoryTimer * 2.0f) + 1.0f) * 0.5f; // Calcule l'alpha pour l'effet de clignotement
                Color promptColor = WHITE; // Couleur du message
                promptColor.a = (unsigned char)(255 * alpha); // Définit la transparence
                DrawText("Press SPACE to play again",SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 + 200,50,promptColor); // Dessine le message
            }
            
            // Vérifie si la touche ESPACE est pressée pour recommencer
            if (IsKeyPressed(KEY_SPACE)) {
                gameState = GameState::MENU; // Retourne à l'état du menu
                menuTimer = 0; // Réinitialise le timer du menu
                for (auto& text : storyTexts) { // Réinitialise tous les textes d'histoire
                    text.alpha = 0.0f; // Réinitialise l'alpha
                    text.isVisible = false; // Rend le texte invisible
                }
            }

        
            break; // Fin du cas FINISHED
        }
    }

    // Dessine l'icône pour jouer ou couper le son
            Rectangle soundButton = { SCREEN_WIDTH - 70, 20, 50, 50 };
            DrawRectangleRec(soundButton, BLANK); // Dessine un rectangle transparent pour la zone cliquable
            if (soundOn) {
                DrawCircle(soundButton.x + soundButton.width / 2, soundButton.y + soundButton.height / 2, 20, GREEN); // Dessine un cercle vert pour indiquer que le son est activé
            } else {
                DrawCircle(soundButton.x + soundButton.width / 2, soundButton.y + soundButton.height / 2, 20, RED); // Dessine un cercle rouge pour indiquer que le son est coupé
            }

            // Vérifie si le bouton est cliqué
            if (CheckCollisionPointRec(GetMousePosition(), soundButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                soundOn = !soundOn; // Inverse l'état du son
                if (soundOn) {
                    PlaySound(menuMusic); // Joue la musique du menu
                } else {
                    StopSound(menuMusic); // Coupe la musique du menu
                }
            }

            // Assure que la musique est jouée ou arrêtée en fonction de l'état du son
            if (soundOn && !IsSoundPlaying(menuMusic)) {
                PlaySound(menuMusic); // Joue la musique du menu si elle n'est pas déjà en cours
            } else if (!soundOn && IsSoundPlaying(menuMusic)) {
                StopSound(menuMusic); // Coupe la musique du menu si elle est en cours
            }
        
        EndDrawing();

}    

    // Libération des ressources et fermeture de la fenêtre
    UnloadSound(menuMusic); // Libère la musique du menu
    UnloadSound(stepSound); // Libère le son des pas
    UnloadSound(victorySound); // Libère le son de victoire
    CloseAudioDevice(); // Ferme le dispositif audio
    UnloadTexture(backgroundTex); // Libère la texture de fond

    CloseWindow(); // Ferme la fenêtre
    return 0; // Retourne 0 pour indiquer que le programme s'est terminé avec succès
}
