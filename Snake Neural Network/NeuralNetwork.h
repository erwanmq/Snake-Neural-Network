#pragma once
#include <Eigen/Dense>
#include <vector>
#include <random>

// enumerator to keep track of each direction
enum class Direction
{
	RIGHT,
	LEFT,
	TOP,
	BOTTOM
};

class NeuralNetwork
{
private:
	std::vector<Eigen::MatrixXd> m_weights;
	std::vector<Eigen::VectorXd> m_biases;

	// vector of all the calculs to each layers
	std::vector<Eigen::MatrixXd> m_activation;

	// vector of the numbers of neurons in each layer
	std::vector<int> m_layer;

private:
	Eigen::MatrixXd activationFunction(Eigen::MatrixXd x);
	void forwardPropagation(const Eigen::MatrixXd& input);
	void initWeights();
	void initBiases();

public:
	NeuralNetwork(const std::vector<int> layers);
	NeuralNetwork(const NeuralNetwork& other);
	~NeuralNetwork();
	

	Direction findDirection(Eigen::MatrixXd input);

	// function that can return all the weights, and can be modified
	const std::vector<Eigen::MatrixXd> getWeigths();
	const std::vector<Eigen::VectorXd> getBiases();

	void setWeigths(std::vector<Eigen::MatrixXd> new_weights);
	void setBiases(std::vector<Eigen::VectorXd> new_biases);

	
	
};

