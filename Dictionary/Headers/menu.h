#pragma once

int main_menu();

int page_of_words_menu(pugi::xml_node node, Translator dict);

Translator::Mode translation_direction();

int syn_choice(std::string word, std::string translate);

bool new_word();