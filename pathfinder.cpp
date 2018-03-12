#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <list>
#include <queue>
#include <algorithm>
#include <chrono>
#include "ActorGraph.hpp"


using namespace std;

typedef bool (*comp)(Actor*, Actor*);

class Compare
{
	public:
		bool operator() (Actor* &q1, Actor* &q2) {
			return q1->dist > q2->dist;
		}
};

bool myCompare (const Actor * q1, const Actor * q2) {
	return q1->dist > q2->dist;
}


string findPairPath(string actor1, string actor2, unordered_map<string, Actor*> * actorData, bool weighted) {
	auto it = actorData->begin();
	auto itE = actorData->end();
	list<Actor*> queue;
	priority_queue<Actor*, vector<Actor*>, Compare> queue1;
	//	vector<GraphEdge*> queue1;
	vector<GraphEdge*>::iterator friendsIter;	
	vector<GraphEdge*>::iterator friendsIterE;

	while(it != itE) {
		it->second->reset();
		it++;
	}

	if(!weighted) {
		it = actorData->find(actor1);
		Actor* start = (*it).second;
		Actor* current;
		Actor* other;
		queue.push_back(start);
		start->visit();
		while(!queue.empty()) {
			current = queue.front();
			queue.pop_front();
			friendsIter = current->friends.begin();
			friendsIterE = current->friends.end();
			while(friendsIter != friendsIterE) {
				other = (Actor*)((*friendsIter)->getActor(current));
				if(!other->isVisited()) {
					queue.push_back(other);
					other->visit();
					other->prev = *friendsIter;
					if(other->name == actor2)
						goto endloops;
				}
				friendsIter++;
			}
		}
	}

	else {		
		auto actorFind = actorData->find(actor1);
		Actor* start = actorFind->second;
		start->dist = 0;
		Actor* current;
		Actor* other;
		queue1.push(start);
		int c;

		while(!queue1.empty()) {
			current = queue1.top();
			queue1.pop();
			if(current->name == actor2)
				goto endloops;

			if(!current->isVisited()) {
				current->visit();
				friendsIter = current->friends.begin();
				friendsIterE = current->friends.end();
				while(friendsIter != friendsIterE) {
					other = (Actor*)((*friendsIter)->getActor(current));
					if(!other->isVisited()) {
					c = current->dist + (*friendsIter)->getWeight();
					if(c < other->dist) {
						other->dist = c;
						queue1.push(other);
						other->prev = *friendsIter;
					}}
					friendsIter++;
				}
			}
		}
cout << start->dist << endl;

	}

endloops:
list<string> result;
auto findActor = actorData->find(actor2);
Actor* tracePath = findActor->second;
result.push_front(tracePath->name);
GraphEdge* previous = tracePath->prev;
stringstream ss;

while(previous != nullptr) {
	result.push_front(previous->getName());
	tracePath = (Actor*)(previous->getActor(tracePath));
	result.push_front(tracePath->name);
	previous = tracePath->prev;
}

auto finalIter = result.cbegin();
auto finalIterE = result.end();
if(result.front() != actor1) {
	ss << endl;
	return ss.str();
}
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
void findPaths(const char* in_filename, const char* out_filename, unordered_map<string, Actor*> * actorData, bool weighted) {
	ifstream infile(in_filename);
	ofstream outfile(out_filename);
	outfile <<"(actor)--[movie#@year]-->(actor)--..." << endl;
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

		string k = findPairPath(actor_name, actor2_name, actorData, weighted);
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
	auto t1 = chrono::high_resolution_clock::now();
	if(weighted == "u")
		graph->loadFromFile(pathName, false);
	if(weighted == "w")
		graph->loadFromFile(pathName, true);
	auto t2 = chrono::high_resolution_clock::now();
	cout << chrono::duration_cast<chrono::milliseconds>(t2-t1).count() << endl;
	t1 = chrono::high_resolution_clock::now();
	findPaths(actors, output,graph->getActors(), (weighted == "w"));
	t2 = chrono::high_resolution_clock::now();
	cout << chrono::duration_cast<chrono::milliseconds>(t2-t1).count() << endl;
	delete(graph);
}

