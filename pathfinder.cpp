#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <list>
#include "ActorGraph.hpp"
using namespace std;
string findPairPath(string actor1, string actor2, unordered_map<string, Actor*> * actorData) {
	auto it = actorData->begin();
	auto itE = actorData->end();
	list<Actor*> queue;

	while(it != itE)
		it->second->devisit();

	auto findActor = actorData->find(actor1);
	findActor->second->visit();
	queue.push_back(findActor->second);

	while(!queue.empty()) {
		
	}

	
}
void findPaths(const char* in_filename, const char* out_filename, unordered_map<string, Actor*> * actorData) {
	ifstream infile(in_filename);
	bool have_header = false;
	vector <string> record;
	while (infile) {
		string s;

		// get the next line
		if (!getline( infile, s )) break;
		if (!have_header) {
			// skip the header
			have_header = true;
			continue;
		}
		istringstream ss( s );
		record.clear();

		while (ss) {
			string next;
			if (!getline( ss, next, '\t' )) break;
			record.push_back( next );
		}
		if (record.size() != 2) {
			continue;
		}
		string actor_name(record[0]);
		string actor2_name(record[1]);

		string pathRep = findPairPath(actor_name, actor2_name, actorData);
	}
	if (!infile.eof()) {
		cerr << "Failed to read " << in_filename << "!\n";
		return;
	}
	infile.close();
}


int main(int argc, char** argv) {
	if(argc != 5)
		exit(-1);
	char* pathName = argv[1];
	string weighted = argv[2];
	char* actors = argv[3];
	char* output = argv[4];

	ActorGraph* graph = new ActorGraph();
	if(weighted == "u")
		graph->loadFromFile(pathName, false);
	if(weighted == "w")
		graph->loadFromFile(pathName, true);
	findPaths(actors, output,graph->getActors());

}

