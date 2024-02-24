#include "display.h"
#include "eadkpp.h"
#include "palette.h"
#include <cstdio>

extern const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Stopwatch";
extern const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

// Couleurs des boutons
EADK::Color buttonColor[2] = {EADK::Color(bt), EADK::Color(bt)}; // Couleurs initiales des boutons

// Indices des boutons
int selectedButtonIndex = 0; // Indice du bouton actuellement sélectionné

// Textes des boutons
const char* buttonText[2] = {"Start", "Stop"};

// Variables pour le chronomètre
bool running = false; // Indique si le chronomètre est en cours d'exécution
int milliseconds = 0; // Nombre de millisecondes écoulées
int seconds = 0; // Nombre de secondes écoulées
int minutes = 0; // Nombre de minutes écoulées

void updateButtonColors() {
  int buttonWidth = 130; // Largeur des boutons augmentée pour inclure le texte
  int buttonHeight = 50;
  int screenWidth = EADK::Screen::Width;
  int screenHeight = EADK::Screen::Height;

  int spacing = 10; // Espacement entre les boutons

  // Calculer la largeur totale des deux boutons et de l'espace entre eux
  int totalButtonWidth = 2 * buttonWidth + spacing;

  // Calculer les coordonnées Y du coin supérieur gauche des boutons
  int buttonY = screenHeight - buttonHeight - 20;

  // Calculer la position du premier bouton
  int startX = (screenWidth - totalButtonWidth) / 2;

  for (int i = 0; i < 2; ++i) {
    // Calculer les coordonnées X du coin supérieur gauche du bouton
    int buttonX = startX + i * (buttonWidth + spacing);

    // Dessiner la bordure noire autour du bouton
    EADK::Display::pushRectUniform(EADK::Rect(buttonX - 1, buttonY - 1, buttonWidth + 2, buttonHeight + 2), EADK::Color(border));

    // Dessiner le bouton à la position calculée
    EADK::Display::pushRectUniform(EADK::Rect(buttonX, buttonY, buttonWidth, buttonHeight),
                                   (i == selectedButtonIndex) ? EADK::Color(hover) : buttonColor[i]);
    
    // Afficher le texte à l'intérieur du bouton
    EADK::Display::drawString(buttonText[i], EADK::Point(buttonX + 45, buttonY + 20), false,
                              EADK::Color(Black), (i == selectedButtonIndex) ? EADK::Color(hover) : buttonColor[i]);
  }
}

void drawTime() {
  // Convertir les millisecondes, les secondes et les minutes en chaînes de caractères
  char timeStr[20];
  snprintf(timeStr, 20, "%02d:%02d:%02d", minutes, seconds, milliseconds); // Utilisation de snprintf pour éviter les débordements de tampon

  // Afficher le temps à l'écran
  EADK::Display::drawString(timeStr, EADK::Point(130, 90), false, EADK::Color(Black), EADK::Color(bg)); // Utilisation d'une taille de police de 2
}

int main(int argc, char * argv[]) {
  // Créer une fenêtre de couleur uniforme (blanche)
  EADK::Display::pushRectUniform(EADK::Screen::Rect, EADK::Color(bg));

  // Dessiner deux boutons centrés en bas de la fenêtre avec un décalage
  updateButtonColors();
  
  EADK::Display::pushRectUniform(EADK::Rect(0, 0, 320, 18),nav);
  // Afficher du texte à l'écran ( titre de l'application )
  EADK::Display::drawString("STOPWATCH", EADK::Point(130, 3), false, EADK::Color(White), EADK::Color(nav));

  // Boucle d'événements pour maintenir la fenêtre ouverte
  while (true) {
    // Détecter les pressions de touches
    EADK::Keyboard::State currentState = EADK::Keyboard::scan();

    // Mettre à jour l'indice du bouton sélectionné en fonction de la touche pressée
    if (currentState.keyDown(EADK::Keyboard::Key::Left)) {
      selectedButtonIndex = (selectedButtonIndex - 1 + 2) % 2; // Déplacer la sélection vers la gauche
      updateButtonColors(); // Mettre à jour les couleurs des boutons
    } else if (currentState.keyDown(EADK::Keyboard::Key::Right)) {
      selectedButtonIndex = (selectedButtonIndex + 1) % 2; // Déplacer la sélection vers la droite
      updateButtonColors(); // Mettre à jour les couleurs des boutons
    } else if (currentState.keyDown(EADK::Keyboard::Key::EXE)) {
      // Si la touche 'EXE' est pressée, effectuer une action en fonction du bouton sélectionné
      if (selectedButtonIndex == 0) {
        // Action lorsque le bouton Start est sélectionné (lancer le chronomètre)
        running = true; // Démarrer le chronomètre
      } else {
        // Action lorsque le bouton Stop est sélectionné (arrêter le chronomètre)
        running = false; // Arrêter le chronomètre
      }
    } else if (currentState.keyDown(EADK::Keyboard::Key::Back)) {
      // Si la touche 'Retour' est pressée, quitter la boucle et fermer la fenêtre
      break;
    }

    // Mettre à jour le chronomètre si nécessaire
    if (running) {
      milliseconds++; // Incrémenter le nombre de millisecondes écoulées
      if (milliseconds >= 100) {
        milliseconds = 0;
        seconds++; // Incrémenter le nombre de secondes écoulées
      }
      if (seconds >= 60) {
        seconds = 0;
        minutes++; // Incrémenter le nombre de minutes écoulées
      }
      // Afficher le temps à l'écran
      drawTime();
    }

    // Mettre en pause pour éviter la surcharge du processeur
    EADK::Timing::msleep(10);
  }

  return 0;
}
