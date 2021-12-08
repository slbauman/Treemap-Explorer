// scan_dir.cc
//
// Samuel Bauman 2021

#include <filesystem>
#include <vector>
#include <algorithm>
#include <string>
#include "scan_dir.h"

using namespace std;


fs_item::fs_item (int _size, string _name, bool _is_dir) {
	name = _name;
	size = _size;
	is_dir = _is_dir;
}


vector<int> fs_item::get_sizes() {
	vector<int> result;
	for (int i = 0; i < children.size(); i ++) {
		result.push_back(children[i].size);
	}
	return result;
}


vector<string> fs_item::get_names() {
	vector<string> result;
	for (int i = 0; i < children.size(); i ++) {
		result.push_back(children[i].name);
	}
	return result;
}


bool fs_item::operator <(const fs_item& rhs) {
	return size > rhs.size;
}


fs_item scan_dir(string path) {
	fs_item result(0, path, true);
	for (const auto & entry : filesystem::directory_iterator(path)) {
		fs_item dir_item(0, entry.path());
		if (filesystem::is_directory(entry.path()) && 
				!filesystem::is_symlink(entry.path())) {
			dir_item = scan_dir(entry.path());
		} else if (filesystem::is_regular_file(entry.path()) && 
				!filesystem::is_symlink(entry.path())) {
			dir_item = fs_item(filesystem::file_size(
						entry.path()), entry.path().filename()); 
		}
		result.children.push_back( dir_item );
		result.size += dir_item.size;
	}
	sort(result.children.begin(), result.children.end());
	return result;
}
