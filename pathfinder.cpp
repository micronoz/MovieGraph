#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <list>
#include "ActorGraph.hpp"
using namespace std;
string findPairPath(string actor1, string actor2, unordered_map<string, Actor*> * actorData, unordered_map<string, Movie*> * movieData) {
	auto it = actorData->begin();
	auto itE = actorData->end();
	list<Actor*> queue;

	while(it != itE) {
		it->second->reset();
		it++;
	}

	auto findActor = actorData->find(actor1);
	findActor->second->visit();
	findActor->second->dist = 0;
	queue.push_back(findActor->second);
	Actor* current = findActor->second;
	unordered_map<string, Movie*>::iterator movieIter;
	vector<Actor*>::iterator adjacentActors;
	vector<Actor*>::iterator adjacentActorsE;
	auto iter = current->movies.begin();
	auto iterE = current->movies.end();
	while(!queue.empty()) {
		current = queue.front();
		current->visit();
		queue.pop_front();

		iter = current->movies.begin();
		iterE = current->movies.end();
		while(iter != iterE) {
			movieIter = movieData->find(*iter);
			adjacentActors = movieIter->second->actors.begin();
			adjacentActorsE = movieIter->second->actors.end();
			while(adjacentActors != adjacentActorsE) {
				if((*adjacentActors)->isVisited() == false) {
					queue.push_back(*adjacentActors);
					(*adjacentActors)->visit();
					(*adjacentActors)->prev = pair<Actor*, string>(current, movieIter->second->name + "#@" + to_string(movieIter->second->year));
				}
				adjacentActors++;
			}
			iter++;
		}
	}

	list<string> result;
	findActor = actorData->find(actor2);
	Actor* tracePath = findActor->second;
	result.push_front(tracePath->name);
	pair<Actor*, string> previous = tracePath->prev;
	stringstream ss;
	
	while(previous != pair<Actor*, string> (nullptr, "")) {
		result.push_front(previous.second);
		result.push_front(previous.first->name);
		previous = previous.first->prev;
	}
	
	auto finalIter = result.cbegin();
	auto finalIterE = result.end();

	while(finalIter != finalIterE) {
		ss << "(" << *finalIter << ")";
		finalIter++;
		if(finalIter != finalIterE) {
			ss << "--[" << *finalIter << "]-->";
			finalIter++;
		}
	}	

	return ss.str();

}
void findPaths(const char* in_filename, const char* out_filename, unordered_map<string, Actor*> * actorData, unordered_map<string, Movie*> * movieData) {
	ifstream infile(in_filename);
	ofstream outfile(out_filename);
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

		string k = findPairPath(actor_name, actor2_name, actorData, movieData);
		outfile << k << endl;

		
	}
	if (!infile.eof()) {
		cerr << "Failed to read " << in_filename << "!\n";
		return;
	}
	infile.close();
	outfile.close();
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
	findPaths(actors, output,graph->getActors(), graph->getMovies());

}

