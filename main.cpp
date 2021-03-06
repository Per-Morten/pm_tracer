//#define PM_TRACER_USE_RAII
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
    PM_TRACER_END("main", "test_1");
}

void
test_2()
{
    PM_TRACER_BEGIN("main", "test_2");
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    test_1();
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    PM_TRACER_END("main", "test_2");
}

void
test_3()
{
    PM_TRACER_BEGIN("main", "test_3");
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    test_2();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    PM_TRACER_END("main", "test_3");
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
    PM_TRACER_END("main", "main_test");

}

int
main()
{
    PM_TRACER_INIT(stdout);
    std::thread thread1(main_test);
    main_test();
    thread1.join();
    PM_TRACER_SHUTDOWN();
    return 0;
}
