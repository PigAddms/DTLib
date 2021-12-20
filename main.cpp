#include <iostream>
#include "MatrixGraph.h"
#include "ListGraph.h"
#include "LinkStack.h"

using namespace std;
using namespace DTLib;



int main()
{

    LinkStack<int>* s = new LinkStack<int>();

    s->push(1);

    cout << s->top() << endl;

    return 0;
}
