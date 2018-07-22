#pragma once
#include <cmath>

double PI = 3.14159265359;

class FunctionToOpt {
private:
	int numberOfParameters;
	double bottomConstrain;
	double topConstrain;
public:
	explicit FunctionToOpt(int numOfParam, double lowConstr, double hiConstr) : numberOfParameters(numOfParam), bottomConstrain(lowConstr), topConstrain(hiConstr) { }
	
	virtual double evaluateFitness(std::vector<double> popMember) = 0;

	int getNumOfParameters() { return numberOfParameters; }
	double getBottomConstrain() { return bottomConstrain; }
	double getTopConstrain() { return topConstrain; }
	void setNumOfParameters(int value) { numberOfParameters = value; }
};

class Griewank : public FunctionToOpt {
public:
	using FunctionToOpt::FunctionToOpt;
	~Griewank() {}
	double evaluateFitness(std::vector<double> popMember) override {
		double sigma = 0;
		double capPi = 1;
		for (int i = 1; i <= getNumOfParameters(); i++) {
			sigma += pow(popMember[i - 1], 2);
			capPi *= cos(popMember[i - 1] / sqrt(i));
		}
		return 1 + (1 / 400) * sigma - capPi;
	}
};
class Easom : public FunctionToOpt { // Nieskalowana (D = 2)
public:
	explicit Easom(int numOfParam, double lowConstr, double hiConstr) : FunctionToOpt(numOfParam, lowConstr, hiConstr) { 
		setNumOfParameters(2); 
	}
	~Easom() {}
	double evaluateFitness(std::vector<double> popMember) override {
		return -cos(popMember[0]) * cos(popMember[1]) * exp(-1 * pow((popMember[0] - PI), 2) - pow((popMember[1] - PI), 2));
	}
};
class Rosenbrock : public FunctionToOpt {
public:
	using FunctionToOpt::FunctionToOpt;
	double evaluateFitness(std::vector<double> popMember) override {
		double sum = 0;
		for (int i = 0; i < getNumOfParameters() - 1; i++) {
			sum += 100 * pow(pow(popMember[i], 2) - popMember[i + 1], 2) + pow(popMember[i] - 1, 2);
		}
		return sum;
	}
};