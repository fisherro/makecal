/*
 * Convert a list of all-day events into an iCalendar file
 */
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <uuid/uuid.h>

std::string make_uuid()
{
    uuid_t uuid;
    uuid_generate(uuid);
    char chars[37] = "";
    uuid_unparse(uuid, chars);
    return chars;
}

//Create a string timestamp for now
std::string make_timestamp()
{
    std::time_t now = std::time(nullptr);
    std::tm tm = *std::gmtime(&now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%dT%H%M%SZ");
    return oss.str();
}

int main()
{
    using namespace std::literals;

    //Read the input file
    //Split each line on the first space
    //First part of line is the date in YYYY-MM-DD format
    //The rest is the description of the event
    using in_event_type = std::pair<std::string, std::string>;
    std::vector<in_event_type> in_events;
    std::string in_line;
    while (std::getline(std::cin, in_line)) {
        if (in_line.empty()) continue;
        auto space { in_line.find(' ') };
        if (in_line.npos == space) {
            std::cerr << "Space not found!\n";
            return EXIT_FAILURE;
        }
        auto date { in_line.substr(0, space) };
        date.erase(std::remove(date.begin(), date.end(), '-'), date.end());
        in_events.push_back(
                std::make_pair(date, in_line.substr(space + 1)));
    }

    //From the data collected above, generate the lines of the iCalendar file
    auto timestamp = make_timestamp();
    std::vector<std::string> outlines;
    outlines.push_back("BEGIN:VCALENDAR");
    outlines.push_back("VERSION:2.0");
    outlines.push_back("PRODID:cx.fisher.makecal");
    for (const auto& in_event: in_events) {
        outlines.push_back("BEGIN:VEVENT");
        outlines.push_back("UID:"s + make_uuid());
        outlines.push_back("DTSTAMP:"s + timestamp);
        outlines.push_back("DTSTART;VALUE=DATE:" + in_event.first);
        outlines.push_back("SUMMARY:" + in_event.second);
        outlines.push_back("END:VEVENT");
    }
    outlines.push_back("END:VCALENDAR");

    //Write the iCalendar file lines out and do folding for long lines
    for (auto outline: outlines) {
        const size_t max_without_eoln = 75 - 2;

        if (outline.size() <= max_without_eoln) {
            std::cout << outline << "\r\n";
        } else {
            std::cout << outline.substr(0, max_without_eoln) << "\r\n";
            outline = outline.substr(max_without_eoln);

            while (outline.size() > (max_without_eoln - 1)) {
                std::cout << ' '
                    << outline.substr(0, max_without_eoln - 1) << "\r\n";
                outline = outline.substr(max_without_eoln - 1);
            }
            std::cout << ' ' << outline << "\r\n";
        }
    }

    return EXIT_SUCCESS;
}

