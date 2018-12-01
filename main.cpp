#include <cstdio>
#include <locale.h>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <cmath>
#include <limits>

using namespace std;

vector<string> string_split(string s, const char delimiter);
void Meth(double **input, double eps, int iter, size_t size);
void LoadFile();
string ln;
int DimIn;
string path = "Path to files";
int DimmArr = 10;
double temp;
string buffer;
ifstream file;
double **input;
int ind = 0;
vector<string> delimStr;

string Name;
size_t size = 0;

int main(int argc, char ** argv)
{
	setlocale(LC_ALL, "Russian");
	double temp;
	ln = "Ru";
	Name = "FileNAme.txt";
	LoadFile();

	Meth(input, 3, 30, size);
	return 0;
    return 0;
}

void LoadFile()
{
	cout << Name << " " << DimIn << endl;
	cout << "Loading file...";
	size = 0;

	file.open(path + ln + "\\" + Name + ".txt");
	while (file)
	{
		getline(file, buffer);
		if (buffer != "") size++;
	}
	file.close();

	input = new double*[size];
	ind = 0;
	file.open(path + ln + "\\" + Name + ".txt");
	while (file)
	{
		getline(file, buffer);
		if (buffer != "")
		{
			delimStr = string_split(buffer, ' ');
			input[ind] = new double[DimmArr];

			for (size_t j = 0;j < DimmArr;j++)
			{
				temp = (stod(delimStr[j]) - minmax[j][0]) / (minmax[j][1] - minmax[j][0]);
				if (temp < 0)	temp = 0;
				if (temp > 1)	temp = 1;
				input[ind][j] = temp;
			}
			ind++;
		}
	}
	file.close();
	cout << "Complete" << endl;
}
void Meth(double **input, double eps, int iter, size_t size)
{

	/*DimIn = 3;
	size = 20;*/
	size_t l1 = size - DimIn;
	if (l1 <= 0) return;
	int luft = 10;
	int i = 0;
	int j = 0;
	int k = 0;
	int k1 = 0;
	int ii = 0;

	double eps1 = (eps * eps)*DimIn*DimIn *  DimmArr * DimmArr;
	double temp2 = 0;
	double temp1 = 0;
	int temp3 = 0;
	int temp4 = 0;
	double **distances = new double*[l1];
	int **neighbor = new int*[l1];

	for (i = 0; i < l1; i++)
	{
		distances[i] = new double[DimmArr];
		neighbor[i] = new int[DimmArr];
		for (k = 0; k < DimmArr; k++)
		{
			distances[i][k] = numeric_limits<double>::max();
			neighbor[i][k] = -1;
		}
	}

	double **rez = new double*[l1];

	for (i = 10; i < l1; i++)
	{
		rez = new double*[size - i + 1];
		for (j = size - 1;j >= i;j--)
		{
			//находим растояния между компонентами Z-веткоров
			temp2 = 0;
			rez[j - i] = new double[DimmArr];
			for (k = 0; k < DimmArr; k++)
			{
				temp1 = (input[j][k] - input[j - i][k]);
				temp2 += temp1 * temp1;
				rez[j - i][k] = temp2;
			}
			//результат записываем в массив 
			//это диагональ матрицы растояний
		}

		for (j = 0;j < l1 - i;j++)
		{
			for (ii = 0; ii < DimmArr; ii++)
			{
				//суммируем результаты растояний для DimIn компонент
				for (k = 1;k < DimIn;k++)
					rez[j][ii] += rez[j + k][ii];
				//проверяем полученный результат растояния от i+j до j вектора
				//являеться ли он соседом 
				temp1 = rez[j][ii];
				if (temp1 > 0 && temp1 < eps1)
				{
					if (temp1 < distances[j][ii])
					{
						distances[j][ii] = temp1;
						neighbor[j][ii] = j + i;
					}
					//так как матрица симметрична, аналогичное растояние и между i+j и j
					if (temp1 < distances[j + i][ii])
					{
						distances[j + i][ii] = temp1;
						neighbor[j + i][ii] = j;
					}
				}
			}
		}

		for (j = size - i - 1;j >= 0;j--)
			delete rez[j];
		delete rez;

		if (i % 30 == 0) cout << "\rFinding neighbors..." << (double)i / l1 << "% completed                     ";
	}
	cout << "\rLoading file...Complete" << endl;
	cout << "Finding values...";

	double div = 0;//среднее логарифмов растояний между векторов на it
	int c = 0;//количество удачно подсчитаных растояний
	string Result;

	for (int it = 0; it <= iter; it++)
	{
		div = 0; c = 0;
		for (k1 = 0; k1 < DimmArr; k1++)
		{
			for (i = 0; i < l1 - iter; i++)
			{
				//Определяем есть ли сосет и не выходим ли мы за границы ряда
				if (neighbor[i][k1] != -1 && neighbor[i][k1] + it < l1)
				{
					temp2 = 0;
					//ПОдсчет растояния между векторами 
					for (k = 0; k < DimIn; k++)//учитывая пространство вложения
					{
						for (ii = 0; ii < k1 + 1; ii++)
						{
							temp1 = (input[i + it + k][ii] - input[neighbor[i][k1] + it + k][ii]);
							temp2 += temp1 * temp1;
						}
					}
					temp2 = sqrt(temp2);

					if (!std::isinf(temp2) && temp2 != 0)//Проверяем удачно ли подсчитано растояние
					{
						div += log(temp2);
						c++;
					}
				}
			}
			div = div / c;
			Result += Name + " " + ln + " " + to_string(DimmArr) + " " + to_string(it) + " " + to_string(div) + "\n";
		}
	}
	cout << "Complete" << endl;
	ofstream ofs;
	ofs.open(path + Name + ".txt", ofstream::out);
	if (ofs.is_open())
		ofs << Result;

	ofs.close();
	cout << "Task Complete" << endl;
}
vector<string> string_split(string s, const char delimiter)
{
	size_t start = 0;
	size_t end = s.find_first_of(delimiter);
	vector<string> output;
	while (end <= string::npos)
	{
		output.emplace_back(s.substr(start, end - start));

		if (end == string::npos)
			break;
		start = end + 1;
		end = s.find_first_of(delimiter, start);
	}
	return output;
}