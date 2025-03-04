#include <stdio.h>

void drukuj_alt(int *tablica, int liczba_elementow) {
  int *i = tablica + liczba_elementow;
  while (tablica < i) {
    if (*tablica > 10 && *tablica < 100) {
      printf("%d\t", *tablica);
    }
    tablica++;
  }
  printf("\n");
}

int main() {
  int liczby[50];
  printf("Oczekiwanie na liczby:\n");

  int i = 0;
  while (i < 50) {
    int liczba;
    scanf("%d", &liczba);
    if (liczba == 0) {
      break;
    }
    liczby[i] = liczba;
    i++;
  }

  drukuj_alt(liczby, i);
  return 0;
}
