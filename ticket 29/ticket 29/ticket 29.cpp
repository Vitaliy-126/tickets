/*билет 29
есть какое - то количество спортсменов и им выставляют оценки какое - то количество судей.
1. написать ввод спортсменов и их оценок, чтобы результат для одного спортсмена выглядел вот так(иван: 7, 4 4, 6 6, 7 8, 4).
если на словах, то у нас есть имя спортсмена + двоеточие, а после через пробел вбиты его полученные оценки.
результатом может быть как и двумерный массив, так и, наверное, отдельный класс.в задании это, насколько я понял, не уточнялось.
2. у каждого спортсмена убрать самую высокую и самую низкую оценки,
а от оставшихся посчитать среднее арифметическое.его же записать к каждому спортсмену в конце через " = *какое-то число*".
выглядеть это будет так : (иван: 7, 4 6, 7 = 7, 05)
3. записать ранее удалённые нами оценки каждого из спортсменов в скобках перед знаком "=" в прошлой записи.пример : (иван : 7, 4 6, 7 (4, 6 8, 4) = 7, 05)
дальше я могу помнить не очень точно
4. составить рейтинг спортсменов по их среднему баллу от самого высокого к самому низкому.
учитывать только тех спортсменов, чей балл является проходным(проходной балл задаётся изначально)
5. составить такой же рейтинг для тех, кто не получил проходной балл.
6 - отсортировать недопущеных по алфавиту по имени
желаю невероятной удачи тому, кто получит эту хуйню*/

#include <iostream>
#include <string>
#include <regex>
#include <Windows.h>

using namespace std;

string* list_of_athletes(int&, int&, double&);
bool check_input(string, int);
int quantity_of_numbers(string, char);
double** marks_of_athletes(string*, int, int);
string* names_of_athletes(string*, int, int);
string* line_breakdown(string, int, char);
double** processed_matrix_of_marks(double**, int, int, double**);
void searching(double*, int&, int&, int);
double** creating_massive(int);
void output_changes(string*, double**, double**, int, int);
void rating_of_athletes(string*, double**, int, int, double);
void sorting_by_score(string*, double*, int);

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int athletes, judges;
	double passing_score;
	string* list = list_of_athletes(athletes, judges, passing_score);
	double** marks = marks_of_athletes(list, athletes, judges);
	string* list_of_names = names_of_athletes(list, athletes, judges);
	double** deleted_marks = creating_massive(athletes);
	delete[]list;
	double** new_marks = processed_matrix_of_marks(marks, athletes, judges, deleted_marks);
	output_changes(list_of_names, new_marks, deleted_marks, athletes, judges);
	rating_of_athletes(list_of_names, marks, athletes, judges, passing_score);
	delete[]marks;
	delete[]new_marks;
	delete[]deleted_marks;
	system("pause");
	return 0;
}

string* list_of_athletes(int& number_of_athletes, int& number_of_judges, double& passing_score) {
	cout << "Введите количество спортсменов: ";
	cin >> number_of_athletes;
	cout << "Введите количество судей: ";
	cin >> number_of_judges;
	cout << "Введите проходной бал: ";
	cin >> passing_score;
	cout << "Перечислите спортсменов в формате(Иван: 5 6 7 8...)" << endl;
	cin.ignore(8192, '\n');
	string* list = new string[number_of_athletes];
	for (int i = 0; i < number_of_athletes; i++) {
		getline(cin, list[i]);
		while (!check_input(list[i], number_of_judges)) {
			getline(cin, list[i]);
		}
	}
	cout << endl;
	return list;
}

bool check_input(string line, int number_of_judges) {
	regex regular(
		"([\\(A-zА-яЁё)]+)"
		"(\\:)"
		"([\\.\\s\\d]*)"
	);
	while ((regex_match(line.c_str(), regular)) == 0)
	{
		cout << "Проверьте ввод(Строка должна быть задана у соответственном формате)." << "\n";
		cout << "----------------------------------------------------------------" << "\n";
		cout << "Перевведите строку:" << "\n";
		return false;
	}

	int count;
	count = quantity_of_numbers(line, ' ');

	if (count != number_of_judges) {
		cout << "Количество оценок должно соответствовать количеству суддей" << endl;
		cout << "---------------------------------------------------------" << "\n";
		cout << "Перевведите строку:" << "\n";
		return false;
	}
	return true;
}

int quantity_of_numbers(string line, char Delimiter) {
	int counter = 0; //лічильник кількості чисел в рядку
	for (int i = 0; i < line.length(); i++) {
		if (i != line.length() - 1) //всі, крім останнього символу, тому що рядки не закінчуються на роздільник
		{
			if (isdigit((unsigned char)line[i]) && line[i] != Delimiter && line[i + 1] == Delimiter) { //якщо поточний символ не є роздільником, а наступний є
				counter++;
			}
		}
		else if (i == line.size() - 1 && line[i] != Delimiter) counter++; //якщо це останній символ і це не роздільник
	}
	return counter;
}

double** marks_of_athletes(string* list, int athletes, int judges) {
	double** marks = new double* [athletes];
	for (int i = 0; i < athletes; i++) {
		marks[i] = new double[judges];
	}
	string* pointer;
	for (int i = 0; i < athletes; i++) {
		pointer = line_breakdown(list[i], judges, ' ');
		for (int j = 0; j < judges; j++) {
			marks[i][j] = stof(pointer[j]);
		}
	}
	return marks;
}

string* names_of_athletes(string* list, int athletes, int judges) {
	string* list_of_names = new string[athletes];
	for (int i = 0; i < athletes; i++) {
		for (int j = 0; j < list[i].length(); j++) {
			if (list[i][j] == ':') break;
			else list_of_names[i] += list[i][j];
		}
	}
	return list_of_names;
}

string* line_breakdown(string line, int counter, char Delimiter) {
	string* str_arr = new string[counter];
	int end, //для знаходження наступної потрібної позиції
		pos = 0; //лічильник для пошуку позиції символу
	for (int i = 0; line[i] != ':' && i < line.length(); i++) {
		pos++;
	}
	pos++;
	for (int number = 0; number < counter; number++) {
		while (pos < line.length()) {
			if (line[pos] == Delimiter) { //якщо змінна рівна роздільнику то здвигаємо позицію
				pos++;
			}
			else { //інакше шукаємо, скільки символів до наступного роздільника 
				end = pos;
				while (line[end] != Delimiter && end < line.length()) {
					end++;
				}
				str_arr[number] = line.substr(pos, end - pos);
				pos = end;
				break;
			}
		}
	}
	return str_arr;
}

double** processed_matrix_of_marks(double** marks, int athletes, int judges, double** deleted_marks) {
	int pos_min,
		pos_max;
	double** processed_marks = new double* [athletes];
	for (int i = 0; i < athletes; i++) {
		processed_marks[i] = new double[judges - 2];
	}

	int correction = 0;
	for (int i = 0; i < athletes; i++) {
		searching(marks[i], pos_min, pos_max, judges);
		for (int j = 0; j < judges; j++) {
			if (j != pos_min && j != pos_max) {
				processed_marks[i][j - correction] = marks[i][j];
			}
			else {
				deleted_marks[i][correction] = marks[i][j];
				correction++;
			}
		}
		correction = 0;
	}
	return processed_marks;
}

void searching(double* numeric_array, int& pos_min, int& pos_max, int size) {
	int post_min = 0;
	int post_max = 0;
	double min = numeric_array[0],
		max = numeric_array[0];
	for (int i = 0; i < size; i++) {
		if (min > numeric_array[i]) {
			min = numeric_array[i];
		}
		if (max < numeric_array[i]) {
			max = numeric_array[i];
		}
	}
	for (int i = 0; i < size; i++) {
		if (numeric_array[i] == min) {
			pos_min = i;
			break;
		}
	}
	for (int i = 0; i < size; i++) {
		if (numeric_array[i] == max) {
			pos_max = i;
			break;
		}
	}
}

double** creating_massive(int size) {
	double** deleted_marks = new double* [size];
	for (int i = 0; i < size; i++) {
		deleted_marks[i] = new double[2];
	}
	return deleted_marks;
}

void output_changes(string* names, double** new_marks, double** deleted_marks, int athletes, int judges) {
	double value = 0;
	for (int m = 0; m < 2; m++) {
		for (int i = 0; i < athletes; i++) {
			for (int j = 0; j < judges - 2; j++) {
				value += new_marks[i][j];
			}
			value /= ((double)judges - 2);
			cout << names[i] << ": ";
			for (int k = 0; k < judges - 2; k++) {
				cout << new_marks[i][k] << " ";
			}
			if (m) {
				cout << "( ";
				for (int k = 0; k < 2; k++) {
					cout << deleted_marks[i][k] << " ";
				}
				cout << ") ";
			}
			cout << "= " << value << endl;
			value = 0;
		}
		cout << endl;
	}

}

void rating_of_athletes(string* names, double** marks, int athletes, int judges, double passing_score) {
	double total_score = 0;
	double* array_of_total = new double[athletes];
	int number_of_losers = 0;
	int number_of_winners;
	for (int i = 0; i < athletes; i++) {
		for (int j = 0; j < judges; j++) {
			total_score += marks[i][j];
		}
		array_of_total[i] = total_score / judges;
		if (array_of_total[i] < passing_score) {
			number_of_losers++;
		}
		total_score = 0;
	}
	number_of_winners = athletes - number_of_losers;
	sorting_by_score(names, array_of_total, athletes);
	int correction = 0;
	string* list_of_losers = new string[number_of_losers];
	for (int i = 0; i < athletes; i++) {
		if (array_of_total[i] < passing_score) {
			list_of_losers[i - correction] = names[i];
		}
		else correction++;
	}
	string* list_of_winners = new string[number_of_winners];
	correction = 0;
	for (int i = 0; i < athletes; i++) {
		if (array_of_total[i] >= passing_score) {
			list_of_winners[i - correction] = names[i];
		}
		else correction++;
	}
	cout << "Список спортсменов, которые смогли:" << endl;
	int i = 0;
	for (i; i < number_of_winners; i++) {
		cout << list_of_winners[i] << " - " << "общий бал: " << array_of_total[i] << endl;
	}
	cout << "Список спортсменов, которые не смогли((:" << endl;
	for (int j = 0; j < number_of_losers; j++) {
		cout << list_of_losers[j] << " - " << "общий бал: " << array_of_total[i] << endl;
		i++;
	}
	delete[]list_of_winners;
}

void sorting_by_score(string* names, double* total, int athletes) {
	for (int i = 0; i < athletes - 1; i++) {
		for (int j = 0; j < athletes - 1 - i; j++) {
			if (total[j] < total[j + 1])
				swap(names[j], names[j + 1]);
		}
	}
	for (int i = 0; i < athletes - 1; i++) {
		for (int j = 0; j < athletes - 1 - i; j++) {
			if (total[j] < total[j + 1])
				swap(total[j], total[j + 1]);
		}
	}
}
