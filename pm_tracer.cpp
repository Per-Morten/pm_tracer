#include <pm_tracer.h>
#include <chrono>
#include <thread>
#include <cinttypes>

static FILE* pm_tracer_file;

void
pm_tracer_initialize(std::FILE* file)
{
    pm_tracer_file = file;
    std::fprintf(pm_tracer_file,
                 "[\n");
}

void
pm_tracer_shutdown()
{
    std::fprintf(pm_tracer_file,
                 "{}\n]\n");
}

void
pm_tracer_log(const char* PM_RESTRICT category,
              const char* PM_RESTRICT name,
              const char* PM_RESTRICT phase)
{
    using namespace std::chrono;
    auto now = high_resolution_clock::now();
    std::uint64_t time = duration_cast<milliseconds>(now.time_since_epoch()).count();
    std::size_t thread_id = std::hash<std::thread::id>()(std::this_thread::get_id());

    std::fprintf(pm_tracer_file,
                "{ "                            \
                "\"cat\": \"%-15s\", "          \
                "\"pid\": 0, "                  \
                "\"tid\": %20zu, "              \
                "\"ts\": %15" PRIu64 ", "       \
                "\"ph\": \"%s\", "              \
                "\"name\": \"%-15s\", "         \
                "\"args\": {}"                  \
                "},\n",
                category,
                thread_id,
                time,
                phase,
                name);
}
