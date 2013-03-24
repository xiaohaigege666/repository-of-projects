#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "delegate.h"

//refer to:http://rsdn.ru/article/cpp/delegates.xml


class App
{
public:
    typedef CDelegate1<void, string> Callback;

    void OutputToConsole(string str)
    {
        cout << str << endl;
    }

    static void OutputToFile(string str)
    {
        ofstream fout("output.txt", ios::out | ios::app);
        fout << str << endl;
        fout.close();
    }
};

#include <crtdbg.h>
inline void EnableMemLeakCheck()
{
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
}


int main()
{
	EnableMemLeakCheck();

    App app;

	//����ί�С�
    App::Callback callback = NULL;
    if(!callback.empty()) callback("1");

	IDelegate1<void,string>* d0=createDelegate(App::OutputToFile);
	printf("OutputToFile d0:%08X\r\n",d0);
	IDelegate1<void,string>* d1=createDelegate(&app, &App::OutputToConsole);
	printf("OutputToConsole d1:%08X\r\n",d1);

	printf("\r\nstart 2\r\n");
	callback += d0;
	if(!callback.empty()) callback("2");

	printf("\r\nstart 3\r\n");
	callback += d1;
	if(!callback.empty()) callback("3");

	printf("\r\nstart 4\r\n");
	callback -= d0;
	if(!callback.empty()) callback("4");

	printf("\r\nstart 5\r\n");
	callback -= d1;
	if(!callback.empty()) callback("5");

	system("pause");
}