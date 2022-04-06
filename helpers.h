#ifndef HELPERS_H
#define HELPERS_H

// idea adapted from https://stackoverflow.com/a/30338543
/// used for tiny csv parsing state machine
enum class CSVState {
    QuotedField,
    UnquotedField,
    QuotedQuote
};

/// contains the values that will be used to determine which member of the place class the field will be unpacked into
enum class HeaderField {
    ZipCode,
    PlaceName,
    State,
    County,
    Latitude,
    Longitude,
    Unknown
};

#endif