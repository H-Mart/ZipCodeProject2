#ifndef ENUMS_H
#define ENUMS_H

// idea adapted from https://stackoverflow.com/a/30338543
/// used for tiny csv parsing state machine
enum class CSVState {
    QuotedField,
    UnquotedField,
    QuotedQuote
};

/// contains the values that will be used to determine which member of the place class the field will be unpacked into
enum class HeaderField : int {
    ZipCode,
    PlaceName,
    State,
    County,
    Latitude,
    Longitude,
    Unknown
};

enum class LengthIndicatorType : int {
    ASCII,
    BCD,
    BINARY
};

#endif