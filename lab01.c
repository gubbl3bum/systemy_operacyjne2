// ZADANIE 1.1.2 
// odczytanie i wypisanie wartości zmienno-przecinkową typu double.
// #include <stdio.h>

// int main() {
//     double d;

//     printf("Enter double value: ");
//     scanf("%lf", &d);
//     printf("double value = %lf\n", d);

//     return 0;
// }
// ZADANIE 1.1.3
// odczytanie i wypisanie cały napis "ala ma kota" przy pomocy funkcji scanf
//#include <stdio.h>

// int main() {
//     char str[20];

//     printf("Enter string: ");
//     scanf("%19[^\n]", str);  // czyta do nowej linii albo maksymalnej długości
//     printf("string value = %s\n", str);

//     return 0;
// }

// ZADANIE 1.1.4
// odczytanie i wypisanie całego napisu "ala ma kota" przy pomocy funkcji fgets
// #include <stdio.h>

// int main() {
//     char str[20];

//     printf("Enter string: ");
//     fgets(str, sizeof(str), stdin);
//     printf("string value = %s\n", str);

//     return 0;
// }

// ZADANIE 1.2
// sprawdzenie, czy dany strumień wejściowy jest pusty
// #include <stdio.h>
//     int main() {    
//         if (feof(stdin)) {        
//             printf("Strumien wejsciowy jest pusty.\n");    
//         } else {        
//             printf("Strumien wejsciowy nie jest pusty.\n");    
//             }    
//     return 0;
//     }

// ZADANIE 1.3.3
// funkcja fibo3 - metoda programowania dynamicznego z ramką dwuzębną
// #include <stdio.h>

// int fibo3(int n) {
//     if (n == 0 || n == 1) return 1;
//     int r0 = 1, r1 = 1, pom;
//     for (int i = 2; i <= n; ++i) {
//         pom = r0;
//         r0 = r1;
//         r1 = r0 + pom;
//     }
//     return r1;
// }

// int main() {
//     int n = 4;
//     printf("fibo3(%d) = %d\n", n, fibo3(n));
//     return 0;
// }

// ile razy należy przesunąć ramkę w prawo, aby wyznaczyć wartość n-tego wyrazu ciągu fibonacciego w funkcji fibo3 dla n >=2
// ramkę przesuwa się n-1 razy (dla n = 0 i n = 1 wartość wynosi 1), dla większych wartości obliczenia rozpoczynają się od indeksu 2

// analiza wywołania fibo3(4)
// początkowe wartości r0 = 1; r1 = 1
// krok 1 (i = 2):
//      pom = r0 -> 1
//      r0 = r1 -> 1
//      r1 = r0 + pom -> 1 + 1 = 2
// krok 2 (i = 3)
//      pom = r0 -> 1
//      r0 = r1 -> 2
//      r1 = r0 + pom -> 2 + 1 = 3
// krok 3 (i = 4)
//      pom = r0 -> 2
//      r0 = r1 -> 3
//      r1 = r0 + pom -> 3 + 2 = 5

// graf obliczeń:
//   r0 = 1, r1 = 1
//          |
//         i = 2
//          ↓
//       pom = r0 -> 1
//       r0 = r1 -> 1
//       r1 = r0 + pom -> 2
//          |
//         i = 3
//          ↓
//       pom = r0 -> 1
//       r0 = r1 -> 2
//       r1 = r0 + pom -> 3
//          |
//         i = 4
//          ↓
//       pom = r0 -> 2
//       r0 = r1 -> 3
//       r1 = r0 + pom -> 5

// która funkcja ma mniejszą złożoność obliczeniową, fibo2 czy fibo3
// obie funkcje mają złożoność czasową O(n) -> iterują liniowo po wartościach od 2 do n
// funkcja fibo3 jest efektywniejsza pod względem zużycia pamięci niż fibo2 -> fibo2 wykorzystuje trzy zmienne, a fibo3 używa tylko dwóch oraz pomocniczej zmiennej, zużywając mniej pamięci.

// ZADANIE 1.3.4
// cztery inne funkcje wyliczające rekurencyjnie wartość ciągu

// klasyczna rekurencja 
// int fib_classic(int n) {
//     if (n == 0) return 1;
//     if (n == 1) return 1;
//     return fib_classic(n - 1) + fib_classic(n - 2);
// }

// rekurencja z memoizacją
// int fib_memo(int n, int memo[]) {
//     if (n == 0) return 1;
//     if (n == 1) return 1;
//     if (memo[n] == 0) {
//         memo[n] = fib_memo(n - 1, memo) + fib_memo(n - 2, memo);
//     }
//     return memo[n];
// }

// rekurencja z akumulacją
// int fib_tail(int n, int a, int b) {
//     if (n == 0) return a;
//     if (n == 1) return b;
//     return fib_tail(n - 1, b, a + b);
// }

// rekurencja z użyciem macierzy
// void multiply(int F[2][2], int M[2][2]) {
//     int x = F[0][0] * M[0][0] + F[0][1] * M[1][0];
//     int y = F[0][0] * M[0][1] + F[0][1] * M[1][1];
//     int z = F[1][0] * M[0][0] + F[1][1] * M[1][0];
//     int w = F[1][0] * M[0][1] + F[1][1] * M[1][1];
//     F[0][0] = x;
//     F[0][1] = y;
//     F[1][0] = z;
//     F[1][1] = w;
// }

// void power(int F[2][2], int n) {
//     if (n == 0 || n == 1)
//         return;
//     int M[2][2] = {{1, 1}, {1, 0}};
//     power(F, n / 2);
//     multiply(F, F);
//     if (n % 2 != 0)
//         multiply(F, M);
// }

// int fib_matrix(int n) {
//     int F[2][2] = {{1, 1}, {1, 0}};
//     if (n == 0)
//         return 0;
//     power(F, n - 1);
//     return F[0][0];
// }

// ZADANIE 1.4
// program wyliczający wartość ciągu {an} przy pomocy trzech funkcji.
// Ciąg {an} zdefiniowany jest jako:

// a(0) = 1
// a(1) = 4
// a(n) = 2 * a(n-1) + 0.5 * a(n-2) dla n >= 2

// #include <stdio.h>

// double sequence(int n) {
//     if (n == 0) return 1;
//     if (n == 1) return 4;
//     return 2 * sequence(n - 1) + 0.5 * sequence(n - 2);
// }

// int main() {
//     for (int i = 0; i < 10; i++) {
//         printf("a(%d) = %.2f\n", i, sequence(i));
//     }
//     return 0;
// }

// ZADANIE 1.4.1
// Funkcja a1 - metoda dziel i zwyciężaj. 
// - dokonaj analizy wywołania a1(4).
// - narysuj drzewo wywołań dla a1(4).

// funkcja oparta na metodzie dziel i zwyciężaj - każdy wraz ciągu jest obliczany rekurencyjnie na podstawie dwóch poprzednich wyrazów

// analiza wywołania a1(4)
// a1(4) wymaga obliczenia a1(3) i a1(2)
// a1(3) wymaga obliczenia a1(2) i a1(1)
// a1(2) wymaga obliczenia a1(1) i a1(0)

//                 a1(4)
//               /     \
//            a1(3)    a1(2)
//           /    \    /   \
//       a1(2)  a1(1) a1(1) a1(0)
//      /   \
//   a1(1) a1(0)

// ZADANIE 1.5
// funkcja f(n) wyliczająca liczbę cyfr liczby całkowitej n (wykorzystanie funkcji log10(x))
// #include <stdio.h>
// #include <math.h>

// int liczba_cyfr(int n) {
//     if (n == 0) return 1;
//     return (int)log10(abs(n)) + 1;
// }

// int main() {
//     int n;
//     printf("Podaj liczbe: ");
//     scanf("%d", &n);
//     printf("Liczba cyfr: %d\n", liczba_cyfr(n));
//     return 0;
// }