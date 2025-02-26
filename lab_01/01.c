#include <stdio.h>

void drukuj(int tablica[], int liczba_elementow) {
  for (int i = 0; i < liczba_elementow; i++) {
    if (tablica[i] > 10 && tablica[i] < 100) {
      printf("%d\t", tablica[i]);
    }
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

  drukuj(liczby, i);
  return 0;
}
