#include <iostream>
#include <cstdio>
#include <iomanip>
#include <stdio.h> //system
#include<unistd.h> //sleep

#include "../plansza/plansza.h"
#include "gui.h"

using std::cout;
using std::cin;
using std::get;
using std::endl;
using std::setw;
using std::setfill;
using std::right;
// using std::left;

/*
Podane pole:
 D  S   G
 A  1   1

 kody ascii:
 A-66   Z-90
 a-97   z-122
*/
char menu()
{
    char temp{};
    system("clear");
    cout<<right<<"STATKI"<<endl;
    cout<<"1. Rozpocznij gre"<<endl;
    cout<<"2. Ustawienia"<<endl;
    cout<<"3. Wyjdz"<<endl;
    cout<<"Wybierz opcje: ";
    cin>>temp;
    cin.get();
    cout<<endl;
    system("clear");

    return temp;
}

//funkcja majaca zwracac podane przez uzytkownika pole
int* zgadnij_pole(int szerokosc, int dlugosc, int glebokosc)
{
    int* pole = new int[2];

    // kolejka(2);
    cout<<"Naciscienie klawisza ESC zakonczy rozgrywke"<<endl;

    int i=0;
    bool warunek = false;
    while(warunek==false)   {
        pole[0] = 0;
        char temp{};
        cout << "Szerokosc: ";
        cin>>temp;
        cin.get(); // Czekamy na wciśnięcie Enter po wprowadzeniu szerokości
        if(temp>='A' && temp<='Z')
            pole[0] = abs(int('A' - temp));
        else if(temp>='a' && temp<='z')
            pole[0] = abs(int('a' - temp));
        else if (temp == 27) { //wyjscie z rozgrywki po nacisnieciu esc
            pole[0] = -10;
            pole[1] = -10;
            pole[2] = -10;

            return pole;
        }

        if(pole[0]>=0&&pole[0]<=szerokosc)
            warunek=true;
        else{
            cout<<"Blad, sprobuj ponownie"<<endl;
            warunek=false;
        }
        i++;
        if(i>10)
            break;
    }
    i=0;
    warunek=false;
    while(warunek==false)   {
        char temp{};
        cout << "Długosc: ";
        cin >> temp;
        cin.get(); // Czekamy na wciśnięcie Enter po wprowadzeniu długości
        if(temp>='0' && temp<='9'){
            pole[1] = temp-'0';

            if(pole[1]>=0&&pole[1]<=dlugosc)
                warunek=true;
            else{
                cout<<"Blad, sprobuj ponownie"<<endl;
            }
        }
        else if (temp == 27) { //wyjscie z rozgrywki po nacisnieciu esc
            pole[0] = -10;
            pole[1] = -10;
            pole[2] = -10;
            return pole;
        }
        else {
            cout<<"Blad, sprobuj ponownie"<<endl;
        }
        i++;
        if(i>10)
            break;
    }

    // Możesz również wczytać głębokość (G), jeśli jest potrzebna
    // cout << "Głębokość: ";
    // cin >> pole[2];
    pole[2] = 0;
    // cin.get(); // Czekamy na wciśnięcie Enter po wprowadzeniu głębokości
    // cout << endl;

    return pole;
}

void wypisz_wierszami(Plansza ***t, int G, int D, int S, bool czy_widoczne)
{
     if (t == nullptr) {
        cout << endl << "Błąd: pusta tablica" << endl<<endl;
        return;
    } cout<<endl<<endl;

    //k 0-8; j 0-6; G 0
    for(int i=0; i<G; i++)  {
        cout << "glebokosc " << i << ':'<< endl;

        //wypisz numer kolumny
        for(int k=0; k<=S; k++)  {
            if(k==0)
                cout << setw(4) << ' ';
            else
                cout << setw(3) << char('A'+k-1);
        } cout << endl;

        for(int j=0; j<D; j++)  {
            for(int k=0; k<S; k++)  {
                //wypisz numer wiersza
                if(k==0) cout << setw(3) << j << '.';
                if((t[i][j][k].czy_uzyte!=0) && (t[i][j][k].statek!=0))
                    cout << setw(3) << '*';

                //wypelnij plansze
                else if((t[i][j][k].czy_uzyte!=0) && (t[i][j][k].statek==0))
                    cout << setw(3) << '.';
                else{
                    if(czy_widoczne == true)
                        cout << setw(3) << t[i][j][k].statek;
                    else
                        cout << setw(3) << '#';
                }
            } cout << endl;
        }
    } cout << endl << endl;
}

void komunikat_przed(int poprzedni_ruch[], int uzytkownik)
{
    system("clear");
    // cout<<endl<<endl;
    cout << "Kolejka gracza: " << uzytkownik << endl;
    // cout << "Pozostale statki przeciwnika: " << pozostale_statki_aktywny_gracz << endl;

    static bool pierwsze_wykonanie = true;
    if (pierwsze_wykonanie == true) {
        // cout << endl << "Błąd: pusta tablica" << endl<<endl;
        pierwsze_wykonanie = false;
        return;
    }
    else{
        cout << "Ruch poprzedniego uzytkownika: " << /*poprzedni_ruch[0] */char('A'+char(poprzedni_ruch[0])) << poprzedni_ruch[1] << endl;
    }
}

//TO DO - przeniesc do gui  - funkcja wypisujaca komunikat po ruchu
int komunikat_po(Plansza*** plansza, int zgadywane_pole[], int pozostale_statki_aktywny_gracz)//g d s
{
    sleep(1);
    cout<<"Podane pole: "<<char('A'+char(zgadywane_pole[0]))<<zgadywane_pole[1]<<'\t';

    int trafione{};
    if(plansza[zgadywane_pole[2]][zgadywane_pole[1]][zgadywane_pole[0]].statek != 0){
        // int rozmiar_poczatkowy_statku = plansza[zgadywane_pole[0]][zgadywane_pole[1]][zgadywane_pole[2]].statek;
        trafione = 1;
        cout<<"TRAFIONE";
    }
    else{
        trafione = 0;
        cout<<"NIE TRAFIONE";
    }
    cout << "\nPozostale statki przeciwnika: " << pozostale_statki_aktywny_gracz << endl << endl;
    sleep(1);
    cout<<"Nacisnij przycisk Enter\n\n";
    cin.get();

    return trafione;
    /*
    mozliwe przypadki:
    funkcja zwraca wartosc ktora jest sprawdzana przez petle nadrzedna z warunkiem zmiany uzytkownika i petla powtarzajaca kolejke
        spudluje
        trafi(przydziel kolejke jeszcze raz)
        trafi caly statek??
    */
}

void wypisz_ruchy(Ruchy* w, int G, int D, int S)
{
    if(w == nullptr)
        return;

    using std::cout;
    using std::endl;

    while(w!=NULL)
    {
        cout << w->numer_ruchu << endl;

        for(int i=0; i<G; i++) {
            for(int j=0; j<D; j++) {
                for(int k=0; k<S; k++) {
                    // cout <<  w->plansza[i][j][k].statek;
                    if((w->plansza[i][j][k].czy_uzyte!=0) && (w->plansza[i][j][k].statek!=0)) {
                        cout << std::setw(3) << '*';
                    }
                    else if((w->plansza[i][j][k].czy_uzyte!=0) && (w->plansza[i][j][k].statek==0)) {
                        cout << std::setw(3) << '.';
                    }
                    else {
                        cout << std::setw(3) << w->plansza[i][j][k].statek;
                    }

                } cout << endl;
            }
        } cout << endl;

        cout << "Uzyte pole: " ;
        for(int i=0; i<3; i++)
            cout << w->uzyte_pole[i]<<'\t';
        cout << endl;

        w = w->nastepny;
    }
}

void komunikaty(int numer)
{
    switch (numer) {
        case 0:
            break;
        case 1:
            cout << "zatopiono!!" << endl << endl;
            break;
        case 2:
            cout << "Pole juz uzyte " << endl << endl;
            break;
        case 3:
            cout << "Blad przy losowaniu" << endl << endl;
            break;

        default:
            break;
    }
}
