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
    int value, xpos, ypos, trueValue;
    int mergeValue[2];
    sf::Color colour;
    sf::RectangleShape icon;

    Bar(sf::Vector2f barSize, int x, int y) {


        
        value = 10;
        mergeValue[0] = 10;
        mergeValue[1] = 10;
        xpos = x;
        ypos = y;
        trueValue = 10;
        barSize.y = barSize.y * value;
        sf::RectangleShape icon1(barSize);
        colour = sf::Color::White;
        icon = icon1;
        icon.setFillColor(colour);
        icon.setPosition(xpos, ypos-barSize.y);
    };

    void changeValue(int newval) {

        sf::Vector2f size = icon.getSize();
        size.y = (size.y / value) * newval;
        this->icon.setPosition(xpos, ypos - size.y);
        this->icon.setSize(size);
        this->value = newval;

    }

    void changeColour(sf::Color newcol) {

        this->icon.setFillColor(newcol);
        this->colour = newcol;

    }

    void reposition(int x) {

        xpos = x;
        icon.setPosition(xpos, ypos);
    };

};


std::vector<Bar>* createBars(int N, int windowWidth, int windowHeight, std::vector<Bar>* barList);
void update(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList);
bool stop(sf::RenderWindow* window, std::vector<Button>* buttonList);
void randomise(std::vector<Bar> *list);
void hiddenQuicksort(std::vector<Bar>* barList, std::vector<Bar>::iterator lo, std::vector<Bar>::iterator hi);
int changeN(sf::RenderWindow* window, std::vector<Button>::iterator button, std::vector<Button>* buttonList, std::vector<Bar>* barList);
bool ordered(std::vector<Bar>* barList, std::vector<Bar>::iterator it);

void bubblesort(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList);
void mergesort(sf::RenderWindow *window, std::vector<Button> *buttonList, std::vector<Bar> *barList);
bool mergeList(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList, std::vector<Bar>::iterator lo, std::vector<Bar>::iterator hi, bool toA);
bool quicksort(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList, std::vector<Bar>::iterator lo, std::vector<Bar>::iterator hi);
void bogosort(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList);
std::vector<Bar>::iterator partition(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList, std::vector<Bar>::iterator lo, std::vector<Bar>::iterator hi);

int main()
{

    const int n_buttons = 8;
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
    std::string buttonNameList[n_buttons] = { "Randomise", "N = ", "Speed", "Instant Sort", "Run Bogosort", "Run Bubblesort", "Run Quicksort", "Run Merge Sort"};
    std::vector<Button> buttonList;

    //creating button objects
    for (int i = 0; i < n_buttons; i++) { 
        Button button(buttonNameList[i], buttonSize.x * i , buttonSize);
        button.font = font;
        buttonList.push_back(button);
    }


    //setting bar parameters

    int n_bars = 10;

    for (auto i = (buttonList).begin(); i != (buttonList).end(); i++) {
        if (i->name == "N = ") {
            i->name = "N = 10";
        }
    }

    std::vector<Bar>* barList = createBars(n_bars, windowWidth, windowHeight, nullptr);



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

                    //identify which button pressed
                    std::vector<Button>::iterator buttonPressed;

                    for (auto i = (buttonList).begin(); i != (buttonList).end(); i++) {

                        if (event.mouseButton.x > (*i).xpos && event.mouseButton.x < (*i).xpos + buttonSize.x) {

                            buttonPressed = i;
                            break;
                        }
                    }

                    
                    if (buttonPressed->name == "Randomise") {

                        //std::cout << "Randomised\n";
                        randomise(barList);

                    }

                    if (buttonPressed->orig_name == "N = ") {
                        
                        n_bars = changeN(&window, buttonPressed, &buttonList, barList);

                        if (n_bars > 0)
                            barList = createBars(n_bars, windowWidth, windowHeight, barList);
                    }

                    if (buttonPressed->name == "Instant Sort") {

                        for (auto i = barList->begin() + 1; i != barList->end(); i++) {
                            (*i).changeColour(sf::Color::Green);
                            (*i).changeValue((*i).trueValue);
                        }

                    }


                    if (buttonPressed->name == "Run Bubblesort") {

                        buttonPressed->name = "Stop";

                        
                        bubblesort(&window, &buttonList, barList);

                        buttonPressed->name = buttonPressed->orig_name;

                    }
                    
                    if (buttonPressed->name == "Run Merge Sort") {

                        buttonPressed->name = "Stop";

                        for (auto i = barList->begin() + 1; i != barList->end(); i++)
                            (*i).changeColour(sf::Color::White);

                        mergesort(&window, &buttonList, barList);

                        buttonPressed->name = buttonPressed->orig_name;

                    }

                    if (buttonPressed->name == "Run Quicksort") {

                        buttonPressed->name = "Stop";

                        for (auto i = barList->begin() + 1; i != barList->end(); i++)
                            (*i).changeColour(sf::Color::White);
                        
                        if (quicksort(&window, &buttonList, barList, barList->begin() + 1, barList->end())) {
                            for (auto i = barList->begin() + 1; i != barList->end(); i++)
                                (*i).changeColour(sf::Color::White);
                        }
                        else {
                            for (auto i = barList->begin() + 1; i != barList->end(); i++)
                                (*i).changeColour(sf::Color::Green);
                        }
                            

                        buttonPressed->name = buttonPressed->orig_name;

                    }

                    if (buttonPressed->name == "Run Bogosort") {

                        buttonPressed->name = "Stop";

                        
                        bogosort(&window, &buttonList, barList);

                        buttonPressed->name = buttonPressed->orig_name;

                    }


                }



            }
        }

        




        
        update(&window, &buttonList, barList);





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

std::vector<Bar>* createBars(int N, int windowWidth, int windowHeight, std::vector<Bar>* barList) {


    if (barList)
        delete barList;

    int spacing = 4;
    sf::Vector2f uiSize(windowWidth, windowHeight / 20);

    if (N >= 100)
        spacing = spacing / (N / 100);
    //std::cout << spacing << "\n";

    int ypos = windowHeight * 0.9;
    sf::Vector2f barSize(((windowWidth * 0.9) / N) - spacing, (ypos - (uiSize.y * 2)) / MAX_VAL);
    barList = new std::vector<Bar>;


    //creating bar objects
    Bar bar0(barSize, -1, -1);
    barList->insert(barList->begin(), bar0);
    for (int i = 0; i < N; i++) {

        Bar newBar(barSize, ((windowWidth * 0.05)) + (barSize.x + spacing) * i, ypos);
        barList->push_back(newBar);
    }
    randomise(barList);

    return barList;
}

void randomise(std::vector<Bar> *list) {


    for (auto i = (*list).begin(); i != (*list).end(); i++) {

        int randInt = (rand() % MAX_VAL) + 1;
        (*i).changeValue(randInt);
        (*i).trueValue = randInt;
        (*i).changeColour(sf::Color::White);
    }

    hiddenQuicksort(list, (*list).begin() + 1, (*list).end());

}

int changeN(sf::RenderWindow* window, std::vector<Button>::iterator button, std::vector<Button>* buttonList, std::vector<Bar>* barList) {


    button->icon.setFillColor(sf::Color::Yellow);

    //save temp value of name and reset name 
    std::string origName = button->name;
    button->name = button->orig_name;

    //set current N value
    int currentN = 0;


    while (1) {

        //create event
        sf::Event event;
        while (window->pollEvent(event)) {

            //window close
            if (event.type == sf::Event::Closed) {
                window->close();
                return -1;
            }

            //return if user clicks elsewhere
            if (event.type == sf::Event::MouseButtonPressed) {
                //reset colour and name string and return -1 (fail)
                button->icon.setFillColor(sf::Color::White);
                button->name = origName;
                return -1;
            }

            //typed input
            if (event.type == sf::Event::TextEntered && button->name.size() < button->orig_name.size() +3) {

                //check if numerical, if so register input
                char enteredChar = static_cast<char>(event.text.unicode);
                if (isdigit(enteredChar)) {

                    //if numeric, convert to int, add to name string, and add didgit to currentN
                    int enteredInt = enteredChar - '0';
                    button->name.push_back(enteredChar);
                    currentN = (currentN * 10) + enteredInt;

                }
            }

            //check for functional key presses
            if (event.type == sf::Event::KeyPressed) {

                //backspace
                if (event.key.code == sf::Keyboard::BackSpace) {

                    //if there are digits after the original name string, delete the last one
                    //and knock it off the value of currentN
                    if (button->name != button->orig_name) {
                        button->name.pop_back();
                        currentN = (currentN - (currentN % 10)) / 10;
                    }
                }

                //enter
                if (event.key.code == sf::Keyboard::Enter) {

                    //check is currentN is valid (cannot be 0 or negative)
                    if (currentN >= 1) {
                        //reset colour and return N
                        button->icon.setFillColor(sf::Color::White);
                        return currentN;
                    }
                    else {
                        //otherwise, reset colour and name string and return -1 (fail)
                        button->icon.setFillColor(sf::Color::White);
                        button->name = origName;
                        return -1;
                    }
                

                }

                //esc
                if (event.key.code == sf::Keyboard::Escape) {
                    //reset colour and name and return -1 (fail)
                    button->icon.setFillColor(sf::Color::White);
                    button->name = origName;
                    return -1;
                    
                }

            }
        }

        update(window, buttonList, barList);

    }


}

void hiddenQuicksort(std::vector<Bar>* barList, std::vector<Bar>::iterator lo, std::vector<Bar>::iterator hi) {

    if (lo < hi) {

        //std::vector<Bar>::iterator pivot = partition(window, buttonList, barList, lo, hi);

        std::vector<Bar>::iterator pivot = lo + (hi - lo) / 2;

        std::vector<Bar>::iterator i = lo - 1;
        std::vector<Bar>::iterator j = hi;

        while (1) {

            do {
                if (i != pivot)
                    i = i + 1;
            } while ((*i).trueValue < (*pivot).trueValue);

            do {
                if (j != pivot)
                    j = j - 1;
            } while ((*j).trueValue > (*pivot).trueValue);


            if (i >= j) {
                break;
            }

            //swap A[i] and A[j]
            int ival = (*i).trueValue;
            int jval = (*j).trueValue;



            (*i).trueValue = (jval);
            (*j).trueValue = (ival);

            if (i == pivot) {
                pivot = j;
            }
            else if (j == pivot) {
                pivot = i;
            }
        }

        hiddenQuicksort(barList, lo, pivot);

        hiddenQuicksort(barList, pivot + 1, hi);


    }

}

bool ordered(std::vector<Bar>* barList, std::vector<Bar>::iterator it) {

    bool check = true;

    //check that each bar below *it is in correct position
    for (auto i = (*barList).begin() + 1; i != it + 1; i++) {
        if ((*i).value != (*i).trueValue) {
            check = false;
            break;
        }
    }

    //if all correct positions below *it, *it is in correct place
    if (check)
        return true;
    else 
        check = true; 
    
    //otherwise reset, and check going up
    for (auto i = it; i != (*barList).end(); i++) {
        if ((*i).value != (*i).trueValue) {
            check = false;
            break;
        }
    }


    if (check)
        return true;
    else 
        return false;
}

void bubblesort(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList) {


    //set colour to white
    for (auto i = (*barList).begin() + 1; i != (*barList).end(); i++) {
        (*i).changeColour(sf::Color::White);
    }

    int pass = 0;

    while (1) {

        //pass through
        bool sorted = true;
        bool stopped = false;
        pass++;
        
        for (auto i = (*barList).begin() + 1; i != (*barList).end() - pass; i++) {
            int currentValue;
            if ((*i).value > (*(i + 1)).value) {
                
                (*i).changeColour(sf::Color::Red);
                (*(i + 1)).changeColour(sf::Color::Red);

                update(window, buttonList, barList);
                
                
                currentValue = (*i).value;
                (*i).changeValue((*(i + 1)).value);
                (*(i + 1)).changeValue(currentValue);

                sorted = false;
            }
            else {
                (*i).changeColour(sf::Color::Cyan);
                (*(i + 1)).changeColour(sf::Color::Cyan);

                update(window, buttonList, barList);
            }

            //update 
            update(window, buttonList, barList);

            //resetting colours
            if (ordered(barList, i))
                (*i).changeColour(sf::Color::Magenta);
            else
                (*i).changeColour(sf::Color::White);

            if (ordered(barList, i+1))
                (*(i+1)).changeColour(sf::Color::Magenta);
            else
                (*(i + 1)).changeColour(sf::Color::White);


            //check if interrupted
            if (stop(window, buttonList)) {
                stopped = true;
                break;
            }
        }


        if (sorted || stopped) {
            for (auto i = (*barList).begin() + 1; i != (*barList).end(); i++) {
                if (sorted)
                    (*i).changeColour(sf::Color::Green);
                else
                    (*i).changeColour(sf::Color::White);
            }
            break;
        }

    }

    return;
}

void mergesort(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList) {


    //create 2 new values to be merged
    for (auto i = barList->begin() + 1; i != barList->end(); i++) {
        i->mergeValue[0] = i->value;
        i->mergeValue[1] = i->value;
        i->changeColour(sf::Color::White);
    }

    update(window, buttonList, barList);

    //run mergelist with full list, set colours to green if sorted successfully
    if (mergeList(window, buttonList, barList, barList->begin() + 1, barList->end(), false)) {
        for (auto i = barList->begin() + 1; i != barList->end(); i++)
            i->changeColour(sf::Color::White);
    }
    else{
        for (auto i = barList->begin() + 1; i != barList->end(); i++)
            i->changeColour(sf::Color::Green);
    }

    update(window, buttonList, barList);

}

bool mergeList(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList, std::vector<Bar>::iterator lo, std::vector<Bar>::iterator hi, bool dir) {

    //create timing clock
    sf::Clock clock;
    sf::Time elapsed1;
    float delay = 0.01;


    //if list is of length 1, return as sorted
    if (hi - lo <= 1)
        return false;

    //create mid point pointer
    std::vector<Bar>::iterator mid = lo + (hi - lo) / 2;

    //recursively merge lists
    //if mergelist returns true, user has clicked stop, so return true to cascade up the recusion
    if (mergeList(window, buttonList, barList, lo, mid, !dir))
        return true;
    if (mergeList(window, buttonList, barList, mid, hi, !dir))
        return true;



    std::vector<Bar>::iterator i = lo;
    std::vector<Bar>::iterator j = mid;
    
    for (auto k = lo; k != hi; k++) {

        
        if (k < hi-1)
            (k + 1)->changeColour(sf::Color::Red);  //i and j set to red, i is dosplayed as k+1
        if (j<hi)
            j->changeColour(sf::Color::Red);
        auto jReset = j;
        k->changeColour(sf::Color::Blue);           //k set to blue

        update(window, buttonList, barList);


        if (i < mid && (j>=hi || i->mergeValue[!dir] <= j->mergeValue[!dir])){
            k->mergeValue[dir] = i->mergeValue[!dir];
            k->changeValue(i->mergeValue[!dir]);
            i++;
        }
        else {

            //shift values along by 1
            for (auto m = j; m > k; m--) {

                m->changeValue((m - 1)->value);
            }

            k->mergeValue[dir] = j->mergeValue[!dir];
            k->changeValue(j->mergeValue[!dir]);
            
            j++;
        }

        //wait for delay
        do {
            elapsed1 = clock.getElapsedTime();
        } while (elapsed1.asSeconds() < delay);

        update(window, buttonList, barList);
        clock.restart();

        //reset colours
        if (k < hi-1)
            (k + 1)->changeColour(sf::Color::White);  
        if (jReset < hi)
            jReset->changeColour(sf::Color::White);

        if (lo == barList->begin() + 1 && hi == barList->end())
            k->changeColour(sf::Color::Magenta);
        else
            k->changeColour(sf::Color::White);

        //check if stopped
        if (stop(window, buttonList))
            return true; // return true if stopped
    }

    return false;  //returns false if not stopped
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
    else if (hi < (*barList).end()){
        (*hi).changeColour(sf::Color::Magenta);
        (*lo).changeColour(sf::Color::Magenta);
    }
    
    return false;
}

std::vector<Bar>::iterator partition(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList, std::vector<Bar>::iterator lo, std::vector<Bar>::iterator hi) {

    std::vector<Bar>::iterator pivot = lo + (hi - lo) / 2;
    (*pivot).changeColour(sf::Color::Blue);
    //std::cout << "pivot is " << (*pivot).value <<"\n";
    //(*pivot).changeValue(666);
    
    std::vector<Bar>::iterator i = lo-1;
    std::vector<Bar>::iterator j = hi;



    while (1) {

        if (stop(window, buttonList)) 
            return (*barList).begin();

        sf::Clock clock;
        sf::Time elapsed1;
        float delay = 0.0;



        do {
            if (i != pivot) {

                if (i != lo-1)
                    (*i).changeColour(sf::Color::White);

                i = i + 1;

                if ((*i).value <= (*pivot).value)
                    (*i).changeColour(sf::Color::Green);
                else
                    (*i).changeColour(sf::Color::Red);
            }


            clock.restart();
            do {
                elapsed1 = clock.getElapsedTime();
            } while (elapsed1.asSeconds() < delay);
            update(window, buttonList, barList);

            if (i != pivot && (*i).value < (*pivot).value) {
                (*i).changeColour(sf::Color::White);
            }

        } while ((*i).value < (*pivot).value);

        do {
            if (j != pivot) {

                if (j != hi)
                    (*j).changeColour(sf::Color::White);

                j = j - 1;

                if ((*j).value >= (*pivot).value)
                    (*j).changeColour(sf::Color::Green);
                else
                    (*j).changeColour(sf::Color::Red);
            }


            clock.restart();
            do {
                elapsed1 = clock.getElapsedTime();
            } while (elapsed1.asSeconds() < delay);
            update(window, buttonList, barList);

            if (j != pivot && (*j).value > (*pivot).value) {
                
            }

        } while ((*j).value > (*pivot).value);


        if (i >= j) {
            (*pivot).changeColour(sf::Color::White);
            (*i).changeColour(sf::Color::White);
            (*j).changeColour(sf::Color::White);
            /*if (pivot == hi || pivot == lo) {
                (*pivot).changeColour(sf::Color::Magenta);
                (*i).changeColour(sf::Color::Magenta);
                (*j).changeColour(sf::Color::Magenta);
            }*/
            return pivot;
        }

        //swap A[i] and A[j]
        int ival = (*i).value;
        int jval = (*j).value;



        clock.restart();

        do {
            elapsed1 = clock.getElapsedTime();
        } while (elapsed1.asSeconds() < delay);

        (*i).changeValue(jval);
        (*j).changeValue(ival);

        update(window, buttonList, barList);
        


        if (i == pivot) {
            pivot = j;
        }
        else if (j == pivot) {
            pivot = i;
        }



        (*pivot).changeColour(sf::Color::Blue);
        (*j).changeColour(sf::Color::Green);
    }
}

void bogosort(sf::RenderWindow* window, std::vector<Button>* buttonList, std::vector<Bar>* barList) {


    sf::Clock clock;
    sf::Time elapsed1;
    float delay = 0.1;

    //set colour to red
    for (auto i = (*barList).begin() + 1; i != (*barList).end(); i++) {
        (*i).changeColour(sf::Color::Red);
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
        if (stop(window, buttonList)) {
            //reset colour
            for (auto i = (*barList).begin() + 1; i != (*barList).end(); i++)
                (*i).changeColour(sf::Color::White);

            return;
        }

        //create temp bar list
        std::vector<Bar> temp_bar_list;
        for (auto i = (*barList).begin() + 1; i != (*barList).end(); i++) {
            temp_bar_list.push_back(*i);
        }




        //randomise order
        for (auto i = (*barList).begin() + 1; i != (*barList).end(); i++) {
            int rand_int = rand() % temp_bar_list.size();
            (*i).changeValue(temp_bar_list[rand_int].value);
            temp_bar_list.erase(temp_bar_list.begin() + rand_int);
        }

        
        //wait for delay
        do {
            elapsed1 = clock.getElapsedTime();
        } while (elapsed1.asSeconds() < delay);


        //update 
        update(window, buttonList, barList);
        clock.restart();

    }

    for (auto i = (*barList).begin() + 1; i != (*barList).end(); i++)
        (*i).changeColour(sf::Color::Green);

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











