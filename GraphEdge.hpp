#ifndef GRAPHEDGE_HPP
#define GRAPHEDGE_HPP

#include <string>
using namespace std;

class GraphEdge {
	public:
		GraphEdge(void* actor1, void* actor2, string movieName, int weight):firstActor(actor1), secondActor(actor2), name(movieName), weight(weight) {
			deleted = false;
		}
		int getWeight() {
			return this->weight;
		}

		void* getActor(void* from) {
			return (this->firstActor == from) ? secondActor : firstActor;
		}

		string getName() {
			return this->name;
		}
	
		void removeEdge() {
			deleted = true;
		}
	
		bool isRemoved() {
			return deleted;
		}
	
		void* firstActor;
		void* secondActor;
	private:
		string name;
		int weight;
		bool deleted;
};


#endif
