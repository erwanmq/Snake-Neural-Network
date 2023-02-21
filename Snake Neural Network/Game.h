#pragma once
#include "Text.h"
#include "Fruit.h"
#include "Grid.h"
#include "Snake.h"
#include "NeuralNetwork.h"
#include <memory>

class Game
{
private:
	// these 4 variables keep track of the state for each player. A snake, a fruit, points, and the state of the game for each player.
	// the vectors are the same size that the number of the player
	std::vector<Snake> m_snakes;
	std::vector<Fruit> m_fruits;
	std::vector<int> m_points;
	std::vector<bool> m_lost;

	// these 2 variables keep track of each player (they are neural networks)
	std::vector<std::unique_ptr<NeuralNetwork>> m_players;
	std::vector<int> m_player_layers{ 4, 12, 12, 4 }; // 4 entries for the 4 directions. 0 if nothing, 1 if wall and 2 if fruit

	// pointer to the Text class to print the score to the game
	std::unique_ptr<Text> m_text_generation;
	
	// variable that stores the number of player
	int m_number_player;
	// iterator of the best snake of the current game
	int m_index_first_snake;

	// variable that stores the number of step between each eating
	// m_step reaches 200, the snake has lost.
	std::vector<int> m_step;

	// number of generation
	int m_generation{ 1 };

	// grid of the game
	std::vector<Grid> m_grid;

	int m_best_score{ 0 };
	

private:
	bool checkFruit(Snake& snake, int index);
	void resetFruit(int index);
	bool checkLost(Snake& snake);
	bool checkWall(Snake& snake);
	// function that return the "input" for each neural network. It returns a (4,1) matrixXd with each value mentionned before
	Eigen::MatrixXd getGameData(Snake& snake, int index);
	// these 2 functions will mix the weights and biases of 2 neural network
	std::vector<Eigen::MatrixXd> crossOverWeights(const std::vector<Eigen::MatrixXd>& second, const std::vector<Eigen::MatrixXd>& first);
	std::vector<Eigen::VectorXd> crossOverBiases(const std::vector<Eigen::VectorXd>& second, const std::vector<Eigen::VectorXd>& first);
	// these 2 function add a mutation in each layer
	std::vector<Eigen::MatrixXd> mutationWeights(const std::vector<Eigen::MatrixXd>& mutation_layer);
	std::vector<Eigen::VectorXd> mutationBiases(const std::vector<Eigen::VectorXd>& mutation_layer);
	// function that will set the new generation of neural network
	void setNewGeneration();

public:
	Game(int number_player);
	~Game();
	Game& operator=(const Game& other);

	void update();
	void drawGame(sf::RenderWindow& win);
	
};

