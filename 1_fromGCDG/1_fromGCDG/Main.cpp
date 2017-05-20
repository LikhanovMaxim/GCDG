#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <set>
#include <cstdlib>
#include <fstream>
#include <Windows.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <deque>
#include <ctime> 
#include <cmath>  
using namespace std;
//const string CATEGORIES_OF_WORDS = "CategoriesOfWords.txt";
//const string CATEGORIES_OF_WORDS = "CategoriesOfWords short version.txt";
const string CATEGORIES_OF_WORDS = "ExampleCategories.txt";
template<typename Out>
void split(const  string &s, char delim, Out result) {
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) {
		*(result++) = item;
	}
}
vector< string> split(const  string &s, char delim) {
	vector< string> elems;
	split(s, delim, back_inserter(elems));
	return elems;
}

//template <typename T>
//std::ostream& operator<< (std::ostream& out, const std::vector<T>& v) {
//	if (!v.empty()) {
//		out << '[';
//		std::copy(v.begin(), v.end(), std::ostream_iterator<T>(out, ", "));
//		out << "\b\b]";
//	}
//	return out;
//}
class Valence{  //    стрелки: /+   \+   /- \+ 
public:
	string letter;
	bool positive;    	// positive: + or -   left:  true=/ or false=\ 
	bool left;
	Valence(string _letter, bool _positive, bool _left){
		letter = _letter;
		positive = _positive;
		left = _left;
	}
};
class Local{
public:
	string letter;
	int location; // -1 left(A\); 0 - middle(\S/);  1 - right(/A)
	Local(){
	}
	Local(string _letter, int _location){
		letter = _letter;
		location = _location;
	}
};
class Rule{
public:
	//int count;
	vector<Local> locals;
	vector<Valence> valence;
	Rule(vector<Local> _locals, vector<Valence> _valence){
		locals = _locals;
		valence = _valence;
	}
};
class Category{
public:
	string name;
	//Rule rules;
	vector<Rule> rules;
	Category(){

	}
	Category(string _name, vector<Rule> _rules){
		name = _name;
		copy(_rules.begin(), _rules.end(), back_inserter(rules));
	}
	/*string getName(){
	return name;
	}
	vector<Rule> getRules(){
	return rules;
	}*/
};
class PositionWord{
public:
	string word;
	int numberOfLine;
	PositionWord(string _word, int _numberOfLine){
		word = _word;
		numberOfLine = _numberOfLine;
	}
};
class Delta{
public:
	vector<Valence> valences;
	int* delta;
	Delta(){

	}
	Delta(vector<Valence> val){
		copy(val.begin(), val.end(), back_inserter(valences));
		delta = new int[val.size()];
		for (int i = 0; i < val.size(); i++){
			delta[i] = -1;
		}
	}
	void print(){
		
		for (int i = 0; i < valences.size(); i++){
			
			if (valences[i].left){
				cout << "/";
			}
			else{
				cout << "\\";
			}
			if (valences[i].positive){
				cout << "+";
			}
			else{
				cout << "-";
			}
			cout << valences[i].letter<<"\t";
		}
		cout << endl;
		for (int i = 0; i < valences.size(); i++){
			cout << delta[i] << "\t";
		}
		cout << endl;
	}
	void printAtElement(){
		cout << "(";
		for (int i = 0; i < valences.size(); i++){
			if (valences[i].left){
				cout << "/";
			}
			else{
				cout << "\\";
			}
			if (valences[i].positive){
				cout << "+";
			}
			else{
				cout << "-";
			}
			cout << valences[i].letter;
			cout <<" " << delta[i];
			if (i != valences.size() - 1){
				cout << ", ";
			}
		}
		cout << ")";
	}
	int find(Valence val){
		return -1;
	}
};
static int numberOfItem;
class Item{
public:
	int number; //Number of item
	vector<Local> local;  //локальная категория
	//int* deltaL; //целочисленные векторы длины Р(векторы избытка)
	//int* deltaR;
	Delta deltaL;
	Delta deltaR;
	Item* itemL; //айтемы, из которых был получен this
	Item* itemR;
	Item(){
	}
	Item(Item* _itemL, Item* _itemR){
		itemL = _itemL;
		itemR = _itemR;
	}
	Item(vector<Local> _local, Delta _deltaL, Delta _deltaR){
		copy(_local.begin(), _local.end(), back_inserter(local));
		deltaL = _deltaL;
		deltaR = _deltaR;
		itemL = NULL;
		itemR = NULL;
	}
	Item(vector<Local> _local, Delta _deltaL, Delta _deltaR, Item* _itemL, Item* _itemR){
		copy(_local.begin(), _local.end(), back_inserter(local));
		deltaL = _deltaL;
		deltaR = _deltaR;
		itemL = _itemL;
		itemR = _itemR;
	}
	void setNumber(int a){
		number = a;
	}
	/*Item(vector<Local> _local, int* _deltaL, int* _deltaR){
		copy(_local.begin(), _local.end(), back_inserter(local));
		deltaL = _deltaL;
		deltaR = _deltaR;
		itemL = NULL;
		itemR = NULL;
	}*/
	
};
class M{
public:
	//Category cat;
	//vector<Local> locals;
	vector<Item> setItem;
	M(){
		
	}
	//M(Item _item){
	//	item = _item; //may be must make copyItem 
	//}
};

class Analyst{
public:
	string fileCategories;
	//string* alphabet;
	//categories;
	vector<Category> categories;
	Local S;
	vector<Valence> valenceL;
	vector<Valence> valenceR;
	//string word;
	vector<string> words;
	M **matrix;
	//Item** matrix;
	int** piL;
	int** piR;
	const int forPi = 2;
	int n;
	const string epsilon = "epsilon";
	Analyst(string nameFile){
		fileCategories = nameFile;
		//FILE* f;
		ifstream file(fileCategories);
		string line;
		if (!file.is_open()){
			cout << fileCategories << " has not opened." << '\n';
		}
		vector<PositionWord> positions;

		if (file.is_open()){
			for (int number = 0; getline(file, line); number++)
			{
				cout << line << '\n';
				//maybe use map. можно ли сразу же перейти на определённую строку?
				/*if (line[0] != '['){
				positions.push_back(new PositionWord(line, number);
				}*/
			}
		}
		file.close();
		//S;
	}
	vector<string> findWord(string word){
		ifstream file(fileCategories);
		string line;
		vector<string> spliter;
		vector<string> categoriesString;
		if (!file.is_open()){
			cout << fileCategories << " has not opened." << '\n';
		}
		if (file.is_open()){
			for (int number = 0; getline(file, line); number++)
			{
				spliter = split(line, ' ');
				if (line.size() != 0 && line[0] != '\n' && word == spliter[0]){
					//cout << "line " << word << " " << line << endl;
					while (getline(file, line) && line.size() != 0 && line[0] != '\n'){
						categoriesString.push_back(line);
					}
					break;
				}
			}
		}

		return categoriesString;
	}
	string deleteBrackets(string str){
		if (str[0] == '['){
			str = str.substr(1, str.size());
		}
		if (str[str.size() - 1] == ']'){
			str = str.substr(0, str.size() - 1);
		}
		int index = str.find('/');
		int index2 = str.find('\\');
		if (index > 0){
			str = str.substr(0, index);
		}
		if (index2 > 0){
			str = str.substr(index, str.size());
		}
		return str;
	}
	void fillLocal(vector<Local>& local, vector<string> spliter, int value){
		Local* temp = NULL;
		int i; // = 0
		int size; // = spliter.size()
		if (value == 1){
			temp = new Local(deleteBrackets(spliter[0]), 0);
			i = 1;
			size = spliter.size();
			local.push_back(*temp);
		}
		if (value == -1){
			i = 0;
			size = spliter.size() - 1;
		}
		for (; i < size; i++){
			Local temp2(deleteBrackets(spliter[i]), value);
			local.push_back(temp2);
		}
		if (value == -1){
			temp = new Local(deleteBrackets(spliter[spliter.size() - 1]), 0);	
			local.push_back(*temp);
		}
	}
	vector<Local> parseLocal(string str){  //-1 left(A\); 0 - middle(\S / );  1 - right(/ A)
		vector<Local> local;
		vector<string> spliter;
		int slash = str.find("/");
		int oppositeSlash = str.find("\\"); /*    \\=\   */
		//if (str.find("eps")){}
		int eps = str.find(epsilon);
		//cout << eps;
		if ((slash == -1 && oppositeSlash == -1) || eps != -1){  //
			//string str2 = str.substr(0, str.size());
			Local temp(deleteBrackets(str), 0);
			local.push_back(temp);
			return local;
		}
		if (slash != -1 && oppositeSlash != -1){
			spliter = split(str, '\\');
			//cout << "spliter" << endl;
			//printVectorString(spliter);
			for (int i = 0; i < spliter.size() - 1; i++){
				Local temp2(deleteBrackets(spliter[i]), -1);
				local.push_back(temp2);
			}
			Local temp(deleteBrackets(spliter[spliter.size() - 1]), 0);
			local.push_back(temp);
			spliter = split(str, '/');
			//cout << "spliter" << endl;
			//printVectorString(spliter);
			for (int i = 1; i < spliter.size(); i++){
				Local temp2(deleteBrackets(spliter[i]), 1);
				local.push_back(temp2);
			}
			//Local temp(deleteBrackets(spliter[0]), 0);
			//local.push_back(temp);

			//Local temp(deleteBrackets(str), 0);
			//local.push_back(temp);
			return local;
		}
		if (slash != -1){
			spliter = split(str, '/');
			fillLocal(local, spliter, 1);
			return local;
		}
		if (oppositeSlash != -1){
			spliter = split(str, '\\');
			fillLocal(local, spliter, -1);
			return local;
		}
		/*
		if (slash == -1 && oppositeSlash == -1){
		Local temp(str.substr(1, str.size - 1), 0);

		}
		else if (slash != -1 && oppositeSlash != -1){

		}
		else if (slash != -1){

		}
		else if (oppositeSlash != -1){

		}
		for (int i = 0; i < str.size(); i++){

		}*/
		return local;
	}
	vector<Valence> parseValence(string str){
		vector<Valence> val;
		//Valence temp;
		string cur;
		bool positive, left; // positive: + or -   left:  true=/ or false=\ 

		bool flag = true;
		for (int i = 0; flag && i < str.size(); i++){
			switch (str[i]){
			case '\\': {
				if (cur.size() > 0){
					Valence temp(cur, positive, left);
					val.push_back(temp);
					cur = "";
				}
				left = false;
				break; }
			case '/': {
				if (cur.size() > 0){
					Valence temp(cur, positive, left);
					val.push_back(temp);
					cur = "";
				}
				left = true;
				break;
			}
			case '+': {
				positive = true;
				break;
			}
			case '-': {
				positive = false;
				break;
			}
			case ' ': {flag = false; break; }
			default: {cur += str[i]; break; }
			}
		}
		Valence temp(cur, positive, left);
		val.push_back(temp);
		return val;
	}
	void printVectorLocal(vector<Local> vec){
		cout << "[";
		for (vector<Local>::const_iterator i = vec.begin(); i != vec.end(); ++i){
			cout << (*i).letter << " " << (*i).location << " ";
		}
		cout << "]";
	}
	void printValence(Valence val){
		cout << val.letter << " ";
		if (val.left){
			cout << "/ ";
		}
		else{
			cout << "\\ ";
		}
		if (val.positive){
			cout << "+" << endl;
		}
		else{
			cout << "-" << endl;
		}
	}
	void printVectorValence(vector<Valence> vec){
		for (vector<Valence>::const_iterator i = vec.begin(); i != vec.end(); ++i){ // positive: + or -   left:  true=/ or false=\ 
			printValence((*i));
		}
	}
	vector<Rule> parseCategory(vector<string> categories){
		vector<Rule> res;
		vector<string> spliter;
		for (vector<string>::const_iterator i = categories.begin(); i != categories.end(); ++i){
			spliter = split(*i, ' ');
			//cout << "First: " << spliter[0] << endl;
			vector<Local> local = parseLocal(spliter[0]);
			//cout << "printVectorLocal:" << endl;
			//printVectorLocal(local);
			//cout << "Second: " << spliter[1] << endl;
			vector<Valence> val = parseValence(spliter[1]);
			//cout << "printVectorValence:" << endl;
			//printVectorValence(val);
			Rule temp(local, val);
			res.push_back(temp);
		}
		return res;
	}
	bool checkIndividual(string word){
		for (vector<Category>::const_iterator i = categories.begin(); i != categories.end(); ++i){
			if ((*i).name == word){
				return false;
			}
		}
		return true;
	}
	void createRules(){
		vector<string> categoriesString;
		for (vector<string>::const_iterator i = words.begin(); i != words.end(); ++i){
			cout << endl << "create Rules " << *i << endl;
			if (checkIndividual(*i)){
				categoriesString = findWord(*i);
				cout << "printVectorString: " << endl;
				printVectorString(categoriesString);
				//cout << "categoriesString:" << endl << categoriesString << endl;
				//parseCategory(categoriesString); //rules
				cout << "parseCategory: " << endl;
				Category cat(*i, parseCategory(categoriesString));
				categories.push_back(cat);
				//categories.push_back(new Category(*i, parseCategory(categoriesString)));
			}
		}
	}
	void printRule(Rule i){
		printVectorLocal((i).locals);
		printVectorValence((i).valence);
	}
	void printVectorRule(vector<Rule> vec){
		for (vector<Rule>::const_iterator i = vec.begin(); i != vec.end(); ++i){
			printRule(*i);
		}
	}
	void printCategories(){
		for (vector<Category>::const_iterator i = categories.begin(); i != categories.end(); ++i){
			cout << (*i).name << ":" << endl;
			printVectorRule((*i).rules);
			cout << endl << endl;
		}
	}
	void printCategory(Category cat){
		printVectorRule(cat.rules);
	}
	bool checkIndividualValence(vector<Valence> valence, Valence v){
		for (vector<Valence>::const_iterator i = valence.begin(); i != valence.end(); ++i){
			if ((*i).letter == v.letter && (*i).left == v.left && (*i).positive == v.positive){
				return false;
			}
		}
		return true;
	}
	void fillVal(vector<Valence> val){ // left:  true=/ or false=\ 
		for (vector<Valence>::const_iterator i = val.begin(); i != val.end(); ++i){
			if ((*i).left && checkIndividualValence(valenceL, (*i))){ //  left: /
				valenceL.push_back((*i));
			}
			if (!(*i).left && checkIndividualValence(valenceR, (*i))){      /* left: \					*/
				valenceR.push_back((*i));
			}
		}

	}
	void fillValences(){
		for (vector<Category>::const_iterator i = categories.begin(); i != categories.end(); ++i){
			Category temp = *i;
			//vector<Valence> rules = temp.rules.valence;
			//for (vector<Valence>::const_iterator j = rules.begin(); j != rules.end(); ++j){
			vector<Rule> rules = temp.rules;
			for (vector<Rule>::const_iterator j = rules.begin(); j != rules.end(); ++j){
				fillVal((*j).valence);
			}
		}
	}
	void printVectorString(vector<string> vec){
		for (vector<string>::const_iterator i = vec.begin(); i != vec.end(); ++i){
			cout << *i << '\n';
		}
	}
	/*void printWords(){
	for (vector<string>::const_iterator i = words.begin(); i != words.end(); ++i)
	cout << *i << '\n';
	}*/
	void initializationPi(bool pi, int size){
		if (pi){
			piL = new int*[size];
			for (int i = 0; i < size; i++){
				piL[i] = new int[n + 1];
			}
		}
		else{
			piR = new int*[size];
			for (int i = 0; i < size; i++){
				piR[i] = new int[n + 1];
			}
		}
		/*	pi = new int*[size];
		for (int i = 0; i < size; i++){
		pi[i] = new int[n];
		}*/
	}
	void initialization(string file){
		string sentence;
		ifstream f(file);
		if (!f.is_open()){
			cout << file << " has not opened." << '\n';
		}
		if (f.is_open()){
			getline(f, sentence);
			cout << sentence << endl;
		}
		f.close();
		words = split(sentence, ' ');
		n = words.size();

		/*
		M;
		alphabet = new string[ASDFKJ];  // хз че хотел
		*/
	}
	vector<Local> getCAlpha(string nameCategory){ //локальная проекция
		for (vector<Category>::const_iterator i = categories.begin(); i != categories.end(); ++i){
			Category temp = *i;
			if (temp.name == nameCategory){
				//	return temp.rules.locals;
			}
		}
		cout << "getCAlpha Not found" << endl;
		vector<Local> null;
		return null;
	}
	Category getCp(string nameCategory){ //валентная проекция
		vector<Rule> res;
		for (vector<Category>::const_iterator i = categories.begin(); i != categories.end(); ++i){
			Category temp = *i;
			if (temp.name == nameCategory){
				return (*i);
				//	return temp.rules.valence;
			}
		}
		cout << "getCp Not found" << endl;
		Category null;
		return null;
	}
	int calculatePv(vector<Valence> temp, Valence v){
		int count = 0;
		//first option
		//cout << "for \n" ;
		//printVectorValence(temp);
		//second option
		for (vector<Valence>::const_iterator i = temp.begin(); i != temp.end(); ++i){
			if ((*i).letter == v.letter && (*i).left == v.left && (*i).positive == v.positive){
				count++;
			}
			if ((*i).letter == v.letter && (*i).left != v.left && (*i).positive != v.positive){
				if (count > 0)
				{
					count--;
				}
			}
		}
		//cout << "P: " << count << endl;
		return count;
	}
	Valence oppositeValence(Valence v){
		Valence res(v.letter, !v.positive, !v.left);
		return res;
	}
	int delta(Valence v, Rule P){
		//cout << "deltaL:" << endl;
		//printValence(v);
		int maximum = 0;
		vector<Valence> temp;//для P'
		//first option:
		for (vector<Valence>::reverse_iterator i = P.valence.rbegin(); i != P.valence.rend(); ++i) { //c конца читаем. суффикс ???????????????++i?????????????????
			//cout << "iteterator for delataL:" << endl;
			//printValence((*i));
			temp.insert(temp.begin(), *i);
			maximum = max(maximum, abs(calculatePv(temp, v)) - abs(calculatePv(temp, oppositeValence(v))));
			//cout << "Maximun: " << maximum << endl;
		}
		//second option
		//maximum = max(maximum, calculatePv(P.valence, v) - calculatePv(P.valence, oppositeValence(v)));
		//cout << maximum << endl;
		return maximum;
	}
	int delta(Valence v, vector<Valence> P){
		//cout << "deltaL:" << endl;
		//printValence(v);
		int maximum = 0;
		vector<Valence> temp;//для P'
		for (vector<Valence>::reverse_iterator i = P.rbegin(); i != P.rend(); ++i) { //c конца читаем. суффикс 
			//cout << "iteterator for delataL:" << endl;
			//printValence((*i));
			temp.insert(temp.begin(), *i);
			maximum = max(maximum, abs(calculatePv(temp, v)) - abs(calculatePv(temp, oppositeValence(v))));
			//cout << "Maximun: " << maximum << endl;
		}
		//cout << maximum << endl;
		return maximum;
	}
	/*int deltaR(Valence P){
	return 0;
	}*/
	void calcFailFuncL(){
		//vector<Valence> cP;
		//vector<Rule> cP;
		Category cP;
		vector<Valence> temp;//для P'
		int piMax;
		//for (vector<Valence>::const_iterator v = valenceL.begin(); v != valenceL.end(); ++v){
		for (int v = 0; v < valenceL.size(); v++){
			//cout << "Valence " << v << endl;
			//printValence(valenceL[v]);
			piL[v][0] = 0;
			for (int i = 0; i < n; i++){
				piMax = 0;  // symbol: everything is going well.
				cP = getCp(words[i]);
				//cout << i << " " << "word: '" << words[i] << "'. getCp:" << endl;
				//printCategory(cP);
				for (int j = 0; j < cP.rules.size(); j++){ //cP = C^p
					//temp.insert(temp.end(), cP[j].valence.begin(), cP[j].valence.end()); //P =cP[j]
					//piMax = max(piMax, deltaL(valenceL[v], temp) + max(piL[v][i - 1] - deltaL(oppositeValence(valenceL[v]), temp), 0)); 
					piMax = max(piMax, delta(valenceL[v], cP.rules[j]) + max(piL[v][i - 1] - delta(oppositeValence(valenceL[v]), cP.rules[j]), 0));
				}
				//cout << "piMax: " << piMax << endl;
				piL[v][i] = piMax;
			}
		}
	}
	void printMatrixInt(int** mat, vector<Valence> val, int m){
		vector<Valence>::const_iterator k = val.begin();
		for (int i = 0; i < val.size(); i++, ++k){
			printValence((*k));
			//cout << (*k).letter << ": ";
			for (int j = 0; j < m; j++){
				cout << mat[i][j] << " ";
			}
			cout << endl;
		}
	}
	void calcFailFuncR(){
		Category cP;
		vector<Valence> temp;//для P'
		int piMax;
		//for (vector<Valence>::const_iterator v = valenceR.begin(); v != valenceR.end(); ++v){
		for (int v = 0; v < valenceR.size(); v++){
			//cout << "Valence " << v << endl;
			//printValence(valenceR[v]);
			piR[v][0] = 0;
			for (int i = 0; i < n; i++){
				piMax = 0;  // symbol: everything is going well.
				cP = getCp(words[n - (i + 1)]);
				//cout << i << " " << "word: '" << words[i] << "'. getCp:" << endl;
				//printCategory(cP);
				for (int j = 0; j < cP.rules.size(); j++){ //cP = C^p
					//temp.insert(temp.end(), cP[j].valence.begin(), cP[j].valence.end()); //P =cP[j]
					//piMax = max(piMax, delta(valenceL[v], temp) + max(piR[v][i - 1] - delta(oppositeValence(valenceL[v]), temp), 0)); 
					piMax = max(piMax, delta((valenceR[v]), cP.rules[j]) + max(piR[v][i - 1] - delta(oppositeValence(valenceR[v]), cP.rules[j]), 0));
				}
				//cout << "piMax: " << piMax << endl;
				piR[v][i] = piMax;
			}
		}

		/*int piMax;
		for (v Vl){
		piR[v][0] = 0;
		for (int i = 1; i < n; i++){
		piMax = 0;
		for (cP Rule(word[n - i + 1])){
		piMax = max(piMax, delta_otr(P) + max(piR[v][i - 1] - delta(P), 0));
		}
		piR[v][i] = piMax;
		}
		}*/
	}
	void intitializationMatrix(){
		matrix = new M*[n];
		for (int i = 0; i < n; i++){
			matrix[i] = new M[n];
		}
		//Items:
		/*matrix = new Item*[n];
		for (int i = 0; i < n; i++){
		matrix[i] = new Item[n];
		}*/
		//
		/*matrix = new M*[n];
		for (int i = 0; i < n; i++){
		matrix[i] = new M[n];
		}*/
	}
	void AddItem(int i, int j, Item& item){ //, (C, )
		//M temp = new M(item);
		item.number = numberOfItem;
		Item* temp = new Item(item.local, item.deltaL, item.deltaR);
		//(*temp).setNumber(numberOfItem);
		//(*temp).number = numberOfItem;
		numberOfItem++;
		matrix[i][j].setItem.push_back(item);
		/*
		if(item.locals == [c'*\B]){
		AddItem()
		}
		if(item.locals == [B/c'*]){
		AddItem();
		}
		*/
	}
	/*	int* calculateDeltaL(){
	int size;
	int* res = new int[size];

	return res;
	}
	*/
	//void printVectorDelta(int* arr, vector<Valence> val){
	//	for (int i = 0; i < val.size(); i++){
	//		cout << val[i].letter;
	//		if (val[i].left){
	//			cout << "/";
	//		}
	//		else{
	//			cout << "\\";
	//		}
	//		if (val[i].positive){
	//			cout << "+\t";
	//		}
	//		else{
	//			cout << "-\t";
	//		}
	//	}
	//	cout << endl;
	//	for (int i = 0; i < val.size(); i++){
	//		cout << arr[i] << "\t";
	//	}
	//	cout << endl;
	//}
	void initializationDelta(int* delta, int size){
		for (int i = 0; i < size; i++){
			delta[i] = -1;
		}

	}
	void printItem(Item item){
		cout << "( I" << item.number << " ) <";
		printVectorLocal(item.local);
		item.deltaL.printAtElement();
		//printVectorDelta(item.deltaL, valenceL);
		item.deltaR.printAtElement();
		//printVectorDelta(item.deltaR, valenceR);
		//print item L R
		if (item.itemL != NULL &&  item.itemR != NULL){
			cout << ", " << item.itemL->number << ", " << item.itemR->number << ">" << endl;
		}
		else{
			cout << ">" << endl;
		}
	}
	void printVectorItem(vector<Item> temp){
		for (vector<Item>::const_iterator v = temp.begin(); v != temp.end(); ++v){
			printItem(*v);
		}
	}
	void printMatixM(){
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				cout << "M[" << i << ", " << j << "]\n";
				printVectorItem(matrix[i][j].setItem);
			}
		}
	}
	int findIndex(vector<Valence> val, Valence cur){
		int index = -1;
		int i = 0;
		for (vector<Valence>::const_iterator v = val.begin(); v != val.end(); ++v){
			if (cur.letter == (*v).letter && cur.left == (*v).left && cur.positive == (*v).positive){
				index = i;
				break;
			}
			i++;
		}
		return index;
	}
	vector<Valence> oppositeVectorValence(vector<Valence> vec){
		vector<Valence> res;
		for (vector<Valence>::const_iterator v = valenceL.begin(); v != valenceL.end(); ++v){
			res.push_back(oppositeValence((*v)));
		}
		return res;
	}
	void Propose(int i){ //CDG-TCS 2014
		bool goodC;
		Category cP;
		Delta* deltaL;
		Delta* deltaR;
		//int* deltaL;
		//int* deltaR;
		Item* item;
		int count;
		//for (int i = 0; i < n; i++){
		cP = getCp(words[i]);
		
		//deltaL = new int[valenceL.size()];
		//initializationDelta(deltaL, valenceL.size());
		//deltaR = new int[valenceR.size()];
		//initializationDelta(deltaR, valenceR.size());
		for (int z = 0; z < cP.rules.size(); z++){
			//cout << "Cp rule:" << endl;
			//printRule(cP.rules[z]);
			//cout << endl;
			//printVectorValence(cP.rules[z].valence);
			goodC = true;
 			deltaL = new Delta(valenceL);
			deltaR = new Delta(oppositeVectorValence(valenceL));
			count = 0;
			for (vector<Valence>::const_iterator v = valenceL.begin(); v != valenceL.end(); ++v){ // positive: + or -   left:  true=/ or false=\ 
				//cout << count << " ";
				//printValence((*v));
				//printVectorRule(cP.rules[count]);
				//printVectorLocal(cP.rules[count]);
				(*deltaL).delta[count] = delta((*v), cP.rules[z].valence);
				(*deltaR).delta[count] = delta(oppositeValence(*v), cP.rules[z].valence);
				//i??????? 
				/*deltaL[count] = delta((*v), cP.rules[i]);
				deltaR[count] = delta(oppositeValence(*v), cP.rules[i]); */

				//int indexOfOppositeV = valenceR.size() - count;
				int indexOfOppositeV = findIndex(valenceR, oppositeValence(*v));
				//cout << "index: "<<indexOfOppositeV << endl;
				//Conditions is not correct!
				if (n - (i + 2) != -1){
					if ((*deltaL).delta[count] > piR[indexOfOppositeV][n - (i + 2)]){
						//cout << "First TRUE:" << (*deltaL).delta[count] << ">" << piR[indexOfOppositeV][n - (i + 1)] << endl;
						goodC = false;
						break;
					}
				}
				else{ //TODO: delete else: increment size of piL and piR.
					if ((*deltaL).delta[count] > 0){
						//cout << "First TRUE:" << (*deltaL).delta[count] << ">" << 0 << endl;
						goodC = false;
						break;
					}
				}
				if (i != 0){ // i!=0
					if ((*deltaR).delta[count] > piL[count][i-1]){
						//cout << "Second TRUE:" << (*deltaR).delta[count] << ">" << piL[count][i] << endl;
						goodC = false;
						break;
					}
				}
				else{
					if ((*deltaR).delta[count] > 0){
						//cout << "Second TRUE:" << (*deltaR).delta[count] << ">" <<0 << endl;
						goodC = false;
						break;
					}
				}
				count++;
			}
			//cout << "deltaL\n";
			//(*deltaL).print();
			//printVectorDelta(deltaL, valenceL);
			//cout << "deltaR\n";
			//(*deltaR).print();
			//printVectorDelta(deltaR, valenceR);
			if (goodC){
				cout << "Add Item!\n";
				item = new Item(cP.rules[z].locals, (*deltaL), (*deltaR)); //itemL = null, itemR = null;
				AddItem(i, i, *item);
			}
		}
	}
/*	bool checkLocalOnMain(vector<Local> vec, Local C){  // -1 left(A\); 0 - middle(\S/);  1 - right(/A)
		for (vector<Local>::const_iterator v = vec.begin(); v != vec.end(); ++v){
			if ((*v).location == 0 && v->letter == C.letter){
				return true;
			}
			else{
				return false;
			}
		}
	}
	bool checkLocalLeftPosition(vector<Local> vec, Local C){ //check this fucntion
		for (vector<Local>::const_iterator v = vec.begin(); v != vec.end(); ++v){
			if ((*v).location == -1 && v->letter == C.letter){
				return true;
			}
			if ((*v).location == 0){
				return false;
			}
		}
		return false;
	}
	*/
	bool checkLocalReducedByLeft(vector<Local> alpha1, vector<Local> alpha2){ // -1 left(A\); 0 - middle(\S/);  1 - right(/A)
		if (alpha1.size() > 1 || alpha1[0].location != 0){
			return false;
		}
		if (alpha1[0].letter != alpha2[0].letter || alpha2[0].location!=-1 ){ 
			return false;
		}
		return true;
	}
	bool checkLocalEpsilon(vector<Local> vec){
		if (vec[0].letter == epsilon){
			return true;
		}
		else{
			return false;
		}
		//
		//for (vector<Local>::const_iterator v = vec.begin(); v != vec.end(); ++v){
		//	if ((*v).letter == epsilon){ //epsilon always is main:  0 - middle(\S/);
		//		return true;
		//	}
		//	else{
		//		return false;
		//	}
		//}
		
	}
	void SubordinateL(int i, int k, int j){
		int count;
		bool goodItems;
		Delta* deltaL;
		Delta* deltaR;
		Item* item;
		//for (vector<Item>::const_iterator I1 = matrix[i][k].setItem.begin(); I1 != matrix[i][k].setItem.end(); ++I1){
		//	for (vector<Item>::const_iterator I2 = matrix[k + 1][j].setItem.begin(); I2 != matrix[k + 1][j].setItem.end(); ++I2){
		for (vector<Item>::iterator I1 = matrix[i][k].setItem.begin(); I1 != matrix[i][k].setItem.end(); ++I1){
			for (vector<Item>::iterator I2 = matrix[k + 1][j].setItem.begin(); I2 != matrix[k + 1][j].setItem.end(); ++I2){
				cout << "\nI1 locals:" << endl;
				printVectorLocal((*I1).local);
				cout << "\nI2 locals:" << endl;
				printVectorLocal((*I2).local);
				cout << endl;
				goodItems = true;
				deltaL = new Delta(valenceL);
				deltaR = new Delta(oppositeVectorValence(valenceL));
				count = 0;
				for (vector<Valence>::const_iterator v = valenceL.begin(); v != valenceL.end(); ++v){ // positive: + or -   left:  true=/ or false=\ 
					cout << count << " ";
					printValence((*v));
					(*deltaL).delta[count] = (*I2).deltaL.delta[count] + max((*I1).deltaL.delta[count] - (*I2).deltaR.delta[count], 0);
					(*deltaR).delta[count] = (*I1).deltaR.delta[count] + max((*I2).deltaR.delta[count] - (*I1).deltaL.delta[count], 0);
					
					int indexOfOppositeV = findIndex(valenceR, oppositeValence(*v));
					if (n - (j + 2) != -1){
						cout << "First :" << (*deltaL).delta[count] << ">" << piR[indexOfOppositeV][n - (j + 2)] << endl;
						if ((*deltaL).delta[count] > piR[indexOfOppositeV][n - (j + 2)]){
							goodItems = false;
							break;
						}
					}
					else{ //TODO: delete else: increment size of piL and piR.
						cout << "First :" << (*deltaL).delta[count] << ">" << 0 << endl;
						if ((*deltaL).delta[count] > 0){							
							goodItems = false;
							break;
						}
					}
					if (i != 0){
						cout << "Second :" << (*deltaR).delta[count] << ">" << piL[count][i - 1] << endl;
						if ((*deltaR).delta[count] > piL[count][i - 1]){							
							goodItems = false;
							break;
						}
					}
					else{
						cout << "Second :" << (*deltaR).delta[count] << ">" << 0 << endl;
						if ((*deltaR).delta[count] > 0){							
							goodItems = false;
							break;
						}
					}
					count++;
				}
				cout << "deltaL\n";
				(*deltaL).print();
				cout << "deltaR\n";
				(*deltaR).print();
				if (goodItems){
					//TODO: если сначала цикла проверять на это условие?
					
					//if (checkLocalOnMain((*I1).local, ) && checkLocalLeftPosition((*I2).local)){ 
					if (checkLocalReducedByLeft((*I1).local, (*I2).local)){
						cout << "Add Item!\n";
						vector<Local> alpha;
						vector<Local>::iterator deleteFirst = (*I2).local.begin();	//delete first local from (*I2).local;
						deleteFirst++;
						copy(deleteFirst, I2->local.end(), back_inserter(alpha));
						//copy((*I2).local.begin(), I2->local.end(), back_inserter(alpha));
						item = new Item(alpha, (*deltaL), (*deltaR), &(*I1), &(*I2));
						AddItem(i, j, *item);
					}
					else if (checkLocalEpsilon((*I1).local)){  // (checkLocalOnMain((*I1).local) && checkLocalLeftPosition((*I2).local) for [C*\B]
						cout << "Add Item!\n";
						//item = new Item((*I2).local, (*deltaL), (*deltaR));
						item = new Item((*I2).local, (*deltaL), (*deltaR), &(*I1), &(*I2)); //itemL, itemR;
						AddItem(i, j, *item);
					}
				}
			}
		}

	}
	bool checkLocalReducedByRight(vector<Local> alpha1, vector<Local> alpha2){ // -1 left(A\); 0 - middle(\S/);  1 - right(/A)
		if (alpha1.size() > 1 || alpha1[0].location != 0){
			return false;
		}
		int indexOfalpha2 = alpha2.size() -1;
		if (alpha1[0].letter != alpha2[indexOfalpha2].letter || alpha2[indexOfalpha2].location != 1){
			return false;
		}
		return true;
	}
	void SubordinateR(int i, int k, int j){//TODO: make for R this code!
		int count;
		bool goodItems;
		Delta* deltaL;
		Delta* deltaR;
		Item* item;
		//for (vector<Item>::const_iterator I1 = matrix[i][k].setItem.begin(); I1 != matrix[i][k].setItem.end(); ++I1){
		//	for (vector<Item>::const_iterator I2 = matrix[k + 1][j].setItem.begin(); I2 != matrix[k + 1][j].setItem.end(); ++I2){
		for (vector<Item>::iterator I1 = matrix[i][k].setItem.begin(); I1 != matrix[i][k].setItem.end(); ++I1){
			for (vector<Item>::iterator I2 = matrix[k + 1][j].setItem.begin(); I2 != matrix[k + 1][j].setItem.end(); ++I2){
				cout << "\nI1 locals:" << endl;
				printVectorLocal((*I1).local);
				cout << "\nI2 locals:" << endl;
				printVectorLocal((*I2).local);
				cout << endl;
				goodItems = true;
				deltaL = new Delta(valenceL);
				deltaR = new Delta(oppositeVectorValence(valenceL));
				count = 0;
				for (vector<Valence>::const_iterator v = valenceL.begin(); v != valenceL.end(); ++v){ // positive: + or -   left:  true=/ or false=\ 
					cout << count << " ";
					printValence((*v));
					(*deltaL).delta[count] = (*I2).deltaL.delta[count] + max((*I1).deltaL.delta[count] - (*I2).deltaR.delta[count], 0);
					(*deltaR).delta[count] = (*I1).deltaR.delta[count] + max((*I2).deltaR.delta[count] - (*I1).deltaL.delta[count], 0);

					int indexOfOppositeV = findIndex(valenceR, oppositeValence(*v));
					if (n - (j + 2) != -1){
						cout << "First :" << (*deltaL).delta[count] << ">" << piR[indexOfOppositeV][n - (j + 2)] << endl;
						if ((*deltaL).delta[count] > piR[indexOfOppositeV][n - (j + 2)]){
							goodItems = false;
							break;
						}
					}
					else{ //TODO: delete else: increment size of piL and piR.
						cout << "First :" << (*deltaL).delta[count] << ">" << 0 << endl;
						if ((*deltaL).delta[count] > 0){
							goodItems = false;
							break;
						}
					}
					if (i != 0){
						cout << "Second :" << (*deltaR).delta[count] << ">" << piL[count][i - 1] << endl;
						if ((*deltaR).delta[count] > piL[count][i - 1]){
							goodItems = false;
							break;
						}
					}
					else{
						cout << "Second :" << (*deltaR).delta[count] << ">" << 0 << endl;
						if ((*deltaR).delta[count] > 0){
							goodItems = false;
							break;
						}
					}
					count++;
				}
				cout << "deltaL\n";
				(*deltaL).print();
				cout << "deltaR\n";
				(*deltaR).print();
				if (goodItems){					
					//TODO: если сначала цикла проверять на это условие?
					//if (checkLocalOnMain((*I1).local, ) && checkLocalLeftPosition((*I2).local)){ 
					if (checkLocalReducedByRight((*I1).local, (*I2).local)){
						cout << "Add Item!\n";
						vector<Local> alpha;
						vector<Local>::iterator deleteLast = (*I2).local.end();	//delete first local from (*I2).local;
						deleteLast--;
						copy(I2->local.begin(), deleteLast, back_inserter(alpha));
						//copy((*I2).local.begin(), I2->local.end(), back_inserter(alpha));
						item = new Item(alpha, (*deltaL), (*deltaR), &(*I1), &(*I2));
						AddItem(i, j, *item);
					}
					else if (checkLocalEpsilon((*I1).local)){  // (checkLocalOnMain((*I1).local) && checkLocalLeftPosition((*I2).local) for [C*\B]
						cout << "Add Item!\n";
						//item = new Item((*I2).local, (*deltaL), (*deltaR));
						item = new Item((*I2).local, (*deltaL), (*deltaR), &(*I1), &(*I2)); //itemL, itemR;
						AddItem(i, j, *item);
					}
				}
			}
		}
	}
	bool isMainCategory(vector<Local> local){ //TODO check all category with some list
		if (local.size() == 1 && local[0].location == 0){
			return true;
		}
		else{
			return false;
		}
	}
	bool isVectorNull(Delta delta){
		for (int i = 0; i < delta.valences.size(); i++){
			if (delta.delta[i] != 0){
				return false;
			}
		}
		return true;
	}
	bool isItemCool(Item item){
		if (!isMainCategory(item.local)){
			return false;
		}
		if (isVectorNull(item.deltaL) && isVectorNull(item.deltaR)){
			return true;
		}
		return false;
	}
	bool isWordCorrcet(int i, int j){
		for (vector<Item>::iterator item = matrix[i][j].setItem.begin(); item != matrix[i][j].setItem.end(); ++item){
			if (isItemCool((*item))){
				return true;
			}
		}
		return false;
	}
	bool Run(string file){
		initialization(file);
		numberOfItem = 1;
		createRules();
		printCategories();
		fillValences();// fill valenceL valenceR

		cout << endl << "Valence L:" << endl;
		printVectorValence(valenceL);
		cout << endl << "Valence R:" << endl;
		printVectorValence(valenceR);
		
		initializationPi(true, valenceL.size());
		initializationPi(false, valenceR.size());
		//cout << endl << "calcFailFuncL:" << endl;
		calcFailFuncL();
		calcFailFuncR();

		cout << endl << "calcFailFuncL(); printMatrixInt:" << endl;
		printMatrixInt(piL, valenceL, n);
		cout << endl << "calcFailFuncR(); printMatrixInt:" << endl;
		printMatrixInt(piR, valenceR, n);

		intitializationMatrix();
		for (int i = 0; i < n; i++){
			//cout << endl<< i << " Propose:" << endl;
			Propose(i);
			//printVectorItem(matrix[i][i].setItem);
		}
		printMatixM();
		for (int l = 1; l < n; l++){
			for (int i = 0; i < n - l; i++){
				int j = i + l;
				cout<<"\nl = "<<l << ".i = " << i << ". j = " << j << ". j - l = " << j - l << endl;
				for (int k = i; k <= j - l; k++){
					SubordinateL(i, k, j);
					SubordinateR(i, k, j);
				}
			}
		}
		cout <<endl<< "Result Matrix" << endl;
		printMatixM();
		if (isWordCorrcet(0, n - 1)){ //??n
		//	Expand(I);
			cout<<"---YES---"<<endl;
			cleanStructure();
			return true;
		}
		else{
			cout<<"---NO---"<<endl;
			return false;
		}
	}
	void cleanStructure(){

	}
};
int main(){
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	unsigned int start_time = clock();
	string file = "ExampleSentence.txt";
	Analyst analyst(CATEGORIES_OF_WORDS);
	cout << "Run:" << endl;
	//for(!exit)
	analyst.Run(file);
	unsigned int end_time = clock(); // конечное время
	unsigned int search_time = end_time - start_time; // искомое время
	cout << "search_time: " << search_time / 1000.0 << endl;
	return 0;
}