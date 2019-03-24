#include <string>

#include "avl_tree.h"

int main()
{
	nwacc::avl_tree<std::string, int> students;

	//students.insert("SNK", 1);
	//students.diplay_tree();
	//std::cout << std::endl << std::endl;
	//students.insert("Heroines", 2);
	//students.diplay_tree();
	//std::cout << std::endl << std::endl;
	//students.insert("Tag", 3);
	//students.diplay_tree();
	//std::cout << std::endl << std::endl;
	//students.insert("Team", 4);
	//students.diplay_tree();
	//std::cout << std::endl << std::endl;
	//students.insert("Frenzy", 5);
	//students.diplay_tree();

	

	for (nwacc::avl_tree<std::string, int>::iterator item = students.first_element(); item != students.end(); item++)
	{
		std::cout << students << std::endl; // TODO: make print key
		/*std::cout << "test" << std::endl;*/
	}
	std::cout << std::endl;
	std::cout << std::endl;



	//for (nwacc::avl_tree<std::string, int>::iterator item = students.last_element(); item != students.begin(); item--)
	//{
	//	//std::cout << students << std::endl; // TODO: make print key
	//	std::cout << "test" << std::endl;
	//}

	std::cout << students << std::endl;
}