#include<iostream>
#include<iomanip>
#include<cstdio>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include<memory>
using namespace std;

void error(string err)
{
    throw runtime_error(err);
}

const string prompt = ">";
const string result = "=";
const char quit = 'q';
const char print = ';';
const char number = '8';

class Token{
public:
    char kind;
    double value;
};

class Token_stream{
public:
    Token get();
    void putback(Token t);
    void ignore(char c);
private:
    bool full=false;
    Token buffer;
};

void Token_stream::putback(Token t)
{
    if(full) error("putback() into a full buffer");
    buffer = t;
    full = true;
}

Token Token_stream::get()
{
    if(full){
        full = false;
        return buffer;
    }
    char ch;
    cin>>ch;

    switch (ch){
    case print:
    case quit:
    case'(':case')':case'+':case'-':case'*':case'/':case'%':
        return Token{ch};
    case'.':
    case'0':case'1':case'2':case'3':case'4':
    case'5':case'6':case'7':case'8':case'9':
    {      cin.putback(ch);
        double val;
        cin>>val;
        return Token{number,val};
    }
    default:
        error("Bad token");
    }
}

void Token_stream::ignore(char c)
{
    if(full&&c==buffer.kind){
       full = false;
       return;
    }
    full = false;
    char ch = 0;
    while (cin>>ch) {
        if(ch==c)return;
    }
}

Token_stream ts;
double expression();

double primary()
{
    Token t = ts.get();
    switch(t.kind){
    case'(':
    {   double d = expression();
        t = ts.get();
        if (t.kind!=')')error("')'expected");
        return d;
    }
    case number:
        return t.value;
    case '+':
        return primary();
    case '-':
        return -primary();
    default:
        error("primary expected");
    }
}

double term()
{
    double left = primary();
    Token t = ts.get();

    while(true){
        switch(t.kind){
        case'%':
        {   double d = primary();
            if(d==0)error("divide by zero");
            left = fmod(left,d);
            t = ts.get();
            break;
        }
        case'*':
        {   left *= primary();
            t = ts.get();
            break;
        }
        case'/':
        {   double d = primary();
            if(d==0.0) error("divide by zero");
            left /= d;
            t = ts.get();
            break;
        }
        default:
            ts.putback(t);
            return left;
        }
    }
}

double expression()
{
    double left = term();
    Token t = ts.get();

    while(true){
        switch(t.kind){
        case'+':
            left += term();
            t = ts.get();
            break;
        case'-':
            left -= term();
            t = ts.get();
            break;
        default:
            ts.putback(t);
            return left;
        }
    }
}
void clean_up_mess()
{
    ts.ignore(print);
}

void calculate()
{
    while(cin)
    try {
          cout<<prompt;
          Token t = ts.get();
          while(t.kind == print) t= ts.get();
          if(t.kind == quit) return;
          ts.putback(t);
          cout<<result<< setprecision(16)<<expression()<< '\n';
        }
    catch (exception&e) {
          cerr<<e.what()<<'\n';
          clean_up_mess();
    }
}



int main()
{
    try {
        calculate();
        return 0;
    }
    catch (runtime_error&e) {
       cerr<<e.what()<<'\n';
       cout<<"please enter the character~to close the window\n";
       for(char ch;cin>>ch;)
           if(ch=='~')return 1;
       return 1;
    }
    catch (...) {
       cerr<<"exception"<<'\n';
       cout<<"please enter the character~to close the window\n";
       for(char ch;cin>>ch;)
           if(ch=='~')return 2;
       return 2;
    }
 }
