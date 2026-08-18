#include <SFML/Graphics.hpp>
#include <vector> 
#include <random>




//Combing the related data for the pipes so I can resuse them
struct Pipe {
    sf::RectangleShape top;
    sf::RectangleShape bottom;
};

//Adding some options for the user to initiate the game sequence.
enum class GameState {
    Start,
    Playing,
    GameOver
};

const float pipeWidth = 80.f;
const float pipeGap = 200.f;

//Creating a function to create the multiple pipes that are going to be used. 
Pipe createPipe(float xPosition, std::mt19937& rng, std::uniform_real_distribution<float>& gapDist) {
    float gapCenterY = gapDist(rng); //Gets a random center for the pipes

    Pipe pipe;
    pipe.top.setSize({pipeWidth, gapCenterY - pipeGap / 2.f});
    pipe.top.setFillColor(sf::Color::Green);
    pipe.top.setPosition({xPosition, 0.f});

    pipe.bottom.setSize({pipeWidth, 600.f - (gapCenterY + pipeGap / 2.f)});
    pipe.bottom.setFillColor(sf::Color::Green);
    pipe.bottom.setPosition({xPosition, gapCenterY + pipeGap/2.f});

    return pipe;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Flappy Bird");

    sf::Font font;
    if (!font.openFromFile("font.ttf")) {
        return -1; //Can't run the game without the font, so bail out
    }

    sf::Text menuText(font);
    menuText.setCharacterSize(24);
    menuText.setFillColor(sf::Color::White);
    menuText.setPosition({150.f, 250.f});



    //The following line will establish the sprite in the graphical window that represents the bird. 
    sf::RectangleShape bird({40.f, 40.f}); //40x40 pixel square -- the braces and the f suffix force these numbers into float literals instead of integers (SFML geometry types work in floating-point coordinates)
    bird.setFillColor(sf::Color::Yellow); //Fills the square with a yellow color
    bird.setPosition({100.f, 300.f}); //Places the bird at the coordinates (100,300) in the window.
    //Also important to remember that positive Y means down and Positive X means right in SFML's coordinate system.

    //The following lines will handle the physics of the bird's movement, including gravity and velocity
    sf::Clock clock; //Tracks the time elapsed between frames, which is important for calculating the bird's movement based on time rather than frame rate. 
    float velocity = 0.f; //The bird's vertical velocity, in pixels per second. Initial condition is 0 (Bird is not moving yet) and lives outside the loop to initialize it only
    const float gravity = 500.f; //The acceleration due to gravity, in pixels per second squared
    const float jumpStrength = -300.f; //The upward force applied to the bird when the space bar is pressed. 

    GameState gameState = GameState::Start; //initial game state is set to start, which means the game is waiting for the player to start playing.
   
    float pipeSpeed = 200.f; //Horizontal speed of the pipes, in pixels per second.(Moving leftward)

    //Also need to add boundaries for the window to tell when the bird hits the cieling and the floor
    float cieling = 0.f;
    float ground = 600.f;
    
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<float> gapDist(150.f, 450.f);
    
    std::vector<Pipe> pipes;
    float pipeSpawnTimer = 2.f;
    const float pipeSpawnInterval = 2.f;

    

    while (window.isOpen()) {
        while(const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    if (gameState == GameState::Start) {
                        gameState = GameState::Playing; //When the space bar is pressed, the game state changes from Start to playing, allowing the bird to start moving and the game to begin. 
                    } else if (gameState == GameState::Playing) {
                        velocity = jumpStrength; //When the space bar is pressed, the bird's vertical velocity is set to a negative value, causing it to move upward on the screen. 
                    }
                }
                if (keyPressed->code == sf::Keyboard::Key::R && gameState == GameState::GameOver) {
                    bird.setPosition({100.f, 300.f}); //Resets the bird's position to the starting point
                    velocity = 0.f; //Resets the bird's velocity to 0, so it doesn't keep moving aftet the reset
                    pipes.clear(); //Clears all the pipes from the screen
                    pipeSpawnTimer = pipeSpawnInterval; //Resets the pipe spawn timer to the intial value
                    gameState = GameState::Playing; //Resets the game state to Start, allowing the player to start a new game. 
                }
                if (keyPressed->code == sf::Keyboard::Key::Escape) {
                    window.close(); //closes the window and exits the game when the Escape key is pressed. 
                }
            }
        }
        float deltaTime = clock.restart().asSeconds(); //Calculates the time elapsed since the last frame(loop), which is used to update the bird's position based on its velocity
        if (gameState == GameState::Playing) {
            pipeSpawnTimer += deltaTime; 
        
            if (pipeSpawnTimer >= pipeSpawnInterval) {
                pipes.push_back(createPipe(800.f, rng, gapDist));
                pipeSpawnTimer = 0.f;

            }
            velocity += gravity * deltaTime; //Updates the bird's vertical velocity based on the acceleration due to gravity
            bird.move({0.f, velocity * deltaTime}); //Updates the bird's position based on its velocity and the time elapsed since the last frame. 


            //The following code is for when the bird hits the cieling and the floor. Restricting the bird from going outside of the window. 
            sf::Vector2f birdPos = bird.getPosition(); //Getting the current location of the bird
            if (birdPos.y < 0.f) { //If the bird's position is above the visible window
                birdPos.y = cieling; //Clamping the birds position to the ceiling
                velocity = 0.f; //Stops the bird from moving
            }

            if (birdPos.y + bird.getSize().y > 600.f) {
                birdPos.y = ground - bird.getSize().y;
                velocity = 0.f;
            }
            bird.setPosition(birdPos); //Doing the actual clamping of the bird

            //The following code is for the behavior of the pipes. 
            for (Pipe& pipe : pipes) {
                pipe.top.move({-pipeSpeed * deltaTime, 0.f});
                pipe.bottom.move({-pipeSpeed * deltaTime, 0.f});

                //Adding the pipe passed counter here gameState
                if (bird.getGlobalBounds().findIntersection(pipe.top.getGlobalBounds()) || bird.getGlobalBounds().findIntersection(pipe.bottom.getGlobalBounds()))
                    gameState = GameState::GameOver;
            }
        
            for (int i = static_cast<int>(pipes.size()) -1; i >= 0; i--) {
                if (pipes[i].top.getPosition().x + pipeWidth < 0.f) {
                    pipes.erase(pipes.begin() + i);
                }
            }
        }    
        window.clear(); //Clears the window to a black color for redrawing (re-rendering) the next frame.
        //After clearing the window the pipes will then be drawn
        for (const Pipe& pipe : pipes) {
            window.draw(pipe.top);
            window.draw(pipe.bottom);
        }
        
        window.draw(bird); //Draws the bird sprite to the window.

        if (gameState == GameState::Start) {
            menuText.setString("Press Space to Start");
            window.draw(menuText);
        }


        window.display(); //Displays the contents of the window to the screen. This is the last step in the rendering process for each frame. 
    }
    
    return 0;

}