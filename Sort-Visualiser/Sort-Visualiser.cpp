// Sort-Visualiser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SFML/Graphics.hpp>

#include <iostream>
#include <random>
#include <ctime>

#define MAX_VAL 1000

class Button {
public:
    int xpos;
    int width;
    std::string name;
    std::string orig_name;
    sf::RectangleShape icon;
    sf::Color colour;
    sf::Color textColour;
    sf::Font font;


    Button(std::string a, int b, sf::Vector2f buttonSize) {

        name = a;
        orig_name = a;
        xpos = b;
        sf::RectangleShape icon1(buttonSize);
        colour = sf::Color::White;
        textColour = sf::Color::Black;

        icon = icon1;
        icon.setFillColor(colour);
        icon.setPosition(xpos, 2);
        icon.setOutlineColor(sf::Color::Black);
        icon.setOutlineThickness(2);

    }
};

class Bar {
public:
    int value, xpos, ypos, order;
    sf::Color colour;
    sf::RectangleShape icon;

    Bar(sf::Vector2f barSize,int o, int x, int y) {


        
        value = 10;
        xpos = x;
        ypos = y;
        order = o;
        barSize.y = barSize.y * value;
        sf::RectangleShape icon1(barSize);
        colour = sf::Color::White;
        icon = icon1;
        icon.setFillColor(colour);
        icon.setPosition(xpos, ypos-barSize.y);
    };

    void change_value(int newval) {

        sf::Vector2f size = icon.getSize();
        size.y = (size.y / value) * newval;
        this->icon.setPosition(xpos, ypos - size.y);
        this->icon.setSize(size);
        this->value = newval;

    }

    void change_colour(sf::Color newcol) {

        this->icon.setFillColor(newcol);
        this->colour = newcol;

    }

    void reposition(int x) {

        xpos = x;
        icon.setPosition(xpos, ypos);
    };

};

void update(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList);
bool stop(sf::RenderWindow* window, std::vector<Button>* buttonList);
void randomise(std::vector<Bar> *list);
void mergesort(sf::RenderWindow *window, std::vector<Button> *buttonList, std::vector<Bar> *barList);
bool quicksort(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList, std::vector<Bar>::iterator lo, std::vector<Bar>::iterator hi);
void bogosort(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList);
std::vector<Bar>::iterator partition(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList, std::vector<Bar>::iterator lo, std::vector<Bar>::iterator hi);

int main()
{

    const int n_buttons = 7;
    float scale = 0.6;


    //setting window parameters
    int windowWidth = sf::VideoMode::getDesktopMode().width * scale;
    int windowHeight = sf::VideoMode::getDesktopMode().height * scale;

    std::srand(std::time(nullptr));
    
    // loading font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) 
    {
        std::cout << "Error no font";
        return -1;
    }

    //setting banner parameters
    sf::Vector2f uiSize(windowWidth, windowHeight / 20);
    sf::RectangleShape uiBanner(uiSize);
    uiBanner.setPosition(0, 0);
    uiBanner.setFillColor(sf::Color::Green);

    //setting button parameters
    sf::Vector2f buttonSize(windowWidth / n_buttons, windowHeight / 20);
    std::string buttonNameList[n_buttons] = { "Randomise", "Inc num bars", "Dec num bars", "Run Bogosort", "Run Bubblesort", "Run Quicksort", "Run Merge Sort"};
    std::vector<Button> buttonList;

    //creating button objects
    for (int i = 0; i < n_buttons; i++) { 
        Button button(buttonNameList[i], buttonSize.x * i , buttonSize);
        button.font = font;
        buttonList.push_back(button);
    }


    //setting bar parameters

    int n_bars = 400;
    int spacing = 4;
    if (n_bars >=100)
        spacing = spacing / (n_bars / 100);
    //std::cout << spacing << "\n";

    int ypos = windowHeight * 0.9;
    sf::Vector2f barSize(((windowWidth * 0.9) /n_bars) - spacing , (ypos - (uiSize.y * 2)) /MAX_VAL);
    std::vector<Bar> barList;
    
    
    //creating bar objects
    Bar bar0(barSize, -1, -1, -1);
    barList.insert(barList.begin(), bar0);
    for (int i = 0; i < n_bars; i++) {
        
        Bar newBar(barSize,i, ((windowWidth * 0.05)) + (barSize.x + spacing)*i, ypos);
        barList.push_back(newBar);
    }
    randomise(&barList);



    //window loop
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Sorting Algorithm Visualiser");
    while (window.isOpen())
    {
        //events
        sf::Event event;
        while (window.pollEvent(event))
        {
            //window close
            if (event.type == sf::Event::Closed)
                window.close();

            //button clicks
            if (event.type == sf::Event::MouseButtonPressed) {

                if (event.mouseButton.button == sf::Mouse::Left && event.mouseButton.y < uiSize.y) {
                    std::string buttonPressed;

                    //identify which button pressed
                    std::vector<Button>::iterator button_it;

                    for (auto i = (buttonList).begin(); i != (buttonList).end(); i++) {

                        if (event.mouseButton.x > (*i).xpos && event.mouseButton.x < (*i).xpos + buttonSize.x) {
                            buttonPressed = (*i).name;
                            button_it = i;
                        }
                    }

                    
                    if (buttonPressed == "Randomise") {

                        //std::cout << "Randomised\n";
                        randomise(&barList);

                    }
                    
                    if (buttonPressed == "Run Merge Sort") {

                        //std::cout << "Randomised\n";
                        mergesort(&window, &buttonList, &barList);

                    }

                    if (buttonPressed == "Run Quicksort") {

                        (*button_it).name = "Stop";

                        //std::cout << "Randomised\n";
                        quicksort(&window, &buttonList, &barList, barList.begin() + 1, barList.end());

                        (*button_it).name = (*button_it).orig_name;

                    }

                    if (buttonPressed == "Run Bogosort") {

                        (*button_it).name = "Stop";

                        //std::cout << "Randomised\n";
                        bogosort(&window, &buttonList, &barList);

                        (*button_it).name = (*button_it).orig_name;

                    }


                }



            }
        }

        




        
        update(&window, &buttonList, &barList);





    }

    return 0;
}

void update(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList) {


    //start clear_draw_display 
    (*window).clear();

    //drawing banner
    //(*window).draw(uiBanner);

    //drawing buttons
    for (auto i = (*buttonList).begin(); i != (*buttonList).end(); i++) {

        (*window).draw((*i).icon);
        sf::Text text((*i).name, (*i).font);
        text.setPosition((*i).xpos, 0);
        text.setFillColor((*i).textColour);
        (*window).draw(text);

    }

    //drawing bars

    for (auto i = (*barList).begin(); i != (*barList).end(); i++) {

        (*window).draw((*i).icon);
    }





    (*window).display();       //end of clear_draw_display


}

void randomise(std::vector<Bar> *list) {


    for (auto i = (*list).begin(); i != (*list).end(); i++) {

        (*i).change_value((rand() % MAX_VAL) + 1);
        
        /*
        if ((*i).order %2 == 0)
            (*i).change_value((*i).order);
        else
            (*i).change_value(MAX_VAL - (*i).order);
        */

    }
}

void mergesort(sf::RenderWindow *window,  std::vector<Button> *buttonList, std::vector<Bar> *barList) {

    //recursive




    


}

bool quicksort(sf::RenderWindow *window, std::vector<Button> *buttonList, std::vector<Bar> *barList, std::vector<Bar>::iterator lo, std::vector<Bar>::iterator hi) {

    if (lo < hi) {

        std::vector<Bar>::iterator pivot = partition(window, buttonList, barList, lo, hi);

        if (pivot == (*barList).begin())
            return true;

        if (quicksort(window, buttonList, barList, lo, pivot))
            return true;
        if (quicksort(window, buttonList, barList, pivot + 1, hi))
            return true;

    }
    
    return false;
}

std::vector<Bar>::iterator partition(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList, std::vector<Bar>::iterator lo, std::vector<Bar>::iterator hi) {

    std::vector<Bar>::iterator pivot = lo + (hi - lo) / 2;
    (*pivot).change_colour(sf::Color::Blue);
    //std::cout << "pivot is " << (*pivot).value <<"\n";
    //(*pivot).change_value(666);
    
    std::vector<Bar>::iterator i = lo-1;
    std::vector<Bar>::iterator j = hi;



    while (1) {

        if (stop(window, buttonList)) {
            (*pivot).change_colour(sf::Color::White);
            return (*barList).begin();
        }


        do {
            if (i!= pivot)
                i = i + 1;
        } while ((*i).value < (*pivot).value);

        do {
            if (j != pivot)
                j = j - 1;
        } while ((*j).value > (*pivot).value);


        if (i >= j) {
            (*pivot).change_colour(sf::Color::White);
            return pivot;
        }

        //swap A[i] and A[j]
        int ival = (*i).value;
        int jval = (*j).value;



        (*i).change_colour(sf::Color::Red);
        (*j).change_colour(sf::Color::Red);

        update(window, buttonList, barList);
        sf::Clock clock;
        sf::Time elapsed1;

        do {
            elapsed1 = clock.getElapsedTime();
        } while (elapsed1.asSeconds() < 0.0);

        

        (*i).change_value(jval);
        (*j).change_value(ival);

        update(window, buttonList, barList);
        
        clock.restart();
        sf::Time elapsed2;

        do {
            elapsed2 = clock.getElapsedTime();
        } while (elapsed2.asSeconds() < 0.0);

        if (i == pivot) {
            pivot = j;
        }
        else if (j == pivot) {
            pivot = i;
        }

        (*i).change_colour(sf::Color::White);
        (*j).change_colour(sf::Color::White);
        (*pivot).change_colour(sf::Color::Blue);
    }
}

void bogosort(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList) {


    //set colour to red
    for (auto i = (*barList).begin() + 1; i != (*barList).end(); i++) {
        (*i).change_colour(sf::Color::Red);
    }


    while (1) {

        //check if sorted
        bool sorted = true;
        for (auto i = (*barList).begin() + 2; i != (*barList).end(); i++) {
            if ((*i).value < (*(i - 1)).value)
                sorted = false;
        }

 
        if (sorted)
            break;

        //check if interrupted
        if (stop(window, buttonList))
            break;

        //create temp bar list
        std::vector<Bar> temp_bar_list;
        for (auto i = (*barList).begin() + 1; i != (*barList).end(); i++) {
            temp_bar_list.push_back(*i);
        }




        //randomise order
        for (auto i = (*barList).begin() + 1; i != (*barList).end(); i++) {
            int rand_int = rand() % temp_bar_list.size();
            (*i).change_value(temp_bar_list[rand_int].value);
            temp_bar_list.erase(temp_bar_list.begin() + rand_int);
        }

        //update 
        update(window, buttonList, barList);


    }

    //reset colour
    for (auto i = (*barList).begin() + 1; i != (*barList).end(); i++) {
        (*i).change_colour(sf::Color::White);
    }


    return;
}

bool stop(sf::RenderWindow* window, std::vector<Button>* buttonList) {

    sf::Event event;
    while ((*window).pollEvent(event))
    {
        //window close
        if (event.type == sf::Event::Closed) {
            (*window).close();
            return true;
        }

        //button clicks
        if (event.type == sf::Event::MouseButtonPressed) {

            if (event.mouseButton.button == sf::Mouse::Left && event.mouseButton.y < (*buttonList)[0].icon.getSize().y) {
                std::string buttonPressed;

                //identify which button pressed
                for (auto i = (*buttonList).begin(); i != (*buttonList).end(); i++) {

                    if (event.mouseButton.x > (*i).xpos && event.mouseButton.x < (*i).xpos + (*i).icon.getSize().x) {
                        buttonPressed = (*i).name;
                    }
                }


                if (buttonPressed == "Stop") {

                    //std::cout << "Randomised\n";
                    return true;

                }
            }
        }
    }

    return false;
}











