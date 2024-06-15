#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>
#include <iostream>

const size_t BLOCK_WIDTH = 2;
const size_t WINDOW_WIDTH = 1200;
const size_t WINDOW_HEIGHT = 700;
const size_t MAX_BLOCKS = WINDOW_WIDTH / BLOCK_WIDTH;


class Block : public sf::Drawable
{

public:
    unsigned int value;
    sf::RectangleShape shape;

    Block(sf::Vector2f pos, unsigned int value, unsigned width): 
        value(value),
        shape(sf::Vector2f(width, value))
    {
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(sf::Vector2f(0, shape.getSize().y));
        shape.setPosition(pos);
    }
    Block() :
        value(0),
        shape(sf::Vector2f(0, value))
    {
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(sf::Vector2f(0, shape.getSize().y));
        shape.setPosition(sf::Vector2f(0,0));
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        target.draw(shape);
    }

};


class SortingApp
{
private:
    sf::RenderWindow window;
    std::vector<Block> arrayOfBlocks;

    sf::Font font;
    sf::Text currentAlgoText;
    sf::Text menuText;

    sf::SoundBuffer soundBuffer;
    sf::Sound beep;

    const size_t SLOW_SPEED = 120;
    const size_t MEDIUM_SPEED = 350;
    const size_t FAST_SPEED = 1000;

public:
    SortingApp(size_t w, size_t h, std::string title):
        window(sf::VideoMode(w, h), title),
        arrayOfBlocks(MAX_BLOCKS),
        font(),
        currentAlgoText("None", font, 26),
        menuText("Q - Bubble Sort\n"
                "W - Selection Sort\n"
                "E - Insertion Sort\n"
                "D - Quick Sort\n"
                "S - Radix Sort\n"
                "R - Randomize array\n"
                "1 - Slow speed\n"
                "2 - Medium speed\n"
                "3 - Fast speed\n", font, 16),
        soundBuffer(),
        beep(soundBuffer)
    {
        window.setFramerateLimit(500);

        font.loadFromFile("Roboto-Regular.ttf");
        currentAlgoText.setPosition(400, 0);
        menuText.setPosition(0, 0);
       
        soundBuffer.loadFromFile("beep-07a.wav");

        randomizeBlocks();
    }

    void randomizeBlocks()
    {
        int temp_array[MAX_BLOCKS] = { 0 };

        for (int i = 0; i < arrayOfBlocks.size(); ++i)
            temp_array[i] = i;

        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine rng(seed);

        std::shuffle(temp_array, temp_array + MAX_BLOCKS - 1, rng);

        for (int i = 0; i < arrayOfBlocks.size(); ++i)
        {
            sf::Vector2f blockPos(i * BLOCK_WIDTH, window.getSize().y);
            arrayOfBlocks[i] = Block(blockPos, (temp_array[i] + 1) , BLOCK_WIDTH);
        }
    }

    void handleEvents()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Q)
                {
                    currentAlgoText.setString("Bubble Sort");
                    bubbleSort();
                }
                if (event.key.code == sf::Keyboard::W)
                {
                    currentAlgoText.setString("Selection Sort");
                    selectionSort();
                }
                if (event.key.code == sf::Keyboard::E)
                {
                    currentAlgoText.setString("Insertion Sort");
                    insertionSort();
                }
                if (event.key.code == sf::Keyboard::D)
                {
                    currentAlgoText.setString("Quick Sort");
                    quickSort(this->arrayOfBlocks, 0, this->arrayOfBlocks.size() - 1);
                    endEffect();
                }
                if (event.key.code == sf::Keyboard::S)
                {
                    currentAlgoText.setString("Radix Sort");
                    radixSort(arrayOfBlocks);
                }
                if (event.key.code == sf::Keyboard::R)
                {
                    randomizeBlocks();
                }
                if (event.key.code == sf::Keyboard::Num1)
                {
                    window.setFramerateLimit(SLOW_SPEED);
                }
                if (event.key.code == sf::Keyboard::Num2)
                {
                    window.setFramerateLimit(MEDIUM_SPEED);
                }
                if (event.key.code == sf::Keyboard::Num3)
                {
                    window.setFramerateLimit(FAST_SPEED);
                }
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window.close();
                }
            }
        }
    }

    void update()
    {

    }

    void render()
    {
        window.clear();
        drawBlocks();
        window.draw(currentAlgoText);
        window.draw(menuText);
        window.display();
    }

    void drawBlocks()
    {
        for (auto& block : arrayOfBlocks)
            window.draw(block);
    }
    
    void bubbleSort() {
        for (int i = 0; i < arrayOfBlocks.size() - 1; i++) {
            for (int j = 0; j < arrayOfBlocks.size() - 1; j++) {
                arrayOfBlocks[i].shape.setFillColor(sf::Color::Green);
                arrayOfBlocks[j + 1].shape.setFillColor(sf::Color::Green);
                if (arrayOfBlocks[j].value > arrayOfBlocks[j + 1].value) 
                {
                    swapBlocks(arrayOfBlocks[j], arrayOfBlocks[j + 1]);
                    std::swap(arrayOfBlocks[j], arrayOfBlocks[j + 1]);
                }
            }
            handleEvents();
            render();
        }
        endEffect();
    }
    void selectionSort() {
        for (int i = 0; i < arrayOfBlocks.size() - 1; i++) {
            int minIndex = i;
            arrayOfBlocks[i].shape.setFillColor(sf::Color::Green);
            for (int j = i + 1; j < arrayOfBlocks.size(); j++) {
                arrayOfBlocks[j].shape.setFillColor(sf::Color::Green);
                if (arrayOfBlocks[j].value < arrayOfBlocks[minIndex].value) {
                    minIndex = j;
                }
                arrayOfBlocks[j].shape.setFillColor(sf::Color::Red); 
            }
            handleEvents();
            render();
            if (minIndex != i) {
                swapBlocks(arrayOfBlocks[minIndex], arrayOfBlocks[i]);
                std::swap(arrayOfBlocks[minIndex], arrayOfBlocks[i]);
            }
            handleEvents();
            render();
            arrayOfBlocks[minIndex].shape.setFillColor(sf::Color::Red);  
            arrayOfBlocks[i].shape.setFillColor(sf::Color::Red);  
        }
        endEffect();
    }

    void insertionSort() {
        for (int i = 1; i < arrayOfBlocks.size(); i++) {
            handleEvents();
            render();
            int key = arrayOfBlocks[i].value;
            arrayOfBlocks[i].shape.setFillColor(sf::Color::Green);
            int j = i - 1;
            while (j >= 0 && arrayOfBlocks[j].value > key) {
                arrayOfBlocks[j].shape.setFillColor(sf::Color::Green);
                arrayOfBlocks[j + 1].value = arrayOfBlocks[j].value;
                swapBlocks(arrayOfBlocks[j + 1], arrayOfBlocks[j]);
                std::swap(arrayOfBlocks[j + 1], arrayOfBlocks[j]);
                
                arrayOfBlocks[j].shape.setFillColor(sf::Color::White);  
                j = j - 1;
            }
            arrayOfBlocks[j + 1].value = key;
            handleEvents();
            render();
            arrayOfBlocks[i].shape.setFillColor(sf::Color::White);  
        }
        endEffect();
    }

    int partition(std::vector<Block>& arrayOfBlocks, int low, int high) {
        int pivot = arrayOfBlocks[high].value;
        arrayOfBlocks[high].shape.setFillColor(sf::Color::Red); 
        int i = (low - 1);
        for (int j = low; j < high; j++) {
            arrayOfBlocks[j].shape.setFillColor(sf::Color::Green);
            if (arrayOfBlocks[j].value < pivot) {
                i++;
                std::swap(arrayOfBlocks[i], arrayOfBlocks[j]);
                swapBlocks(arrayOfBlocks[i], arrayOfBlocks[j]);

            }
            handleEvents();
            render();
            arrayOfBlocks[j].shape.setFillColor(sf::Color::White);  
        }
        std::swap(arrayOfBlocks[i + 1], arrayOfBlocks[high]);
        swapBlocks(arrayOfBlocks[i + 1], arrayOfBlocks[high]);
        handleEvents();
        render();
        arrayOfBlocks[high].shape.setFillColor(sf::Color::White); 
        arrayOfBlocks[i + 1].shape.setFillColor(sf::Color::White); 
        return (i + 1);
    }

    void quickSort(std::vector<Block>& arrayOfBlocks, int low, int high) {
        if (low < high) {
            int pi = partition(arrayOfBlocks, low, high);
            quickSort(arrayOfBlocks, low, pi - 1);
            quickSort(arrayOfBlocks, pi + 1, high);
        }
    }

    void countSort(std::vector<Block>& arrayOfBlocks, int exp) {
        int n = arrayOfBlocks.size();
        std::vector<Block> output(n, Block());
        int count[10] = { 0 };

        for (int i = 0; i < n; i++) {
            count[(arrayOfBlocks[i].value / exp) % 10]++;
        }

        for (int i = 1; i < 10; i++) {
            count[i] += count[i - 1];
        }

        for (int i = n - 1; i >= 0; i--) {
            output[count[(arrayOfBlocks[i].value / exp) % 10] - 1] = arrayOfBlocks[i];
            count[(arrayOfBlocks[i].value / exp) % 10]--;
        }

        for (int i = 0; i < n; i++) {
            swapBlocks(arrayOfBlocks[i], output[i]);
            arrayOfBlocks[i] = output[i];
            arrayOfBlocks[i].shape.setFillColor(sf::Color::Green);
            handleEvents();
            render();
            arrayOfBlocks[i].shape.setFillColor(sf::Color::Red);
        }
    }

    void radixSort(std::vector<Block>& arrayOfBlocks) {
        size_t maxVal = 0;

        for (int i = 0; i < arrayOfBlocks.size(); ++i) {
            if (arrayOfBlocks[i].value >= maxVal) {
                maxVal = arrayOfBlocks[i].value;
            }
            std::cout << arrayOfBlocks[i].value << std::endl;

        }
        for (int exp = 1; maxVal / exp > 0; exp *= 10) {
            countSort(arrayOfBlocks, exp);
        }
        endEffect();
    }



    void swapBlocks(Block& b1, Block& b2)
    {
        sf::Vector2f temp_pos = b1.shape.getPosition();
        b1.shape.setPosition(b2.shape.getPosition());
        b2.shape.setPosition(temp_pos);
        b1.shape.setFillColor(sf::Color::Yellow);
        b2.shape.setFillColor(sf::Color::Yellow);
        beep.play();
    }

    void endEffect()
    {
        for (auto& block : arrayOfBlocks)
        {
            block.shape.setFillColor(sf::Color::Magenta);
            beep.play();
            render();
        }
    }

    const bool isRunning() const { return window.isOpen(); }


};

int main()
{


    SortingApp app(1200,700,"Sorting Visualiser");


    while (app.isRunning())
    {
        app.handleEvents();
        app.update();
        app.render();
    }

    return 0;
}

