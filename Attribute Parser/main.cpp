#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <regex>
#include <map>

std::string get_tag(std::string&);
std::string get_query(std::string&, std::map <std::string, std::vector<std::string>>&, std::map<std::string, std::map<std::string, std::string>>&);
bool get_qeuery_helper(std::string&, std::string&, std::map <std::string, std::vector<std::string>>&);
std::map<std::string, std::string> attr_parser(std::string);
void construct_tag_heirarchy(int&, int&, std::string, std::vector<std::string>&, std::map<std::string, std::vector<std::string>>&);

int main(void) {

	std::fstream infs;
	infs.open("input.txt", std::fstream::in);

	if (!infs.is_open()) {
		std::cout << "Could not open input..." << std::endl;
		return -1;
	}

	// control number variables
	int nLines = 0;
	int nQueries = 0;

	// get control numbers
	// line used throught
	std::string line;
	std::getline(infs, line);
	sscanf_s(line.c_str(), "%d %d", &nLines, &nQueries);
	//sscanf_s(&line[0], "%d %d", &n_Lines, &n_Queries);

	// hrml and queries string vectors
	std::vector<std::string> hrml_vector;
	std::vector<std::string> queries_vector;

	// get hrml from input
	// i used throught
	int i = 0;
	do {
		std::getline(infs, line);
		hrml_vector.push_back(line);
		++i;
	} while (i < nLines);
	// get queries from input
	i = 0;
	do {
		std::getline(infs, line);
		queries_vector.push_back(line);
		++i;
	} while (i < nQueries && !infs.eof());

	// get tags and attributes from lines
	std::map<std::string, std::map<std::string, std::string>> tag_attributes;
	for (i = 0; i < nLines; ++i) {
		if (hrml_vector[i][1] != '/') {
			// getting tag name
			std::string tag_name = get_tag(hrml_vector[i]);
			// get attributes
			std::map<std::string, std::string> attributes = attr_parser(hrml_vector[i]);
			// push tag and attributes to vector
			tag_attributes.insert(std::make_pair(tag_name, attributes));
		}
	}

	// construct tag heirarchy
	i = 0;
	line = get_tag(hrml_vector[0]);
	std::map <std::string, std::vector<std::string>> tag_heirarchy;
	construct_tag_heirarchy(i, nLines, line, hrml_vector, tag_heirarchy);

	// print values
	for (auto s : queries_vector) {

		std::string value = get_query(s, tag_heirarchy, tag_attributes);

		std::cout << value << std::endl;
	}

	return 0;
}

std::string get_tag(std::string& hrml) {
	int idx = hrml.find_first_of(' ');

	if (idx < 0) {
		idx = hrml.find_first_of('>');
	}

	return hrml.substr(1, idx - 1);
}

void construct_tag_heirarchy(int& i, int& nLines,std::string parent_tag_name,std::vector<std::string>& hrml_vector,std::map <std::string, std::vector<std::string>>& tag_heirarchy) {

	std::vector<std::string> sub_tags_vector;
	for (; i < nLines; ++i) {

		if (hrml_vector[i][1] != '/') {
			std::string sub_tag_name = get_tag(hrml_vector[i]);
			sub_tags_vector.push_back(sub_tag_name);
			construct_tag_heirarchy(++i, nLines, sub_tag_name, hrml_vector, tag_heirarchy);
		}
		else {
			int idx = hrml_vector[i].find_first_of('>');
			std::string sub_tag_name = hrml_vector[i].substr(2, idx - 2);
			if (sub_tag_name == parent_tag_name) {
				break;
			}
		}
	}
	
	tag_heirarchy.insert(make_pair(parent_tag_name, sub_tags_vector));
}

std::map<std::string, std::string> attr_parser(std::string s_currentLine)
{
	std::map<std::string, std::string> attributes;

	std::regex r("[\\w|\\character]+\\s*\\=\\s*\\\"[\\w|\\s|\\character|%|!|$|.]+\\\"");
	std::smatch m;

	while (std::regex_search(s_currentLine, m, r)) {

		int idx = m.str().find_first_of('=');
		std::string attr = m.str().substr(0, idx);
		std::string val = m.str().substr(idx+1);

		attr.erase(std::remove(attr.begin(), attr.end(), ' '), attr.end());
		val.erase(std::remove(val.begin(), val.end(), ' '), val.end());
		val.erase(std::remove(val.begin(), val.end(), '"'), val.end());

		attributes.insert(std::make_pair(attr, val));

		s_currentLine = m.suffix();
	}

	return attributes;
}

std::string get_query(
	std::string& query, 
	std::map <std::string, std::vector<std::string>>& tag_heirarchy, 
	std::map<std::string, std::map<std::string, std::string>>& tag_attributes
)
{
	std::vector<std::string> tags;

	int idx = 0;
	int pos = 0;
	while ((pos = query.find_first_of('.', idx)) != std::string::npos) {
		tags.push_back(query.substr(idx, pos - idx));
		idx = (pos + 1);
	} 

	pos = query.find_first_of('~');
	tags.push_back(query.substr(idx, pos - idx));

	std::string attr = query.substr(pos + 1);

	for (std::string t : tags) {
		if (tag_attributes.find(t) == tag_attributes.end()) {
			return "Not Found!";
		}
	}

	for (int i = 0; i < tags.size() - 1; ++i) {
		if (!get_qeuery_helper(tags[i], tags[i + 1], tag_heirarchy)) {
			return "Not Found!";
		}
	}

	if (tags.size() == 1) {
		for (auto const& it : tag_heirarchy) {
			if (it.first != tags[0]) {
				for (auto const& s : it.second) {
					if (s == tags[0]) {
						return "Not Found!";
					}
				}
			}
		}
	}

	if (tag_attributes.at(tags.at(tags.size() - 1)).find(attr) == tag_attributes.at(tags.at(tags.size() - 1)).end()) {
		return "Not Found!";
	}
	else {
		return tag_attributes.at(tags.at(tags.size() - 1))[attr];
	}
}

bool get_qeuery_helper(
	std::string& parent_tag,
	std::string& sub_tag,
	std::map <std::string, std::vector<std::string>>& tag_heirarchy
)
{
	if (tag_heirarchy.find(parent_tag) != tag_heirarchy.end()) {
		for (auto const& s : tag_heirarchy[parent_tag]) {
			if (sub_tag == s) return true;
		}
	}

	return false;
}