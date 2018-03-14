#include <chrono>
#include "MatrixMultiply.hpp"
#include <string>
#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

void getCore(Actor* start, int& N) {
	if (start->degree < N) {
		for (GraphEdge* x : start->friends) {
			if (x->isRemoved()) continue;
			x->removeEdge();
			((Actor*)(x->getActor(start)))->degree--;
		}
		start->visit();
	}
	
	for (auto it = start->friends.begin(); it != start->friends.end(); it++) {
		if (!(*it)->isRemoved())
		getCore((Actor*)((*it)->getActor(start)), N);
	}

	return;

}

void buildDegree(ActorGraph*& graph) {
	auto actorBase = graph->getActors();
	Actor* current;
	auto it = actorBase->begin();
	auto itE = actorBase->end();
	int i;

	while(it != itE) {
		current = it->second;
		i = 0;
		for(GraphEdge* y : current->friends)
			i++;
		current->degree = i;
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
	getCore((graph->getActors()->begin())->second, i);

	ofstream outfile(output1);
	auto it = graph->getActors()->begin();
	auto itE = graph->getActors()->end();

	while (it != itE) {
		if (it->second->isVisited()) continue;
		outfile << it->first << '\n';
	}	
	outfile.close();
	auto t2 = chrono::high_resolution_clock::now();
	cout << "Total time: "<<chrono::duration_cast<chrono::milliseconds>(t2-t1).count() << endl;
	delete(graph);
}
