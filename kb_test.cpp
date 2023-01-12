#include <iostream>
#include <termio.h>
#include <stdio.h>
#include <unistd.h> 

//read(0,&ch,1)阻塞进程，有输入才会往下走
//当前自定义的获取键盘信号只适用于终端输入
//无法在有图片显示基础上进行键盘信号捕捉
class listenKeyBoard
{
public:
    listenKeyBoard();
    ~listenKeyBoard();
    bool kbhit();
    int readch();
private:
    struct termios initial_settings, new_settings;
    int peek_character = -1;
}; 
listenKeyBoard::listenKeyBoard()
{
	tcgetattr(0,&initial_settings);
	new_settings = initial_settings;
    new_settings.c_lflag &= (~ICANON);
	new_settings.c_cc[VMIN] = 1;
	new_settings.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_settings);
}
listenKeyBoard::~listenKeyBoard()
{
	tcsetattr(0, TCSANOW, &initial_settings);
}
bool listenKeyBoard::kbhit()
{
	unsigned char ch;
	int nread;
    
	if (peek_character != -1) return true;
    new_settings.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);
	if(nread == 1) 
	{
		peek_character = ch;
        // std::cout << "get keyboard input" << std::endl;
		return true;
	}
	return false;
}
int listenKeyBoard::readch()
{
	char ch;
	if(peek_character != -1) 
	{
		ch = peek_character;
		peek_character = -1;
		return ch;
	}
    else
    {
        return -1;
    }
}

int main()
{
    listenKeyBoard test;
    while(1)
    {
        test.kbhit();
        int ch = test.readch();
        if(ch != -1)
        {
            std::cout << ch << std::endl;
        }
    }
}