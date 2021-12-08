// scan_dir.h
//
// Samuel Bauman 2021

#include <filesystem>
#include <vector>
#include <string>

using namespace std;


struct fs_item {
	string name;
	int size;
	bool is_dir;
	vector<fs_item> children;
	fs_item (int _size = 0, string _name = "", bool _is_dir = false);
	bool operator <(const fs_item& rhs);
	vector<int> get_sizes();
	vector<string> get_names();
};


fs_item scan_dir(string);

