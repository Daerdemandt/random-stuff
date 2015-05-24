#!/usr/bin/env python3
from functools import reduce
from operator import or_

def read_input():
	with open("input.txt") as input_file:
		number_of_people, number_of_plays = (int(s) for s in input_file.readline().strip().split())
		plays = [tuple(int(s) for s in line.strip().split()) for line in input_file]
		return number_of_people, number_of_plays, plays
				
def get_plays_results(number_of_people, plays):
	ownage = {i + 1:set() for i in range(number_of_people)}
	for play in plays:
		winner, loser = None, None
		if play[2] == 1:
			winner, loser = play[0], play[1]
		if play[2] == 2:
			winner, loser = play[1], play[0]
		if play[2] == 3:
			continue
		ownage[winner].add(loser)
	return ownage

def get_cycles(edges):
	reachables = edges
	something_was_updated = True
	while something_was_updated:
		something_was_updated = False
		for starting_node in edges:
			reachables_extended = reduce(or_, (reachables[node] for node in reachables[starting_node]), set())
			if (reachables_extended - reachables[starting_node]):
				something_was_updated = True
			reachables[starting_node] |= reachables_extended
	all_cycle_nodes = set(node for node in reachables if node in reachables[node])
	result = []
	while all_cycle_nodes:
		for node in all_cycle_nodes:
			break
		cycle = all_cycle_nodes & reachables[node]
		all_cycle_nodes -= cycle
		result.append(cycle)
	return result

def translate_relationships(relationships, cycles):
	for node in relationships:
		for cycle in cycles:
			if relationships[node] & cycle:
				relationships[node] -= cycle
				relationships[node].add(min(cycle))
	for cycle in cycles:
		only_node = min(cycle)
		only_node_rels = relationships[only_node] - set([only_node])
		for node in cycle:
			del relationships[node]
		relationships[only_node] = only_node_rels


def get_max_company_size(number_of_people, number_of_plays, plays):
	relationships = get_plays_results(number_of_people, plays)
	cycles = get_cycles(relationships)
	translate_relationships(relationships, cycles)
	all_nodes = set(relationships)
	subordinate_nodes = reduce(or_, relationships.values(), set())
	root_nodes = all_nodes - subordinate_nodes
	cycle_nodes_sizes = {min(cycle):len(cycle) for cycle in cycles}
	def node_size(node):
		if node in cycle_nodes_sizes:
			return cycle_nodes_sizes[node]
		return 1
		
	boss_node_size = node_size(min(root_nodes, key=node_size))
	return number_of_people - boss_node_size + 1

def main():
	print(get_max_company_size(*read_input()))

main()
