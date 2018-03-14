#include <chrono>
#include "MatrixMultiply.hpp"
#include <string>
#include "ActorGraph.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<string> findConnections(const vector<string>& input, ActorGraph* & graph) {
	//Get an array of actors alphabetically sorted
	//Build the adjacency matrix
	//Square it
	//For each actor in the adjacency matrix choose the actors that have 1 and select top 4
	//For each actor in the adjacency matrix choose the actors that have 0 and select top 4

	auto it = graph->getActors()->begin();
	auto itE = graph->getActors()->end();
	vector<string> actorNames;
	vector<string>::iterator it1;
	vector<string>::iterator it2;
	while(it != itE) {
		actorNames.push_back(it->first);
		it++;
	}

	vector<vector<int>> adjacency(actorNames.size());

	for (int i = 0; i<actorNames.size(); i++)
		adjacency[i] = vector<int>(actorNames.size());

	sort(actorNames.begin(), actorNames.end());
	it = graph->getActors()->begin();
	int indexH, indexV;
	vector<string>::iterator itBegin = actorNames.begin();
	Actor* f;
	cout << "Starting adjacency" << endl;
	while(it != itE) {
		it1 = equal_range(actorNames.begin(), actorNames.end(), it->first).first;
		indexH = it1-itBegin;
		for(GraphEdge* x: it->second->friends) {
			f = (Actor*)(x->getActor(it->second));
			it2 = equal_range(actorNames.begin(), actorNames.end(), f->name).first;
			indexV = it2 - itBegin;
			adjacency[indexV][indexH] = 1;
		}
		it++;
	}
	cout << "End adjacency" << endl;
	vector<string>::iterator it3;
	vector<vector<int>> inputAdj(input.size());
	int i = 0;
	int actorIndex;
	for (string actor : input) {
		it3 = equal_range(actorNames.begin(), actorNames.end(), actor).first;
		actorIndex = it3-itBegin;
		inputAdj[i] = adjacency[actorIndex];
		i++;
	}
	cout << "Number of inputs: " << input.size() << endl;
	cout << "Number of rows: " << inputAdj.size() << endl;
	cout << "Adjacent rows: " << adjacency.size() << endl;
	cout << "Adjacent columns: " << adjacency[0].size() << endl;
	MatrixOperations<int> k(inputAdj, adjacency);
	cout << "Starting multiply" << endl;
	auto t1 = chrono::high_resolution_clock::now();
	auto productM = k.matrixMultiply();
	auto t2 = chrono::high_resolution_clock::now();
	cout << "End multiply" << endl;
	cout << "Time taken: "<<chrono::duration_cast<chrono::milliseconds>(t2-t1).count() << endl;
	cout << "Output dimension vertical: " << productM.size() << " Horizontal: " << productM[0].size() << endl;

	int indexCheck, max;
	vector<string> collaborated;
	vector<string> notCollaborated;
	string name;
	string current;
	vector<string> output;
	int x = 0;
	
	for (int iV = 0; iV < productM.size(); iV++) {
		collaborated.clear();
		notCollaborated.clear();
		while(collaborated.size() != 4 || notCollaborated.size() != 4) {
			max = 0;
			for(int jV = 0; jV < productM[0].size(); jV++) {
				if(productM[iV][jV] > max) {
					max = productM[iV][jV];
					indexCheck = jV;
				}
			}
			name = actorNames[indexCheck];
			current = input[x];
			if(name == current) goto skip;
			if(inputAdj[iV][indexCheck] == 0 && notCollaborated.size() != 4)
				notCollaborated.push_back(name);
			else if(inputAdj[iV][indexCheck] == 1 && collaborated.size() != 4)
				collaborated.push_back(name);
			skip:
				productM[iV][indexCheck] = -1;
		}
		x++;
		for (string outputS : collaborated)
			output.push_back(outputS);
		for (string outputS : notCollaborated)
			output.push_back(outputS);
		
	}
	return output;
}

void printPairs(const char* in_filename, const char* out_filename1, const char* out_filename2, ActorGraph* & graph) {
	ifstream infile(in_filename);
	bool secondPass = false;

	ofstream outfile(out_filename1);
	outfile <<"Actor1, Actor2, Actor3, Actor4" << endl;
	ofstream outfile2(out_filename2);
	outfile2 <<"Actor1, Actor2, Actor3, Actor4" << endl;
	bool have_header = false;
	vector <string> record;
	vector <string> input;
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
		if (record.size() != 1) {
			continue;
		}
		input.push_back(record[0]);
		//string actor2_name(record[1]);



	}
	if (!infile.eof()) {
		cerr << "Failed to read " << in_filename << "!\n";
		return;
	}
	vector<string> output = findConnections(input, graph);
	auto it = output.begin();
	auto itE = output.end();

	while(it != itE) {
		for(int i = 0; i<4; i++) {
			outfile << *it << '\t';
			it++;
		}
		outfile << '\n';
		for(int j = 0; j<4; j++) {
			outfile2 << *it << '\t';
			it++;
		}
		outfile2 << '\n';
	}
	
	infile.close();
	outfile.close();
	outfile2.close();
}




int main(int argc, char** argv) {
	if(argc != 5)
		exit(-1);
	char* pathName = argv[1];
	char* actors = argv[2];
	char* output1 = argv[3];
	char* output2 = argv[4];

	ActorGraph* graph = new ActorGraph();
	auto t1 = chrono::high_resolution_clock::now();
	graph->loadFromFile(pathName, false);
	printPairs(actors, output1, output2, graph);
	auto t2 = chrono::high_resolution_clock::now();
	cout << chrono::duration_cast<chrono::milliseconds>(t2-t1).count() << endl;
	delete(graph);
}
