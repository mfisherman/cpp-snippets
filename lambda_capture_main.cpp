#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// The code outlines the following scenario:
// There is a main thread running, and you want to run a set of threads in parallel.
// At the same time, you want to keep track of the threads by keeping a "clean up function" for each thread.
// An example could be that you want to run a long running operation
// (e.g. disk access, networking, RPC) and want to synchronize and run some clean ups before you continue.

// It also outlines how lambda captures work and how it is possible to keep an object alive in the lambda using a shared pointer.

// The scenario is mapped to the code in the following way:
// "Context" is the class which keeps the list of clean up functions.
// Each thread calls the function "some_function".
// "MyClass" is an additional object which is captured in the lambda.
// "RegisterPtr" is the class which creates the thread and its clean up function and registers the clean up function.

void some_function()
{
    std::cout << "some_function" << std::endl;
}

class Context
{
public:
    Context()
    {
        std::cout << "Context::Context()" << std::endl;
    }
    ~Context()
    {
        std::cout << "Context::~Context()" << std::endl;
    }

    void register_function(std::function<void()> function)
    {
        functions.emplace_back(std::move(function));
    }

    // This doesn't work.
    void register_unique_function(std::unique_ptr<std::function<void()>> function)
    {
        // unique_functions.push_back(std::move(function));
    }

    void run_functions()
    {
        for (const auto &function : functions)
        {
            function();
        }
    }

    void clear_functions()
    {
        functions.clear();
    }

private:
    std::vector<std::function<void()>> functions;
    // This doesn't work.
    // std::vector<std::unique_ptr<std::function<void()>>> unique_functions;
};

class MyClass
{
public:
    MyClass()
    {
        std::cout << "MyClass::MyClass()" << std::endl;
        x = 1;
    }
    ~MyClass()
    {
        std::cout << "MyClass::~MyClass()" << std::endl;
    }

    void say_hello()
    {
        std::cout << "MyClass::say_hello() " << (x++) << std::endl;
    }

private:
    int x;
};

class RegisterPtr
{
public:
    static void register_shared_ptr(Context &c)
    {
        auto thread = std::make_shared<std::thread>(some_function);
        auto obj = std::make_shared<MyClass>();
        // A std::function needs to be copyable. However, a std::unique_ptr is not, so we can't use that.
        // Instead, the thread is a shared pointer.
        c.register_function([obj, thread]()
                            {
                                std::cout << "From shared_ptr" << std::endl;
                                thread->join();
                                obj->say_hello();
                            });
    }
};

int main(int argc, char **argv)
{
    std::cout << "Start of main" << std::endl;
    {
        Context c;
        {
            RegisterPtr::register_shared_ptr(c);
        }

        c.run_functions();
        c.clear_functions();
    }

    std::cout << "End of main" << std::endl;
    return 0;
}
