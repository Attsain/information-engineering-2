#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class CustomRectangleShape : public sf::RectangleShape{
private:
    int Vspeed_ = 0;
    int Hspeed_ = 0;
    int Rspeed_ = 0;

    int left;
    int right;
    int top;
    int bottom;



public:
    CustomRectangleShape(sf::Vector2f size, sf::Vector2f position){
        setPosition(position);
        setSize(size);
    }

    void setSpeed(int Vspeed,int Hspeed,int Rspeed){
        Vspeed_ = Vspeed;
        Hspeed_ = Hspeed;
        Rspeed_ = Rspeed;
    }

    void animate(const sf::Time &elapsed){
        move(Vspeed_*elapsed.asSeconds(), Hspeed_*elapsed.asSeconds());
        rotate(Rspeed_*elapsed.asSeconds());
    }

    void setBounds(int a, int b, int c, int d){
        left = a;
        right = b;
        top = c;
        bottom = d;
    }

    void bounds(){
        sf::FloatRect rectangle_bounds = getGlobalBounds();

        if(rectangle_bounds.top <= top){
            Hspeed_ = abs(Hspeed_);
        }

        if(rectangle_bounds.top + rectangle_bounds.height >= bottom){
            Hspeed_ = abs(Hspeed_) * -1;
        }

        if(rectangle_bounds.left <= left ){
            Vspeed_ = abs(Vspeed_);
        }

        if(rectangle_bounds.left + rectangle_bounds.width >= right){
            Vspeed_ = abs(Vspeed_) * -1;
        }
    }



    bool isClicked(sf::Vector2i &mouse_position){
        sf::FloatRect rectangle_bounds = getGlobalBounds();

        if ((mouse_position.y >= rectangle_bounds.top && mouse_position.y <= rectangle_bounds.top + rectangle_bounds.height) && (mouse_position.x >=rectangle_bounds.left && mouse_position.x <= rectangle_bounds.left + rectangle_bounds.width)){
            setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
            return true;
        }else{
            return false;}
        return false;
    }

    void moveInDirection(const sf::Time &elapsed, bool &selected){
        sf::FloatRect rectangle_bounds = getGlobalBounds();
        if (selected){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if(rectangle_bounds.top <= top){
                    move(0, 0);
                }
                else{
                    move(0, abs(Vspeed_)*elapsed.asSeconds()*(-1));
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if(rectangle_bounds.top + rectangle_bounds.height >= bottom){
                    move(0,0);
                }
                else{
                    move(0, abs(Vspeed_)*elapsed.asSeconds());
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                if(rectangle_bounds.left <= left ){
                    move(0,0);
                }
                else{
                    move(abs(Hspeed_)*elapsed.asSeconds()*(-1), 0);
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                if(rectangle_bounds.left + rectangle_bounds.width >= right){
                    move(0,0);
                }
                else{
                    move(abs(Hspeed_)*elapsed.asSeconds(), 0);
                }
            }
        }

    }
};

bool fifarafa(bool &sibidibi, bool &fabadaba);


bool fifarafa(bool &sibidibi, bool &fabadaba) {
    if (sibidibi){
        if (fabadaba){
            return true;
        }
    }
    return false;
}

int main() {
    // create the window

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    // create some shapes
    sf::CircleShape circle(50.0);
    circle.setPosition(100.0, 300.0);
    circle.setFillColor(sf::Color(100, 250, 50));

    sf::RectangleShape rectangle(sf::Vector2f(120.0, 60.0));
    rectangle.setPosition(500.0, 400.0);
    rectangle.setFillColor(sf::Color(100, 50, 250));

    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(0.0, 0.0));
    triangle.setPoint(1, sf::Vector2f(0.0, 100.0));
    triangle.setPoint(2, sf::Vector2f(140.0, 40.0));
    triangle.setOutlineColor(sf::Color::Red);
    triangle.setOutlineThickness(2);
    triangle.setPosition(600.0, 100.0);
    sf::Clock clock;

    int rectangle_velocity_x = 50;
    int rectangle_velocity_y = 200;
    int rectangle_angular_velocity = 10;

    bool flag_y = false;
    bool flag_x = false;

    sf::Vector2f size(120.0, 60.0);
    sf::Vector2f position(120.0, 60.0);
    CustomRectangleShape rectangle2(size, position);
    rectangle2.setFillColor(sf::Color(95, 55, 230));
    rectangle2.setSpeed(100, 100, 10);
    bool selected;

    // run the program as long as the window is open
    while (window.isOpen()) {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();


        sf::FloatRect rectangle_bounds = rectangle.getGlobalBounds();
        sf::FloatRect rectangle2_bounds = rectangle2.getGlobalBounds();

        if(rectangle_bounds.top <= 0){
            rectangle_velocity_y = abs(rectangle_velocity_y);
            rectangle.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
        }

        if(rectangle_bounds.top + rectangle_bounds.height >= window.getSize().y){
            rectangle_velocity_y = abs(rectangle_velocity_y) * -1;
            rectangle.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
        }

        if(rectangle_bounds.left <= 0 ){
            rectangle_velocity_x = abs(rectangle_velocity_x);
            rectangle.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
        }

        if(rectangle_bounds.left + rectangle_bounds.width >= window.getSize().x){
            rectangle_velocity_x = abs(rectangle_velocity_x) * -1;
            rectangle.setFillColor(sf::Color(rand() % 256, rand() % 256, rand() % 256));
        }

        rectangle.move(rectangle_velocity_x*dt,rectangle_velocity_y*dt);
        rectangle.rotate(rectangle_angular_velocity*dt);


        // check all the window's events that were triggered since the last iteration of the loop
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Left){
                    bool bimbirimbim = rectangle2.isClicked(mouse_pos);
                    bool parampampam = (event.mouseButton.button == sf::Mouse::Left);
                    selected = fifarafa(parampampam,bimbirimbim);
                    std::cout << "Mouse clicked:" << mouse_pos.x << ", " << mouse_pos.y << std::endl << rectangle2_bounds.top << " " <<  rectangle2_bounds.left << std::endl;
                }
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        rectangle2.setBounds(0, window.getSize().x, 0, window.getSize().y);
        rectangle2.bounds();
        rectangle2.moveInDirection(elapsed, selected);

        // draw everything here...
        window.draw(circle);
        window.draw(rectangle);
        window.draw(triangle);
        window.draw(rectangle2);

        // end the current frame
        window.display();
    }

    return 0;
}
