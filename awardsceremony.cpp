#include <chrono>
#include "MatrixMultiply.hpp"
#include <string>
#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

void resetConnection(Actor*& actor) {
	for (GraphEdge* k : actor->friends)
		((Actor*)(k->getActor(actor)))->considerN();
}

void removeVertex(Actor*& actor) {
	Actor* x;
	for(auto it = actor->friends.begin(); it != actor->friends.end(); it++) {
		x = ((Actor*)((*it)->getActor(actor)));
		if (!x->isConsidered())
			x->degree--;
		x->considerY();
	}
	resetConnection(actor);
}

void getCore(unordered_map<string, Actor*>* start, int& N) {
startCore:
	auto it = start->begin();
	auto itE = start->end();

	while(it != itE) {
		if ((it)->second->degree < N && !it->second->isVisited()) {
			removeVertex((it)->second);
			(it)->second->visit();
			goto startCore;
		}
		it++;
	}

}

void buildDegree(ActorGraph*& graph) {
	auto actorBase = graph->getActors();
	Actor* current;
	Actor* other;
	auto it = actorBase->begin();
	auto itE = actorBase->end();
	int i;

	while(it != itE) {
		current = it->second;
		i = 0;
		for(GraphEdge* y : current->friends) {
			other = ((Actor*)(y->getActor(current)));
			if(!other->isConsidered()) {
				i++;	
				other->considerY();
			}
		}
		current->degree = i;
		resetConnection(current);
		it++;
	}
}

int main(int argc, char** argv) {
	if(argc != 4)
		exit(-1);
	char* pathName = argv[1];
	char* core = argv[2];
	char* output1 = argv[3];

	ActorGraph* graph = new ActorGraph();
	auto t1 = chrono::high_resolution_clock::now();
	graph->loadFromFile(pathName, false);
	buildDegree(graph);
	int i;
	char * pEnd;
	i = strtol(core, &pEnd, 10);
	cout << "Starting core with degree: " << i << endl;
	getCore((graph->getActors()), i);
	cout << "End of core" << endl;

	ofstream outfile(output1);
	auto it = graph->getActors()->begin();
	auto itE = graph->getActors()->end();

	cout << "Starting write" << endl;
	vector<string> outputVec;
	outfile << "Actor" << '\n';
	while (it != itE) {
		if (!(it->second->isVisited())) { 
			outputVec.push_back(it->second->name);
		}
		it++;
	}
	sort(outputVec.begin(), outputVec.end());

	for (auto itF = outputVec.begin(); itF != outputVec.end(); itF++)
		outfile << *itF << '\n';

	cout << "End write" << endl;
	outfile.close();
	auto t2 = chrono::high_resolution_clock::now();
	cout << "Total time: "<<chrono::duration_cast<chrono::milliseconds>(t2-t1).count() << endl;
	delete(graph);
}
