#pragma once

struct Translator
{
private:
	pugi::xml_document doc;
	pugi::xml_document doc1;
public:
	enum Mode
	{
		LatToRus,
		RusToLat,
		Error
	};
	Translator(Translator::Mode choice);
	std::string Translate(const std::string &word, pugi::xml_node &word_node);
	void Show_page(pugi::xml_node start_node);
	void Next_page(pugi::xml_node &input_node);
	void Prev_page(pugi::xml_node &start_node);
	void Syn_word(const std::string &word, pugi::xml_node &person, const Translator::Mode &mode);
	pugi::xml_node Find_word(const std::string &word, pugi::xml_node begin);
	void Add_word(const std::string &word, const std::string &other_word, const Translator::Mode &mode);
	bool Save_all(const Translator::Mode &path);
	void Words_learned(pugi::xml_node &person, const Translator::Mode &mode);
	void Show_learned();
};