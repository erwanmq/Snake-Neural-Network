#include "NeuralNetwork.h"
#include <iostream>


NeuralNetwork::NeuralNetwork(const std::vector<int> layer)
	:m_layer{ layer }
{
	initWeights();
	initBiases();
}

// copy constructor
NeuralNetwork::NeuralNetwork(const NeuralNetwork& other)
{
	if (this != &other)
	{
		m_weights = other.m_weights;
		m_biases = other.m_biases;
		m_layer = other.m_layer;
	}
	
}

NeuralNetwork::~NeuralNetwork() = default;


Eigen::MatrixXd NeuralNetwork::activationFunction(Eigen::MatrixXd x)
{
	// sigmoïd activation function
	return (1.0 / (1.0 + (-x).array().exp()));
}

// call the forwardPropagation, get all the result, search for the best probability in the 4 outputs
// and return the direction the neural network wants
Direction NeuralNetwork::findDirection(Eigen::MatrixXd input)
{
	forwardPropagation(input); // call the forward propagation
	double compare{ -1 }; // create a double value that keep track of the highest probability
	int index{}; // create an int value to get the index of the best probability

	// for each value of the 4 outpus
	for (int i{ 0 }; i < static_cast<int>(m_activation.back().size()); i++)
	{
		// check if the current value is higher than the previous
		if (compare < m_activation.back().coeff(i))
		{
			// if yes, store his index and value
			compare = m_activation.back().coeff(i);
			index = i;
		}
	}
	// clear all the result of all the activations
	m_activation.clear();

	// create a temporary variable of direction that will be return
	Direction direction;
	// switch case of the index of the highest probability
	switch (index)
	{
	case 0: direction = Direction::RIGHT; break;
	case 1: direction = Direction::LEFT; break;
	case 2: direction = Direction::TOP; break;
	case 3: direction = Direction::BOTTOM; break;
	default: direction = Direction::RIGHT; break;
	}

	return direction;	
	
}

const std::vector<Eigen::MatrixXd> NeuralNetwork::getWeigths()
{
	return m_weights;
}

const std::vector<Eigen::VectorXd> NeuralNetwork::getBiases()
{
	return m_biases;
}

void NeuralNetwork::setWeigths(std::vector<Eigen::MatrixXd> new_weights)
{
	m_weights.clear();
	m_weights = new_weights;
}

void NeuralNetwork::setBiases(std::vector<Eigen::VectorXd> new_biases)
{
	m_biases.clear();
	m_biases = new_biases;
}

void NeuralNetwork::init()
{
	m_weights.clear();
	m_biases.clear();
	initWeights();
	initBiases();
}


void NeuralNetwork::forwardPropagation(const Eigen::MatrixXd& input)
{
	// get the first result with input
	m_activation.push_back(activationFunction((m_weights[0] * input).colwise() + m_biases[0]));
	// for each layers
	for (int i{ 0 }; i < static_cast<int>(m_layer.size() - 2); i++)
	{
		// calcul each result
		m_activation.push_back(activationFunction((m_weights[i + 1] * m_activation[i]).colwise() + m_biases[i + 1]));
	}
}

void NeuralNetwork::initWeights()
{
	for (int i{ 1 }; i < static_cast<int>(m_layer.size()); i++)
	{
		// create random value of weights
		m_weights.push_back(Eigen::MatrixXd::Random(m_layer[i], m_layer[i - 1]));
	}
}

void NeuralNetwork::initBiases()
{
	for (int i{ 1 }; i < static_cast<int>(m_layer.size()); i++)
	{
		// create random value of biases
		m_biases.push_back(Eigen::VectorXd::Random(m_layer[i], 1));
	}
}
