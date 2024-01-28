#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>

// Нахождение r1 (Равна значению функции)
double findr1(double x, double y) {
	return sqrt(x) * y;
}

// Нахождение r2 и r3
double findr23(double h, double x, double y, double r) {
	x = x + (h / 2);
	y = y + (r / 2);
	return findr1(x, y);
}

// Нахождение r4
double findr4(double h, double x, double y, double r3) {
	x = x + h;
	y = y + r3;
	return findr1(x, y);
}

// 1. Метод Рунге-Кутты
void RungeKutty() {
	// исходные параметры
	int a = 0; // начало отрезка
	int b = 4; // конец отрезка
	double x = 0; //x0
	double y = 1; //y0
	double h = 0.2; // шаг
	int n = (b - a) / h; // количество шагов
	double r1, r2, r3, r4; // доп. значения

	// цикл для решения ду методом Рунге-Кутты
	for (int i = 0; i <= n; i++) {
		//Вывод найденных значений
		printf("%3d\t x = %.2f\t y = %.5f\n", i, x, y);
		//Нахождение дополнительных значений
		r1 = h * findr1(x, y);
		r2 = h * findr23(h, x, y, r1);
		r3 = h * findr23(h, x, y, r2);
		r4 = h * findr4(h, x, y, r3);
		//Переход на следующее значение функции и следующее значение аргумента
		y = y + (r1 + (2 * r2) + (2 * r3) + r4) / 6;
		x = x + h;
	}
}

// Нахождения интеграла методом Парабол
double Integral(double* x, double* y, double a, double b, double h, int n, int root) {
	double SumEven = 0; // сумма четных
	double SumOdd = 0; // сумма нечетных
	double Integ = 0; // интеграл

	for (int i = 0; i <= n; i++) {
		//Вывод найденных значений
		printf("%3d\t x = %.5f\t y = %.5f\n", i, x[i], y[i]);
		//Нахождение дополнительных значений
		if (y[i] == 0)
		{
			printf("Корень x = %.5f\t y = %.5f\n", x[i], y[i]);
			root = 1;
			break;
		}
		if (i != 0 && i != n)
		{
			if (i % 2 == 0)
				SumEven = SumEven + y[i];
			else
				SumOdd = SumOdd + y[i];
		}
		//Переход на следующее значение функции и следующее значение аргумента
		x[i + 1] = x[i] + h;
		y[i + 1] = sin(2.0 * x[i + 1] + 1.0);
	}
	Integ = (b - a) / (6 * (n / 2)) * ((y[0] + y[n]) + 2.0 * SumEven + 4.0 * SumOdd);
	return Integ;
}

// 2.Метод Парабол
void Parabol() {
	double pi = 3.14159265359;

	// исходные параметры
	double a = 0.0; // начало отрезка
	double b = pi / 3.0; // конец отрезка
	int n = 10; // количество шагов, (n/2) - число разбиений
	double h = (b - a) / n; // шаг

	// начальное значение переменных
	double* x = calloc(n + 1, sizeof(double)); x[0] = a; //x0
	double* y = calloc(n + 1, sizeof(double)); y[0] = sin(2.0 * x[0] + 1.0); //y0
	double integral1, integral2;
	// Доп. значения
	int root = 0; //Если корень найден, то root = 1
	double pogresh;

	//Ищем интегралы
	integral1 = Integral(x, y, a, b, h, n, root);
	if (root == 0)
	{
		printf("Интеграл 1 = %.10f\n", integral1);
		//Для оценки погрешности увеличиваем количество шагов(n) в 2 раза:
		n = n * 2;
		h = (b - a) / n; //пересчитываем шаг
		x = realloc(x, (n + 1) * sizeof(double)); x[0] = a; //перезписываем память и x0
		y = realloc(y, (n + 1) * sizeof(double)); y[0] = sin(2.0 * x[0] + 1.0); //перезписываем память и y0
		integral2 = Integral(x, y, a, b, h, n, root);

		if (root == 0)
		{
			printf("Интеграл 2 = %.10f\n", integral2);
			//Ищем погрешность
			pogresh = (integral2 - integral1) / 15;
			printf("\nПогрешность = %.10f\n", fabs(pogresh)); //fabs выводит моудуль, т.к. abs округляет до 0.
		}
	}
}

// f(x)
double function(double x) {
	return ((3 * pow(x, 2)) - 2 * x - 1);
}

// f'(x)
double derivative(double x) {
	return (6 * x - 2);
}

// 3.Метод Хорд
void Chord() {
	// Исходные параметры
	double a = -10; // начало отрезка
	double b = 10; // конец отрезка
	double h = 1; // шаг
	double* arrX = calloc(0, sizeof(double)); //Подозрения на отрезки изоляции
	double* otrX = calloc(0, sizeof(double)); //Отрезки изоляции

	// Доп. параметры
	int k = 0; // кол-во подозрений на отрезки
	int otr = 0; // кол-во отрезков
	int flag = 0; //Если знак производной поменяется, то flag = 1
	double j = 0; //X на отрезке
	int i = 0; //текущий шаг подозрений
	int t = 0; //текущий шаг отрезков
	int ri = 0; //индекс корней
	int root = 0; //Подсчет корней
	double* x = calloc(1, sizeof(double)); //искомый корень
	double* rootX = calloc(1, sizeof(double)); //все найденные корни
	double Fa = 0; //функция от начала отрезка изоляции
	double Fb = 0; //функция от конца отрезка изоляции

	// Находим отрезки изоляции
	for (j = a; j < b; j = j + h) {
		if (function(j) * function(j + h) < 0)
		{
			k++;
			arrX = realloc(arrX, (2 * k + 1) * sizeof(double)); // увеличиваем длину массива
			//arrX[i] подозрительные на отрезки изоляции
			arrX[i] = j; i++;
			arrX[i] = j + h; i++;
		}
		if (function(j) == 0)
		{
			root++;
			rootX = realloc(rootX, root * sizeof(double)); //массив для корней
			rootX[ri] = j; ri++;
		}
	}
	if (k == 0 && root == 0)
		printf("Кореней нет");
	else
	{
		// Проверка, что производная сохраняет знак на подозрительных отрезках изоляции
		double h1 = h / 10; //новый шаг для проверки
		for (i = 0; i < 2 * k; i += 2) // перебор подозрений
		{
			flag = 0;
			printf("Подозрительный на отрезок изоляции: [%.2f; %.2f]\n", arrX[i], arrX[i + 1]);
			for (j = arrX[i]; j < arrX[i + 1] - h1; j += h1) {
				if (derivative(j) * derivative(j + h1) < 0) {
					flag = 1; //Знак не сохраняется
					break;
				}
			}
			if (flag == 0)
			{
				printf("[%.2f,%.2f] - Отрезок изоляции\n", arrX[i], arrX[i + 1]);
				otr++;
				otrX = realloc(otrX, (2 * otr) * sizeof(double));
				otrX[t] = arrX[i]; t++;
				otrX[t] = arrX[i + 1]; t++;
			}
		}

		//Применение метода хорд
		for (int t = 0; t < 2 * otr; t += 2)  // перебор отрезков
		{
			a = otrX[t];
			b = otrX[t + 1];
			Fb = function(b);
			for (i = 0; root < root + 1 * otr; i++) {
				x = realloc(x, (i + 1) * sizeof(double));
				Fa = function(a);
				x[i] = a - ((Fa * (b - a)) / (Fb - Fa));
				a = x[i];
				if ((round(x[i - 1] * 100000000)) == (round(x[i] * 100000000)))
				{
					root++;
					rootX = realloc(rootX, root * sizeof(double)); //массив для корней
					rootX[ri] = x[i]; ri++;
					break;
				}
			}
		}

		printf("\nОтвет:\n");
		// Результат c точностью k = %.7f
		for (int i = 0; i < root; i++)  //массив корней (ответ)
		{
			printf("Корень %d = %.7f\n", i + 1, rootX[i]);
		}
	}
}

// функция вычисления определителя матрицы
double determinant(double** matrix, int n) {
	double det = 0;
	int znak = 1;

	double** minor = calloc((n - 1), sizeof(double*));
	for (int i = 0; i < (n - 1); i++) {
		minor[i] = calloc((n - 1), sizeof(double));
		for (int j = 0; j < (n - 1); j++) {
			minor[i][j] = 0;
		}
	}

	if (n == 1)
		det = matrix[0][0];
	else if (n == 2) //для матрицы 2x2
		det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	else {
		for (int k = 0; k < n; k++) { // рекурсивно вычисляем определитель для каждого элемента первой строки
			int i = 0;
			int j = 0;
			if (k % 2 == 0)
				znak = 1;
			else
				znak = -1;
			for (int row = 1; row < n; row++) {
				for (int col = 0; col < n; col++) {
					if (col != k) {
						minor[i][j++] = matrix[row][col];
						if (j == n - 1) {
							j = 0;
							i++;
						}
					}
				}
			}
			det += matrix[0][k] * determinant(minor, n - 1) * znak; // рекурсивный вызов для каждого элемента первой строки
		}
	}

	return det;
}

// 4. Интерполирование функции
void Interpolation() {
	// исходные параметры
	int degree; // степень искомого многочлена
	int n; // найти количество введенных узлов
	printf("Введите количество узлов: ");
	scanf("%d", &n);
	double* arrX = calloc(n, sizeof(double));
	double* arrY = calloc(n, sizeof(double));
	double mainDet; // определитель матрицы
	double* val = calloc(n, sizeof(double)); //Искомые значения

	//Ввод узлов
	for (int i = 0; i < n; i++) {
		printf("x[%d] = ", i);
		scanf("%lf", &arrX[i]);
		printf("y[%d] = ", i);
		scanf("%lf", &arrY[i]);
	}

	// Заполнение двумерного массива, возводим соответствующие узлы в степени
	double** matrix = calloc(n, sizeof(double*));
	for (int i = 0; i < n; i++) {
		printf("\n");
		degree = n - 1;
		matrix[i] = calloc(n, sizeof(double));
		for (int j = 0; j < n; j++, degree--) {
			matrix[i][j] = pow(arrX[i], degree);
			printf("%f\t", matrix[i][j]);
		}
	}
	printf("\n");

	// Решить СЛУ методом Крамера
	mainDet = determinant(matrix, n); // Нахождение определителя матрицы
	printf("Определитель матрицы: %.10f\n", mainDet);
	for (int column = 0; column < n; column++) {
		double** matrixDet = calloc(n, sizeof(double*));
		for (int i = 0; i < n; i++) {
			matrixDet[i] = calloc(n, sizeof(double));
			for (int j = 0; j < n; j++) {
				if (j == column) {
					matrixDet[i][j] = arrY[i];
				}
				else
					matrixDet[i][j] = matrix[i][j];
			}
		}
		val[column] = (determinant(matrixDet, n)) / mainDet; //Искомые значения
	}

	//Вывод ответа
	printf("\ny = ");
	for (int i = 0, j = n - 1; i < n, j >= 0; i++, j--) {
		printf("%.2f", val[i]);
		if (j > 0)
		{
			printf("x");
			if (j > 1)
				printf("^%d", j);
			if (val[i + 1] >= 0)
				printf("+");
		}
	}
}

int main() {
	system("chcp 1251>nul"); //для работы с русским языком
	int n, end;
	do
	{
		printf("\nВыберите нужную тему:");
		printf("\n1. Метод Рунге-Кутты");
		printf("\n2. Метод Парабол");
		printf("\n3. Метод Хорд");
		printf("\n4. Интерполирование функции\n");
		scanf("%d", &n);
		switch (n)
		{
		case 1: RungeKutty(); break;
		case 2: Parabol(); break;
		case 3: Chord(); break;
		case 4: Interpolation(); break;
		default: printf("Такой темы нет\n"); break;
		}
		printf("\nВы хотите выбрать другую тему? (1 - да, 0 - нет).\n");
		scanf("%d", &end);
	} while (end > 0);
	return 0;
}
