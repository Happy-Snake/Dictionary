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

int main_menu()			//����� ������ ������
{
	int key = 0;
	int code;
	const char *items[] = { "����� �������", "����� �����������",
							"����� ����������������� �������", "�������" };
	do
	{
		system("cls");
		cout << "�������� ����� ������:" << endl;
		key = (key + 4) % 4;

		drawMenu(items, 4, key);

		Control_keys(code, key);
	} while (code != 13);
	system("cls");
	return key;
};

int page_of_words_menu(xml_node node, Translator dict)		//���� �������
{
	int key = 0;
	int code;
	const char *items[] = { "������������ ������", "������������ �����",
							"� �������� ����" };
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

Translator::Mode translation_direction()			//����� ����� ��� ������
{
	int key = 0;
	int code;
	const char *items[] = { "� �������� �� ���������", "� ���������� �� �������",
							"� �������� ����" };
	do
	{
		system("cls");
		cout << "�������� ����������� ��������:" << endl;
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

int syn_choice(std::string word, std::string translate) //����� ��������� ���� �� ��� ���, ���� �� ����� ������� ���������� �������� 
{
	int key = 0;
	int code;
	const char *items[] = { "�������� ������� ��������", "��������� � ���������������� �������",
							"� �������� ����" };
	do
	{
		system("cls");
		cout << "�����: " << word << endl
			<< "�������: " << translate << endl;
		cout << "�������� ��������:" << endl;
		key = (key + 3) % 3;

		drawMenu(items, 3, key);

		Control_keys(code, key);
	} while (code != 13);
	system("cls");
	return key;
};

bool new_word()				//����� ��������� � ����������� ����� �� ��� ���, ���� �� ����� ������� ���������� ��������
{
	int key = 0;
	int code;
	const char *items[] = { "�������� ��� ����� � �������", "� �������� ����" };
	do
	{
		system("cls");
		cout << "���������� ����� ��� � �������" << endl;
		key = (key + 2) % 2;

		drawMenu(items, 2, key);

		Control_keys(code, key);
	} while (code != 13);
	system("cls");
	return !key;
};