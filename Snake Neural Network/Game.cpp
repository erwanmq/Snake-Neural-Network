#include "Game.h"
#include <iostream>


Game::Game(int number_player)
	:m_number_player{ number_player }
{
	// create a unique_ptr of the class Text
	m_text_generation = std::make_unique<Text>(std::string("DejaVuSansMono.ttf"), sf::String("Génération : 1\nPopulation : " + std::to_string(m_number_player)), sf::Vector2f(WIDTH - 200.f, 30.f));

	// these 7 calls resize the vector to the same number as the number of players 
	m_snakes.resize(number_player);
	m_players.resize(number_player);
	m_lost.resize(number_player);
	m_points.resize(number_player);
	m_fruits.resize(number_player);
	m_step.resize(number_player);
	m_grid.resize(number_player);

	// fill m_lost, m_points and m_step to false, 0 and 0 respectively
	std::fill(m_lost.begin(), m_lost.end(), false);
	std::fill(m_points.begin(), m_points.end(), 0);
	std::fill(m_step.begin(), m_step.end(), 0);

	// create a random position for the first fruit 
	m_fruits[0].setSize(m_snakes[0].getSize());
	m_fruits[0].setRandomPosition();
	

	// all next fruits get the same value of the first (like that all fruits are in the same position)
	for (int i{ 1 }; i < static_cast<int>(m_fruits.size()); i++)
	{
		m_fruits[i] = m_fruits[0];
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
	Eigen::MatrixXd data(4, 1); // create a matrix of (4, 1) (4 directions)
	// supposed there is nothing (all 0)
	data << 0, 0, 0, 0; // right side, left side, top side, bot side

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
		data(0) = 2;
	}
	if (m_grid[index].isThereAFruit(pos_x, pos_y, Direction::LEFT))
	{
		data(1) = 2;
	}
	if (m_grid[index].isThereAFruit(pos_x, pos_y, Direction::TOP))
	{
		data(2) = 2;
	}
	if (m_grid[index].isThereAFruit(pos_x, pos_y, Direction::BOTTOM))
	{
		data(3) = 2;
	}

	if (m_grid[index].isThereMyBody(pos_x, pos_y, Direction::RIGHT) && snake.getDirection() != Direction::LEFT)
	{
		data(0) = 3;
	}
	if (m_grid[index].isThereMyBody(pos_x, pos_y, Direction::LEFT) && snake.getDirection() != Direction::RIGHT)
	{
		data(1) = 3;
	}
	if (m_grid[index].isThereMyBody(pos_x, pos_y, Direction::TOP) && snake.getDirection() != Direction::BOTTOM)
	{
		data(2) = 3;
	}
	if (m_grid[index].isThereMyBody(pos_x, pos_y, Direction::BOTTOM) && snake.getDirection() != Direction::TOP)
	{
		data(3) = 3;
	}

	// return the "input" MatrixXd
	return data;
}

std::vector<Eigen::MatrixXd> Game::crossOverWeights(const std::vector<Eigen::MatrixXd>& second, const std::vector<Eigen::MatrixXd>& first)
{
	std::vector<Eigen::MatrixXd> temp = second;
	
	// loop through all the weights
	for (int i{ 0 }; i < static_cast<int>(temp.size()); i++)
	{
		for (int j{ 0 }; j < static_cast<int>(temp[i].size()); j++)
		{
			if (j % 2 == 0)
				temp[i].coeffRef(j) = first[i](j);
		}
	}	
	return temp;
}
// similar to the crossOverWeights
std::vector<Eigen::VectorXd> Game::crossOverBiases(const std::vector<Eigen::VectorXd>& second, const std::vector<Eigen::VectorXd>& first)
{
	std::vector<Eigen::VectorXd> temp = second;
	for (int i{ 0 }; i < static_cast<int>(temp.size()); i++)
	{
		for (int j{ 0 }; j < static_cast<int>(temp[i].size()); j++)
		{
			if (j % 2 == 0)
				temp[i].coeffRef(j) = first[i](j);
		}
		
	}

	return temp;
}

void Game::setNewGeneration()
{
	// create a vector of pair and links the players to their points
	std::vector<std::pair<std::unique_ptr<NeuralNetwork>, int>> paired_vector;
	// loop for add to the previous vector the pairs
	for (int i{ 0 }; i < m_number_player; i++)
	{
		// make_pair to create a pair and std::move to move the content of each unique_ptr to the paired_vector
		paired_vector.push_back(std::make_pair(std::move(m_players[i]), m_points[i]));
	}
	// sort the paired_vector relatively to the points
	std::sort(paired_vector.begin(), paired_vector.end(), [](const auto& a, const auto& b) { return a.second > b.second; });

	// crossOver the bests
	for (int i{ 10 }; i < m_number_player - 10; i++)
	{
		int random1{ rand() % 11 };
		int random2{};
		do {
			random2 = rand() % 11;
		} while (random2 == random1);
		
		paired_vector[i].first.get()->setWeigths(crossOverWeights(paired_vector[random2].first.get()->getWeigths(), paired_vector[random1].first.get()->getWeigths()));
		paired_vector[i].first.get()->setBiases(crossOverBiases(paired_vector[random2].first.get()->getBiases(), paired_vector[random1].first.get()->getBiases()));

		paired_vector[i].first.get()->setWeigths(mutationWeights(paired_vector[i].first.get()->getWeigths()));
		paired_vector[i].first.get()->setBiases(mutationBiases(paired_vector[i].first.get()->getBiases()));
	}
	for (int i{ m_number_player - 10 }; i < m_number_player; i++)
	{
		paired_vector[i].first.get()->init();
	}
	
	// reassign each value to the m_players vector
	for (int i{ 0 }; i < m_players.size(); i++)
	{
		m_players[i] = std::move(paired_vector[i].first);
	}

	m_generation++;
	// reset the class Game using the operator=
	*this = { m_number_player };
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
	// loop through all players
	for (int i{ 0 }; i < static_cast<int>(m_players.size()); i++)
	{
		m_grid[i].initObjectsGrid();
		// check if the snake not lost
		if (!checkLost(m_snakes[i]) && m_step[i] <= 200)
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
		else // if the snake lost
		{
			m_points[i]--;
			m_lost[i] = true; // turn to true the value and the snake can't move anymore
		}			
			
	}
	if (m_lost[0] && std::equal(m_lost.begin() + 1, m_lost.end(), m_lost.begin())) // if all snakes lost
	{
		setNewGeneration(); // set the new generation and reset the class
	}
	// each time the function is called and after all movements, search for the first snake of the game
	m_index_first_snake = static_cast<int>(std::distance(m_points.begin(), std::max_element(m_points.begin(), m_points.end())));
	// and print his score
	m_text_generation->setString(sf::String("Génération : " + std::to_string(m_generation) + "\nPopulation : " + std::to_string(m_number_player)));
}

void Game::drawGame(sf::RenderWindow& win)
{
	// for each snake of the game
	for (int i{ 0 }; i < static_cast<int>(m_snakes.size()); i++)
	{
		if (i == static_cast<int>(m_index_first_snake))
			m_snakes[i].draw(win, true);
		else
			m_snakes[i].draw(win, false);
	}
		
	m_text_generation->drawText(win); // draw the text

	for (int i{ 0 }; i < static_cast<int>(m_fruits.size()); i++)
	{
		if (i == static_cast<int>(m_index_first_snake))
			m_fruits[i].drawFruit(win, true);
		else
			m_fruits[i].drawFruit(win, false);
	}

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
