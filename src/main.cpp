#include <iostream>
#include <string>

void secret_function_1() {
    std::cout << "[FLAG1] Bravo, tu as trouvé la salle secrète !" << std::endl;
}

void secret_function_2() {
    std::cout << "[FLAG2] Encore un ? Tu vas trop loin..." << std::endl;
}

void intro() {
    std::cout << "Bienvenue dans le Labyrinthe Numérique.\n";
    std::cout << "Utilise les touches: gauche (g), droite (d), haut (h), bas (b).\n";
    std::cout << "Quitter : q\n";
}

int main() {
    std::string chemin = "";
    std::string input;

    intro();

    while (true) {
        std::cout << "Entrez une direction : ";
        std::cin >> input;

        if (input == "q") break;

        chemin += input;

        if (chemin == "gdbb") {
            secret_function_1();  // cachée dans le chemin
        } else if (chemin == "ddghg") {
            secret_function_2();  // chemin avancé
        } else if (chemin.size() > 10) {
            std::cout << "Tu t'égares... essaie autre chose.\n";
            chemin = "";
        } else {
            std::cout << "Tu avances...\n";
        }
    }

    std::cout << "Merci d'avoir joué !\n";
    return 0;
}
