
# 🎯 QuizGame - Application de Quiz en Qt

QuizGame est une application interactive de quiz développée en **Qt (C++)**, permettant aux utilisateurs de tester leurs connaissances dans plusieurs domaines. Elle propose une interface fluide, des sons de feedback, un système de score avancé et une expérience ludique et motivante. 🚀🎮

![Accueil](screenshots/HomePage.png)

---

## ⚙️ Fonctionnalités Principales

- ✅ **Affichage dynamique des questions généré par API Gemini** avec des boutons de réponse interactifs.
- 🎯 **Système de score intelligent** :
  - +10 points pour chaque bonne réponse
  - Bonus de vitesse si réponse en moins de 5 secondes (+5)
  - Bonus de série (streak) de bonnes réponses (+2 par bonne réponse consécutive)
  - Bouton **Double Points** utilisable une fois par question pour doubler les points
- 📊 **Statistiques sauvegardées** avec `QSettings` :
  - Nombre total de parties jouées
  - Score total et score moyen
  - Pourcentage de bonnes réponses global
- 🧠 **Effets visuels & sonores** :
  - Feedback visuel vert/rouge selon la réponse
  - Sons différents pour bonnes ou mauvaises réponses
- ⏱️ **Calcul du temps de réponse** pour chaque question
- 💾 **Sauvegarde persistante** entre les sessions de jeu (via `QSettings`)
- 🎨 Interface moderne et responsive avec Qt Widgets

---

## 📂 Arborescence Simplifiée

```
QuizGame/
├── main.cpp
├── MainWindow.ui
├── src/
│   ├── mainwindow.cpp
│   └── gemini.cpp
├── headers/
│   ├── mainwindow.h
│   └── gemini.h
├── assets/
│   ├── sounds/
│   └── images/
├── settings.ini
└── screenshots/
    └── HomePage.png
```

---

## 🔧 Technologies utilisées

- **Qt 5/6**
- **C++**
- **QSettings** pour la sauvegarde des données
- **QTimer**, **QLabel**, **QGraphicsEffect**, etc. pour l’UI
- **QSoundEffect** pour les sons de feedback
- **GeminiAPI pour genérer les questions et les réponses

---

## 📈 Exemple de statistiques enregistrées

```plaintext
TotalGames     = 12
ScoreSum       = 950
AvgScore       = 79.17
CorrectNmbr    = 92
CorrectPercent = 76.67%
```

---

## 🚀 Démarrage rapide

1. Cloner le projet :
   ```bash
   git clone https://github.com/ayoubelhilali/QuizGame.git
   cd QuizGame
   ```

2. Ouvrir avec Qt Creator ou compiler avec `qmake` / `cmake`.

3. Lancer et jouer !

---

## 👨‍💻 Auteur

Ayoub Elhilali  

---

> *Les quiz ne rendent pas juste plus intelligent… ils rendent aussi les soirées plus fun !*
