#include "Game.h"
#include <iostream>


Game::Game(int number_player)
	:m_number_player{ number_player }
{
	// create a unique_ptr of the class Text
	m_text_generation = std::make_unique<Text>(std::string("DejaVuSansMono.ttf"), sf::String("Génération : 1\nPopulation : " + std::to_string(m_number_player)), sf::Vector2f(WIDTH - 200.f, 30.f));

	// these 6 calls resize the vector to the same number as the number of players 
	m_snakes.resize(number_player);
	m_players.resize(number_player);
	m_lost.resize(number_player);
	m_points.resize(number_player);
	m_fruits.resize(number_player);
	m_step.resize(number_player);

	// fill m_lost, m_points and m_step to false, 0 and 0 respectively
	std::fill(m_lost.begin(), m_lost.end(), false);
	std::fill(m_points.begin(), m_points.end(), 0);
	std::fill(m_step.begin(), m_step.end(), 0);

	// create a random position for the first fruit 
	m_fruits[0].setSize(m_snakes[0].getSize());
	m_fruits[0].setRandomPosition();
	m_grid.addObjectToTheGrid(2, m_fruits[0].getPosition().x, m_fruits[0].getPosition().y);
	

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
	/*if (snake.getHeadPosition().x >= m_fruits[index].getPosition().x && snake.getHeadPosition().y >= m_fruits[index].getPosition().y &&
		snake.getHeadPosition().x <= m_fruits[index].getPosition().x + m_fruits[index].getSize().x && snake.getHeadPosition().y <= m_fruits[index].getPosition().y + m_fruits[index].getSize().y)
		return true;*/

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
	if (snake.getHeadPosition().x < 0 || snake.getHeadPosition().y < 0
		|| snake.getHeadPosition().x > WIDTH || snake.getHeadPosition().y > HEIGHT)
		return true;

	return false;
}

// function that return a MatrixXd that stores the different values of the different directions of the snake
Eigen::MatrixXd Game::getGameData(Snake& snake, int index)
{
	Eigen::MatrixXd data(4, 1); // create a matrix of (4, 1) (4 directions)
	// supposed there is nothing (all 0)
	data << 0, 0, 0, 0; // right side, left side, top side, bot side

	// for each pixel from the head to the right wall
	for (int i{ static_cast<int>(snake.getHeadPosition().x) }; i <= WIDTH; i++) // right side
	{
		// check if the next square is a wall or not
		if (WIDTH - snake.getHeadPosition().x <= 2*snake.getSize().x)
			data.coeffRef(0) = 1; // wall

		// check if there is a fruit in the right direction
		if (i >= m_fruits[index].getPosition().x && snake.getHeadPosition().y >= m_fruits[index].getPosition().y &&
			i <= m_fruits[index].getPosition().x + m_fruits[index].getSize().x && snake.getHeadPosition().y <= m_fruits[index].getPosition().y + m_fruits[index].getSize().y)
			data.coeffRef(0) = 2; // fruit
	}

	// for each pixel from the left wall to the head
	for (int i{ 0 }; i <= snake.getHeadPosition().x; i++) // left side
	{
		if (snake.getHeadPosition().x <= 3*snake.getSize().x)
			data.coeffRef(1) = 1;
		
		if (i >= m_fruits[index].getPosition().x && snake.getHeadPosition().y >= m_fruits[index].getPosition().y &&
			i <= m_fruits[index].getPosition().x + m_fruits[index].getSize().x && snake.getHeadPosition().y <= m_fruits[index].getPosition().y + m_fruits[index].getSize().y)
			data.coeffRef(1) = 2;
	}

	// for each pixel from the upper wall to the head
	for (int i{ 0 }; i <= snake.getHeadPosition().y; i++) // top side
	{
		if (snake.getHeadPosition().y <= 3*snake.getSize().y)
			data.coeffRef(2) = 1;

		if (i >= m_fruits[index].getPosition().y && snake.getHeadPosition().x >= m_fruits[index].getPosition().x &&
			i <= m_fruits[index].getPosition().y + m_fruits[index].getSize().y && snake.getHeadPosition().x <= m_fruits[index].getPosition().x + m_fruits[index].getSize().x)
			data.coeffRef(2) = 2;
	}

	// for each pixel from the head to the lower wall
	for (int i{ static_cast<int>(snake.getHeadPosition().y) }; i <= HEIGHT; i++) // top side
	{
		if (HEIGHT - snake.getHeadPosition().y <=  3 * snake.getSize().y)
			data.coeffRef(3) = 1;

		if (i >= m_fruits[index].getPosition().y && snake.getHeadPosition().x >= m_fruits[index].getPosition().x &&
			i <= m_fruits[index].getPosition().y + m_fruits[index].getSize().y && snake.getHeadPosition().x <= m_fruits[index].getPosition().x + m_fruits[index].getSize().x)
			data.coeffRef(3) = 2;
	}

	std::cout << data << "\n\n";
	// return the "input" MatrixXd
	return data;
}

std::vector<Eigen::MatrixXd> Game::crossOverWeights(const std::vector<Eigen::MatrixXd>& second, const std::vector<Eigen::MatrixXd>& first)
{
	std::vector<Eigen::MatrixXd> temp(second.size());
	// loop through all the weights
	for (int i{ 0 }; i < static_cast<int>(second.size()); i++)
	{
		// change the weights of the half first part
		if (i <= static_cast<int>(second.size() / 2))
			temp[i] = first[i];
		else // stay unchanged
			temp[i] = second[i];
	}	
	return temp;
}
// similar to the crossOverWeights
std::vector<Eigen::VectorXd> Game::crossOverBiases(const std::vector<Eigen::VectorXd>& second, const std::vector<Eigen::VectorXd>& first)
{
	std::vector<Eigen::VectorXd> temp(second.size());
	for (int i{ 0 }; i < static_cast<int>(second.size()); i++)
	{
		if (i <= static_cast<int>(second.size() / 2))
			temp[i] = first[i];
		else
			temp[i] = second[i];
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
	for (int i{ 20 }; i < m_number_player; i++)
	{
		int random1{ rand() % 11 };
		int random2{};
		do {
			random2 = rand() % 11;
		} while (random2 == random1);
		
		std::vector<Eigen::MatrixXd> value_returned = crossOverWeights(paired_vector[random2].first.get()->getWeigths(), paired_vector[random1].first.get()->getWeigths());
		paired_vector[i].first.get()->setWeigths(value_returned);
		paired_vector[i].first.get()->setBiases(crossOverBiases(paired_vector[random2].first.get()->getBiases(), paired_vector[random1].first.get()->getBiases()));

		paired_vector[i].first.get()->setWeigths(mutationWeights(paired_vector[i].first.get()->getWeigths()));
		paired_vector[i].first.get()->setBiases(mutationBiases(paired_vector[i].first.get()->getBiases()));
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

		temp[i].coeffRef(random_index) = random_value / 10000.f;
	}
	return temp;
}

std::vector<Eigen::VectorXd> Game::mutationBiases(const std::vector<Eigen::VectorXd>& mutation_layer)
{
	std::vector<Eigen::VectorXd> temp = mutation_layer;
	for (int i{ 0 }; i < static_cast<int>(temp.size()); i++)
	{
		int random_index{ std::rand() % (static_cast<int>(temp[i].size() + 1))};
		int random_value{ std::rand() % 10000 };

		temp[i].coeffRef(random_index) = random_value / 10000.0;
	}
	return temp;
}

void Game::update()
{
	// loop through all players
	for (int i{ 0 }; i < static_cast<int>(m_players.size()); i++)
	{
		// check if the snake not lost
		if (!checkLost(m_snakes[i]) && m_step[i] <= 200)
		{
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
			m_lost[i] = true; // turn to true the value and the snake can't move anymore
		}			
			
	}
	if (m_lost[0] && std::equal(m_lost.begin() + 1, m_lost.end(), m_lost.begin())) // if all snakes lost
	{
		setNewGeneration(); // set the new generation and reset the class
	}
	// each time the function is called and after all movements, search for the first snake of the game
	m_index_first_snake = std::distance(m_points.begin(), std::max_element(m_points.begin(), m_points.end()));
	// and print his score
	m_text_generation->setString(sf::String("Génération : " + std::to_string(m_generation) + "\nPopulation : " + std::to_string(m_number_player)));
}

void Game::drawGame(sf::RenderWindow& win)
{
	// for each snake of the game
	for (int i{ 0 }; i < static_cast<int>(m_snakes.size()); i++)
	{
		// draw full color if the snake is the first
		//if (i == static_cast<int>(m_index_first_snake - m_points.begin()))
		//	m_snakes[i].draw(win, true);
		//else // draw the other snake will alpha lower
		if (i == static_cast<int>(m_index_first_snake))
			m_snakes[i].draw(win, true);
		else
			m_snakes[i].draw(win, false);
	}
		
	m_text_generation->drawText(win); // drax the text

	for (int i{ 0 }; i < static_cast<int>(m_fruits.size()); i++)
	{
		if (i == static_cast<int>(m_index_first_snake))
			m_fruits[i].drawFruit(win, true);
		else
			m_fruits[i].drawFruit(win, false);
	}

	//m_grid.drawGrid(win);
	//for(Fruit fruit: m_fruits)
	//	fruit.drawFruit(win); // draw the fruits
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
