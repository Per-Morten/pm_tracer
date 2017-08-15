# pm_tracer
pm_tracer is a simplistic "library" for generating manual instrumentation tracing logs  
that can be viewed in chrome://tracing.  

The implementation is written in C++, currently linking with C is planned (not implemented).  
It is also planned to have proper C support.  

# Install
Installation of pm_tracer is pretty straight forward, just include the necessary  
header file "pm_tracer.h", compile the "pm_tracer.cpp" file, and you are ready.  

# Usage
Using pm_tracer only requires 3-4 function calls. Two of these being the init and shutdown  
methods.  
Additionally you must ensure that no other output is sent to the same output as the pm_tracer.  
As this might confuse the JSON parser in chrome://tracing.  

# Example
```cpp
#define PM_TRACER_USE_RAII
// PM_TRACER_USE_RAII supports a destructor mechanism that
// stops the timing automatically at end of scope.
#include <pm_tracer.h>
#include <chrono>
#include <thread>

void
test_1()
{
    PM_TRACER_BEGIN("main", "test_1");
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
}

void
test_2()
{
    // Without PM_TRACER_USE_RAII you need to explicitly stop
    // the timing.
    PM_TRACER_BEGIN("main", "test_2");
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    test_1();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    PM_TRACER_BEGIN("main", "test_2");
}

void
test_3()
{
    PM_TRACER_BEGIN("main", "test_3");
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    test_2();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
}

void
main_test()
{
    PM_TRACER_BEGIN("main", "main_test");
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
    test_3();
    std::this_thread::sleep_for(std::chrono::milliseconds(4));
    test_1();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    test_2();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    test_3();
    std::this_thread::sleep_for(std::chrono::milliseconds(40));
}

int
main()
{
    // PM_TRACER_INIT accepts any FILE* so you can open and write to custom
    // files.
    // pm_tracer does not own those files, so you have to manually close
    // them when finished.
    PM_TRACER_INIT(stdout);
    std::thread thread1(main_test);
    main_test();
    thread1.join();
    PM_TRACER_SHUTDOWN();
    return 0;
}

```
