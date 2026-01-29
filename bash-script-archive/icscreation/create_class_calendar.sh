#!/usr/bin/env bash

# Function to generate an ICS file for a class
generate_ics() {
    local class_name="$1"
    local start_time="$2"
    local end_time="$3"
    local days_per_week="$4"
    local days_of_week="$5"
    local end_date="$6"

    # ICS file header
    echo "BEGIN:VCALENDAR" > "${class_name}.ics"
    echo "VERSION:2.0" >> "${class_name}.ics"
    echo "CALSCALE:GREGORIAN" >> "${class_name}.ics"

    # ICS event details
    echo "BEGIN:VEVENT" >> "${class_name}.ics"
    echo "SUMMARY:${class_name}" >> "${class_name}.ics"
    echo "DTSTART;TZID=America/New_York:${start_time}" >> "${class_name}.ics"
    echo "DTEND;TZID=America/New_York:${end_time}" >> "${class_name}.ics"
    echo "RRULE:FREQ=WEEKLY;COUNT=${days_per_week};BYDAY=${days_of_week};UNTIL=${end_date}" >> "${class_name}.ics"
    echo "END:VEVENT" >> "${class_name}.ics"

    # ICS file footer
    echo "END:VCALENDAR" >> "${class_name}.ics"

    echo "ICS file for ${class_name} created successfully."
}

# User input
read -p "Enter class name: " class_name
read -p "Enter start time (YYYYMMDDTHHMMSS format): " start_time
read -p "Enter end time (YYYYMMDDTHHMMSS format): " end_time
read -p "Enter how many days a week it occurs: " days_per_week
read -p "Enter the days of the week it occurs (e.g., MO,WE,FR): " days_of_week
read -p "Enter the end date (YYYYMMDD format): " end_date

# Generate the ICS file
generate_ics "$class_name" "$start_time" "$end_time" "$days_per_week" "$days_of_week" "$end_date"
