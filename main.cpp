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


    while (window.isOpen()) {
        while(const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    velocity = jumpStrength;; //When the space bar is pressed, the bird's vertical velocity is set to a negative value, causing it to move upward on the screen. 
                }
            }
        }
        float deltaTime = clock.restart().asSeconds(); //Calculates the time elapsed since the last frame(loop), which is used to update the bird's position based on its velocity
        velocity += gravity * deltaTime; //Updates the bird's vertical velocity based on the acceleration due to gravity
        bird.move({0.f, velocity * deltaTime}); //Updates the bird's position based on its velocity and the time elapsed since the last frame. 


        window.clear(); //Clears the window to a black color for redrawing (re-rendering) the next frame.
        window.draw(bird); //Draws the bird sprite to the window.
        window.display(); //Displays the contents of the window to the screen. This is the last step in the rendering process for each frame. 
    }
    
    return 0;

}