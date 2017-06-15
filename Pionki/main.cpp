/*Wymagania na 3
--------------

 * przynajmniej 3 rozne zasoby
 * przynajmniej 2 rozne watki przetwarzajace po kilka roznych zasobow
 * watek wyswietlajacy stan zasobow (przynajmniej raz na sekunde) przy uzyciu biblioteki ncurses
 * niewielkie problemy lub brak problemow zwiazanych z synchronizacja
 * uzycie podstawowych metod synchronizacji oraz C++11 (`std::thread`, `std::mutex`, `std::lock_guard`, `std::sleep_for`, `std::chrono`)
 */

 /*Pionki na macierzy (projekt na 3)
---------------------------------

Zasoby:
 * `KOLEJKA1` - pierwsza kolejka liczb
 * `KOLEJKA2` - druga kolejka liczb
 * `MACIERZ` - macierz rozmiaru MxN przechowujaca pionki.

Watki:
 * `PRODUCENT` - watek ktory w jednej iteracji dodaje losowa liczbe z zakresu (0..M-1) do kolejki `ZRODLO1` oraz inna losowa liczbe z zakresu (0..N-1) do kolejki `ZRODLO2`
 * `PRZETWARZACZ` - watek ktory w jednej iteracji pobiera (o ile istnieje) jedna wspolrzedna z kolejki `ZRODLO1` oraz druga wspolrzedna z kolejki `ZRODLO2`, generuje pionek i wstawia go do macierzy `MACIERZ`. Dodatkowo, watek ten w kazdej iteracji przesuwa kazdy pionek umiejscowiony na planszy o jedna pozycje w bok
 * `NCURSES` - watek ktory co sekunde uzyskuje dostep do macierzy `MACIERZ` i wyswietla na ekranie jej stan (rozmieszczenie pionkow) przy uzyciu biblioteki ncurses
*/

#include "header.h"

using namespace std;
//MUTEX
    mutex mx;

//Watki:
    void Producent();
    void Przetwarzacz();
    void Ncurses();

//Zasoby:
    queue <int> Kolejka1;
    queue <int> Kolejka2;
    int MACIERZ[M][N];


int main()
{
    int MACIERZ[M][N];
    vector <thread> V_Producenic;
    vector <thread> V_Przetwarzacze;
    vector <thread> V_Ncurses;
    srand( time(NULL));

    cout << "Start watku 'Producent'"<<endl<<endl;
    cout << "Start watku 'Przetwarzacz'"<<endl<<endl;
    cout << "Start watku 'NCurses'"<<endl<<endl;

    for(int i=0; i<10; i++)
    {
        V_Producenic.push_back(thread(Producent));
        V_Przetwarzacze.push_back(thread(Przetwarzacz));
        V_Ncurses.push_back(thread(Ncurses));

    }

    for (auto& thread : V_Producenic)
    {
        thread.join();
    }
    for (auto& thread : V_Przetwarzacze)
    {
        thread.join();
    }
    for (auto& thread : V_Ncurses)
    {
        thread.join();
    }
    cout << "done!\n";

    return 0;
}


void Producent()
{
    cout << "PRODUCENT"<<endl;

    //Losowanie X i Y
    int X =(rand() % M);
    int Y =(rand() % N);

    cout << "Wylosowanie X: "<<X<<endl;
    cout << "Wylosowanie Y: "<< Y<<endl;

    //Dodanie lokalizacji do kolejek
    mx.lock();
    Kolejka1.push(X);
    Kolejka2.push(Y);
    mx.unlock();

}

void Przetwarzacz()
{
    cout << "Przetwarzacz: sprawdz kolejke:"<<endl;
    mx.lock();
    if(Kolejka1.empty() && Kolejka2.empty())
    {
        cout << "Kolejka jest pusta." << endl;
    }
    else
    {
        int X = Kolejka1.front();
        int Y = Kolejka2.front();
        Kolejka1.pop();
        Kolejka2.pop();
        cout << "Wylosowanie X: "<<X<<endl;
        cout << "Wylosowanie Y: "<< Y<<endl;
    }
    mx.unlock();
}

void Ncurses()
{
    cout << "NCURSES"<<endl;
    this_thread::sleep_for(chrono::seconds(1));
}



