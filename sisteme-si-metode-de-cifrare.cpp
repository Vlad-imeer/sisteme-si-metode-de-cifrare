﻿#include <iostream>
using std::cout;
using std::cin;

#include <iomanip>
using std::setw;

#include <string>
using std::string;
using std::getline;

#include <algorithm>
using std::transform;
using std::reverse;
using std::sort;

#include <vector>
using std::vector;

#include <cmath>
using std::ceil;

// ========= General Helpers =========

const auto LITERA_INLOCUITOARE = 'Q';

const auto ALFABET_CLASIC = string("ABCDEFGHIJKLMNOPQRSTUVWXYZ");

typedef void (*callback)();

void AlegereCifrareDescifrare(callback callback_cifrare, callback callback_descifrare)
{
    auto optiune = 0;

    do
    {
        cout << "\n1. Cifrare";
        cout << "\n2. Descifrare";
        cout << "\n0. Inapoi";
        cout << "\n~> ";

        cin >> optiune;

        switch (optiune)
        {
        case 0:
            {
                return;
            }
        case 1:
            {
                callback_cifrare();
                return;
            }
        case 2:
            {
                callback_descifrare();
                return;
            }
        default:
            {
                cout << "\nOptiune invalida.\n";
            }
        }
    }
    while (optiune != 0);
}

void AfisareText(const string& mesaj, const string& text, const bool afisare_raw = true)
{
    cout << "\n====== " << mesaj << ":\n";

    for (auto caracter : text)
    {
        if (caracter < 'A' || caracter > 'Z')
        {
            continue;
        }
        cout << setw(3) << caracter;
    }

    cout << '\n';

    for (auto caracter : text)
    {
        if (caracter < 'A' || caracter > 'Z')
        {
            continue;
        }
        cout << setw(3) << caracter - 'A';
    }

    cout << '\n';

    if (afisare_raw)
    {
        cout << "\nRaw: " << text << '\n';
    }
}

string CitireText(const string& interfata)
{
    auto mesaj_input = string();
    auto buffer = string();
    cout << '\n' << interfata << ": ";

    getline(cin, buffer);
    if (buffer.empty() || buffer[0] == '\n' || buffer[0] == '\r' || buffer[0] == '\f')
    {
        getline(cin, mesaj_input);
    }
    else
    {
        mesaj_input = buffer;
    }

    transform(mesaj_input.begin(), mesaj_input.end(), mesaj_input.begin(), ::toupper);
    return mesaj_input;
}

string NormalizareText(const string& text, char litera_inlocuitoare)
{
    if (litera_inlocuitoare >= 'a' && litera_inlocuitoare <= 'z')
    {
        litera_inlocuitoare -= 'a' - 'A';
    }

    auto ret = string();
    for (auto caracter : text)
    {
        if (caracter < 'A' || caracter > 'Z')
        {
            ret += litera_inlocuitoare;
            continue;
        }
        ret += caracter;
    }

    return ret;
}

// ========= Cezar (Substitutie Monoalfabetica folosind un offset)  =========

string GetAlfabetShiftat(const int offset)
{
    auto alfabet_shiftat = string();
    for (auto caracter : ALFABET_CLASIC)
    {
        alfabet_shiftat += (caracter + offset - 'A') % 26 + 'A';
    }
    return alfabet_shiftat;
}

void CifrareCezar()
{
    auto mesaj = NormalizareText(CitireText("Text care trebuie cifrat"), LITERA_INLOCUITOARE);

    auto offset = 0;
    cout << "\nOffset: ";
    cin >> offset;

    AfisareText("Alfabetul clasic", ALFABET_CLASIC, false);
    AfisareText("Alfabetul shiftat", GetAlfabetShiftat(offset), false);

    auto mesaj_cifrat = string();
    for (auto caracter : mesaj)
    {
        mesaj_cifrat += (caracter + offset - 'A') % 26 + 'A';
    }

    AfisareText("Mesajul initial", mesaj, false);
    AfisareText("Mesajul cifrat", mesaj_cifrat);
}

void DescifrareCezar()
{
    auto mesaj = CitireText("Text care trebuie descifrat");

    auto offset = 0;
    cout << "\nOffset: ";
    cin >> offset;

    mesaj = NormalizareText(mesaj, (LITERA_INLOCUITOARE + offset - 'A') % 26 + 'A');

    AfisareText("Alfabetul clasic", ALFABET_CLASIC, false);
    AfisareText("Alfabetul shiftat", GetAlfabetShiftat(offset), false);

    auto mesaj_descifrat = string();
    for (auto caracter : mesaj)
    {
        mesaj_descifrat += ((caracter - offset - 'A') % 26 + 26) % 26 + 'A';
    }

    AfisareText("Mesajul initial", mesaj, false);
    AfisareText("Mesajul descifrat", mesaj_descifrat);
}

// ========= Metoda Substitutiei =========

string GetAlfabetSubstitutie(const string& parola)
{
    auto parola_cu_litere_unice = string();
    for (auto caracter : parola)
    {
        if (parola_cu_litere_unice.find(caracter) == string::npos)
        {
            parola_cu_litere_unice += caracter;
        }
    }
    AfisareText("Parola cu litere unice", parola_cu_litere_unice, false);

    auto alfabet_de_substitutie = string(parola_cu_litere_unice);
    for (auto caracter : ALFABET_CLASIC)
    {
        if (alfabet_de_substitutie.find(caracter) == string::npos)
        {
            alfabet_de_substitutie += caracter;
        }
    }
    reverse(alfabet_de_substitutie.begin(), alfabet_de_substitutie.end());
    AfisareText("Alfabetul de substitutie", alfabet_de_substitutie, false);

    return alfabet_de_substitutie;
}

void CifrareSubstitutie()
{
    auto mesaj = NormalizareText(CitireText("Text care trebuie cifrat"), LITERA_INLOCUITOARE);
    auto parola = NormalizareText(CitireText("Parola"), LITERA_INLOCUITOARE);

    AfisareText("Alfabetul clasic", ALFABET_CLASIC, false);

    auto alfabet_de_substitutie = GetAlfabetSubstitutie(parola);

    auto mesaj_cifrat = string();
    for (auto caracter : mesaj)
    {
        mesaj_cifrat += alfabet_de_substitutie[ALFABET_CLASIC.find(caracter)];
    }

    AfisareText("Mesajul initial", mesaj, false);
    AfisareText("Mesajul cifrat", mesaj_cifrat);
}

void DescifrareSubstitutie()
{
    auto mesaj = CitireText("Text care trebuie descifrat");
    auto parola = NormalizareText(CitireText("Parola"), LITERA_INLOCUITOARE);

    AfisareText("Alfabetul clasic", ALFABET_CLASIC, false);

    auto alfabet_de_substitutie = GetAlfabetSubstitutie(parola);
    mesaj = NormalizareText(mesaj, ALFABET_CLASIC[alfabet_de_substitutie.find(LITERA_INLOCUITOARE)]);

    auto mesaj_descifrat = string();
    for (auto caracter : mesaj)
    {
        mesaj_descifrat += ALFABET_CLASIC[alfabet_de_substitutie.find(caracter)];
    }

    AfisareText("Mesajul initial", mesaj, false);
    AfisareText("Mesajul descifrat", mesaj_descifrat);
}

// ========= Sisteme Polialfabetice (Vigenere) =========

void CifrareVigenere()
{
    auto mesaj = NormalizareText(CitireText("Text care trebuie cifrat"), LITERA_INLOCUITOARE);
    auto parola = NormalizareText(CitireText("Parola"), LITERA_INLOCUITOARE);

    AfisareText("Alfabetul clasic", ALFABET_CLASIC, false);

    auto parola_repetata = string();
    for (size_t index = 0; index < mesaj.size(); index++)
    {
        parola_repetata += parola[index % parola.size()];
    }

    AfisareText("Mesajul initial", mesaj, false);
    AfisareText("Parola repetata", parola_repetata, false);

    auto mesaj_cifrat = string();
    for (size_t index = 0; index < mesaj.size(); index++)
    {
        mesaj_cifrat += ((mesaj[index] - 'A') + (parola_repetata[index] - 'A')) % 26 + 'A';
    }

    AfisareText("Mesajul cifrat", mesaj_cifrat);
}

void DescifrareVigenere()
{
    auto mesaj = CitireText("Text care trebuie descifrat");
    auto parola = NormalizareText(CitireText("Parola"), LITERA_INLOCUITOARE);

    AfisareText("Alfabetul clasic", ALFABET_CLASIC, false);

    auto parola_repetata = string();
    for (size_t index = 0; index < mesaj.size(); index++)
    {
        auto caracter_curent = parola[index % parola.size()];
        if (mesaj[index] < 'A' || mesaj[index] > 'Z')
        {
            mesaj[index] = ((LITERA_INLOCUITOARE - 'A') + (caracter_curent - 'A') + 26) % 26 + 'A';
        }
        parola_repetata += caracter_curent;
    }

    AfisareText("Mesajul initial", mesaj, false);
    AfisareText("Parola repetata", parola_repetata, false);

    auto mesaj_cifrat = string();
    for (size_t index = 0; index < mesaj.size(); index++)
    {
        mesaj_cifrat += ((mesaj[index] - 'A') - (parola_repetata[index] - 'A') + 26) % 26 + 'A';
    }

    AfisareText("Mesajul cifrat", mesaj_cifrat);
}

// ========= Metoda Transpozitiei =========

vector<size_t> GetIndecsiParola(const string& parola)
{
    auto parola_sortata = string(parola);
    sort(parola_sortata.begin(), parola_sortata.end());
    auto indecsi_parola = vector<size_t>(parola.size(), 0);

    cout << "\n====== " << "Parola sortata:\n";
    for (char caracter : parola_sortata)
    {
        cout << setw(3) << caracter;
    }
    cout << '\n';
    for (size_t index = 0; index < parola.size(); index++)
    {
        cout << setw(3) << index + 1;

        for (size_t index_indecsi = 0; index_indecsi < indecsi_parola.size(); index_indecsi++)
        {
            if (indecsi_parola[index_indecsi])
            {
                continue;
            }
            if (parola[index_indecsi] == parola_sortata[index])
            {
                indecsi_parola[index_indecsi] = index + 1;
                break;
            }
        }
    }
    cout << '\n';

    cout << "\n====== " << "Parola initiala cu indecsii parolei sortate:\n";
    for (char caracter : parola)
    {
        cout << setw(3) << caracter;
    }
    cout << '\n';
    for (size_t index = 0; index < parola.size(); index++)
    {
        cout << setw(3) << indecsi_parola[index];
    }
    cout << '\n';

    return indecsi_parola;
}

void CifrareTranspozitie()
{
    auto mesaj = NormalizareText(CitireText("Text care trebuie cifrat"), LITERA_INLOCUITOARE);
    auto parola = NormalizareText(CitireText("Parola"), LITERA_INLOCUITOARE);
    auto grupare = 5;
    cout << "\nNumar litere in fiecare grup al mesajului cifrat: ";
    cin >> grupare;

    if (grupare == 0)
    {
        grupare = -1;
    }

    AfisareText("Alfabetul clasic", ALFABET_CLASIC, false);

    auto indecsi_parola = GetIndecsiParola(parola);

    // === Matrice inițială

    auto numar_linii = static_cast<size_t>(
        ceil(static_cast<double>(mesaj.size()) / static_cast<double>(parola.size())));
    cout << "\n====== " << "Mesajul scris pe " << numar_linii << " randuri de cate " << parola.size() <<
        " caractere:\n";
    auto matrice = vector<vector<char>>(numar_linii, vector<char>(parola.size(), LITERA_INLOCUITOARE));
    for (size_t index = 0; index < parola.size(); index++)
    {
        cout << setw(3) << indecsi_parola[index];
    }
    cout << '\n';
    size_t index_mesaj = 0;
    for (auto& linie : matrice)
    {
        for (auto& coloana : linie)
        {
            if (index_mesaj < mesaj.size())
            {
                coloana = mesaj[index_mesaj];
                index_mesaj++;
            }
            cout << setw(3) << coloana;
        }
        cout << '\n';
    }

    // === Matrice transpusă

    cout << "\n====== " << "Mesajul scris pe " << numar_linii << " randuri de cate " << parola.size() <<
        " caractere, cu coloanele in ordine crescatoare in functie de indecsi:\n";
    auto matrice_ordonata = vector<vector<char>>(numar_linii, vector<char>(parola.size(), LITERA_INLOCUITOARE));
    for (size_t index_coloana = 1; index_coloana <= parola.size(); index_coloana++)
    {
        for (size_t index_indecsi = 0; index_indecsi < indecsi_parola.size(); index_indecsi++)
        {
            if (indecsi_parola[index_indecsi] == index_coloana)
            {
                for (size_t index_linie = 0; index_linie < numar_linii; index_linie++)
                {
                    matrice_ordonata[index_linie][index_coloana - 1] = matrice[index_linie][index_indecsi];
                }
                break;
            }
        }
    }

    for (size_t index = 0; index < parola.size(); index++)
    {
        cout << setw(3) << index + 1;
    }
    cout << '\n';
    for (auto& linie : matrice_ordonata)
    {
        for (auto& coloana : linie)
        {
            cout << setw(3) << coloana;
        }
        cout << '\n';
    }

    // === Mesaj cifrat

    auto mesaj_cifrat = string();
    auto counter_grupare = 0;
    for (size_t coloana = 0; coloana < parola.size(); coloana++)
    {
        for (size_t linie = 0; linie < numar_linii; linie++)
        {
            if (counter_grupare == grupare)
            {
                mesaj_cifrat += ',';
                counter_grupare = 0;
            }
            mesaj_cifrat += matrice_ordonata[linie][coloana];
            counter_grupare++;
        }
    }

    AfisareText("Mesajul cifrat", mesaj_cifrat);
}

void DescifrareTranspozitie()
{
    auto mesaj = NormalizareText(CitireText("Text care trebuie descifrat"), LITERA_INLOCUITOARE);
    auto parola = NormalizareText(CitireText("Parola"), LITERA_INLOCUITOARE);

    AfisareText("Alfabetul clasic", ALFABET_CLASIC, false);

    auto indecsi_parola = GetIndecsiParola(parola);

    // === Matrice inițială

    auto numar_linii = static_cast<size_t>(
        ceil(static_cast<double>(mesaj.size()) / static_cast<double>(parola.size())));
    cout << "\n====== " << "Mesajul scris pe " << parola.size() << " coloane de cate " << numar_linii <<
        " caractere:\n";
    auto matrice = vector<vector<char>>(numar_linii, vector<char>(parola.size(), LITERA_INLOCUITOARE));
    for (size_t index = 0; index < parola.size(); index++)
    {
        cout << setw(3) << index + 1;
    }
    cout << '\n';
    size_t index_mesaj = 0;
    for (size_t coloana = 0; coloana < parola.size(); coloana++)
    {
        for (size_t linie = 0; linie < numar_linii; linie++)
        {
            if (index_mesaj < mesaj.size())
            {
                matrice[linie][coloana] = mesaj[index_mesaj];
                index_mesaj++;
            }
            else
            {
                break;
            }
        }
    }
    for (auto& linie : matrice)
    {
        for (auto& coloana : linie)
        {
            cout << setw(3) << coloana;
        }
        cout << '\n';
    }

    // === Matrice transpusă

    cout << "\n====== " << "Mesajul scris pe " << parola.size() << " coloane de cate " << numar_linii <<
        " caractere, cu coloanele in ordinea originala a parolei ordonate:\n";
    auto matrice_ordonata = vector<vector<char>>(numar_linii, vector<char>(parola.size(), LITERA_INLOCUITOARE));
    for (size_t index_index_parola = 0; index_index_parola < parola.size(); index_index_parola++)
    {
        for (size_t index_linie = 0; index_linie < numar_linii; index_linie++)
        {
            matrice_ordonata[index_linie][index_index_parola] =
                matrice[index_linie][indecsi_parola[index_index_parola] - 1];
        }
    }

    for (size_t index = 0; index < parola.size(); index++)
    {
        cout << setw(3) << indecsi_parola[index];
    }
    cout << '\n';
    for (auto& linie : matrice_ordonata)
    {
        for (auto& coloana : linie)
        {
            cout << setw(3) << coloana;
        }
        cout << '\n';
    }

    // === Mesaj descifrat

    auto mesaj_descifrat = string();
    for (auto& linie : matrice_ordonata)
    {
        for (auto& coloana : linie)
        {
            mesaj_descifrat += coloana;
        }
    }

    while (*(mesaj_descifrat.end() - 1) == LITERA_INLOCUITOARE)
    {
        mesaj_descifrat.pop_back();
    }

    AfisareText("Mesajul descifrat", mesaj_descifrat);
}

int main()
{
    auto optiune = 0;

    do
    {
        cout << "\n1. Cezar (Substitutie Monoalfabetica folosind un offset)";
        cout << "\n2. Metoda Substitutiei";
        cout << "\n3. Sisteme Polialfabetice (Vigenere)";
        cout << "\n4. Metoda Transpozitiei";
        cout << "\n0. Iesire";
        cout << "\n~> ";

        cin >> optiune;

        switch (optiune)
        {
        case 0:
            {
                cout << "\nLa revedere!\n";
            }
            break;
        case 1:
            {
                AlegereCifrareDescifrare(CifrareCezar, DescifrareCezar);
            }
            break;
        case 2:
            {
                AlegereCifrareDescifrare(CifrareSubstitutie, DescifrareSubstitutie);
            }
            break;
        case 3:
            {
                AlegereCifrareDescifrare(CifrareVigenere, DescifrareVigenere);
            }
            break;
        case 4:
            {
                AlegereCifrareDescifrare(CifrareTranspozitie, DescifrareTranspozitie);
            }
            break;
        default:
            {
                cout << "\nOptiune invalida.\n";
            }
        }
    }
    while (optiune != 0);

    return EXIT_SUCCESS;
}
