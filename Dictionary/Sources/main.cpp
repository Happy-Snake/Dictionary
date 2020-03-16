#include <windows.h>
#include <string.h>
#include <iostream>
#include "../Headers/pugixml.hpp"
#include "../Headers/translator.h"
#include "../Headers/menu.h"

using namespace std;
using namespace pugi;

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	short answer;
	do
	{
		answer = main_menu();			//Выбор режима работы
		switch (answer)
		{
		case 0:			//Режим словаря
		{
			xml_node start_node(NULL);
			Translator::Mode choice = translation_direction();		//Выбор языка словаря
			if (choice != Translator::Error)
			{
				Translator dict(choice);
				int cont;
				do
				{
					cont = page_of_words_menu(start_node, choice);			//Вывод страниицы слов до тех пор, пока не будет выбрано дальнейшее действие
					switch (cont)
					{
					case 0:
						dict.Next_page(start_node);
						break;
					case 1:
						dict.Prev_page(start_node);
						break;
					}
				} while (cont != 2);
			}
			break;
		}
		case 1:			//Режим переводчика
		{
			xml_node word_node(NULL);
			string input_word, result;
			Translator::Mode choice = translation_direction();		//Выбор языка вводимого слова
			if (choice != Translator::Error)
			{
				Translator input(choice);
				cout << "Введите слово: ";
				getline(cin, input_word);
				if (!(result = input.Translate(input_word, word_node)).empty())			//Проверка, что искомое слово есть в словаре
				{

					switch (syn_choice(input_word, result))
					{
					case 0:
					{
						cout << "Для возврата назад нажмите Enter" << endl
							<< "Слово: " << input_word << endl
							<< "Введите перевод введенного слова: ";
						string synonym;
						getline(cin, synonym);
						if (synonym != "")			//Выход из цикла, если введен Enter
						{
							input.Syn_word(synonym, word_node, choice);			//Добавление синонима
						}
						break;
					}
					case 1:
						(input.Words_learned(word_node, choice));
					}
				}
				else
				{
					if (new_word())				//Если слово не было найдено, предлагается его добавить
					{
						cout << "Для возврата назад нажмите Enter" << endl
							<< "Слово: " << input_word << endl
							<< "Введите перевод введенного слова: ";
						string translate_word;
						getline(cin, translate_word);
						if (translate_word != "")			//Выход из цикла, если введен Enter
						{
							input.Add_word(input_word, translate_word, choice);				//Добавление слова
						}
					}
				}
			}
			break;
		}
		case 2:
		{
			Translator::Mode choice = translation_direction();
			if (choice != Translator::Error)
			{
				Translator dict(choice);
				dict.Show_learned();

			}
			break;
		}
		}
	} while (answer != 3);
	return 0;
}