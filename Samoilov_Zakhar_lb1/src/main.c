#include <stdio.h>
#define N 100

int index_f_zero(int size, int arr[]); // Поиск первого нуля в массиве чисел
int index_l_zero(int size, int arr[]); // Поиск последнего нуля в массиве чисел
int sum_between(int arr[], int first, int last); // Сумма чисел между первым и последним нулями
int sum_before_and_after(int arr[], int size, int first, int last); // Сумма чисел до первого и после последнего нолей

int main(){
	int par; // Передаваемое значение - параметр
	int index_first_zero, index_last_zero; // Индексы первого и последнего нулей
	char n; // Проверка пробела и enter
	int lenght = 0; // Количество чисел
	int numbers[N]; // Массив передаваемых чисел
	
	scanf("%d%c", &par, &n); // Ввод параметра
	while (n != '\n' &&  lenght < N){ // Ввод чисел в массив 
		scanf("%d%c", &numbers[lenght], &n);
		if (numbers[lenght] < 0) // Модуль отрицательного числа
			numbers[lenght] *= -1; 
		lenght++;
	};
	switch (par){
		case 0: 
			index_first_zero = index_f_zero(lenght, numbers);
			if (index_first_zero == lenght)
				printf("Данные некорректны");
			else printf ("%d", index_first_zero);
			break;
		case 1: 
			index_last_zero = index_l_zero(lenght, numbers);
			if (index_last_zero == lenght)
				printf("Данные некорректны");
			else printf ("%d", index_last_zero);
			break;
		case 2: index_first_zero = index_f_zero(lenght, numbers);
			index_last_zero = index_l_zero(lenght, numbers);
			if (index_first_zero == lenght || index_last_zero == lenght || index_first_zero == index_last_zero)
				printf("Данные некорректны");
			else printf("%d", sum_between(numbers, index_first_zero, index_last_zero));
			break;
		case 3: index_first_zero = index_f_zero(lenght, numbers);
			index_last_zero = index_l_zero(lenght, numbers);
			if (index_first_zero == lenght && index_last_zero == lenght)
				printf("Данные некорректны");
			else printf("%d", sum_before_and_after(numbers, lenght, index_first_zero, index_last_zero));
			break;
		default:
			printf("Данные некорректны");
			break;
	};
	return 0;
}

int index_f_zero(int size, int arr[]){
	int index = 0;
	for (index; index < size; index++){
		if (arr[index] == 0){
			return index;
		};
	}
	return size;
}
int index_l_zero(int size, int arr[]){
	int index = size - 1;
	for (index; index >= 0; index--){
		if (arr[index] == 0){
			return index;
		};
	};
	return size;
}
int sum_between(int arr[], int first, int last){
	int sum = 0;
	for (int index = first+1; index < last; index++)
		sum += arr[index];
	return sum;
}	
int sum_before_and_after(int arr[], int size, int first, int last){
	int sum = 0;
	if (first != size)
		for (int index = 0; index < first; index++)
			sum += arr[index];
	if (last != size)
		for (int index = last + 1; index < size; index++)
			sum += arr[index];
	return sum;
}
		
		
