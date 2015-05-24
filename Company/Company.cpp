/*
	Company.cpp - task #4 solution
	Uses some features introduced in C++11 ("-std=c++11" key for g++)
*/

#include <fstream> // for reading from file
#include <string> // for filename
#include <iostream> // to output result
#include <algorithm> // for set operations
#include <set>
#include <tuple>
#include <map>

using namespace std;

struct input_data {
	int number_of_people;
	int number_of_plays;
	set<tuple<int, int, int> > plays;
};

input_data read_input_data(string filename) {
	input_data * result = new input_data;
	ifstream input_file(filename);
	if (input_file.is_open()) {
		input_file >> result->number_of_people >> result->number_of_plays;
		int a, b, c;
		while (input_file >> a >> b >> c) {
			result->plays.emplace(a, b, c);
		}
	}
	return *result;
}

typedef map<int, set<int> > Dict;

Dict get_plays_result(input_data);
set< set< int> > get_cycles(Dict);
void translate_relationships(Dict &, set<set<int> > &);


Dict get_plays_results(input_data input){
	auto resultp = new Dict;
	auto result = *resultp;
	for (int i = 1; i <= input.number_of_people; i++) {
		result.emplace(i, set<int>());
	}
	for (auto play :input.plays) {
		int winner, loser;
		switch (get<2>(play)) {
			case 1:
				winner = get<0>(play);
				loser = get<1>(play);
				result[winner].insert(loser);
				break;
			case 2:
				winner = get<1>(play);
				loser = get<0>(play);
				result[winner].insert(loser);
				break;
			case 3:
				break;
			}
	}
	return result;
}

bool contains(set <int> the_set, int element) {
	return the_set.find(element) != the_set.end();
}

void substract_sets(set< int> &from, set< int> &what, set< int> &where) {
	set_difference(from.cbegin(), from.cend(), what.cbegin(), what.cend(), inserter(where, where.begin()));
}

void intersect_sets(set< int> &first, set< int> &second, set< int> &where) {
	set_intersection(first.cbegin(), first.cend(), second.cbegin(), second.cend(), inserter(where, where.begin()));
}

void remove_from_set(set <int> & from, set <int> & what) {
	for (auto to_remove: what) {
		auto position = from.find(to_remove);
		if (position != from.end()) {
			from.erase(position);
		}
	}
}

set< set< int> > get_cycles(Dict edges) {
	auto reachables = edges;
	bool something_was_updated = true;
	while (something_was_updated) {
		something_was_updated = false;
		for (auto elem : reachables) {
			int starting_node_num = elem.first;
			set <int> reachables_extended;
			for (int node_num: reachables[starting_node_num]) {
				reachables_extended.insert(reachables[node_num].cbegin(), reachables[node_num].cend());
			}
			// check if extended contains something that original does not
			set <int> diff;
			substract_sets(reachables_extended, reachables[starting_node_num], diff);
			if (not diff.empty()) {
				reachables[starting_node_num].insert(diff.cbegin(), diff.cend());
				something_was_updated = true;
			}
		}
	}
	set <int> all_cycle_nodes;
	for (auto elem: reachables) {
		if (contains(elem.second, elem.first)) {
			all_cycle_nodes.insert(elem.first);
		}
	}
	auto resultp = new set< set< int> >;
	auto result = *resultp;
	set <int> tmp;
	while (!all_cycle_nodes.empty()) {
		auto node = *all_cycle_nodes.begin();
		auto cyclep = new set <int>;
		auto cycle = *cyclep;
		intersect_sets(all_cycle_nodes, reachables[node], cycle);
		result.insert(cycle);
		remove_from_set(all_cycle_nodes, cycle);
	}
	return result;
}

void translate_relationships(Dict &relationships, set<set<int> > &cycles) {
	//We want to treat cycles as nodes, and we need to modify dict accordingly.
	//For each cycle, we'll treat its minimal node as its node (removing all other nodes)
	set <int> nodes_to_remove;
	auto add = [&] (int nodenum) {nodes_to_remove.insert(nodenum);};
	for (auto cycle: cycles) { // note that min element is first and thus omitted
		for_each(next(cycle.begin()), cycle.end(), add);
	}
	for (auto node: nodes_to_remove) {
		relationships.erase(node);
	}
	for (auto elem: relationships) {
		remove_from_set(relationships[elem.first], nodes_to_remove);
	}
}

int get_smallest_node_size(set <int> & nodes, map <int, int> sizes, int default_size = 1) {
	auto iter = nodes.begin();
	auto size = [&] (int nodenum) {return sizes.count(nodenum) ? sizes[nodenum] : default_size;};
	auto smallest_size = size(*iter);
	for (iter; iter != nodes.end(); iter++) {
		if (size(*iter) < smallest_size) {
			smallest_size = size(*iter);
		}
	}
	return smallest_size;
}

void set_subordinate_nodes(Dict & relationships, set <int> &dest) {
	auto add_to_sub = [&] (int nodenum) {dest.insert(nodenum);};
	for (auto entry: relationships) {
		for (auto num: entry.second) {
			add_to_sub(num);
		}
	}
}

void set_cycle_node_size(set < set<int> > & cycles, map<int, int> &sizes) {
	for (auto iter = cycles.begin(); iter != cycles.end(); iter++) {
		int cycle_size = (*iter).size();
		int num = *((*iter).begin());
		sizes[num] = cycle_size;
	}
}

int get_max_company_size(input_data input) {
	auto relationships = get_plays_results(input);
	auto cycles = get_cycles(relationships);
	translate_relationships(relationships, cycles);

	set <int> all_nodes;
	auto add_to_all = [&] (pair <int, set<int> > entry) {all_nodes.insert(entry.first);};
	for_each(relationships.begin(), relationships.end(), add_to_all);
	
	set <int> subordinate_nodes;
	set_subordinate_nodes(relationships, subordinate_nodes);
	
	set <int> root_nodes;
	substract_sets(all_nodes, subordinate_nodes, root_nodes);
	
	map <int, int> cycle_node_size;
	set_cycle_node_size(cycles, cycle_node_size);
	
	// Boss must be picked from smallest root node
	auto boss_node_size = get_smallest_node_size(root_nodes, cycle_node_size);
	return input.number_of_people - boss_node_size + 1;
}

int main(){
	cout << get_max_company_size(read_input_data("input.txt")) << endl;
	return 0;
}
