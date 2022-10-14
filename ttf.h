#ifndef TTF_H


//
// This Conversion program was written by Marmayogi, Astrologer and Palmist, Sri Mahakali Jyothida Nilayam, Coimbatore, India.
// This Converts TTF to a Type 2 CIDFont, with base font Type 42, which can be accessed through a postscript program.
// This is particulary useful for Indian Languages having glyphs in excess of 255 in the character set.
// 
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.
//

#include <stdint.h>			// Refer for NULL definition 

// What is the use of little endian and big-endian?
// i)  A big-endian system stores the most significant byte of a word at the smallest memory address and the least significant byte at the largest. 
// ii) A little-endian system, in contrast, stores the least-significant byte at the smallest address.
//
// Note that 'Big Endian' is a bit more human-readable. Bits are stored in memory as they appear in logical order (most-significant values first), just like for any human-used number system.
// 'First' means lower address.
//
// Big Endian is used by Motorolla processors and Little Endian is used by Intel processors.
// Since TTF files are stored in Big-Endian format, the Endianess of Windows systems is different where files are in Little Endian.
// So, the following macros are required.

#undef HIBYTE
#undef LOBYTE
#undef HIWORD
#undef LOWORD
#undef SWAPWORD
#undef SWAPLONG
#undef SWAPLONGLONG

#define HIBYTE(word)                ((uint8_t)((word) >> 8))                                        /* get higher byte */
#define LOBYTE(word)                ((uint8_t)((word) & 0xff))                                      /* get lower byte */
#define HIWORD(dword)               ((uint16_t)((dword) >> 16))                                     /* get lower word */
#define LOWORD(dword)               (((uint16_t)(dword)) & 0xffff)                                  /* get higher word */
#define HILONG(qword)               ((uint32_t)((qword) >> 32))                                     /* get lower long(dword) */
#define LOLONG(qword)               (((uint32_t)(qword)) & 0xffffffff)                              /* get higher long(dword) */
#define SWAPWORD(word)              ((((uint16_t)LOBYTE(word)) << 8) | (uint16_t)(HIBYTE(word)))    /* 16 bits: convert big endian to little endian */
#define SWAPLONG(dword)             ( (((uint32_t)SWAPWORD(LOWORD(dword))) << 16) | (uint32_t)SWAPWORD(HIWORD(dword)) )   /* 32 bits: convert big endian to little endian */
#define SWAPLONGLONG(qword)         ( (((uint64_t)SWAPLONG(LOLONG(qword))) << 32) | (uint64_t)SWAPLONG(HILONG(qword)) )   /* 64 bits: convert big endian to little endian */

//
// The offset subtable, structured below, begins with the scaler type of the font. 
// The number of tagged tables in the 'sfnt' follows.The table directory itself and any subtables are not included in this count. 
// The entries for searchRange, entrySelector and rangeShift are used to facilitate quick binary searches of the table directory that follows. 
// Unless a font has a large number of tables, a sequential search will be fast enough.
//
typedef struct STTFOffsetSubTable {
    uint32_t scalerType;                // A tag to indicate the OFA scaler to be used to rasterize this font. Fonts with TrueType outlines produced for OS X or iOS only are encouraged to use 'true' (0x74727565) for the scaler type value. Fonts for Windows or Adobe products must use 0x00010000.
    uint16_t numOfTables;               // number of tables
    uint16_t searchRange;               // (maximum power of 2 <= numTables)*16
    uint16_t entrySelector;             // log2(maximum power of 2 <= numTables)
    uint16_t rangeShift;                // numTables*16-searchRange
} STTFOffsetSubTable;

// The structure of the table directory follows the offset subtable. 
// Entries in the table directory must be sorted in ascending order by tag. 
// Each table in the font file must have its own table directory entry. Table 5 documents the .
typedef struct STTFTableDirectory {
    char tag[4];                        // The tables have names known as tags. Tags have the type uint32. Currently defined tag names consist of four characters. Tag names with less than four characters have trailing spaces.
    uint32_t checksum;                  // Check sum
    uint32_t offset;                    // Offset from beginning of file
    uint32_t length;                    // length of the table in bytes
} STTFTableDirectory;

//
// The name table header gives 
// i)   the format of the table,
// ii)  the number of name records (rows) in the table, and
// iii) the offset in bytes to the multilingual character name strings.
// The format of the structure of name table header is given below.
//
typedef struct STTFNameTable { // The 'name' table. Total elements 3.
    uint16_t format;                    //format selector. Always 0
    uint16_t nrCount;                   // The number of nameRecords in this name table.
    uint16_t stringOffset;              // Offset in bytes to the beginning of the name character strings.
} STTFNameTable;

// The name records array follows the name table header.
// Six entries are provided for each name record.

typedef struct STTFNameRecord { // The 'namerecord' entries in 'name' table whose total elements are 6.
    uint16_t platformID;                // Platform identifier code.
    uint16_t platformSpecificID;        // Platform-specific encoding identifier.
    uint16_t languageID;                // Language identifier.
    uint16_t nameID;                    // Name identifier.
    uint16_t length;                    // Name string length in bytes.
    uint16_t offset;                    // Name string offset in bytes from stringOffset. Refer name table header STTFNameTable.
} STTFNameRecord;

typedef struct STTFHeadTable { // The 'head' table. Total elements 17.
    struct {
        int16_t whole;                  // Whole part of format. If format is 2.5 then, whole will hold 2.
        int16_t frac;                   // Fractional part of format. If format is 2.5 then, fraction is 5.
    } version;                          // 0x00010000 if (version 1.0)
    struct {
        int16_t whole;                  // Whole part of format. If format is 2.5 then, whole will hold 2.
        int16_t frac;                   // Fractional part of format. If format is 2.5 then, fraction is 5.
    } fontRevision;                     // set by font manufacturer
    uint32_t checkSumAdjustment;        // To compute: set it to 0, calculate the checksum for the 'head' table and put it in the table directory, sum the entire font as a uint32_t, then store 0xB1B0AFBA - sum. (The checksum for the 'head' table will be wrong as a result. That is OK; do not reset it.)
    uint32_t magicNumber;               // set to 0x5F0F3CF5
    uint16_t flags;                     // Name string length in bytes.
    uint16_t unitsPerEm;                // range from 64 to 16384. Any value in this range is valid. In fonts that have TrueType outlines, a power of 2 is recommended as this allows performance optimizations in some rasterizers.
    int64_t created;                    // international date
    int64_t modified;                   // international date
    int16_t xMin;                       // for all glyph bounding boxes.
    int16_t yMin;                       // for all glyph bounding boxes.
    int16_t xMax;                       // for all glyph bounding boxes.
    int16_t yMax;                       // for all glyph bounding boxes.
    uint16_t macStyle;                  // bit 0 bold; bit 1 italic; bit 2 underline;  bit 3 outline; bit 4 shadow, bit 5 condensed (narrow) and bit 6 extended.
    uint16_t lowestRecPPEM;             // smallest readable size in pixels.
    int16_t fontDirectionHint;          // {0 Mixed directional glyphs, 1 Only strongly left to right glyphs, 2 Like 1 but also contains neutrals, -1 Only strongly right to left glyphs, -2 Like -1 but also contains neutrals}
    int16_t indexToLocFormat;           // {0 for short offsets, 1 for long}
    int16_t glyphDataFormat;            // {0 for current format}
} STTFHeadTable;

typedef struct STTFHheaTable {  // The 'hhea' table. Total elements 17.
    struct {
        int16_t whole;                  // Whole part of format. If format is 2.5 then, whole will hold 2.
        int16_t frac;                   // Fractional part of format. If format is 2.5 then, fraction is 5.
    } version;                          // 0x00010000 if (version 1.0)
    int16_t ascent;                 // Distance from baseline of highest ascender
    int16_t descent;                // Distance from baseline of lowest descender
    int16_t lineGap;                // typographic line gap. Negative LineGap values are treated as zero in some legacy platform implementations.
    uint16_t advanceWidthMax;       // must be consistent with horizontal metrics
    int16_t minLeftSideBearing;     // must be consistent with horizontal metrics
    int16_t minRightSideBearing;    // must be consistent with horizontal metrics
    int16_t xMaxExtent;             // max(lsb + (xMax-xMin))
    int16_t caretSlopeRise;         // used to calculate the slope of the caret (rise/run) set to 1 for vertical caret
    int16_t caretSlopeRun;          // 0 for vertical
    int16_t caretOffset;            // The amount by which a slanted highlight on a glyph needs to be shifted to produce the best appearance. Set to 0 for non-slanted fonts.
    int16_t reserved_1;             // set value to 0
    int16_t reserved_2;             // set value to 0
    int16_t reserved_3;             // set value to 0
    int16_t reserved_4;             // set value to 0
    int16_t metricDataFormat;       // 0 for current format
    uint16_t numOfLongHorMetrics;   // number of advance widths in metrics table
} STTFHheaTable;

//
// If numOfLongHorMetrics is less than the total number of glyphs, then the hMetrics array is followed by an array for the left side bearing values of the remaining glyphs.
// The number of elements in the leftSideBearings array will be derived from the numGlyphs field in the 'maxp' table minus numberOfHMetrics
//
typedef struct STTFHmtxTable_HMetrix {  // 'hmtx' Table: Paired advance width and left side bearing values for each glyph. Records are indexed by glyph ID.
    uint16_t advanceWidth;              // Advance width, in font design units.
    int16_t lsb;                        // Glyph left side bearing, in font design units.
} STTFHmtxTable_HMetrix;
typedef struct STTFHmtxTable_LeftSideBearings {  // 'hmtx' Table: Left side bearings for glyph IDs greater than or equal to numOfLongHorMetrics(Refer 'hhea' table).
    int16_t leftSideBearings;           // Here the advanceWidth is assumed to be the same as the advanceWidth for the last entry above structure.
} STTFHmtxTable_LeftSideBearings;

typedef struct STTFMaxpTable {  // The 'mapf' table. Total elements 15.
    struct {
        int16_t whole;                  // Whole part of format. If format is 2.5 then, whole will hold 2.
        int16_t frac;                   // Fractional part of format. If format is 2.5 then, fraction is 5.
    } version;                          // 0x00010000 if (version 1.0)
    uint16_t numGlyphs;                 // the number of glyphs in the font
    uint16_t maxPoints;                 // points in non-compound glyph
    uint16_t maxContours;               // contours in non-compound glyph
    uint16_t maxComponentPoints;        // points in compound glyph
    uint16_t maxComponentContours;      // contours in compound glyph
    uint16_t maxZones;                  // set to 2
    uint16_t maxTwilightPoints;         // points used in Twilight Zone (Z0)
    uint16_t maxStorage;                // number of Storage Area locations
    uint16_t maxFunctionDefs;           // number of FDEFs
    uint16_t maxInstructionDefs;        // number of IDEFs
    uint16_t maxStackElements;          // maximum stack depth
    uint16_t maxSizeOfInstructions;     // byte count for glyph instructions
    uint16_t maxComponentElements;      // number of glyphs referenced at top level
    uint16_t maxComponentDepth;         // levels of recursion, set to 0 if font has only simple glyphs
} STTFMaxpTable;

typedef struct STTFPostTable {  // The 'post' table. Total elements 9.
    struct {
        int16_t whole;                  // Whole part of format. If format is 2.5 then, whole will hold 2.
        int16_t frac;                   // Fractional part of format. If format is 2.5 then, fraction is 5.
    } format;                           // Format of this table: {0x00010000 for version 1.0, 0x00020000 for version 2.0, 0x00025000 for version 2.5, 0x00030000 for version 3.0}
    struct {
        int16_t whole;                  // Whole part of format. If format is 2.5 then, whole will hold 2.
        int16_t frac;                   // Fractional part of format. If format is 2.5 then, fraction is 5.
    } italicAngle;                      // Italic angle in counter-clockwise degrees from the vertical. Zero for upright text, negative for text that leans to the right (forward).
    int16_t underlinePosition;          // This is the suggested distance of the top of the underline from the baseline (negative values indicate below baseline).
    int16_t underlineThickness;         // Suggested values for the underline thickness. In general, the underline thickness should match the thickness of the underscore character (U+005F LOW LINE), and should also match the strikeout thickness, which is specified in the OS/2 tabl
    uint32_t isFixedPitch;              // Set to 0 if the font is proportionally spaced, non-zero if the font is not proportionally spaced (i.e. monospaced).
    uint32_t minMemType42;              // Minimum memory usage when a TrueType font is downloaded as a Type 42 font
    uint32_t maxMemType42;              // Maximum memory usage when a TrueType font is downloaded as a Type 42 font
    uint32_t minMemType1;               // Minimum memory usage when a TrueType font is downloaded as a Type 1 font
    uint32_t maxMemType1;               // Maximum memory usage when a TrueType font is downloaded as a Type 1 font
} STTFPostTable;

typedef struct STTFOS2Table {  // The 'OS/2' table. Total elements 15.
    uint16_t version;                   // 0x0005
    int16_t xAvgCharWidth;              // average weighted advance width of lower case letters and space.
    uint16_t usWeightClass;             // visual weight (degree of blackness or thickness) of stroke in glyphs.
    uint16_t usWidthClass;              // relative change from the normal aspect ratio (width to height ratio) as specified by a font designer for the glyphs in the font.
    uint16_t fsType;                    // characteristics and properties of this font (set undefined bits to zero).
    int16_t ySubscriptXSize;            // recommended horizontal size in pixels for subscripts.
    int16_t ySubscriptYSize;            // recommended vertical size in pixels for subscripts.
    int16_t ySubscriptXOffset;          // recommended horizontal offset for subscripts
    int16_t ySubscriptYOffset;          // recommended vertical offset form the baseline for subscripts
    int16_t ySuperscriptXSize;          // recommended horizontal size in pixels for superscripts
    int16_t ySuperscriptYSize;          // recommended vertical size in pixels for superscripts
    int16_t ySuperscriptXOffset;        // recommended horizontal offset for superscripts
    int16_t ySuperscriptYOffset;        // recommended vertical offset from the baseline for superscripts
    int16_t yStrikeoutSize;             // width of the strikeout stroke
    int16_t yStrikeoutPosition;         // position of the strikeout stroke relative to the baseline
    int16_t sFamilyClass;               // classification of font-family design.
    uint8_t panose[10];                 // 10 byte series of number used to describe the visual characteristics of a given typefac
    uint32_t ulUnicodeRange1;           // Bits 0–31
    uint32_t ulUnicodeRange2;           // Bits 32–63
    uint32_t ulUnicodeRange3;           // Bits 64–95
    uint32_t ulUnicodeRange4;           // Bits 96–127
    uint8_t achVendID[4];               // four character identifier for the font vendor
    uint16_t fsSelection;               // 2-byte bit field containing information concerning the nature of the font patterns
    uint16_t usFirstCharIndex;          // The minimum Unicode index in this font.
    uint16_t usLastCharIndex;           // The maximum Unicode index in this font.
    int16_t sTypoAscender;              // The typographic ascender for this font. This is not necessarily the same as the ascender value in the 'hhea' table.
    int16_t sTypoDescender;             // The typographic descender for this font. This is not necessarily the same as the descender value in the 'hhea' table.
    int16_t sTypoLineGap;               // The typographic line gap for this font. This is not necessarily the same as the line gap value in the 'hhea' table.
    uint16_t usWinAscent;               // The ascender metric for Windows. usWinAscent is computed as the yMax for all characters in the Windows ANSI character set.
    uint16_t usWinDescent;              // The descender metric for Windows. usWinDescent is computed as the -yMin for all characters in the Windows ANSI character set.
    uint32_t ulCodePageRange1;          // Bits 0-31
    uint32_t ulCodePageRange2;          // Bits 32-63
    int16_t sxHeight;                   // The distance between the baseline and the approximate height of non-ascending lowercase letters measured in FUnits.
    int16_t sCapHeight;                 // The distance between the baseline and the approximate height of uppercase letters measured in FUnits.
    uint16_t usDefaultChar;             // The default character displayed by Windows to represent an unsupported character. (Typically this should be 0.)
    uint16_t usBreakChar;               // The break character used by Windows.
    uint16_t usMaxContext;              // The maximum length of a target glyph OpenType context for any feature in this font.
    uint16_t usLowerOpticalPointSize;   // Proposed for version 5 The lowest size (in twentieths of a typographic point), at which the font starts to be used. This is an inclusive value.
    uint16_t usUpperOpticalPointSize;   // Proposed for version 5 The highest size (in twentieths of a typographic point), at which the font starts to be used. This is an exclusive value. Use 0xFFFFU to indicate no upper limit.
} STTFOS2Table;

//
// There are two versions of this table, the short and the long. 
// The version used is specified in the Font Header ('head') table in the indexToLocFormat field. {0 for short offsets, 1 for long}
// The choice of long or short offsets is dependent on the maximum possible offset distance.
//
typedef struct STTFLocaTable_Short {  // 'loca' Table: short version
    uint16_t offsets;                   // The actual local offset divided by 2 is stored. The value of n is the number of glyphs in the font + 1. The number of glyphs in the font is found in the maximum profile table
} STTFLocaTable_short;
typedef struct STTFLocaTable_Long {  // 'loca' Table: long version
    uint32_t offsets;                   // The actual local offset is stored. The value of n is the number of glyphs in the font + 1. The number of glyphs in the font is found in the maximum profile table.
} STTFLocaTable_long;

//
// The 'prep' table stores the instructions that make up the control value program, 
// a set of TrueType instructions that will be executed once when the font is first accessed and again whenever the font, point size or transformation matrix change. 
// It consists of an ordered list of instructions opcodes. Each opcode is a byte. 
// Any instruction is valid in the CV Program but since no glyph is associated with it, instructions intended to move points within a particular glyph outline cannot be used in the CV Program.
//
// Note: The tag 'prep' is anachronistic: in earlier specifications, the table was referred to as the Pre Program table.
//
typedef struct STTFPrepTable {  // 'prep' Table
    uint8_t controlValueProgram;        // Set of instructions executed whenever the point size or font transformation change. Size n is the number of uint8_t items that fit in the size of the table.
} STTFPrepTable;

//
// This table is similar to the CVT Program, except that it is only run once, when the font is first used. 
// It is used only for FDEFs and IDEFs. Thus the CVT Program need not contain function definitions. 
// However, the CVT Program may redefine existing FDEFs or IDEFs.
//
typedef struct STTFFpgmTable {  // 'fpgm' Table. This means 'Font Program'.
    uint8_t instructions;        // Instructions. Length of this array is the number of uint8 items that fit in the size of the table.
} STTFFpgmTable;

//
// This table contains a list of values that can be referenced by instructions. 
// They can be used, among other things, to control characteristics for different glyphs. 
// The length of the table must be an integral number of FWORD units. 
//
typedef struct STTFCvtTable {  // 'cvt' Table. This means 'Control Value Table'.
    int16_t controlValues;        // List of n values referenceable by instructions. The length 'n' is the number of FWORD items that fit in the size of the table.
} STTFCvtTable;


#if __GNUC__ || __CYGWIN__			// gcc
#define sprintf_s snprintf
#define fprintf_s fprintf
#define memcpy_s memcpy_s
#define strcpy_s strcpy_s
#define strcat_s strcat_s
#define strerror_s strerror_s
#define _itoa_s _itoa_s
#define _strnicmp strncasecmp 
#define _stricmp strcasecmp 
#define _getch getch 

#endif


#if __GNUC__ || __CYGWIN__			// gcc
    static inline char* strcpy_s(char* strDest, size_t size, const char* strSource)
    {
        return strncpy(strDest, strSource, size);
    }
    static inline char* strcat_s(char* strDest, size_t size, const char* strSource)
    {
        return strncat(strDest, strSource, size);
    }
    static inline void* memcpy_s(void* dest, size_t destSize, const void* src, size_t count)
    {
        return memcpy(dest, src, count);
    }
    #if (_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && !_GNU_SOURCE

    static inline int strerror_s(char* buffer, size_t sizeInBytes, int errnum)
    {
        return strerror_r(errnum, buffer, sizeInBytes);
    }
    #else
    static inline char* strerror_s(char* buffer, size_t sizeInBytes, int errnum)
    {
        //return strerror_r(errnum, buffer, sizeInBytes);			// This works but error message is not copied into buffer
        const char* ptrerr = strerror(errnum);						// use this non-thread safe function. No other go!
        strcpy_s(buffer, sizeInBytes, ptrerr);
        return buffer;
    }
    #endif
    static inline char* _itoa_s(int value, char* buffer, size_t size, int radix)
    {
        snprintf(buffer, size, "%-d", value);
        return buffer;
    }
#endif



#define TTF_H
#endif	
