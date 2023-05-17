# Snake-Neural-Network

Welcome to the basic game called Snake!

Everyone knows this game, it's a snake that must to eat to survive and grow. But the player doesn't know the game.
Indeed, the player is a neural network that will train to learn the game and improve with each generation.

## Rules of the game
By default, the snake has twelve entrances which correspond to the four directions with the vision of fruits, walls and tail.
It has 2 hidden layers of 8 neurons each for an output of 4 neurons to the 4 directions.

The snake grow thanks to the genetic algorithm. In this project, the game starts with 1,000 snakes. After the game is over, the 1,000 snakes are sorted with their points they won.
1 point for a fruit eaten and -1 point if it walks too much (300 here). After sorting, the top 30% best snakes are selected to be part of the next generation. 20% of them is a new snake 
and 50% of them are reproduced thanks to the best.


## What can be improve?

After some research, it appears that the best way for a snake to grow faster is to give its entrances the distance to each object. 
For example, if my snake is too big, it will maybe enter between his body and not see its tail before the disaster.

![image](https://github.com/erwanmq/Snake-Neural-Network/assets/93351393/7e94bbfe-2b33-4fbb-ac66-304aa30c6b02)

The graphics are a bit ugly but I did this for the concept and not for the graphics!
