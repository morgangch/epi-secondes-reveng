# 🧠 Atelier VMV – Reverse Engineering d’un jeu C++ (SFML)

Bienvenue dans cet atelier où vous allez **analyser, tester et démonter** un petit programme en C++... **sans avoir accès au code source** au départ. L’objectif : **débusquer les mécanismes cachés** d’un jeu de labyrinthe !

---

# 🎯 Objectif de l’atelier

Vous partez d’un **exécutable** (compilé) d’un jeu simple en apparence… mais qui contient :
* Des zones invisibles secrètes
* Des combinaisons de touches spéciales
* Des variables jamais affichées
* Des comportements inattendus

Votre mission : **comprendre et révéler tous les secrets du jeu**, à la fois **en jouant**, puis **en analysant le binaire avec Ghidra**.

---

## 📁 Matériel fourni

* Le fichier exécutable : `maze-win64.exe` (pour Windows)
* Aucun accès au code source
* Outils recommandés :
  * Ghidra (déjà installé sur vos machines)
  * Bloc-notes ou fichier `.txt` pour noter vos observations

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
  > 🔍 Ce sont des cases avec la valeur `5` dans le tableau (`SECRET`) qui semblent être des chemins normaux.

* Si on spamme les touches `A`, `B`, etc. → il peut se passer quelque chose
  > 🔍 Une combinaison très précise déclenche un message (cf. Partie 2)

* Il y a **des messages supplémentaires** dans la console si on explore certaines zones
  > 🔍 « ✨ Vous avez découvert toutes les zones secrètes ! »

---

## 🧩 Étape 3 – Déduire les mécanismes

Questionner les élèves :

* Certaines touches déclenchent-elles des événements invisibles ?
* Peut-on imaginer des mécanismes **non accessibles naturellement** ?
* Peut-on explorer le fichier pour deviner les structures internes ?

---

# 🕵️ Partie 2 – Analyse statique avec Ghidra

## 🛠️ Étape 1 : Importer le binaire

1. Ouvrez **Ghidra**.
2. Créez un projet.
3. Importez le binaire du jeu.
4. Lancez l’analyse automatique (`YES` à toutes les options par défaut).

---

## 🧠 Étape 2 : Observation globale

Repérez :

* Le point d’entrée (`main`)
* Les chaînes de caractères visibles (`Window`, `Maze`, `Félicitations`, etc.)
* Les **fonctions non appelées**
  * `secretEnding` ?
  * `debugMode` ?

* Les **variables globales** :
  * `hiddenScore` ?
  * `hiddenMessage` ?
  * `konamiUnlocked` ?

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