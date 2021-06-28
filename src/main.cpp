#include <SFML/Graphics.hpp>
#include <stdexcept>

int main() {
    sf::RenderWindow window(sf::VideoMode(500, 500), "Genetic Image");

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    sf::RenderTexture renderTexture;
    if (!renderTexture.create(500, 500))
        throw std::runtime_error("error creating render texture");
    renderTexture.clear();
    renderTexture.draw(shape);
    renderTexture.display();
    const sf::Texture& texture = renderTexture.getTexture();

    sf::Texture originalImage;
    if (!originalImage.loadFromFile("res/lena.png"))
        throw std::runtime_error("error loading original image");

    sf::Sprite displayedSprite(originalImage);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(displayedSprite);
        window.display();
    }
    return 0;
}