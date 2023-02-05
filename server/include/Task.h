#include <queue>
using std::queue;

class Task
{
public:
    virtual void process() = 0;
    virtual ~Task() {}
};

