# 🧠 Atelier VMV – Reverse Engineering d’un jeu C++ (SFML)

Bienvenue dans cet atelier où vous allez **analyser, tester et démonter** un petit programme en C++... **sans avoir accès au code source** au départ. L’objectif : **débusquer les mécanismes cachés** d’un jeu de labyrinthe !

---

# 🎯 Objectif de l’atelier

Vous partez d’un **exécutable** (compilé) d’un jeu simple en apparence… mais qui contient :
* Des zones invisibles secrètes
* Des combinaisons de touches spéciales
* Des variables jamais affichées
* Des comportements inattendus

Votre mission : **comprendre et révéler tous les secrets du jeu**, à la fois **en jouant**, puis **en analysant le binaire avec des outils dédiés**.

---

## 📁 Matériel fourni

* Le fichier exécutable : `maze-win64.exe` (pour Windows)
* Aucun accès au code source
* Outils recommandés :
  * [Ghidra, Binary Ninja](https://dogbolt.org) (accessibles en ligne)
  * ChatGPT (pour la Partie 2)
  * Bloc-notes ou fichier `.txt` pour noter vos observations

---

## Prérequis

* Formez une équipe de 4 personnes

---

# 🕹️ Partie 1 – Exécution naïve

## 🔄 Étape 1 – Jouer normalement

💡 Encourager les élèves à noter *tout comportement inattendu*
💬 Leur demander : « qu’est-ce qui pourrait être un indice ? » (couleurs, collision, affichage console…)

**Spoiler – victoire normale** :

> 🔍 Le jeu affiche « Félicitations, vous avez terminé le labyrinthe ! » en atteignant la case `(14, 9)` (END).

---

## 🧪 Étape 2 – Tester des comportements étranges

### À observer :
* Le joueur peut **passer dans certaines cases invisibles**
  > 🔍 Ce sont des cases ayant la même couleur que les murs, les traverser toutes permet de débloquer un message indiquant "Vous avez découvert toutes les zones secrètes"

* Si on spamme les touches `A`, `B`, etc. → il peut se passer quelque chose
  > 🔍 Une combinaison très précise déclenche un message (cf. Partie

---

## 🧩 Étape 3 – Déduire les mécanismes

Questionner les élèves :

* Certaines touches déclenchent-elles des événements invisibles ?
* Peut-on imaginer des mécanismes **non accessibles naturellement** ?
* Peut-on explorer le fichier pour deviner les structures internes ?
* Certains enchainements de touches provoquent des messages secrets : taper "RICK" dans l'interface de jeu, ou faire le konami code (flèche haut, flèche haut, flèche bas, flèche bas, flèche gauche, flèche droite, flèche gauche, flèche droite, b, a) provoquent des textes cachés.

---

# 🕵️ Partie 2 – Analyse statique avec Ghidra / Binary Ninja

## 🛠️ Étape 1 : Importer le binaire

1. Ouvrez [**Dogbolt**](https://dogbolt.org).
2. Importez le binaire du jeu.
3. Regardez les options, cochez Binary Ninja et Ghidra (surtout) car ils seront les plus bavards.

---

## 🧠 Étape 2 : Observation globale

Repérez :

* Le point d’entrée (`main`)
* Les chaînes de caractères visibles (`Window`, `Maze`, `Félicitations`, etc.)
* Les **fonctions non appelées**
  * `secretEnding` ?
  * `ultraSecretEnding` ?

* Les **variables globales** :
  * `hiddenScore` ?
  * `hiddenMessage` ?
  * `konamiUnlocked` ?
  * `developerMode` ?

### Chaînes visibles :
* `"Maze Game"`, `"Félicitations"`, `"Vous avez découvert toutes les zones secrètes !"`
* `"🎮 Code Konami déverrouillé !"`
  > 🔍 Le **code Konami bien présent** et est une étape à trouver

### Variables :
* `hiddenScore = 1337`
  > 🔍 Jamais affichée
* `hiddenMessage = "Bravo, vous êtes curieux."`
  > 🔍 Peut être affichée en modifiant le code ou via Ghidra
* `konamiUnlocked`
  > 🔍 Booléen modifié si la bonne séquence est jouée
* `developerMode`
  > 🔍 Variable basée sur les variables d'env, affiche un message si la valeur est à 42.

---

## 🔍 Étape 3 : Analyse des fonctions

Tentez d’analyser ces fonctions :

* `main()` : toute la logique du jeu y est présente
* `canMove()` : logique de déplacement
* `getColor()` : intéressant car le `SECRET` a la **même couleur que le PATH**, donc **invisible**
* `secretEnding()` : jamais appelée **directement**

### Questions à résoudre :

* Quelles touches déclenchent un effet ?
* Que se passe-t-il si une variable est à `true` ?
* Y a-t-il un test de `playerPos == {2,2}` ?
* Quelle est la condition d’affichage de `"secret ending"` ?
* Est-ce qu’une condition jamais atteinte pourrait être forcée (ex: patch mémoire) ?

## 💣 Spoilers / corrections

| Élément caché                | Détail technique                                                   |
| ---------------------------- | ------------------------------------------------------------------ |
| Zones secrètes               | Valeur `5` dans `mazeData`, alias `Cell::SECRET`                   |
| Détection de combo           | Comparaison avec un `std::vector<sf::Keyboard::Key>` de 10 touches |
| Combo déclencheur            | 🔍 `↑ ↑ ↓ ↓ ← → ← → B A` (Konami Code)                             |
| Fin secrète déclenchée par   | `if (konamiUnlocked && allSecretZonesDiscovered)`                  |
| Variables jamais affichées   | `hiddenMessage`, `hiddenScore`                                     |
| Possibilité de patch mémoire | ✅ `konamiUnlocked = true`, ou appel direct à `secretEnding()`      |

---

# 🧠 Partie 3 – Exploitation et synthèse

## ✨ Objectif final

L’objectif final est que les élèves soient capables de :

* Cartographier **toutes les zones secrètes**
* Identifier **la combinaison de touches spéciale** (Konami code)
* Révéler la chaîne `hiddenMessage`
* Provoquer la **fin secrète** (par action ou triche)
* Comprendre que **le code source n’est pas obligatoire** pour déduire une logique

---

## 💡 Défis bonus + corrigés

| Défi bonus                    | Solution                                                                                                |
| ----------------------------- | ------------------------------------------------------------------------------------------------------- |
| Modifier le message en hex    | Ouvrir `.exe` dans HxD → rechercher `"Félicitations"` ou `"Bravo"`                                      |
| Forcer une variable booléenne | CheatEngine : recherche `konamiUnlocked` (booléen → true)                                               |
| Activer `secretEnding`        | Patch condition                                                                                         |
| Reconstruire le code          | Utiliser Ghidra pour extraire les fonctions `main`, `secretEnding`, etc., puis recompiler partiellement |