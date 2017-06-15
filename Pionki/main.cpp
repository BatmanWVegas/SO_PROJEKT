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
    void Producent(int ile);
    void Przetwarzacz(int ile);
    void Ncurses();

//Zasoby:
    queue <int> Kolejka1;
    queue <int> Kolejka2;
    volatile char MACIERZ[M][N];
    bool run=true;

int main()
{
    srand(time(NULL));

        for(int i=0; i<M ;i++)
        {
            for(int j=0; j<N; j++)
            {
                MACIERZ[i][j] = '-';
            }
        }

    thread TProducenic(Producent, 20);
    thread TPrzetwarzacze = thread(Przetwarzacz, 20);
    thread TNcurses = thread(Ncurses);

    TPrzetwarzacze.join();
    TProducenic.join();
    TNcurses.join();


    return 0;
}


void Producent(int ile)
{
do{
        int X;
        int Y;

        X =(rand() % M);
        Y =(rand() % N);



        mx.lock();
        Kolejka1.push(X);
        Kolejka2.push(Y);
        mx.unlock();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ile --;
}while(ile);



}

void Przetwarzacz(int ile)
{

    do
    {

            if(Kolejka1.empty() && Kolejka2.empty())
            {
                continue;
            }
            else
            {

                MACIERZ[2][2] = 'X';
                for(int i=0; i<M ;i++)
                {
                    for(int j=0; j<N; j++)
                    {
                       if(MACIERZ[i][j]== 'X')
                       {
                            MACIERZ[i+1][j]== '-';
                       }
                    }
                }

                mx.lock();
                MACIERZ[Kolejka1.front()][Kolejka2.front()] = 'X';
                Kolejka1.pop();
                Kolejka2.pop();
                mx.unlock();
            }



        std::this_thread::sleep_for(std::chrono::seconds(1));
        ile--;
    }while(ile);

    run=false;


}

void Ncurses()
{
    initscr();
for (int i = 0; i < 10; i++)
{

    refresh();
    mx.lock();
        for(int i=0; i<M ;i++)
        {
            for(int j=0; j<N; j++)
            {
                    move(i,j);
                    printw("%c", MACIERZ[i][j]);

            }
        }
    mx.unlock();

        std::this_thread::sleep_for(std::chrono::seconds(1));

}
    getch();
    endwin();                  // zakończenie tryby curses
}



