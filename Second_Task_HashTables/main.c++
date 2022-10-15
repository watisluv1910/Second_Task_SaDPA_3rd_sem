#include <climits>
#include <bitset>

#include "table_class.h"

void printMenu();
void toBin(const string& textFileName, const string& binFileName);

template<typename K, typename V>
void initTable(const string bin_file_name, HashTable<K, V>& table);

string readWord(std::ifstream& bin_file);
char readChar(std::ifstream& bin_file);

template<typename K>
K receiveKey(typename K);

int main()
{
	printMenu();

	std::cout << "\nYour choise is:\n";

	unsigned path = 0; // Menu navigation variable
	std::cin >> path;

	// cin.clear and cin.ignore are used to prevent fake input:
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<int>::max(), '\n');

	string bin_file_name = "test_hash.dat", txt_file_name = "test_hash.txt";

	HashTable<string, string> dict;

	while (path)
	{
		switch (path)
		{
		case 1:

			toBin(txt_file_name, bin_file_name);
			initTable(bin_file_name, dict);
			
			dict.print();

			break;

		case 2: 

			try
			{
				dict.printNode(receiveKey(string()));
			}
			catch (const std::invalid_argument& invArg)
			{
				std::cout << invArg.what() << '\n';
			}

			break;

		case 3:

			try
			{
				dict.remove(receiveKey(string()), txt_file_name);
				toBin(txt_file_name, bin_file_name);

				dict.print();
			}
			catch (const std::invalid_argument& invArg)
			{
				std::cout << invArg.what() << '\n';
			}

			break;

		default:

			std::cerr << "\nError: Wrong path entered.\n"; // Error 0

			break;
		}

		system("pause");
		system("cls"); // Clear screen

		printMenu();

		// Next menu navigation step:
		std::cout << "\nYour choise is:\n";
		std::cin >> path;

		// cin.clear and cin.ignore are used to prevent fake input
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<int>::max(), '\n');
	}

	return 0;
}

void printMenu()
{
    std::cout << "Practical task num.2\nIKBO-24-21 Nasevich V.V.\n"
        "Variant num. 15.\n\nMenu:\n"
        "1)Enter <1> to show all table data.\n"      
		"2)Enter <2> to show certain table data.\n"
		"3)Enter <3> to delete certain table data.\n"
        "4)Enter <0> to end the programm.\n";
}

void toBin(const string& txt_file_name, const string& bin_file_name)
{
	std::ifstream txt_file(txt_file_name, std::ios::in); // Reading
	std::ofstream bin_file(bin_file_name, std::ios::out 
		| std::ios::binary); // Writing

	if (!txt_file.is_open() || !bin_file.is_open())
	{
		std::cerr << "Cannot open the file(s).\n";
		return;
	}

	for (auto it = std::istream_iterator<char>(txt_file); 
		it != std::istream_iterator<char>(); 
		it++)
	{
		bin_file << std::bitset<CHAR_BIT>(*it) << std::endl;
	}

	txt_file.close();
	bin_file.close();
}

template<typename K, typename V>
void initTable(const string bin_file_name, HashTable<K, V>& table) 
{
	std::ifstream bin_file(bin_file_name, std::ios::in 
		| std::ios::binary); // Reading

	if (!bin_file.is_open())
	{
		std::cerr << "Cannot open the file(s).\n";
		return;
	}

	bin_file.seekg(0, std::ios_base::end); // go to end of the file
	auto bin_file_size = bin_file.tellg(); // current position is the size of the file
	bin_file.seekg(0, std::ios_base::beg); // go back to the beginning of the file

	typedef HashNode<string, string> Node;
	Node temp_node;

	while (bin_file.tellg() != bin_file_size)
	{
		temp_node.key_ = readWord(bin_file);
		temp_node.value_ = readWord(bin_file);

		table.insert(temp_node.key_, temp_node.value_);
	}

	bin_file.close();	
}

string readWord(std::ifstream& bin_file)
{
	string str = "";
	char ch = readChar(bin_file);

	while (ch != '_' && ch != ';')
	{
		str.push_back(ch);
		ch = readChar(bin_file);
	}

	return str;
}

char readChar(std::ifstream& bin_file)
{
	char c_str[CHAR_BIT + 1];
	int value;

	if (bin_file.read(reinterpret_cast<char*>(&c_str), sizeof(c_str)))
	{
		c_str[CHAR_BIT] = '\0';
		value = std::stoi(string(c_str), 0, 2);
	}
	else
	{
		return ';';
	}

	return (char)value;
}

template<typename K>
K receiveKey(typename K) 
{
	K key = K();

	std::cout << "\nEnter the key (bithday in dd.mm.yyyy format):\n";
	std::cin >> key;

	return key;
}
