#ifndef Movie_HPP
#define Movie_HPP

#include <vector>
#include <string>

using namespace std;

class Movie {
	public:
		Movie(string nameIn, int yearIn):name(nameIn), year(yearIn) {}
	
		void addActor(const string & actorName) {
			actors.push_back(actorName);
		}

		string getMovie() {
			return this->name;
		}

		int getYear() {
			return this->year;
		}

	protected:
		string name;
		int year;
		vector<string> actors;


};
#endif
