#pragma once

#ifdef __cplusplus
    #include <cstdio>
#else
    #include <stdio.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(_MSC_VER)
    #define PM_RESTRICT __restrict
#elif defined(__GNUC__) || defined(__clang__)
    #define PM_RESTRICT __restrict__
#else
    #define PM_RESTRICT
#endif

///////////////////////////////////////////////////////////
// Internal, not to be used.
///////////////////////////////////////////////////////////
void
pm_tracer_initialize(FILE* file);

void
pm_tracer_shutdown();

void
pm_tracer_log(const char* PM_RESTRICT category,
              const char* PM_RESTRICT name,
              const char* PM_RESTRICT phase);

#ifdef PM_TRACER_USE_RAII
    class pm_tracer
    {
    public:
        pm_tracer(const char* PM_RESTRICT category,
                  const char* PM_RESTRICT name)
            : cat_(category)
            , name_(name)
        {
            pm_tracer_log(category, name, "B");
        }

        ~pm_tracer()
        {
            pm_tracer_log(cat_, name_, "E");
        }

    private:
        const char* cat_;
        const char* name_;
    };

#endif // PM_TRACER_USE_RAII
///////////////////////////////////////////////////////////
// EO Internal, not to be used.
///////////////////////////////////////////////////////////

#define PM_TRACER_INIT(file) \
pm_tracer_initialize(file);

#define PM_TRACER_SHUTDOWN() \
pm_tracer_shutdown();

#ifdef PM_TRACER_USE_RAII
    #define PM_TRACER_BEGIN(category, name) \
    pm_tracer t_##__LINE__(category, name);

    #define PM_TRACER_END(category, name)

#else
    #define PM_TRACER_BEGIN(category, name) \
    pm_tracer_log(category, name, "B");

    #define PM_TRACER_END(category, name)  \
    pm_tracer_log(category, name, "E");

#endif // PM_TRACER_USE_RAII

#ifdef __cplusplus
}
#endif
