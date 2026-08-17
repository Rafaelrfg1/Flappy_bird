#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Flappy Bird");

    //The following line will establish the sprite in the graphical window that represents the bird. 
    sf::RectangleShape bird({40.f, 40.f}); //40x40 pixel square -- the braces and the f suffix force these numbers into float literals instead of integers (SFML geometry types work in floating-point coordinates)
    bird.setFillColor(sf::Color::Yellow); //Fills the square with a yellow color
    bird.setPosition({100.f, 300.f}); //Places the bird at the coordinates (100,300) in the window.
    //Also important to remember that positive Y means down and Positive X means right in SFML's coordinate system.

    //The following lines will handle the physics of the bird's movement, including gravity and velocity
    sf::Clock clock; //Tracks the time elapsed between frames, which is important for calculating the bird's movement based on time rather than frame rate. 
    float velocity = 0.f; //The bird's vertical velocity, in pixels per second. Initial condition is 0 (Bird is not moving yet) and lives outside the loop to initialize it only
    const float gravity = 500.f; //The acceleration due to gravity, in pixels per second squared
    const float jumpStrength = -400.f; //The upward force applied to the bird when the space bar is pressed. 


    //Following is for the pipes 
    float pipeWidth = 80.f; //Width of the pipes
    float pipeGap = 200.f; //Vertical gap between the top and bottom pipes.
    float gapCenterY = 300.f; //Vertical position of the center of the gap between the pipes.
    float pipeSpeed = 200.f; //Horizontal speed of the pipes, in pixels per second.(Moving leftward)

    //Also need to add boundaries for the window to tell when the bird hits the cieling and the floor
    float cieling = 0.f;
    float ground = 600.f;
    
    sf::RectangleShape topPipe({pipeWidth, gapCenterY - pipeGap / 2.f}); //Top pipe
    topPipe.setFillColor(sf::Color::Green); //Fills the top pipe with green color
    topPipe.setPosition({700.f, 0.f}); //Places the top pipe at the right edge of the window, at the top (y=0)

    sf::RectangleShape bottomPipe({pipeWidth, 600.f - (gapCenterY + pipeGap / 2.f)}); //Bottom pipe
    bottomPipe.setFillColor(sf::Color::Green); //Fills the bottom pipt with green color
    bottomPipe.setPosition({700.f, gapCenterY + pipeGap / 2.f}); //Places the bottom pipe at the right edge of the window. 

    while (window.isOpen()) {
        while(const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    velocity = jumpStrength; //When the space bar is pressed, the bird's vertical velocity is set to a negative value, causing it to move upward on the screen. 
                }
            }
        }
        float deltaTime = clock.restart().asSeconds(); //Calculates the time elapsed since the last frame(loop), which is used to update the bird's position based on its velocity
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

        topPipe.move({-pipeSpeed * deltaTime, 0.f}); // Makes the top pipe move left (No changes vertically)
        bottomPipe.move({-pipeSpeed * deltaTime, 0.f}); // Makes the bottom pipe move left (No changes vertically)

        window.clear(); //Clears the window to a black color for redrawing (re-rendering) the next frame.
        window.draw(topPipe); //Draws the top pipt to the window.
        window.draw(bottomPipe); //Draws the bottom pipt to the window. 
        window.draw(bird); //Draws the bird sprite to the window.
        window.display(); //Displays the contents of the window to the screen. This is the last step in the rendering process for each frame. 
    }
    
    return 0;

}