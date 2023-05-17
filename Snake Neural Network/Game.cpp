#include "Game.h"
#include <iostream>


Game::Game(int number_population)
	:m_number_population{ number_population }
{
	// create a unique_ptr of the class Text
	m_text_generation = std::make_unique<Text>(std::string("DejaVuSansMono.ttf"), sf::String("Génération : 1\nPopulation: 1/" + std::to_string(m_number_population)
		+ "\nBest score: 0"), sf::Vector2f(WIDTH + 20.f, 30.f));

	// these 7 calls resize the vector to the same number as the number of players 
	m_snakes.resize(number_population);
	m_players.resize(number_population);
	m_lost.resize(number_population);
	m_points.resize(number_population);
	m_fruits.resize(number_population);
	m_step.resize(number_population);
	m_grid.resize(number_population);

	// fill m_lost, m_points and m_step to false, 0 and 0 respectively
	std::fill(m_lost.begin(), m_lost.end(), false);
	std::fill(m_points.begin(), m_points.end(), 0);
	std::fill(m_step.begin(), m_step.end(), 0);
	

	
	for (int i{ 0 }; i < static_cast<int>(m_fruits.size()); i++)
	{
		m_fruits[i].setSize(m_snakes[0].getSize());
		m_fruits[i].setRandomPosition();
		// We don't want that the fruit spawn the front of the snake's head.
		while (m_fruits[i].getPosition().y == m_snakes[i].getPosition().y && m_fruits[i].getPosition().x >= m_snakes[i].getPosition().x)
		{
			m_fruits[i].setRandomPosition();
		}
	}
	
	// initialise all players to a unique_ptr of Neural Network 
	for(auto& player: m_players)
		player = std::make_unique<NeuralNetwork>(m_player_layers);
}

Game::~Game() = default;

bool Game::checkFruit(Snake& snake, int index)
{
	// check if the head of the snake is inside the fruit position
	if (snake.getHead().getGlobalBounds().intersects(m_fruits[index].getFruit().getGlobalBounds()))
		return true;

	return false;
}

void Game::resetFruit(int index)
{
	// if the fruit has been touched, it is reset to a new position and the numbers of point are increased.
	m_fruits[index].setRandomPosition();
}

bool Game::checkLost(Snake& snake)
{
	// return a boolean value if the snake lost or not
	if (snake.isHeadInBody() || checkWall(snake))
		return true;
		
	return false;
}

bool Game::checkWall(Snake& snake)
{
	// check if the head touched a wall
	if (static_cast<int>(snake.getHeadPosition().x / CONV_CASE_WIDTH) == 0 || static_cast<int>(snake.getHeadPosition().y / CONV_CASE_HEIGHT) == 0
		|| static_cast<int>(snake.getHeadPosition().x / CONV_CASE_WIDTH) == NB_CASE_WIDTH - 1 || static_cast<int>(snake.getHeadPosition().y / CONV_CASE_HEIGHT) == NB_CASE_HEIGHT - 1)
		return true;

	return false;
}

// function that return a MatrixXd that stores the different values of the different directions of the snake
Eigen::MatrixXd Game::getGameData(Snake& snake, int index)
{
	Eigen::MatrixXd data(12, 1); // create a matrix of (4, 1) (4 directions)
	// supposed there is nothing (all 0)
	data << 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0; // right side, left side, top side, bot side for walls, fruits, tail

	int pos_x = static_cast<int>(snake.getHead().getPosition().x / CONV_CASE_WIDTH);
	int pos_y = static_cast<int>(snake.getHead().getPosition().y / CONV_CASE_HEIGHT);
	if (m_grid[index].isThereAWall(pos_x, pos_y, Direction::RIGHT))
	{
		data(0) = 1;
	}
	if (m_grid[index].isThereAWall(pos_x, pos_y, Direction::LEFT))
	{
		data(1) = 1;
	}
	if (m_grid[index].isThereAWall(pos_x, pos_y, Direction::TOP))
	{
		data(2) = 1;
	}
	if (m_grid[index].isThereAWall(pos_x, pos_y, Direction::BOTTOM))
	{
		data(3) = 1;
	}

	if (m_grid[index].isThereAFruit(pos_x, pos_y, Direction::RIGHT))
	{
		data(4) = 1;
	}
	if (m_grid[index].isThereAFruit(pos_x, pos_y, Direction::LEFT))
	{
		data(5) = 1;
	}
	if (m_grid[index].isThereAFruit(pos_x, pos_y, Direction::TOP))
	{
		data(6) = 1;
	}
	if (m_grid[index].isThereAFruit(pos_x, pos_y, Direction::BOTTOM))
	{
		data(7) = 1;
	}

	if (m_grid[index].isThereMyBody(pos_x, pos_y, Direction::RIGHT) && snake.getDirection() != Direction::LEFT)
	{
		data(8) = 1;
	}
	if (m_grid[index].isThereMyBody(pos_x, pos_y, Direction::LEFT) && snake.getDirection() != Direction::RIGHT)
	{
		data(9) = 1;
	}
	if (m_grid[index].isThereMyBody(pos_x, pos_y, Direction::TOP) && snake.getDirection() != Direction::BOTTOM)
	{
		data(10) = 1;
	}
	if (m_grid[index].isThereMyBody(pos_x, pos_y, Direction::BOTTOM) && snake.getDirection() != Direction::TOP)
	{
		data(11) = 1;
	}

	// return the "input" MatrixXd
	return data;
}

std::vector<Eigen::MatrixXd> Game::crossOverWeights(const std::vector<Eigen::MatrixXd>& second, const std::vector<Eigen::MatrixXd>& first)
{
	std::vector<Eigen::MatrixXd> temp = second;
	
	int random{ static_cast<int>(rand() % second.size()) }; // find a random layer of weights

	for (int i{ random }; i < second.size(); i++)
	{
		temp[i] = first[i]; // change every layers after by another one
	}
	return temp;
}
// similar to the crossOverWeights
std::vector<Eigen::VectorXd> Game::crossOverBiases(const std::vector<Eigen::VectorXd>& second, const std::vector<Eigen::VectorXd>& first)
{
	std::vector<Eigen::VectorXd> temp = second;

	int random{ static_cast<int>(rand() % second.size()) };

	for (int i{ random }; i < second.size(); i++)
	{
		temp[i] = first[i];
	}

	return temp;
}

void Game::setNewGeneration()
{
	// create a vector of pair and links the players to their points
	std::vector<std::pair<std::unique_ptr<NeuralNetwork>, int>> paired_vector;
	// loop for add to the previous vector the pairs
	for (int i{ 0 }; i < m_number_population; i++)
	{
		// make_pair to create a pair and std::move to move the content of each unique_ptr to the paired_vector
		paired_vector.push_back(std::make_pair(std::move(m_players[i]), m_points[i]));
	}
	// sort the paired_vector relatively to the points
	std::sort(paired_vector.begin(), paired_vector.end(), [](const auto& a, const auto& b) { return a.second > b.second; });


	int best{ static_cast<int>(m_number_population * 0.3f) }; // We keep 30% of the bests
	int new_player{ static_cast<int>(m_number_population * 0.2f) }; // We have 20% of new players
	// The lasts will be reproduced by the firsts

	// loop through the reproduced snakes
	for (int i{ best }; i < m_number_population - new_player; i++)
	{
		int random1{ rand() % best + 1 }; // find one parents from the best snakes
		int random2{};
		do
		{
			random2 = rand() % best + 1; // find another which is not the same
		} while(random2 == random1);

		// crossOver weights and biases
		paired_vector[i].first.get()->setWeigths(crossOverWeights(paired_vector[random2].first.get()->getWeigths(), paired_vector[random1].first.get()->getWeigths()));
		paired_vector[i].first.get()->setBiases(crossOverBiases(paired_vector[random2].first.get()->getBiases(), paired_vector[random1].first.get()->getBiases()));

	}

	// create new players
	for (int i{ m_number_population - new_player }; i < m_number_population; i++)
	{
		paired_vector[i].first.get()->init();
	}


	// loop through all snakes and apply some mutations
	for (int i{ 0 }; i < m_number_population; i++)
	{
		// 5% to have a mutation
		int random{ rand() % 100 };
		if (random/100.f <= 0.05) 
		{
			paired_vector[i].first.get()->setWeigths(mutationWeights(paired_vector[i].first.get()->getWeigths()));
		}	
		random = rand() % 100;
		if (random/100.f <= 0.05)
		{
			paired_vector[i].first.get()->setBiases(mutationBiases(paired_vector[i].first.get()->getBiases()));
		}
	}
	
	
	// reassign each value to the m_players vector
	for (int i{ 0 }; i < m_players.size(); i++)
	{
		m_players[i] = std::move(paired_vector[i].first);
	}

	// shuffle the vector
	std::shuffle(m_players.begin(), m_players.end(), std::random_device());

	m_generation++;
	m_best_score = 0;
	// reset the class Game using the operator=
	*this = { m_number_population };
}

std::vector<Eigen::MatrixXd> Game::mutationWeights(const std::vector<Eigen::MatrixXd>& mutation_layer)
{
	std::vector<Eigen::MatrixXd> temp = mutation_layer;
	for (int i{ 0 }; i < static_cast<int>(mutation_layer.size()); i++)
	{
		int random_index{ std::rand() % (static_cast<int>(mutation_layer.size() + 1)) };
		int random_value{ std::rand() % 10000 };

		temp.at(i).coeffRef(random_index) = random_value / 10000.f;
	}
	return temp;
}

std::vector<Eigen::VectorXd> Game::mutationBiases(const std::vector<Eigen::VectorXd>& mutation_layer)
{
	std::vector<Eigen::VectorXd> temp = mutation_layer;
	for (int i{ 0 }; i < static_cast<int>(temp.size()); i++)
	{
		int random_index{ std::rand() % (static_cast<int>(temp[i].size()))};
		int random_value{ std::rand() % 10000 };

		temp.at(i).coeffRef(random_index) = random_value / 10000.0;
	}
	return temp;
}

void Game::update()
{
	for (int i{ 0 }; i < m_number_population; i++)
	{
		m_grid[i].initObjectsGrid();
		// check if the snake not lost
		if (!checkLost(m_snakes[i]) && m_step[i] <= 300)
		{
			for (const auto& position : m_snakes[i].getAllBodyPosition())
				m_grid[i].addObjectInTheGrid(3, static_cast<int>(position.x), static_cast<int>(position.y));
			m_grid[i].addObjectInTheGrid(2, static_cast<int>(m_fruits[i].getPosition().x), static_cast<int>(m_fruits[i].getPosition().y));


			// get the data from the game
			Eigen::MatrixXd input_nn{ getGameData(m_snakes[i], i) };
			// find the direction the neural network wants
			Direction direction{ (m_players[i]->findDirection(input_nn)) };
			// apply the direction to the snake class
			m_snakes[i].movements(direction);
			// update the snake
			m_snakes[i].update();

			// check if the head touched a fruit
			if (checkFruit(m_snakes[i], i))
			{
				// if yes, reset the fruit and add a body to the snake
				resetFruit(i);
				m_snakes[i].addBody();
				m_points[i]++;
				m_step[i] = 0;
			}

			m_step[i]++;

		}
		else
		{
			if (m_step[i] >= 300)
			{
				m_points[i]--;
			}
			m_lost[i] = true; // turn to true the value and the snake can't move anymore
		}
			
		
	}

	if (std::all_of(m_lost.begin(), m_lost.end(), [](bool v) { return v; }))// if the snake lost
	{
		setNewGeneration(); // set the new generation and reset the class
	}
	
	
	// each time the function is called and after all movements, search for the first snake of the game
	m_index_first_snake = static_cast<int>(std::distance(m_points.begin(), std::max_element(m_points.begin(), m_points.end())));
	// and print his score
	if (m_best_score <= m_points[m_index_first_snake])
		m_best_score = m_points[m_index_first_snake];

	m_text_generation->setString(sf::String("Génération : " + std::to_string(m_generation) + "\nPopulation :" + std::to_string(m_number_population)
	+ "\nBest score: " + std::to_string(m_best_score)));
}

void Game::drawGame(sf::RenderWindow& win)
{
	for (int i{ 0 }; i < m_number_population; i++)
	{
		if (!m_lost[i])
		{
			m_snakes[i].draw(win);
			m_fruits[i].drawFruit(win);
		}
		
	}
	m_text_generation->drawText(win); // draw the text

	
	
	//m_grid[m_current_player].printGrid();
	//m_grid[0].drawGrid(win);
}

// operator= to reset the game
Game& Game::operator=(const Game& other)
{
	if (this != &other)
	{
		m_snakes = other.m_snakes; // reset the snakes
		m_fruits = other.m_fruits; // reset the fruits
		m_points = other.m_points; // reset the points
		m_lost = other.m_lost; // reset the lost vector
		m_step = other.m_step;
	}

	return *this;
}
