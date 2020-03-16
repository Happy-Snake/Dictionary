#include <iostream>
#include <iomanip>
#include <conio.h>
#include <windows.h>
#include "../Headers/pugixml.hpp"
#include "../Headers/translator.h"
#include "../Headers/menu.h"

using namespace std;
using namespace pugi;

void Control_keys(int &code, int &key)
{
	code = _getch();
	if (code == 224)
	{
		code = _getch();
		if (code == 80) key++;
		if (code == 72) key--;
	}
};

void drawMenu(const char **menuItems, int sz, int current)
{
	for (int i = 0; i < sz; ++i)
		cout << setw(3) << (current == i ? "->" : "") << menuItems[i] << endl;
};

int main_menu()			//Выбор режима работы
{
	int key = 0;
	int code;
	const char *items[] = { "Режим словаря", "Режим переводчика",
							"Режим пользовательского словаря", "Закрыть" };
	do
	{
		system("cls");
		cout << "Выберите режим работы:" << endl;
		key = (key + 4) % 4;

		drawMenu(items, 4, key);

		Control_keys(code, key);
	} while (code != 13);
	system("cls");
	return key;
};

int page_of_words_menu(xml_node node, Translator dict)		//Меню словаря
{
	int key = 0;
	int code;
	const char *items[] = { "Перелистнуть вперед", "Перелистнуть назад",
							"К главному меню" };
	do
	{
		system("cls");
		dict.Show_page(node);
		key = (key + 3) % 3;

		drawMenu(items, 3, key);

		Control_keys(code, key);
	} while (code != 13);
	system("cls");
	return key;
};

Translator::Mode translation_direction()			//Выбор языка для работы
{
	int key = 0;
	int code;
	const char *items[] = { "С русского на латинский", "С латинского на русский",
							"К главному меню" };
	do
	{
		system("cls");
		cout << "Выберите направление перевода:" << endl;
		key = (key + 3) % 3;

		drawMenu(items, 3, key);

		Control_keys(code, key);
	} while (code != 13);
	system("cls");
	if (key == 0)
		return Translator::RusToLat;
	if (key == 1)
		return Translator::LatToRus;
	if (key == 2)
		return Translator::Error;
};

int syn_choice(std::string word, std::string translate) //Вывод страниицы слов до тех пор, пока не будет выбрано дальнейшее действие 
{
	int key = 0;
	int code;
	const char *items[] = { "Добавить вариант перевода", "Сохранить в пользовательский словарь",
							"К главному меню" };
	do
	{
		system("cls");
		cout << "Слово: " << word << endl
			<< "Перевод: " << translate << endl;
		cout << "Выберите действие:" << endl;
		key = (key + 3) % 3;

		drawMenu(items, 3, key);

		Control_keys(code, key);
	} while (code != 13);
	system("cls");
	return key;
};

bool new_word()				//Вывод сообщения о ненайденном слове до тех пор, пока не будет выбрано дальнейшее действие
{
	int key = 0;
	int code;
	const char *items[] = { "Добавить это слово в словарь", "К главному меню" };
	do
	{
		system("cls");
		cout << "Введенного слова нет в словаре" << endl;
		key = (key + 2) % 2;

		drawMenu(items, 2, key);

		Control_keys(code, key);
	} while (code != 13);
	system("cls");
	return !key;
};