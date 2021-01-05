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
#include <regex> //не обов'язкове
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
void sort_alphabetically(string*, int);
void swap_strings(string*, string*);
void output_rating(int, int, double* , string* , string*);

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	int athletes, judges;
	double passing_score;
	string* list = list_of_athletes(athletes, judges, passing_score); //функція початкового вводу, повертає кількість атлетів,суддей; прохідний бал та список атлетів
	double** marks = marks_of_athletes(list, athletes, judges); //записуємо оцінки атлетів в окремий двувимірний масив
	string* list_of_names = names_of_athletes(list, athletes, judges); //записуємо імена атлетів в окремий масив
	double** deleted_marks = creating_massive(athletes); //створюємо двувимірний масив для оцінок,які будуть видалені
	delete[]list;
	double** new_marks = processed_matrix_of_marks(marks, athletes, judges, deleted_marks); //створюємо двувимірний масив для оцінок без видалених
	output_changes(list_of_names, new_marks, deleted_marks, athletes, judges); //виводимо зміни, які стались після прибирання оцінок
	rating_of_athletes(list_of_names, marks, athletes, judges, passing_score); //виводимо рейтинги для атлетів з прохідним та непрохідним балами
	delete[]marks;
	delete[]new_marks;
	delete[]deleted_marks;
	delete[]list_of_names;
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
	string* list = new string[number_of_athletes]; //створюємо масив рядків
	for (int i = 0; i < number_of_athletes; i++) {
		getline(cin, list[i]);
		while (!check_input(list[i], number_of_judges)) { //не обов'язковий цикл
			getline(cin, list[i]);
		}
	}
	cout << endl;
	return list;
}

bool check_input(string line, int number_of_judges) { //не обов'язкова функція 
	regex regular(
		"([\\s(A-zА-яЁё)]+)"
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
	count = quantity_of_numbers(line, ' '); //обраховуємо кількість оцінок

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
			if (isdigit((unsigned char)line[i]) && line[i] != Delimiter && line[i + 1] == Delimiter) { //якщо поточний символ є числом і не є роздільником, а наступний роздільник
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
	string* pointer; //створюємо тимчасовий покажчик, в якому буде зберігатись список оцінок суддей
	for (int i = 0; i < athletes; i++) {
		pointer = line_breakdown(list[i], judges, ' '); //витягуємо з поточної строки масив оцінок
		for (int j = 0; j < judges; j++) {
			marks[i][j] = stof(pointer[j]);
		}
	}
	return marks;
}

string* names_of_athletes(string* list, int athletes, int judges) {
	string* list_of_names = new string[athletes];
	for (int i = 0; i < athletes; i++) {
		for (int j = 0; j < list[i].length(); j++) { //перебираємо всі символи строки
			if (list[i][j] == ':') break; //до двокрапки
			else list_of_names[i] += list[i][j]; //записуємо таким чином посимвольно імена атлетів
		}
	}
	return list_of_names;
}

string* line_breakdown(string line, int counter, char Delimiter) {
	string* str_arr = new string[counter];
	int end, //для знаходження наступної потрібної позиції
		pos = 0; //лічильник для пошуку позиції символу
	for (int i = 0; line[i] != ':' && i < line.length(); i++) { //початкова позиція розпочинається після двокрапки
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
				while (line[end] != Delimiter && end < line.length()) { //поки символ не дорівнює роздільнику і поки не закінчились символи в строці
					end++;
				}
				str_arr[number] = line.substr(pos, end - pos); //присвоюємо в масив строк витягнуте число
				pos = end; //розпочинаємо наступну позицію з роздільника
				break;
			}
		}
	}
	return str_arr;
}

double** processed_matrix_of_marks(double** marks, int athletes, int judges, double** deleted_marks) {
	int pos_min, //позиція першого мінімального числа
		pos_max; //позиція першого максимального числа
	double** processed_marks = new double* [athletes];
	for (int i = 0; i < athletes; i++) {
		processed_marks[i] = new double[judges - 2];
	}

	int correction = 0;
	for (int i = 0; i < athletes; i++) {
		searching(marks[i], pos_min, pos_max, judges); //пошук позицій
		for (int j = 0; j < judges; j++) {
			if (j != pos_min && j != pos_max) { //якщо поточне j не є мінімальною і максимальною позицією
				processed_marks[i][j - correction] = marks[i][j]; //вписуємо ці оцінки в новий масив без видалених оцінок
			}
			else {
				deleted_marks[i][correction] = marks[i][j]; //записуємо сюди видалені оцінки
				correction++;
			}
		}
		correction = 0;
	}
	return processed_marks;
}

void searching(double* numeric_array, int& pos_min, int& pos_max, int size) {
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
	double* array_of_total = new double[athletes]; //створюємо масив для загальних балів
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

	sorting_by_score(names, array_of_total, athletes); //викликаємо функцію, яка сортує атлетів та їх бали за рейтингом
	int correction = 0;
	string* list_of_losers = new string[number_of_losers]; //створюємо масив з іменами, тих, хто не пройшов
	for (int i = 0; i < athletes; i++) {
		if (array_of_total[i] < passing_score) {
			list_of_losers[i - correction] = names[i];
		}
		else correction++;
	}
	string* list_of_winners = new string[number_of_winners]; //створюємо масив з іменами, тих, хто пройшов
	correction = 0;
	for (int i = 0; i < athletes; i++) {
		if (array_of_total[i] >= passing_score) {
			list_of_winners[i - correction] = names[i];
		}
		else correction++;
	}
	output_rating(number_of_winners, number_of_losers, array_of_total, list_of_winners, list_of_losers);
	delete[]list_of_winners;
	delete[]list_of_losers;
	delete[]array_of_total;
}

void sorting_by_score(string* names, double* total, int athletes) {
	for (int i = 0; i < athletes - 1; i++) {
		for (int j = 0; j < athletes - 1 - i; j++) {
			if (total[j] < total[j + 1]) {
				swap(names[j], names[j + 1]);
				swap(total[j], total[j + 1]);
			}
		}
	}
}

void sort_alphabetically(string* names, int size) {
	int min;
	for (int i = 0; i < size - 1; i++) {                      //проходы алгоритма
		for (int j = 0; j < size - i - 1; j++) {                  //цикл для перебора элементов массива
			min = (names[j].size() < names[j + 1].size()) ? names[j].size() : names[j + 1].size();
			for (int k = 0; k <= min; k++) {
				if (k < min) {
					if (tolower(names[j][k]) > tolower(names[j + 1][k])) {
						swap_strings(&names[j], &names[j + 1]);
						break;
					}
					else if (tolower(names[j][k]) < tolower(names[j + 1][k]))
						break;
				}
				else if (k == min && names[j].size() > names[j + 1].size())                //сравниваем кол-во символов в строках
					swap_strings(&names[j], &names[j + 1]);
			}
		}
	}
}

void swap_strings(string* s1, string* s2) {
	string temp;
	temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}

void output_rating(int number_of_winners, int number_of_losers, double* array_of_total,string* list_of_winners,string* list_of_losers) {
	int i = 0;
	if (number_of_winners != 0) {
		cout << "Список спортсменов, которые смогли:" << endl;
		for (i; i < number_of_winners; i++) {
			cout << list_of_winners[i] << " - " << "общий бал: " << array_of_total[i] << endl;
		}
	}
	else cout << "отсутствует" << endl;
	cout << endl;
	
	if (number_of_losers != 0) {
		cout << "Список спортсменов, которые не смогли((:" << endl;
		for (int j = 0; j < number_of_losers; j++) {
			cout << list_of_losers[j] << " - " << "общий бал: " << array_of_total[i] << endl;
			i++;
		}
		cout << endl;
		sort_alphabetically(list_of_losers, number_of_losers);
		cout << "Отсортированый список недопущенных:" << endl;
		for (int j = 0; j < number_of_losers; j++) {
			cout << list_of_losers[j] << endl;
			i++;
		}
	}
	else cout << "Списки спортсменов,которые не смогли и отсортированый - пустые." << endl;
	cout << endl;
}