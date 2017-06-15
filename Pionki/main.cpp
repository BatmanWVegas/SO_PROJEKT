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
    mutex macierz;

//Watki:
    void Producent(int ile);
    void Przetwarzacz(int ile);
    void Ncurses();

//Zasoby:
    queue <int> Kolejka1;
    queue <int> Kolejka2;
    int MACIERZ[M][N];
    bool run=true;

int main()
{
    srand(time(NULL));
    initscr();

    thread TProducenic(Producent, 20);
    TProducenic.join();

    thread TPrzetwarzacze = thread(Przetwarzacz, 20);
    TPrzetwarzacze.join();

    thread TNcurses = thread(Ncurses);
    TNcurses.join();

    return 0;
}


void Producent(int ile)
{
    do
    {

        //cout << "PRODUCENT"<<endl;
        bool stop = false;
        int X;
        int Y;
        //Losowanie X i Y
        do{


            X =(rand() % M);
            Y =(rand() % N);

            macierz.lock();
                if(MACIERZ[X][Y]==0)
                    {
                        stop = true;
                    }
            macierz.unlock();

        }while(stop == false);

        //cout << "Wylosowanie X: "<<X<<endl;
        //cout << "Wylosowanie Y: "<< Y<<endl;

        //Dodanie lokalizacji do kolejek
        mx.lock();
            Kolejka1.push(X);
            Kolejka2.push(Y);
        mx.unlock();

        ile--;
        //std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }while(ile);

}

void Przetwarzacz(int ile)
{
    do
    {
        int X;
        int Y;

        //cout << "Przetwarzacz: sprawdz kolejke:"<<endl;
        mx.lock();
        if(Kolejka1.empty() && Kolejka2.empty())
            {
                //cout << "Kolejka jest pusta." << endl;
            }
        else
            {
                X = Kolejka1.front();
                Y = Kolejka2.front();
                Kolejka1.pop();
                Kolejka2.pop();
                //cout << "POBRANO Z KOLEJKI X: "<<X<<endl;
                //cout << "POBRANO Z KOLEJKI Y: "<< Y<<endl;
            }
        mx.unlock();

        macierz.lock();
        MACIERZ[X][Y] = 1;
        for(int i=0; i<M; i++)
        {
            for(int j=0; j<N;j++)
            {
                if(MACIERZ[i][j] == 1 &&
                   MACIERZ[i][j] != MACIERZ[X][Y] &&
                   MACIERZ[i+1][j] ==0)
                {
                    MACIERZ[i+1][j] = MACIERZ[i][j];
                    MACIERZ[i][j] = 0;
                }
                else if(MACIERZ[i][j] ==1 &&
                        MACIERZ[i][j] != MACIERZ[X][Y] &&
                        MACIERZ[i-1][j] ==0)
                {
                    MACIERZ[i-1][j] = MACIERZ[i][j];
                    MACIERZ[i][j] =0;
                }
            }
        }
        macierz.unlock();
        //std::this_thread::sleep_for(std::chrono::milliseconds(200));
        ile--;
    }while(ile);

    run=false;


}

void Ncurses()
{


   do
    {   //M/N

        clear();
        macierz.lock();
        for(int i=0; i<M ;i++)
        {
            for(int j=0; j<N; j++)
            {
                if(MACIERZ[i][j]== 1)
                {
                    mvprintw( i+5, j+30, "X" );
                }
                else
                {
                    mvprintw( i+5, j+30, "O" );
                }
            }
        }
        macierz.unlock();
        refresh();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }while(run);
    getch();
    endwin();                  // zakończenie tryby curses
}



