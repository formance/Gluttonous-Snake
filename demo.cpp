#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <thread>
#include <list>
#include <chrono>
#include <random>

std::default_random_engine generate_random;

//Game interface class
class gameInterface
{
public:
    gameInterface();
    ~gameInterface();
    int key_input;
    cv::Mat img;
private:
    int h_img = 500;
    int w_img = 500;
};
gameInterface::gameInterface()
{
    img = cv::Mat::zeros(h_img, w_img, CV_8UC3);
}
gameInterface::~gameInterface()
{
    img.release();
}

class snake:public gameInterface
{
public:
    snake();
    ~snake();
    void start();
private:
    int snake_length = 4;
    int snake_step = 5; //per step 5 pixels
    int max_length = 50;
    int time_interval = 200;//ms
    int score = 0; 
    int box_width = 400;
    int box_height = 400;

    cv::Point food;
    cv::Rect snake_box;
    cv::Mat img_box;

    std::list<cv::Point> snake_list;
    void drawSnake(int flag=1);
    void generateFood();
};
snake::snake()
{
    snake_box = cv::Rect(50,100,box_width,box_height);
    img_box = img(snake_box);
    cv::Point snake_node = cv::Point(snake_step,snake_step*(snake_length+1)); //snake end pos in box coordinate
    for(int i=0;i<snake_length;i++)
    {
        snake_list.push_front(snake_node);
        snake_node.x += snake_step; 
    }
    generateFood();
}
snake::~snake()
{
    snake_list.clear();
}
void snake::generateFood()
{
    std::uniform_int_distribution<> dist1(1,box_width/snake_step-1);
    std::uniform_int_distribution<> dist2(1,box_height/snake_step-1);
    while(1)
    {
        food.x = dist1(generate_random)*snake_step;
        food.y = dist2(generate_random)*snake_step;
        auto it = std::find(snake_list.begin(),snake_list.end(),food); // food can not override with snake
        if(it == snake_list.end()) break;
    }
    // std::cout << food << std::endl;
}
void snake::drawSnake(int flag)
{
    img.setTo(cv::Scalar(0,0,0));
    if(flag == 0)
    {
        std::string text = "Game Over!";
        cv::putText(img,text,cv::Point(200,80),1,1.5,cv::Scalar(255,255,255));
    }
    int node_radius = snake_step/2 + 1 ;
    for(auto it = snake_list.begin(); it != snake_list.end();it++)
    {
        if(it != snake_list.begin())
        {
            cv::circle(img_box,*it,node_radius,cv::Scalar(255,255,255),-1);
        }
        else
        {
            cv::circle(img_box,*it,node_radius,cv::Scalar(0,255,0),-1);
        }
    }
    cv::circle(img_box,food,node_radius,cv::Scalar(255,0,0),-1);
    cv::rectangle(img,snake_box,cv::Scalar(255,255,255),2);
    std::string title = "This Is Game Snake";
    cv::putText(img,title,cv::Point(180,20),1,1,cv::Scalar(0,0,255));
    std::string text = "Score: " + std::to_string(score);
    cv::putText(img,text,cv::Point(50,80),1,1.2,cv::Scalar(0,255,255));
    cv::imshow("",img);
    key_input = cv::waitKey(1);
}
void snake::start()
{
    cv::Rect box = cv::Rect(0,0,box_width,box_height);
    int direction = 83; // default move direction ->
    cv::Point next_node,snake_head;
    auto start_time = std::chrono::steady_clock::now();
    while(1)
    {
        drawSnake();
        while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-start_time).count() < time_interval)
        {
            if(key_input > 0) break;
        }
        if(key_input > 0) 
        {
            if(abs(direction - key_input) == 1 || abs(direction - key_input) == 3) //identical or opposite direction change is not allowed
            {
                // std::cout << "direction is changed " << std::endl;
                direction = key_input;
            }
        }
        snake_head = snake_list.front();
        switch (direction)
        {
        case 81: //move left
            next_node.x = snake_head.x - snake_step;
            next_node.y = snake_head.y;
            break;
        case 82: //move up 
            next_node.x = snake_head.x;
            next_node.y = snake_head.y - snake_step;
            break;
        case 83: //move right 
            next_node.x = snake_head.x + snake_step;
            next_node.y = snake_head.y;
            break;
        case 84: //move down 
           next_node.x = snake_head.x;
            next_node.y = snake_head.y + snake_step;
            break;
        default:
            break;
        }
        if(!box.contains(next_node))
        {
            drawSnake(0);
            std::this_thread::sleep_for(std::chrono::seconds(1));
            break;
        }
        if(next_node == food)
        {
            snake_list.push_front(next_node);
            score += 10 ;
            generateFood();
        }
        else
        {
            snake_list.push_front(next_node);
            snake_list.pop_back();
        }
        start_time = std::chrono::steady_clock::now();
    }
}

int main()
{
    snake demo_snake;
    demo_snake.start();
}