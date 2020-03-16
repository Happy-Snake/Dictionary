#include <string.h>
#include <sstream>
#include <iostream>
#include "../Headers/pugixml.hpp"
#include "../Headers/translator.h"

using namespace std;
using namespace pugi;

Translator::Translator(Translator::Mode choice)		//Конструктор открывает файлы словарей
{
	if (choice == Translator::RusToLat)
	{
		doc.load_file("..\\Dictionary\\Files\\Russian_dict.xml");
		doc1.load_file("..\\Dictionary\\Files\\Latin_dict.xml");
	}
	if (choice == Translator::LatToRus)
	{
		doc.load_file("..\\Dictionary\\Files\\Latin_dict.xml");
		doc1.load_file("..\\Dictionary\\Files\\Russian_dict.xml");
	}
};

string Translator::Translate(const string &word, xml_node &word_node)			//Поиск слова в словаре, ф-я возвращает строку с переводом (слова записаны через запятую) и изменяет принимаемый node -> word_node на выходе указывает на искомую запись
{
	xml_node begin = doc.child("begin");
	string output;
	for (word_node = begin.first_child(); word_node; word_node = word_node.next_sibling())				//Проход по тегам p
	{
		xml_node strong = word_node.first_child();					//Тег слова
		if (word_node.child_value(strong.name()) == word)
		{
			for (xml_node em = strong.next_sibling(); em; em = em.next_sibling())
				output += string(em.child_value()) + ", ";							//Формирование строки вывода
			output = output.substr(0, output.size() - 2);							//Удаление последней запятой
			return output;
		}
	}
	return output;
};

void Translator::Show_page(xml_node start_node)				//Вывод страницы
{
	int consr = 1;
	if (start_node == NULL)
		start_node = doc.child("begin").first_child();
	for (int i = 0; i < 20 && start_node; i++)			//Проход 20-ти тегам начиная со start_node (или пока не дойдет до конца файла)
	{
		xml_node strong = start_node.first_child();
		cout << strong.child_value() << "  --  ";
		for (xml_node em = strong.next_sibling(); em; em = em.next_sibling())
		{
			cout << em.child_value() << ", ";
		}
		cout << "\b \b";
		cout << "\b \b";
		cout << endl;
		start_node = start_node.next_sibling();
	}
	return;
};

void Translator::Next_page(xml_node &input_node)			//Переход на 20 тегов назад
{
	if (input_node == NULL)
		input_node = doc.child("begin").first_child();
	for (int i = 0; i < 20 && input_node; i++)
		input_node = input_node.next_sibling();
	return;
};

void Translator::Prev_page(xml_node &input_node)			//Переход на 20 тегов вперед
{
	if (input_node == NULL)
		input_node = doc.child("begin").first_child();
	for (int i = 0; i < 20 && input_node; i++)
		input_node = input_node.previous_sibling();
	return;
};

void Translator::Syn_word(const std::string &synonym, xml_node &person, const Translator::Mode &mode)				//Сохранение синонима в файлы словарей
{
	person.append_child("em1").text().set(synonym.c_str());					//Добаление нового тега в запись слова на языке_1 и запись в этот тег синонима на языке_2 (в словаре_1)
	string word = person.child_value(person.first_child().name());			//Запоминание этого слова на языке_1
	xml_node begin = doc1.child("begin");
	if (xml_node other_node = Find_word(synonym, begin))					//Если слово-синоним на языке_2 уже есть в словаре_2,
		other_node.append_child("em1").text().set(word.c_str());			//то к этой записи добавляем тег с запомненным словом на языке_1
	else
	{
		begin = doc1.child("begin").append_child("p1");						//Если нет, то в словаре_2 создаем новый тег,
		begin.append_child("strong1").text().set(synonym.c_str());			//куда записываем сначала слово-синоним на языке_2,
		begin.append_child("em1").text().set(word.c_str());					//а потом записываем запомненное слово на языке_1
	}
	if (Save_all(mode))
		cout << "Синоним успешно добавлен!" << endl;
	else
		cout << "Произошла непредвиденная ошибка" << endl;
	system("pause");
};

xml_node Translator::Find_word(const string &word, xml_node begin)			//Поиск слова в словаре, начиная с тега begin одного мз словарей
{
	xml_node output_node(NULL);
	for (xml_node p = begin.first_child(); p; p = p.next_sibling())
			if (p.child_value(p.first_child().name()) == word)
				return output_node = p;
	return output_node;
};

void Translator::Add_word(const string &word, const string &other_word, const Translator::Mode &mode)				//Добавление нового слова и его перевода в оба словаря
{
	xml_node begin = doc.child("begin").append_child("p1");
	begin.append_child("strong1").text().set(word.c_str());		//Создание новой записи в словаре_1 и запись туда сначала слова_1, а потом его перевода
	begin.append_child("em1").text().set(other_word.c_str());
	begin = doc1.child("begin");
	if (xml_node other_node = Find_word(other_word, begin))				//Если перевод уже есть в словаре_2,
		other_node.append_child("em1").text().set(word.c_str());		//то добавляем к этой записи слово_1
	else
	{																			//Если перевода нет в словаре_2,
		begin = doc1.child("begin").append_child("p1");							//то добавляем новую запись в словарь_2,
		begin.append_child("strong1").text().set(other_word.c_str());			//куда записываем сначала  перевод,
		begin.append_child("em1").text().set(word.c_str());						//а потом слово_1
	}
	if (Save_all(mode))
		cout << "Слово успешно добавлено!" << endl;
	else
		cout << "Произошла непредвиденная ошибка" << endl;
	system("pause");
};

bool Translator::Save_all(const Translator::Mode &mode)				//Сохранение изменений в файлы
{
	xml_node decl = doc.prepend_child(node_declaration);			//Подгововка файлов (установка версии, кодировки)
	decl.append_attribute("version") = "1.0";
	decl.append_attribute("encoding") = "windows-1251";
	xml_node decl1 = doc1.prepend_child(node_declaration);
	decl1.append_attribute("version") = "1.0";
	decl1.append_attribute("encoding") = "windows-1251";
	string path, other_path;
	if (mode == RusToLat)
	{
		path = "..\\Dictionary\\Files\\Russian_dict.xml";
		other_path = "..\\Dictionary\\Files\\Latin_dict.xml";
	}
	else
	{
		path = "..\\Dictionary\\Files\\Latin_dict.xml";
		other_path = "..\\Dictionary\\Files\\Russian_dict.xml";
	}
	if (doc.save_file(path.c_str()) && doc1.save_file(other_path.c_str()))		//Сохранение
		return 1;
	else
		return 0;
};

void Translator::Words_learned(xml_node &person, const Translator::Mode &path)
{
	xml_node begin = doc1.child("begin");
	string node_name;
	node_name = person.name();
	if (node_name == "learned")
	{
		cout << "Слово уже изучено" << endl;
		system("pause");
	}
	else
	{
		person.set_name("learned");
		if (Save_all(path))
			cout << "Слово добавлено в изученные" << endl;
		else
			cout << "Произошла непредвиденная ошибка" << endl;
		system("pause");
	}
};

void Translator::Show_learned()
{

	xml_node first_node = doc.child("begin").first_child();
	string node_n;
	cout << "Сохраненные слова:" << endl;
	while (first_node)
	{
		node_n = first_node.name();
		if (node_n == "learned")
		{
			xml_node strong = first_node.first_child();
			cout << strong.child_value() << "  --  ";
			for (xml_node em = strong.next_sibling(); em; em = em.next_sibling())

				cout << em.child_value() << ", ";

			cout << "\b \b";
			cout << "\b \b";
			cout << endl;

		}
		first_node = first_node.next_sibling();
	}
	system("pause");
	return;
};