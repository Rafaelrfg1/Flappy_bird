#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Flappy Bird");

    //The following line will establish the sprite in the graphical window that represents the bird. 
    sf::RectangleShape bird({40.f, 40.f}); //40x40 pixel square -- the braces and the f suffix force these numbers into float literals instead of integers (SFML geometry types work in floating-point coordinates)
    bird.setFillColor(sf::Color::Yellow); //Fills the square with a yellow color
    bird.setPosition({100.f, 300.f}); //Places the bird at the coordinates (100,300) in the window.
    //Also important to remember that positive Y means down and Positive X means right in SFML's coordinate system.


    while (window.isOpen()) {
        while(const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }
        window.clear(); //Clears the window to a black color for redrawing (re-rendering) the next frame.
        window.draw(bird); //Draws the bird sprite to the window.
        window.display(); //Displays the contents of the window to the screen. This is the last step in the rendering process for each frame. 
    }
    
    return 0;

}