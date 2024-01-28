#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>

// ���������� r1 (����� �������� �������)
double findr1(double x, double y) {
	return sqrt(x) * y;
}

// ���������� r2 � r3
double findr23(double h, double x, double y, double r) {
	x = x + (h / 2);
	y = y + (r / 2);
	return findr1(x, y);
}

// ���������� r4
double findr4(double h, double x, double y, double r3) {
	x = x + h;
	y = y + r3;
	return findr1(x, y);
}

// 1. ����� �����-�����
void RungeKutty() {
	// �������� ���������
	int a = 0; // ������ �������
	int b = 4; // ����� �������
	double x = 0; //x0
	double y = 1; //y0
	double h = 0.2; // ���
	int n = (b - a) / h; // ���������� �����
	double r1, r2, r3, r4; // ���. ��������

	// ���� ��� ������� �� ������� �����-�����
	for (int i = 0; i <= n; i++) {
		//����� ��������� ��������
		printf("%3d\t x = %.2f\t y = %.5f\n", i, x, y);
		//���������� �������������� ��������
		r1 = h * findr1(x, y);
		r2 = h * findr23(h, x, y, r1);
		r3 = h * findr23(h, x, y, r2);
		r4 = h * findr4(h, x, y, r3);
		//������� �� ��������� �������� ������� � ��������� �������� ���������
		y = y + (r1 + (2 * r2) + (2 * r3) + r4) / 6;
		x = x + h;
	}
}

// ���������� ��������� ������� �������
double Integral(double* x, double* y, double a, double b, double h, int n, int root) {
	double SumEven = 0; // ����� ������
	double SumOdd = 0; // ����� ��������
	double Integ = 0; // ��������

	for (int i = 0; i <= n; i++) {
		//����� ��������� ��������
		printf("%3d\t x = %.5f\t y = %.5f\n", i, x[i], y[i]);
		//���������� �������������� ��������
		if (y[i] == 0)
		{
			printf("������ x = %.5f\t y = %.5f\n", x[i], y[i]);
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
		//������� �� ��������� �������� ������� � ��������� �������� ���������
		x[i + 1] = x[i] + h;
		y[i + 1] = sin(2.0 * x[i + 1] + 1.0);
	}
	Integ = (b - a) / (6 * (n / 2)) * ((y[0] + y[n]) + 2.0 * SumEven + 4.0 * SumOdd);
	return Integ;
}

// 2.����� �������
void Parabol() {
	double pi = 3.14159265359;

	// �������� ���������
	double a = 0.0; // ������ �������
	double b = pi / 3.0; // ����� �������
	int n = 10; // ���������� �����, (n/2) - ����� ���������
	double h = (b - a) / n; // ���

	// ��������� �������� ����������
	double* x = calloc(n + 1, sizeof(double)); x[0] = a; //x0
	double* y = calloc(n + 1, sizeof(double)); y[0] = sin(2.0 * x[0] + 1.0); //y0
	double integral1, integral2;
	// ���. ��������
	int root = 0; //���� ������ ������, �� root = 1
	double pogresh;

	//���� ���������
	integral1 = Integral(x, y, a, b, h, n, root);
	if (root == 0)
	{
		printf("�������� 1 = %.10f\n", integral1);
		//��� ������ ����������� ����������� ���������� �����(n) � 2 ����:
		n = n * 2;
		h = (b - a) / n; //������������� ���
		x = realloc(x, (n + 1) * sizeof(double)); x[0] = a; //������������� ������ � x0
		y = realloc(y, (n + 1) * sizeof(double)); y[0] = sin(2.0 * x[0] + 1.0); //������������� ������ � y0
		integral2 = Integral(x, y, a, b, h, n, root);

		if (root == 0)
		{
			printf("�������� 2 = %.10f\n", integral2);
			//���� �����������
			pogresh = (integral2 - integral1) / 15;
			printf("\n����������� = %.10f\n", fabs(pogresh)); //fabs ������� �������, �.�. abs ��������� �� 0.
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

// 3.����� ����
void Chord() {
	// �������� ���������
	double a = -10; // ������ �������
	double b = 10; // ����� �������
	double h = 1; // ���
	double* arrX = calloc(0, sizeof(double)); //���������� �� ������� ��������
	double* otrX = calloc(0, sizeof(double)); //������� ��������

	// ���. ���������
	int k = 0; // ���-�� ���������� �� �������
	int otr = 0; // ���-�� ��������
	int flag = 0; //���� ���� ����������� ����������, �� flag = 1
	double j = 0; //X �� �������
	int i = 0; //������� ��� ����������
	int t = 0; //������� ��� ��������
	int ri = 0; //������ ������
	int root = 0; //������� ������
	double* x = calloc(1, sizeof(double)); //������� ������
	double* rootX = calloc(1, sizeof(double)); //��� ��������� �����
	double Fa = 0; //������� �� ������ ������� ��������
	double Fb = 0; //������� �� ����� ������� ��������

	// ������� ������� ��������
	for (j = a; j < b; j = j + h) {
		if (function(j) * function(j + h) < 0)
		{
			k++;
			arrX = realloc(arrX, (2 * k + 1) * sizeof(double)); // ����������� ����� �������
			//arrX[i] �������������� �� ������� ��������
			arrX[i] = j; i++;
			arrX[i] = j + h; i++;
		}
		if (function(j) == 0)
		{
			root++;
			rootX = realloc(rootX, root * sizeof(double)); //������ ��� ������
			rootX[ri] = j; ri++;
		}
	}
	if (k == 0 && root == 0)
		printf("������� ���");
	else
	{
		// ��������, ��� ����������� ��������� ���� �� �������������� �������� ��������
		double h1 = h / 10; //����� ��� ��� ��������
		for (i = 0; i < 2 * k; i += 2) // ������� ����������
		{
			flag = 0;
			printf("�������������� �� ������� ��������: [%.2f; %.2f]\n", arrX[i], arrX[i + 1]);
			for (j = arrX[i]; j < arrX[i + 1] - h1; j += h1) {
				if (derivative(j) * derivative(j + h1) < 0) {
					flag = 1; //���� �� �����������
					break;
				}
			}
			if (flag == 0)
			{
				printf("[%.2f,%.2f] - ������� ��������\n", arrX[i], arrX[i + 1]);
				otr++;
				otrX = realloc(otrX, (2 * otr) * sizeof(double));
				otrX[t] = arrX[i]; t++;
				otrX[t] = arrX[i + 1]; t++;
			}
		}

		//���������� ������ ����
		for (int t = 0; t < 2 * otr; t += 2)  // ������� ��������
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
					rootX = realloc(rootX, root * sizeof(double)); //������ ��� ������
					rootX[ri] = x[i]; ri++;
					break;
				}
			}
		}

		printf("\n�����:\n");
		// ��������� c ��������� k = %.7f
		for (int i = 0; i < root; i++)  //������ ������ (�����)
		{
			printf("������ %d = %.7f\n", i + 1, rootX[i]);
		}
	}
}

// ������� ���������� ������������ �������
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
	else if (n == 2) //��� ������� 2x2
		det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	else {
		for (int k = 0; k < n; k++) { // ���������� ��������� ������������ ��� ������� �������� ������ ������
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
			det += matrix[0][k] * determinant(minor, n - 1) * znak; // ����������� ����� ��� ������� �������� ������ ������
		}
	}

	return det;
}

// 4. ���������������� �������
void Interpolation() {
	// �������� ���������
	int degree; // ������� �������� ����������
	int n; // ����� ���������� ��������� �����
	printf("������� ���������� �����: ");
	scanf("%d", &n);
	double* arrX = calloc(n, sizeof(double));
	double* arrY = calloc(n, sizeof(double));
	double mainDet; // ������������ �������
	double* val = calloc(n, sizeof(double)); //������� ��������

	//���� �����
	for (int i = 0; i < n; i++) {
		printf("x[%d] = ", i);
		scanf("%lf", &arrX[i]);
		printf("y[%d] = ", i);
		scanf("%lf", &arrY[i]);
	}

	// ���������� ���������� �������, �������� ��������������� ���� � �������
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

	// ������ ��� ������� �������
	mainDet = determinant(matrix, n); // ���������� ������������ �������
	printf("������������ �������: %.10f\n", mainDet);
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
		val[column] = (determinant(matrixDet, n)) / mainDet; //������� ��������
	}

	//����� ������
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
	system("chcp 1251>nul"); //��� ������ � ������� ������
	int n, end;
	do
	{
		printf("\n�������� ������ ����:");
		printf("\n1. ����� �����-�����");
		printf("\n2. ����� �������");
		printf("\n3. ����� ����");
		printf("\n4. ���������������� �������\n");
		scanf("%d", &n);
		switch (n)
		{
		case 1: RungeKutty(); break;
		case 2: Parabol(); break;
		case 3: Chord(); break;
		case 4: Interpolation(); break;
		default: printf("����� ���� ���\n"); break;
		}
		printf("\n�� ������ ������� ������ ����? (1 - ��, 0 - ���).\n");
		scanf("%d", &end);
	} while (end > 0);
	return 0;
}
