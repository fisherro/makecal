# makecal

This is a simple program that takes a list of all-day events in this format...

    2020-01-01 New Years Day
    2020-12-25 Christmas Day

...and generates an iCalendar file.

It reads from stdin and writes to stdout.

Note that the output can be validated with [iCalendar.org's validator](https://icalendar.org/validator.html).
