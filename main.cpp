//
// This Conversion program was written by Marmayogi, Astrologer and Palmist, Sri Mahakali Jyothida Nilayam, Coimbatore, India.
// This Converts TTF to a Type 2 CIDFont, with base font Type 42, which can be accessed through a postscript program.
// This is particulary useful for Indian Languages having glyphs in excess of 255 in the character set.
// 
// Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "ttf.h"

const char cSP = ' ';
void psInitPostscript(FILE *fps, const char* pIndianFontName)
{
	static const char *arrPS[] = {
		"%!PS-Adobe-3.0",
		"%%Pages: (atend)",
		"%%LanguageLevel: 3",
		"%%Creator: Marmayogi, Astrologer and Palmist, Sri Mahakali Jothida Nilayam, Coimbatore, India.",
		"%%Title: (TTF2PostscriptCID Conversion Software)",
		"%%DocumentPaperSizes: a4",
		"%%Orientation: Portrait",
		"%%DocumentMedia: Plain 595 842 80 white ()",
		"%%EndComments",
		"%%BeginDefaults",
        "%%PageMedia: Plain",
        "%%PageOrder: Ascend",
        "%%PageOrientation: Portrait",
		"%%EndDefaults",
		"%----------------------------------------------------------",
		"%%BeginProlog",
		"%%EndProlog",
		"%----------------------------------------------------------",
		"%%Page: Marmayogi    1",
		"newpath",
		"%----------------------------------------------------------",
		"/myHelvetica {/Helvetica findfont exch scalefont setfont} bind def",			                //  15 myHelvetica						# Helvetica is data font
        "/myTimesRoman {/Times-Roman findfont exch scalefont setfont} bind def",		                //  15 myTimesRoman						# Times-Roman is data font
        "/myTimesBold {/Times-Bold findfont exch scalefont setfont} bind def",		                    //  15 myTimesBold						# Times-Bold is data font
        "%----------------------------------------------------------",
		"/CTXT {dup stringwidth pop 3 -1 roll 2 copy lt {sub neg 2 div 4 -1 roll add 3 -1 roll} {pop pop 3 1 roll} ifelse  moveto show} bind def",	// Center text. usage: X Y Width (Text) CTX. For example: 36 300 500 (Marmayogi) CTXT
	};
    const short totalElements = sizeof(arrPS) / sizeof(char*);
    short ii = -1;
	while (++ii < totalElements) {
        fprintf(fps, "%s\n", arrPS[ii]);
	};
    fprintf(fps, "/myfont {/%s findfont exch scalefont setfont} bind def\n", pIndianFontName);

}
void psPageNumber(FILE* fps, const int pPageNumber)
{
    const int cPgNum_x = 530;						// Page number which should be written at the bootm right side. (x-coordinate)
    const int cPgNum_y = 5;			                // Page number which should be written at the bootm right side. (y-coordinate)
    fprintf(fps, "gsave\n");
    fprintf(fps, "11 myTimesRoman\n");
    fprintf(fps, "%d  %d moveto (Page) show\n", cPgNum_x, cPgNum_y);	// Page title
    fprintf(fps, "11 myTimesBold\n");
    fprintf(fps, "6 0 rmoveto (%d) show\n", pPageNumber);				// page number (starts from one)
    fprintf(fps, "grestore\n");
}
void psWriteFooter(FILE* fps)
{
    static const char* psLogo = "This conversion software was designed, developed and distributed by Marmayogi, Sri Mahakali Jothida Nilayam, Coimbatore, India.";
    const short cFooterLogo_x = 36;					// footer logo x co-ordinate
    const short cFooterLogo_y = 5;		            // footer logo y co-ordinate
    const short cFooterLine_x = 36;			        // footer line x co-ordinate 
    const short cFooterLine_y = 15;	                // footer line y co-ordinate 
    const short cLineLength = 500;					// line length in points
    // set footer
    fprintf(fps, "gsave\n");
    fprintf(fps, "newpath\n");
    fprintf(fps, "9 myTimesRoman %d %d %d (%s) CTXT\n", cFooterLogo_x, cFooterLogo_y, cLineLength, psLogo);					// write logo at footer
    fprintf(fps, "0.5 setgray	%d %d moveto 36 %d add 0 rlineto stroke\n", cFooterLine_x, cFooterLine_y, cLineLength);		// write footer line hanging over logo
    fprintf(fps, "grestore\n");
}
void psSendPageToDevice(FILE* fps)
{
    fprintf(fps, "showpage\n");			// print page to postscript device
}
void psFlushReport(FILE* fps, const short pPageNum)
{
    psWriteFooter(fps);
    psPageNumber(fps, pPageNum);        // write page number
    psSendPageToDevice(fps);            // print page to postscript device}
}
void psInitNextPage(FILE* fps, const int pPageOrdinal)
{
    fprintf(fps, "%sPage: Marmayogi %4d\n", "%%", pPageOrdinal);		// page number is made up of a label and an ordinal number
}
void printAlphabet_T42(FILE* fps, const short pTotalGlyphs, const char *pIndianFontName)
{
	//
	// This font has been convertedf from ttf to CIDFont type 2 with base font type 42 (double byte). 
	// Print Alphabets in 8 X 16 matrix form
	//
	// 1. fps is input parameter of type FILE which holds Postscript program instructions.
	// 2. pTotalGlyphs is input parameter indicating total glyphs associated with the font.
	//
    psInitPostscript(fps, pIndianFontName);
    const char* pFontName = "myfont";                   // Font to print Glyphs.
    const char* pTitleFontName = "myHelvetica";         // Font to print Titles
    int pagenum = 0;
	const short lcResidual = pTotalGlyphs % 128;
	const short lcLoop = pTotalGlyphs / 128 + (lcResidual > 0);
	for (short ii = 0; ii < lcLoop; ii++) {
		for (short jj = 0, cntGlyph=0; jj < 8; jj++) {
			unsigned int titlecode = ii * 128 + (jj << 4);
			fprintf(fps, "10 %s\n", pTitleFontName);													// set font to print table title
			fprintf(fps, "50 %d add 760 moveto (%d) show\n", 35 * jj, titlecode);						// print column title
			fprintf(fps, "350 %d add 760 moveto (%d) show\n", 30 * jj, titlecode);						// print column title
			fprintf(fps, "50 750 moveto %d 0 rlineto stroke\n", 530);									// Horizontal line
			fprintf(fps, "35 735 moveto 0 -%d rlineto stroke\n", 615);									// Vertical line
			fprintf(fps, "13 %s\n", pTitleFontName);													// set font to print table title
			fprintf(fps, "50 790 260 (Glyph) CTXT\n");													// Write title 'Glyph' by centering at LHS.
			fprintf(fps, "350 790 230 (CID) CTXT\n");													// Write title 'CID' by centering at RHS.
			for (short kk = 0; kk < 16; kk++) {
				if (lcResidual && ii == lcLoop - 1 && cntGlyph == lcResidual) goto Label_Getout;
				uint16_t cid = ii * 128 + jj * 16 + kk;													// CID value corresponding to the Glyph.
				const short xPos_1 = 35 * jj;															// X-Coordinate.
				const short xPos_2 = 30 * jj;															// X-Coordinate.
				const short yPos = 40 * kk;																// Y-Coordinate.
				fprintf(fps, "10 %s\n", pTitleFontName);												// set font to print row title
				fprintf(fps, "20 725 %d sub moveto (%1X) show\n", yPos, kk);							// print row title.
				fprintf(fps, "12 %s\n", pFontName);												        // set font to print alphabets.
                fprintf(fps, "50 %d add 725 %d sub moveto <%04x> show\n", xPos_1, yPos, cid);           // print alphabets
                fprintf(fps, "11 %s\n", pTitleFontName);												// set font to print character code.
				fprintf(fps, "350 %d add 725 %d sub moveto (%d) show\n", xPos_2, yPos, cid);			// print character code of alphabets.
				cntGlyph++;
			}
		}
		Label_Getout:
		psFlushReport(fps, ++pagenum);
		if (ii < lcLoop - 1) {
			psInitNextPage(fps, pagenum +1);		// setup for next page
		}
	}
}
void debugSwap()
{
    //
    // This functions tests macros defined in ttf.h
    //


    uint16_t val_16 = 0x6475;
    printf("Big Endian     : 0x%04x\n", val_16);
    printf("  High byte    : 0x%02x\n", HIBYTE(val_16));
    printf("  Low Byte     : 0x%02x\n", LOBYTE(val_16));
    printf("Little Endian  : 0x%04x\n\n", SWAPWORD(val_16));

    uint32_t val_32 = 0x15251626;
    printf("Big Endian     : 0x%08x\n", val_32);
    printf("  High Word    : 0x%02x\n", LOWORD(val_32));
    printf("  Low Word     : 0x%02x\n", HIWORD(val_32));
    printf("Little Endian  : 0x%08x\n\n", SWAPLONG(val_32));

    uint64_t val_64 = 0x3848394915251626;           // 38 48 39 49 | 15 25 16 26

#if _MSC_VER			// Visual Studio
    printf("Big Endian     : 0x%016llx\n", val_64);
    printf("  High DWord   : 0x%08lx\n", HILONG(val_64));
    printf("  Low DWord    : 0x%08lx\n", LOLONG(val_64));
    printf("Little Endian  : 0x%016llx\n\n", SWAPLONGLONG(val_64));
#elif __GNUC__	|| __CYGWIN__		// gcc
    printf("Big Endian     : 0x%016lx\n", val_64);
    printf("  High DWord   : 0x%08x\n", HILONG(val_64));
    printf("  Low DWord    : 0x%08x\n", LOLONG(val_64));
    printf("Little Endian  : 0x%016lx\n\n", SWAPLONGLONG(val_64));
#endif

}
void printString(const char *pString)
{
    //
    // This functions printfs pString parameter which is Hexadecimal string representing Subtable and Directory for Table entries.
    // Total bytes of this string will be sizeof(STTFOffsetSubTable) + sizeof(STTFTableDirectory) * NumberOfTables.
    //

    const char cSP = ' ';
    unsigned int scalerType, numOfTables, searchRange, entrySelector, rangeShift;
#if _MSC_VER			// Visual Studio c++ compiler.
    int retvalue = sscanf_s(pString, "%8x", &scalerType);
    retvalue = sscanf_s(pString+8, "%4x", &numOfTables);		// total 1 fields
    retvalue = sscanf_s(pString+12, "%4x", &searchRange);
    retvalue = sscanf_s(pString+16, "%4x", &entrySelector);
    retvalue = sscanf_s(pString+20, "%4x", &rangeShift);
#elif __GNUC__			// gcc compiler.
    sscanf(pString, "%8x", &scalerType);
    sscanf(pString+8, "%4x", &numOfTables);		// total 1 fields
    sscanf(pString+12, "%4x", &searchRange);
    sscanf(pString+16, "%4x", &entrySelector);
    sscanf(pString+20, "%4x", &rangeShift);
#endif

    printf("\n   ScalarType             NumOfTables    SearchRange      EntrySelector    RangeShift\n");
    printf("   %u(0x%08X) %*c%u(0x%04X) %*c%u(0x%04X) %*c%u(0x%04X) %*c%u(0x%04X)\n\n",
        scalerType, scalerType,
        5, cSP, numOfTables, numOfTables,
        5, cSP, searchRange, searchRange,
        5, cSP, entrySelector, entrySelector,
        7, cSP, rangeShift, rangeShift
    );
    printf("        Table     Checksum                       Offset                 Length\n");
    printf("        ---------------------------------------------------------------------------------\n");
    for (uint16_t ii = 0, addr=24; ii < numOfTables; ii++, addr += 32) {
        char tag[4] = {' '};
        unsigned int checksum, offset, length;
#if _MSC_VER			// Visual Studio c++ compiler.
        for (short jj = 0; jj < 4; jj++) {
            unsigned int byte;
            int retvalue = sscanf_s(pString+addr+jj*2, "%2x", &byte);		// extract table name
            tag[jj] = byte & 0xff;
        }
        retvalue = sscanf_s(pString+addr+8, "%8x", &checksum);		// extract checksum
        retvalue = sscanf_s(pString+addr+16, "%8x", &offset);		// extract offset
        retvalue = sscanf_s(pString+addr+24, "%8x", &length);		// extract length

#elif __GNUC__			// gcc compiler.
        for (short jj = 0; jj < 4; jj++) {
            unsigned int byte;
            sscanf(pString+addr+jj*2, "%2x", &byte);		// extract table name
            tag[jj] = byte & 0xff;
        }
        sscanf(pString+addr+8, "%8x", &checksum);		// extract checksum
        sscanf(pString+addr+16, "%8x", &offset);		// extract offset
        sscanf(pString+addr+24, "%8x", &length);		// extract length
#endif
        char bufCheckSum[50], bufOffset[50], bufLength[50];
        sprintf_s(bufCheckSum, sizeof(bufCheckSum), "%u(0x%08X)", checksum, checksum);
        sprintf_s(bufOffset, sizeof(bufOffset), "%u(0x%08X)", offset, offset);
        sprintf_s(bufLength, sizeof(bufLength), "%u(0x%08X)", length, length);
        printf("   %2d)  %.*s %*c%-25s %*c%-17s %*c%-17s\n",
            ii + 1,
            4, tag,
            5, cSP, bufCheckSum,
            5, cSP, bufOffset,
            5, cSP, bufLength
        );
    }
    getchar();
}
void debugString()
{
    const char* string[] = {
        "000100000008008000030000636D617001B8B66D0000008C0000131C676C79667DF9A7780000"
        "13A80000BEF46865616418F1DCC20000D29C000000366868656109AB08690000D2D400000024"
        "686D74788ABE40E10000D2F8000008546C6F63610337D3CD0000DB4C0000042E6D6178700227"
        "018C0000DF7C000000207072657068068C850000DF9C0000000700",

        "000100000008008000030000636D617001B8B66D0000008C0000131C676C79665E1E87780000"
        "13A80000BEF46865616418F1DCC20000D29C000000366868656109AB08690000D2D400000024"
        "686D74788ABE40E10000D2F8000008546C6F63610337D3CD0000DB4C0000042E6D6178700227"
        "018C0000DF7C000000207072657068068C850000DF9C0000000700",

        "00010000000A008000030020636D617001B8B66D000000AC0000131C676C79665E1E87780000"
        "13C80000BEF46865616418F1DCC20000D2BC000000366868656109AB08690000D2F400000024"
        "686D74788ABE40E10000D318000008546C6F63610337D3CD0000DB6C0000042E6D6178700227"
        "018C0000DF9C000000206E616D65970DBC3F0000DFBC0000056E706F737427E56ADE0000E52C"
        "0000157F7072657068068C850000FAAC0000000700",

    };
    for (short ii = 0; ii < 3; ii++) {
        printString(string[ii]);
    }
}
void debugSubTables(const STTFOffsetSubTable pSubTable, const char* pTrueTypeFontFile)
{
    const char* ptrLine = "   ------------------------------------------------------------------------------";
    fprintf(stdout, "\n Debug(SubTables)................................................................\n");
    fprintf(stdout, "   True Type Font File                : %s\n", pTrueTypeFontFile);
    fprintf(stdout, "   Scalar Type                        : %u(0x%08X)\n", pSubTable.scalerType, pSubTable.scalerType);
    fprintf(stdout, "   Number of Tables                   : %u(0x%04X)\n", pSubTable.numOfTables, pSubTable.numOfTables);
    fprintf(stdout, "   Search Range                       : %u(0x%04X)\n", pSubTable.searchRange, pSubTable.searchRange);
    fprintf(stdout, "   Entry Selector                     : %u(0x%04X)\n", pSubTable.entrySelector, pSubTable.entrySelector);
    fprintf(stdout, "   Range Shift                        : %u(0x%04X)\n", pSubTable.rangeShift, pSubTable.rangeShift);
    fprintf(stdout, "%s\n", ptrLine);
    fprintf(stdout, "\nHit any key to continue.....\n");	     getchar();
}
void debugListOfTables(const STTFTableDirectory* pListOfTables, const short pTotalTables, const short pTotalGlyps, const char* pPostScriptFontName, const char* pTrueTypeFontFile)
{
    const char* ptrLine = "       --------------------------------------------------------";
    fprintf(stdout, "\n Debug(Directory of Tables)......................................................\n");
    fprintf(stdout, "   True Type Font File                : %s\n", pTrueTypeFontFile);
    fprintf(stdout, "   Postscript Font Name               : %s\n", pPostScriptFontName);
    fprintf(stdout, "   Total Number of Tables             : %d\n", pTotalTables);
    fprintf(stdout, "   Total Number of Glyps              : %d\n", pTotalGlyps);
    fprintf(stdout, "%s\n", ptrLine);
    fprintf(stdout, "       Table         Checksum           Offset           Length\n");
    fprintf(stdout, "%s\n", ptrLine);
    short ii = 0;
    do {
        fprintf(stdout, "  %2d)   %.*s %*c%12u %*c%12u %*c%12u\n",
	        ii + 1,
	        4, pListOfTables[ii].tag,               // The tables have names known as tags. Tags have the type uint32. Currently defined tag names consist of four characters. Tag names with less than four characters have trailing spaces.
	        4, cSP, pListOfTables[ii].checksum,     // Check sum
	        4, cSP, pListOfTables[ii].offset,       // Offset from beginning of file
	        4, cSP, pListOfTables[ii].length        // length of the table in bytes
        );
    } while (++ii < pTotalTables);
    fprintf(stdout, "%s\n", ptrLine);
    fprintf(stdout, "\nHit any key to continue.....\n");	     getchar();
}
void debugListOfNameRecords(const STTFNameRecord* pListOfNameRecords, char **pNameList, const short pTotalNameRecords, const char* pTrueTypeFontFile)
{
    static const char* asPlatform[] = {
        "Unicode", "Macintosh", "Reserved", "Microsoft"
    };
    /**
    static const char* asPlatformSpecific_Macintosh[] = { // total 33
        "Roman", "Japanese", "Chinese(Traditional)",    "Korean",       "Arabic",  "Hebrew",             "Greek", "Russian", "RSymbol", 
        "Devanagari", "Gurmukhi", "Gujarati",           "Oriya",        "Bengali", "Tamil",              "Telugu",   "Kannada", "Malayalam",
        "Sinhalese", "Burmese", "Khmer",                "Thai",         "Laotian", "Georgian",            "Armenian",   "Chinese(Simplified)", "Tibetan",
        "Mongolian", "Geez", "Slavic",                  "Vietnamese",   "Sindhi", "(Uninterpreted)",
    };
    */
    static const char* asPlatformSpecific_Unicode[] = {
        "Version 1.0",          // Version 1.0 Semantics
        "Version 1.1",          // Version 1.1 Semantics
        "ISO-10646",            // ISO-10646 1993 Semantics
        "Unicode",              // Unicode semantics(BMP only)
        "Unicode"               // Unicode semantics(non-BMP allowed)
        "Unicode Variation"     // Unicode Variation Sequences
        "Last Resort"           // Last Resort
    };
    static const char* asNameString[] = { // total 25
        "Copyright", "Family", "Style",                 "Unique",    "Font", "Version",                     "PostScript", "Trade Mark", "Manufacturer",
        "Designer",  "Description", "Vendor URL",        "Designer URL", "License", "License URL",          "Reserved",    "Preferred Family.", "Preferred Subfamily",
        "Compatible ", "Sample text", "PostScript",     "WWS family", "WWS subfamily", "Light",             "Dark",        "Variations",
    };
    const char* ptrLine = "       --------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
    fprintf(stdout, "\n Debug(NameRecords).........................................................................\n");
    fprintf(stdout, "   True Type Font File                : %s\n", pTrueTypeFontFile);
    fprintf(stdout, "   Total Number of Name Records       : %d\n", pTotalNameRecords);
    fprintf(stdout, "%s\n", ptrLine);
    fprintf(stdout, "       Platform         Platform Specific      Language    Name                           Length    Offset     Name\n");
    fprintf(stdout, "       Identifier       Identifier           Identifier    Identifier                    (bytes)   (bytes)     String\n");
    fprintf(stdout, "%s\n", ptrLine);
    short ii = 0;
    do {
        //printf("%2d) platformID=%d specific=%d", ii+1, pListOfNameRecords[ii].platformID, pListOfNameRecords[ii].platformSpecificID); getchar();
        char bufPlatform[20], bufPlatformSpecific[20], bufName[30];
        sprintf_s(bufPlatform, sizeof(bufPlatform), "%1u (%s)", pListOfNameRecords[ii].platformID, asPlatform[pListOfNameRecords[ii].platformID]);
        sprintf_s(bufPlatformSpecific, sizeof(bufPlatformSpecific), "%1u (%s)", pListOfNameRecords[ii].platformSpecificID, asPlatformSpecific_Unicode[pListOfNameRecords[ii].platformSpecificID]);
        const char* sName = (pListOfNameRecords[ii].nameID <= 25) ? asNameString[pListOfNameRecords[ii].nameID] : pListOfNameRecords[ii].nameID >= 256 ? "Variations" :"Reserved";
        sprintf_s(bufName, sizeof(bufName), "%3u (%s)", pListOfNameRecords[ii].nameID, sName);
        fprintf(stdout, "  %2d)  %-14s %*c%-15s %*c%5u %*c%-25s %*c%5u %*c%5u %*c%.*s\n",
            ii + 1,
            bufPlatform,                                       // Platform identifier code.
            2, cSP, bufPlatformSpecific,                       // Platform-specific encoding identifier.
            10, cSP, pListOfNameRecords[ii].languageID,        // Language identifier.
            3, cSP, bufName,                                   // Name identifier.
            6, cSP, pListOfNameRecords[ii].length,             // Name string length in bytes.
            4, cSP, pListOfNameRecords[ii].offset,             // Name string offset in bytes from stringOffset.
            4, cSP, 110, pNameList[ii]                         // Name string offset in bytes from stringOffset.
        );
    } while (++ii < pTotalNameRecords);
    fprintf(stdout, "%s\n", ptrLine);
    fprintf(stdout, "\nHit any key to continue.....\n");	     getchar();
}
void debugListOfCMapEncodingRecords(const STTFCmapTable_Encoding* pListOfCmapEncodingRecords, const short pTotalEncodingRecords, const char* pTrueTypeFontFile)
{
    static const char* asPlatform[] = {
        "Unicode", "Macintosh", "Reserved", "Microsoft"
    };
    // BMP characters have these characteristics: Their code point values are between 0 and 65535 (or U+0000 and U+FFFF ). 
    // They can be encoded in a variable-length encoding using 8, 16, or 24 bits (1 to 3 bytes). 
    // They can be encoded in a fixed-length encoding using 16 bits (2 bytes).
    static const char* asPlatformSpecific[4][33] = {
        {// Unicode
            "Version 1.0",              // Version 1.0 Semantics
            "Version 1.1",              // Version 1.1 Semantics
            "ISO-10646",                // ISO-10646 1993 Semantics
            "Unicode BMP-only",         // Unicode semantics(BMP only). BMP=Basic Multilingual Plane 
            "Unicode non-BMP allowed",  // Unicode semantics(non-BMP allowed)
            "Unicode Variation",        // Unicode Variation Sequences
            "Last Resort",              // Last Resort
        },
        {// Macintosh
            "Roman", "Japanese", "Chinese(Traditional)",    "Korean",       "Arabic",  "Hebrew",             "Greek", "Russian", "RSymbol",
            "Devanagari", "Gurmukhi", "Gujarati",           "Oriya",        "Bengali", "Tamil",              "Telugu",   "Kannada", "Malayalam",
            "Sinhalese", "Burmese", "Khmer",                "Thai",         "Laotian", "Georgian",            "Armenian",   "Chinese(Simplified)", "Tibetan",
            "Mongolian", "Geez", "Slavic",                  "Vietnamese",   "Sindhi", "(Uninterpreted)",
        },
        {// Reserved
            "",
        },
        {// Microsoft
          "Symbol",             // A format 4 subtable would be used, typically with up to 224 graphic characters assigned at code positions beginning with 0xF020.
          "Unicode BMP-only",   // Fonts that support only Unicode BMP characters (U+0000 to U+FFFF) on the Windows platform must use encoding 1 with a format 4 subtable.
          "Shift-JIS",
          "Shift-PRC",
          "BigFive",
          "Johab",
          "Reserved",
          "Reserved",
          "Reserved",
          "Reserved",
          "Unicode UCS-4"       // Fonts that support Unicode supplementary-plane characters (U+10000 to U+10FFFF) with Format 12 subtable.",    // Unicode full repertoire
        },
    };
    static const char* asRemarks[4][11] = { // 
        {// Unicode platform (platform ID = 0)
        "deprecated",
        "deprecated",
        "deprecated",
        "Encoding ID 3 should be used in conjunction with 'cmap' subtable formats 4 or 6.",
        "Encoding ID 4 should be used in conjunction with 'cmap' subtable formats 10 or 12.",
        "Encoding ID 5 should use 'cmap' subtable format 14.",
        "Encoding ID 6 should use 'cmap' subtable format 13.",
      },
      {// Macintosh platform (platform ID = 1)
          "",
      },
      {// Deprecated
          "",
      },
      {// Microsoft platform (platform ID = 3)

          "Symbol (beginning with U+F020 typically with up to 224 graphic characters)",     // A format 4 subtable would be used, typically with up to 224 graphic characters assigned at code positions beginning with 0xF020.
          "Unicode BMP-only (UCS-2) characters (U+0000 to U+FFFF) with Format 4 subtable.", // Fonts that support only Unicode BMP characters (U+0000 to U+FFFF) on the Windows platform must use encoding 1 with a format 4 subtable.
          "Shift-JIS",
          "Shift-PRC",
          "BigFive",
          "Johab",
          "Reserved",
          "Reserved",
          "Reserved",
          "Reserved",
          "Unicode UCS-4. Fonts that support Unicode supplementary-plane characters (U+10000 to U+10FFFF) with Format 12 subtable.",    // Unicode full repertoire
     }
    };
    const char* ptrLine = "       -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
    fprintf(stdout, "\n Debug(CMap Encoding Subtables).............................................................................................................................................................\n");
    fprintf(stdout, "   True Type Font File                      : %s\n", pTrueTypeFontFile);
    fprintf(stdout, "   Total Number of Cmap Encoding Subtables  : %d\n", pTotalEncodingRecords);
    fprintf(stdout, "%s\n", ptrLine);
    fprintf(stdout, "       Platform         Platform Specific                  Offset\n");
    fprintf(stdout, "       Identifier       Encoding Identifier               (bytes)    Remarks\n");
    fprintf(stdout, "%s\n", ptrLine);
    short ii = 0;
    do {
        const uint16_t pid = pListOfCmapEncodingRecords[ii].platformID;             // Platform ID
        const uint16_t psid = pListOfCmapEncodingRecords[ii].platformSpecificID;    // platform Specific ID
        //printf("%2d) platformID=%d specific=%d", ii+1, pListOfNameRecords[ii].platformID, pListOfNameRecords[ii].platformSpecificID); getchar();
        char bufPlatform[20], bufPlatformSpecific[50];
        sprintf_s(bufPlatform, sizeof(bufPlatform), "%1u (%s)", pid, asPlatform[pid]);
        sprintf_s(bufPlatformSpecific, sizeof(bufPlatformSpecific), "%2u (%s)", psid, asPlatformSpecific[pid][psid]);
        fprintf(stdout, "  %2d)  %-14s %*c%-28s %*c%5u %*c%s\n",
            ii + 1,
            bufPlatform,                                       // Platform identifier code.
            2, cSP, bufPlatformSpecific,                       // Platform-specific encoding identifier.
            7, cSP, pListOfCmapEncodingRecords[ii].offset,     // Byte offset from beginning of table to the subtable for this encoding.
            3, cSP, asRemarks[pid][psid]                       // Byte offset from beginning of table to the subtable for this encoding.
        );
    } while (++ii < pTotalEncodingRecords);
    fprintf(stdout, "%s\n", ptrLine);
    fprintf(stdout, "\nHit any key to continue.....\n");	     getchar();
}
void debugCMapSegmeentArrayFormat_4(const STTFCmapTable_Format_4 pCmapFormat, const STTFCmapTable_Format_4_Segment_Array pSegArray, const uint16_t pSegmentCount, const uint16_t pPlatformID, const uint16_t pPlatformSpecificID, const char* pTrueTypeFontFile)
{
    static const char* asPlatform[] = {
        "Unicode", "Macintosh", "Reserved", "Microsoft"
    };
    // BMP characters have these characteristics: Their code point values are between 0 and 65535 (or U+0000 and U+FFFF ). 
    // They can be encoded in a variable-length encoding using 8, 16, or 24 bits (1 to 3 bytes). 
    // They can be encoded in a fixed-length encoding using 16 bits (2 bytes).
    static const char* asPlatformSpecific[4][33] = {
        {// Unicode
            "Version 1.0 semantics",                                        // Version 1.0 Semantics
            "Version 1.1 semantics",                                        // Version 1.1 Semantics
            "ISO 10646 1993 semantics (deprecated)",                        // ISO-10646 1993 Semantics
            "Unicode 2.0 or later semantics (BMP only)",                    // Unicode semantics(BMP only). BMP=Basic Multilingual Plane 
            "Unicode 2.0 or later semantics (non-BMP characters allowed)",  // Unicode semantics(non-BMP allowed)
            "Unicode Variation Sequences",                                  // Unicode Variation Sequences
            "Last Resort",                                                  // Last Resort
        },
        {// Macintosh
            "Roman", "Japanese", "Chinese(Traditional)",    "Korean",       "Arabic",  "Hebrew",             "Greek", "Russian", "RSymbol",
            "Devanagari", "Gurmukhi", "Gujarati",           "Oriya",        "Bengali", "Tamil",              "Telugu",   "Kannada", "Malayalam",
            "Sinhalese", "Burmese", "Khmer",                "Thai",         "Laotian", "Georgian",            "Armenian",   "Chinese(Simplified)", "Tibetan",
            "Mongolian", "Geez", "Slavic",                  "Vietnamese",   "Sindhi", "(Uninterpreted)",
        },
        {// Reserved
            "",
        },
        {// Microsoft
          "Symbol",                     // A format 4 subtable would be used, typically with up to 224 graphic characters assigned at code positions beginning with 0xF020.
          "Unicode BMP-only (UCS-2)",   // Fonts that support only Unicode BMP characters (U+0000 to U+FFFF) on the Windows platform must use encoding 1 with a format 4 subtable.
          "Shift-JIS",
          "Shift-PRC",
          "BigFive",
          "Johab",
          "Reserved",
          "Reserved",
          "Reserved",
          "Reserved",
          "Unicode UCS-4"               // Fonts that support Unicode supplementary-plane characters (U+10000 to U+10FFFF) with Format 12 subtable.",    // Unicode full repertoire
        },
    };
    const char* ptrLine_1 = "        -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
    const char* ptrLine_2 = "        -----------------------------------------------------------------------------------                           -----------------------------------------------------------------------------------";

    fprintf(stdout, "\n Debug(CMap Format 4)....................................................................................................................................................................................\n");
    fprintf(stdout, "   True Type Font File                             : %s\n", pTrueTypeFontFile);
    fprintf(stdout, "   Platform Identifier                             : %u (%s)\n", pPlatformID, asPlatform[pPlatformID]);
    fprintf(stdout, "   Platform Specific Identifier                    : %u (%s)\n", pPlatformSpecificID, asPlatformSpecific[pPlatformID][pPlatformSpecificID]);
    fprintf(stdout, "   Format                                          : %u\n", pCmapFormat.format);           // Subtable format; set to 4.
    fprintf(stdout, "   Length of the subtable                          : %u bytes\n", pCmapFormat.length);     // This is the length in bytes of the subtable.
    fprintf(stdout, "   segCountX2 (2 * segCount)                       : %u\n", pCmapFormat.segCountX2);       // 2 × segCount.
    fprintf(stdout, "   searchRange (2 * ((2**floor(log2(segCount))))   : %u\n", pCmapFormat.searchRange);      // Maximum power of 2 less than or equal to segCount, times 2. i.e. ((2**floor(log2(segCount))) * 2, where “**” is an exponentiation operator)
    fprintf(stdout, "   entrySelector (floor(log2(segCount)))           : %u\n", pCmapFormat.entrySelector);    // Log2 of the maximum power of 2 less than or equal to numTables (log2(searchRange/2), which is equal to floor(log2(segCount)))
    fprintf(stdout, "   rangeShift  ((segCount * 2) - searchRange)      : %u\n", pCmapFormat.entrySelector);    // segCount times 2, minus searchRange ((segCount * 2) - searchRange)

    fprintf(stdout, "%s\n", ptrLine_1);
    fprintf(stdout, "        ....Start Code....           .....End Code.....         Identifier       Identifier                           ....Start Code....           .....End Code.....         Identifier       Identifier\n");
    fprintf(stdout, "        Decimal    Unicode           Decimal    Unicode              Delta      RangeOffset                           Decimal    Unicode           Decimal    Unicode              Delta      RangeOffset\n");
    fprintf(stdout, "%s\n", ptrLine_2);

    const uint16_t segmentLHS = pSegmentCount / 2 + (pSegmentCount % 2);    // Segments at left hand side.
    const uint16_t segmentRHS = pSegmentCount / 2;                          // Segments at right hand side.
    for (uint16_t ii=0, jj=0; ii < segmentLHS; ii++, jj++) {
        fprintf(stdout, "  %3d)  %-5u %*cU+%04X %*c%-5u %*cU+%04X %*c%5u %*c%5u",
            ii + 1,
            pSegArray.startCode[ii],                    // Starting character code for each segment.
            6, cSP, pSegArray.startCode[ii],            // Unicode Point.
            10, cSP, pSegArray.endCode[ii],             // Ending character code for each segment, last = 0xFFFF.
            6, cSP, pSegArray.endCode[ii],              // Unicode Point.
            13, cSP, pSegArray.idDelta[ii],             // Delta for all character codes in segment.
            11, cSP, pSegArray.idRangeOffset[ii]        // Offset in bytes to glyph indexArray, or 0.
         );
        if (jj < segmentRHS) {
            uint16_t kk = segmentLHS + jj;
            fprintf(stdout, "%*c %3d)  %-5u %*cU+%04X %*c%-5u %*cU+%04X %*c%5u %*c%5u\n",
                20, cSP,
                kk + 1,
                pSegArray.startCode[kk],                    // Starting character code for each segment.
                6, cSP, pSegArray.startCode[kk],            // Unicode Point.
                10, cSP, pSegArray.endCode[kk],             // Ending character code for each segment, last = 0xFFFF.
                6, cSP, pSegArray.endCode[kk],              // Unicode Point.
                13, cSP, pSegArray.idDelta[kk],             // Delta for all character codes in segment.
                11, cSP, pSegArray.idRangeOffset[kk]        // Offset in bytes to glyph indexArray, or 0.
            );
        }
        else fprintf(stdout, "\n");
    }
    fprintf(stdout, "%s\n", ptrLine_1);
    fprintf(stdout, "\nHit any key to continue.....\n");	     getchar();
}
void debugCMapSegmeentArrayFormat_12(const STTFCmapTable_Format_12 pCmapFormat, const STTFCmapTable_SequentialMapGroup_Record *pGroupRecord, const uint16_t pPlatformID, const uint16_t pPlatformSpecificID, const char* pTrueTypeFontFile)
{
    static const char* asPlatform[] = {
        "Unicode", "Macintosh", "Reserved", "Microsoft"
    };
    // BMP characters have these characteristics: Their code point values are between 0 and 65535 (or U+0000 and U+FFFF ). 
    // They can be encoded in a variable-length encoding using 8, 16, or 24 bits (1 to 3 bytes). 
    // They can be encoded in a fixed-length encoding using 16 bits (2 bytes).
    static const char* asPlatformSpecific[4][33] = {
        {// Unicode
            "Version 1.0 semantics",                                        // Version 1.0 Semantics
            "Version 1.1 semantics",                                        // Version 1.1 Semantics
            "ISO 10646 1993 semantics (deprecated)",                        // ISO-10646 1993 Semantics
            "Unicode 2.0 or later semantics (BMP only)",                    // Unicode semantics(BMP only). BMP=Basic Multilingual Plane 
            "Unicode 2.0 or later semantics (non-BMP characters allowed)",  // Unicode semantics(non-BMP allowed)
            "Unicode Variation Sequences",                                  // Unicode Variation Sequences
            "Last Resort",                                                  // Last Resort
        },
        {// Macintosh
            "Roman", "Japanese", "Chinese(Traditional)",    "Korean",       "Arabic",  "Hebrew",             "Greek", "Russian", "RSymbol",
            "Devanagari", "Gurmukhi", "Gujarati",           "Oriya",        "Bengali", "Tamil",              "Telugu",   "Kannada", "Malayalam",
            "Sinhalese", "Burmese", "Khmer",                "Thai",         "Laotian", "Georgian",            "Armenian",   "Chinese(Simplified)", "Tibetan",
            "Mongolian", "Geez", "Slavic",                  "Vietnamese",   "Sindhi", "(Uninterpreted)",
        },
        {// Reserved
            "",
        },
        {// Microsoft
          "Symbol",                     // A format 4 subtable would be used, typically with up to 224 graphic characters assigned at code positions beginning with 0xF020.
          "Unicode BMP-only (UCS-2)",   // Fonts that support only Unicode BMP characters (U+0000 to U+FFFF) on the Windows platform must use encoding 1 with a format 4 subtable.
          "Shift-JIS",
          "Shift-PRC",
          "BigFive",
          "Johab",
          "Reserved",
          "Reserved",
          "Reserved",
          "Reserved",
          "Unicode UCS-4"               // Fonts that support Unicode supplementary-plane characters (U+10000 to U+10FFFF) with Format 12 subtable.",    // Unicode full repertoire
        },
    };
    const char* ptrLine_1 = "        -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------";
    const char* ptrLine_2 = "        ---------------------------------------------------------------                 ---------------------------------------------------------------                 ---------------------------------------------------------------";
    fprintf(stdout, "\n Debug(CMap Format 12).................................................................................................................................................................................................................\n");
    fprintf(stdout, "   True Type Font File                             : %s\n", pTrueTypeFontFile);
    fprintf(stdout, "   Platform Identifier                             : %u (%s)\n", pPlatformID, asPlatform[pPlatformID]);
    fprintf(stdout, "   Platform Specific Identifier                    : %u (%s)\n", pPlatformSpecificID, asPlatformSpecific[pPlatformID][pPlatformSpecificID]);
    fprintf(stdout, "   Format                                          : %u\n", pCmapFormat.format);           // Subtable format; set to 4.
    fprintf(stdout, "   Length of the subtable                          : %u bytes\n", pCmapFormat.length);     // This is the length in bytes of the subtable.
    fprintf(stdout, "   Number of Groups                                : %u\n", pCmapFormat.numGroups);        // segCount times 2, minus searchRange ((segCount * 2) - searchRange)
    fprintf(stdout, "%s\n", ptrLine_1);
    fprintf(stdout, "        .....Start Code.....      ......End Code......       Identifier                 .....Start Code.....      ......End Code......       Identifier                 .....Start Code.....      ......End Code......       Identifier\n");
    fprintf(stdout, "        Decimal      Unicode      Decimal      Unicode      Start Glyph                 Decimal      Unicode      Decimal      Unicode      Start Glyph                 Decimal      Unicode      Decimal      Unicode      Start Glyph\n");
    fprintf(stdout, "%s\n", ptrLine_2);
    const short remainder = pCmapFormat.numGroups % 3;                          // modulus of 3.
    const short dividend = pCmapFormat.numGroups / 3;                           // modulus of 3.
    const uint16_t groupLHS = dividend + (remainder == 1 || remainder == 2);    // Group at left hand side.
    const uint16_t groupMid = dividend + (remainder == 2);                      // Group at middle.
    const uint16_t groupRHS = dividend;                                         // Group at right hand side.
    for (uint32_t ii=0, jj=0, kk=0; ii < groupLHS; ii++, jj++, kk++) {
        fprintf(stdout, "  %3d)  %-7u %*cU+%06X %*c%-7u %*cU+%06X %*c%7u",
            ii + 1,
            pGroupRecord[ii].startCharCode,                     // Starting character code for each segment.
            4, cSP, pGroupRecord[ii].startCharCode,             // Unicode Point.
            5, cSP, pGroupRecord[ii].endCharCode,               // Ending character code for each segment, last = 0xFFFF.
            4, cSP, pGroupRecord[ii].endCharCode,               // Unicode Point.
            9, cSP, pGroupRecord[ii].startGlyphID               // Glyph index corresponding to the starting character code. subsequent charcters are mapped to sequential glyphs
         );
        if (jj < groupMid) {
            uint16_t mm = groupLHS + jj;
            fprintf(stdout, "%*c %3d)  %-7u %*cU+%06X %*c%-7u %*cU+%06X %*c%7u",
                10, cSP,
                mm + 1,
                pGroupRecord[mm].startCharCode,                     // Starting character code for each segment.
                4, cSP, pGroupRecord[mm].startCharCode,             // Unicode Point.
                5, cSP, pGroupRecord[mm].endCharCode,               // Ending character code for each segment, last = 0xFFFF.
                4, cSP, pGroupRecord[mm].endCharCode,               // Unicode Point.
                9, cSP, pGroupRecord[mm].startGlyphID               // Glyph index corresponding to the starting character code. subsequent charcters are mapped to sequential glyphs
            );
        }
        if (kk < groupRHS) {
            uint16_t nn = groupLHS + groupMid + kk;
            fprintf(stdout, "%*c %3d)  %-7u %*cU+%06X %*c%-7u %*cU+%06X %*c%7u\n",
                10, cSP,
                nn + 1,
                pGroupRecord[nn].startCharCode,                     // Starting character code for each segment.
                4, cSP, pGroupRecord[nn].startCharCode,             // Unicode Point.
                5, cSP, pGroupRecord[nn].endCharCode,               // Ending character code for each segment, last = 0xFFFF.
                4, cSP, pGroupRecord[nn].endCharCode,               // Unicode Point.
                9, cSP, pGroupRecord[nn].startGlyphID               // Glyph index corresponding to the starting character code. subsequent charcters are mapped to sequential glyphs
            );
        }
        else fprintf(stdout, "\n");
    }
    fprintf(stdout, "%s\n", ptrLine_1);
    fprintf(stdout, "\nHit any key to continue.....\n");	     getchar();
}
short getTable(const STTFTableDirectory * pListOfTables, const uint16_t pTotalDir, const char *pSearchTag)
{
    for (short ii = 0; ii < pTotalDir; ii++) {
        char tag[5];
        sprintf_s(tag, 5, "%.*s", 4, pListOfTables[ii].tag);
        if (!_stricmp(tag, pSearchTag)) return ii;	    // success
    }
    return -1;		// not found.
}
const char *getWeight(const uint16_t pWeightClass)
{
    static const char* asWeightClass[] = {
        "Error", 
        "Thin", "Extra-light", "Light",       "Regular", "Medium", "Semi-bold",       "Bold", "Extra-bold", "Black"     // 9 elements.
    };
    const char* ptrWeightClass;
    switch (pWeightClass) {
        case 100: ptrWeightClass = asWeightClass[1];        break;      // Thin
        case 200: ptrWeightClass = asWeightClass[2];        break;      // Extra-light (Ultra-light)
        case 300: ptrWeightClass = asWeightClass[3];        break;      // Light
        case 400: ptrWeightClass = asWeightClass[4];        break;      // Normal(Regular)
        case 500: ptrWeightClass = asWeightClass[5];        break;      // Medium
        case 600: ptrWeightClass = asWeightClass[6];        break;      // Semi-bold (Demi-bold)
        case 700: ptrWeightClass = asWeightClass[7];        break;      // Bold
        case 800: ptrWeightClass = asWeightClass[8];        break;      // Extra-bold (Ultra-bold)
        case 900: ptrWeightClass = asWeightClass[9];        break;      // Black (Heavy)
        default: ptrWeightClass = asWeightClass[0];         break;      // Error
    }
    return ptrWeightClass;
}
void writeHexData(FILE* fcid, const char* pData, const uint32_t pLengthOfData)
{
    //
    // Ths function embeds the table data within angle brackets and write into file.
    //  i)   Table data is embedded as Hexadecimal characters. i.e., each byte will be represented by two Hex digits whose range is from 0 to F.
    //  ii)  Make sure that table data aligns at long boundary (4 byes). Append enough pads ("00") in order to align on long boundary.
    //  iii) Before closing angle bracket, append "00" in order to make length of table data odd.
    //
    // 1) fcid is file pointer of type FILE. The table data is dumped into this file as Hex strings covered with angle brackets.
    // 2) pData is an input parameter that points to data. This is in decimal form. This will be converted to Hex characters and then will be written ino file.
    // 3) pLengthOfData an input parameter indicating the data length in bytes.
    // 
    // This function returns nothing.
    //

    const short lcBytesPerLine = 38;
    fprintf(fcid, "<");                                             // Begin printing of Hex data with angle bracket.
    uint32_t ii = 0;                                                // flush
    do {
        fprintf(fcid, "%02X", pData[ii++] & 0xff);                  // Begin printing of Hex data with opening angle bracket.
        if (!(ii % lcBytesPerLine)) fprintf(fcid, "\n");            // Continue printing in next line after multiples of lcBytesPerLine.
    } while (ii < pLengthOfData);

    short residualBytes = pLengthOfData % 4;                        // Table Data should be aligned at long boundary (4 bytes).
    if (residualBytes) residualBytes = 4 - residualBytes;           // Additional bytes required to make Table Data to align at long boundary.
    //printf("pLengthOfData=%u residualBytes=%d", pLengthOfData, residualBytes); getchar();
    while (residualBytes-- > 0) fprintf(fcid, "00");                // append pad to make up long boundary.
    fprintf(fcid, "00>\n");                                         // Append '00' before closing angle bracket. This is required in order to make the table length odd within angle brackets.
}
void embedTablesAsHexStrings_sfnts(FILE *fttf, FILE* fcid, const STTFOffsetSubTable pSubTable, const STTFTableDirectory* pListOfTables, const uint16_t pNumOfTables)
{
    static const char* asTablesRequired [] = {
        "cmap",         // character to glyph mapping
        "glyf",         // glyph data
        "head",         // font header
        "hhea",         // horizontal header
        "hmtx",         // horizontal metrics
        "loca",         // index to location
        "maxp",         // maximum profile
        "name",         // naming
        "post",         // PostScript
        "prep",         // control value program
    };
    uint16_t totalRequiredTables = sizeof(asTablesRequired) / sizeof(char*);
    bool *aFound = new bool[totalRequiredTables];
    if (!aFound) {
        fprintf(stdout, "convertBinaryToHex(): Boolean Array  Memory Allocation error. Data size: %u\n", totalRequiredTables); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }

    uint16_t totalAvailableTables=0, kk = 0;
    while (kk < totalRequiredTables) {
        if (getTable(pListOfTables, pNumOfTables, asTablesRequired[kk]) >= 0) {
            totalAvailableTables++;       aFound[kk] = true;
        }
        else aFound[kk] = false;
        kk++;
    }

    // Find out Maximum power of 2 which should be less than or equal to totalAvailableTables
    unsigned int val = 1;       // Initialize with 1 which is value corresponding to 2 to the power 0. This will hold values generated from powers of 2.
    short exp = 1;              // Initialize exponenet with 1. This will hold powers of 2. The exp will be 1, 2, 3, 4 or 5 which is depending upon the number of tables associated with the ttf font.
    while ((val <<= 1) <= totalAvailableTables) exp++;
    exp--;                      // decrement power
    val >>= 1;                  // divide by 2; val will be always <= totalAvailableTables.
    //printf("tablesAvailable=%d exp=%d val=%d", totalAvailableTables, exp, val); getchar();
    STTFOffsetSubTable subtable;
    subtable.scalerType = SWAPLONG(pSubTable.scalerType);                   // assign scalar type.
    subtable.numOfTables = SWAPWORD(totalAvailableTables);                  // Tables available to embed into sftns array.
    subtable.searchRange = SWAPWORD(val * 16);                              // (maximum power of 2 <= numTables)*16, Where 16 is size of table directory.
    subtable.entrySelector = SWAPWORD(exp);                                 // log2(maximum power of 2 <= numTables).
    subtable.rangeShift = SWAPWORD(totalAvailableTables * 16 - val * 16);   // numTables*16 - searchRange.

    uint32_t tableDirLen = sizeof(STTFOffsetSubTable) + sizeof(STTFTableDirectory) * totalAvailableTables;
    char* tabledata = new char[tableDirLen];
    if (!tabledata) {
        fprintf(stdout, "convertBinaryToHex(): Memory Allocation error. Data size: %u\n", tableDirLen); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    memcpy_s(static_cast<void*>(tabledata), tableDirLen, static_cast<const void*>(&subtable), sizeof(STTFOffsetSubTable));       // copy from object STTFOffsetSubTable.
    uint32_t offsetFromTop = tableDirLen;                           // First table 'cmap' begins at this offset
    short datalen = sizeof(STTFOffsetSubTable);
    for (uint16_t ii = 0; ii < totalRequiredTables; ii++) {
        if (!aFound[ii]) continue;
        const short idx = getTable(pListOfTables, pNumOfTables, asTablesRequired[ii]);
        const short residue = offsetFromTop % 4;                                                                // Verify that offsetFromTop is divisible by 4 so that it falls at long boundary.
        if (residue) offsetFromTop += 4 - residue;                                                              // Make sure that offset falls at long boundary. 
        STTFTableDirectory dirEntry;
        memcpy_s (static_cast<void*>(dirEntry.tag), 4, static_cast<const void*>(&pListOfTables[idx].tag), 4);   // copy tag
        dirEntry.checksum = SWAPLONG(pListOfTables[idx].checksum);                                              // Check sum
        dirEntry.offset = SWAPLONG(offsetFromTop);                                                              // Offset
        dirEntry.length = SWAPLONG(pListOfTables[idx].length);                                                  // length of the table in bytes
        memcpy_s(static_cast<void*>(tabledata+datalen), tableDirLen-datalen, static_cast<const void*>(&dirEntry), sizeof(STTFTableDirectory));       // copy from object STTFOffsetSubTable.
        offsetFromTop += pListOfTables[idx].length;                         // Next table offset.
        datalen += sizeof(STTFTableDirectory);                              // length of tabledata.
    }
    writeHexData(fcid, tabledata, tableDirLen);     // Embed table data into sfnts array.
    delete []tabledata;                             // release table data allocated memory.
    for (uint16_t ii = 0; ii < totalRequiredTables; ii++) {
        if (!aFound[ii]) continue;
        const short idx = getTable(pListOfTables, pNumOfTables, asTablesRequired[ii]);
        //fprintf(stdout, "%2d) Table:%.*s, Offset:%u, length:%u\n", ii, 4, pListOfTables[idx].tag, pListOfTables[idx].offset, pListOfTables[idx].length);
        char *data = new char [pListOfTables[idx].length];
        if (!data) {
            fprintf(stdout, "convertBinaryToHex(): Memory Allocation error for Table %.*s. Table size: %u\n", 4, pListOfTables[idx].tag, pListOfTables[idx].length); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
        }
        uint32_t seekOffset = pListOfTables[idx].offset;                     // find out seek offset.
        if (fseek(fttf, seekOffset, SEEK_SET)) {
            fprintf(stdout, " main(): File seek error. Table: %.*s, Offset: %u\n", 4, pListOfTables[idx].tag, pListOfTables[idx].offset); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
        }
        fread(data, pListOfTables[idx].length, 1, fttf);
        if (errno) {
            fprintf(stdout, " convertBinaryToHex(): File error while reading Table %.*s with size: %u. Error(%d)\n", 4, pListOfTables[idx].tag, pListOfTables[idx].length, errno); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
        }
        writeHexData(fcid, data, pListOfTables[idx].length);
        delete []data;                                          // release data allocated memory.
    }
    delete []aFound;                                 // release allocated memory.
}
bool processCommandLine(const int argc, char* argv[], short &pArgId, bool* isdisplay)
{
    //
    // This function assists in processing command line arguments.
    //
    // 1. argc is the total number of command line arguments
    // 2. argv is the list of arguemnts from command line.
    // 3. filename is ttf filename. This should have ttf extension.
    //    i)   -d is to display Subtable, Directory of Tables, and Name records from 'name' table.
    // 4. isdisplay is the output parameter that decides to display reports or not.
    //

    if (argc == 1) return false;                    // display usage
    if (argc < 3) {
        if (argv[1][0] == '-') {
            return false;                                   // display usage
        }
        else {
            pArgId = 1;                return true;        // argv[1] is filename
        }
    }
    if (argc < 4) {
        if (argv[1][0] == '-') {
            if (argv[1][1] == 'd') *isdisplay = true;
            else return false;                              // display usage
            if (argv[2]) {
                pArgId = 2;             return true;        // argv[2] is filename
            }
        }
        else return false;                                  // display usage
    }
    return pArgId > 0;                 // if pArgId is zero, then filename is missing and display usage message.
}
int main(int argc, char* argv[])
{
    //
    // main -d filename.ttf
    //		-d refers to display. With this flag, this programs prints Subtable, Directory of Tables, and Name records from 'name' table. 
    //
    bool isdisplay = false;                     // Initialize with false
    short argIdx = 0;                           // This will indicate the argument id (1 or 2) having filename.
    if (argc < 2 || argc > 4|| !processCommandLine(argc, argv, argIdx, &isdisplay)) {
        fprintf(stdout, "usage: main -d filename.ttf");
        fprintf(stdout, "       -d display reports.\n");
        printf("\nhit any key....");	getchar();
        return(1);				// exit with error 1
    }
    const char* ptr;
    char* strTrueTypeFontFile = argv[argIdx];       // ttf filename
    if (!(ptr = strstr(strTrueTypeFontFile, ".ttf"))) {
        fprintf(stdout, "Input file '%s' does not have file extension 'ttf'.", strTrueTypeFontFile);
        printf("\nhit any key....");	getchar();
        return(1);				// exit with error 1
    }
    const short lcFileNameSize = 256;
    char t42Filenamet[lcFileNameSize], psFilename[lcFileNameSize];
    strcpy_s(t42Filenamet, lcFileNameSize, strTrueTypeFontFile);            // copy ttf file name
    strcpy_s(psFilename, lcFileNameSize, strTrueTypeFontFile);              // copy ttf file name
    char *ptr1 = strstr(t42Filenamet, ".ttf");          memcpy(static_cast<void*>(ptr1 + 1), "t42", 3);      // replace ttf with t42.
    char* ptr2 = strstr(psFilename, ".ttf");            memcpy(static_cast<void*>(ptr2 + 1), "ps", 3);       // replace ttf with ps.
    //printf("%s...\t%s...\n", t42Filenamet, psFilename); getchar();

    const char* sFileMode = "rb";
    FILE *fttf, *fcid, *fps;

#if _MSC_VER			// Visual Studio
    if (fopen_s(&fttf, strTrueTypeFontFile, sFileMode)) {
        printf("File name: %s\n", strTrueTypeFontFile);
        perror("The following error occurred");return (1);
    }
    else printf("\nFile %s is opened for reading\n", strTrueTypeFontFile);
    // open t42 file to write
    if (fopen_s(&fcid, t42Filenamet, "w")) {
        printf("File name: %s\n", t42Filenamet);
        perror("The following error occurred");
        return (1);
    }
    else printf("File %s is opened for writing\n", t42Filenamet);
    // open ps file to write
    if (fopen_s(&fps, psFilename, "w")) {
        printf("File name: %s\n", psFilename);
        perror("The following error occurred");
        return (1);
    }
    else printf("File %s is opened for writing\n", psFilename);

#elif __GNUC__	|| __CYGWIN__		// gcc
    if (!(fttf = fopen(strTrueTypeFontFile, sFileMode))) {
        printf("File name: %s\n", strTrueTypeFontFile);
        perror("The following error occurred");
	  return (1);
    }
    else printf("\nFile %s is opened for reading\n", strTrueTypeFontFile);
    // open t42 file to write
    if (!(fcid = fopen(t42Filenamet, "w"))) {
        printf("File name: %s\n", t42Filenamet);
        perror("The following error occurred");
        return (1);
    }
    else printf("File %s is opened for writing\n", t42Filenamet);
    // open ps file to write
    if (!(fps = fopen(psFilename, "w"))) {
        printf("File name: %s\n", psFilename);
        perror("The following error occurred");
        return (1);
    }
    else printf("File %s is opened for writing\n", psFilename);

#endif

    // Offset Sub table 
    STTFOffsetSubTable ttfSubTable;
    fread(&ttfSubTable, sizeof(STTFOffsetSubTable), 1, fttf);
    if (errno) {
        fprintf(stdout, " main(): File error while reading object STTFOffsetSubTable. Error(%d)\n", errno); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }

    ttfSubTable.scalerType = SWAPLONG(ttfSubTable.scalerType);	  // A tag to indicate the OFA scaler to be used to rasterize this font. Fonts with TrueType outlines produced for OS X or iOS only are encouraged to use 'true' (0x74727565) for the scaler type value. Fonts for Windows or Adobe products must use 0x00010000.
    ttfSubTable.numOfTables = SWAPWORD(ttfSubTable.numOfTables);	  // convert from big endian to little endian
    ttfSubTable.searchRange = SWAPWORD(ttfSubTable.searchRange); 	  // (maximum power of 2 <= numTables)*16
    ttfSubTable.entrySelector = SWAPWORD(ttfSubTable.entrySelector);	  // log2(maximum power of 2 <= numTables)
    ttfSubTable.rangeShift = SWAPWORD(ttfSubTable.rangeShift);     	  // numTables*16-searchRange
    const uint16_t numOfTables = ttfSubTable.numOfTables;		  // store number of tables.

    // List of Tables
    STTFTableDirectory* listOfTables = new STTFTableDirectory[numOfTables];
    if (!listOfTables) {
        fprintf(stdout, " main(): Memory Allocation error of Names. Total Tables: %d.\n", numOfTables); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    for (short ii = 0; ii < numOfTables; ii++) {
        fread(listOfTables + ii, sizeof(STTFTableDirectory), 1, fttf);
        if (errno) {
            fprintf(stdout, " main(): File error while reading object STTFTableDirectory(%d/%d). Error(%d)\n", ii, numOfTables, errno); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
        }
        listOfTables[ii].checksum = SWAPLONG(listOfTables[ii].checksum);    // Check sum
        listOfTables[ii].offset = SWAPLONG(listOfTables[ii].offset);        // Offset from beginning of file
        listOfTables[ii].length = SWAPLONG(listOfTables[ii].length);        // length of the table in bytes
    }


    // Name Table
    const short idxNameTable = getTable(listOfTables, numOfTables, "name");        // collect index corresponding to 'name' table.   
    if (idxNameTable == -1) {
        fprintf(stdout, " main(): The 'name' Table is not present: Total Tables: %d.\n", numOfTables); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    if (fseek(fttf, listOfTables[idxNameTable].offset, SEEK_SET)) {
        fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxNameTable].offset); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    STTFNameTable nameTable;
    fread(&nameTable, sizeof(STTFNameTable), 1, fttf);
    if (errno) {
        fprintf(stdout, " main(): File error while reading object STTFNameTable. Error(%d)\n", errno); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    nameTable.format = SWAPWORD(nameTable.format);                  //format selector. Always 0
    nameTable.nrCount = SWAPWORD(nameTable.nrCount);                // The number of nameRecords in this name table.
    nameTable.stringOffset = SWAPWORD(nameTable.stringOffset);      // Offset in bytes to the beginning of the name character strings.
    const uint16_t cntNameRecord = nameTable.nrCount;               // The number of nameRecords in this name table.

    // Name Records
    STTFNameRecord* listOfNameRecords = new STTFNameRecord[cntNameRecord];
    if (!listOfNameRecords) {
        fprintf(stdout, " main(): Memory Allocation error of Names. Total Name Records: %d.\n", cntNameRecord); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    for (short ii = 0; ii < cntNameRecord; ii++) {
        fread(listOfNameRecords + ii, sizeof(STTFNameRecord), 1, fttf);
        if (errno) {
            fprintf(stdout, " main(): File error while reading object STTFNameRecord (%d/%d). Error(%d)\n", ii, cntNameRecord, errno); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
        }
        listOfNameRecords[ii].platformID = SWAPWORD(listOfNameRecords[ii].platformID);                   // Platform identifier code.
        listOfNameRecords[ii].platformSpecificID = SWAPWORD(listOfNameRecords[ii].platformSpecificID);   // Platform-specific encoding identifier.
        listOfNameRecords[ii].languageID = SWAPWORD(listOfNameRecords[ii].languageID);                   // Language identifier.
        listOfNameRecords[ii].nameID = SWAPWORD(listOfNameRecords[ii].nameID);                           // Name identifier.
        listOfNameRecords[ii].length = SWAPWORD(listOfNameRecords[ii].length);                           // Name string length in bytes.
        listOfNameRecords[ii].offset = SWAPWORD(listOfNameRecords[ii].offset);                           // Name string offset in bytes from stringOffset.
    }
    char** nameList = new char*[cntNameRecord];
    if (!nameList) {
        fprintf(stdout, "main(): Memory Allocation error for namelist. Count of Name Records: %d\n", cntNameRecord); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    for (short ii = 0; ii < cntNameRecord; ii++) {
        nameList[ii] = NULL;		// initialize each pointer with NULL
    }

    //printf("offset=%lu stringOffset=%u\n", listOfTables[idxNameTable].offset, nameTable.stringOffset); getchar();
    for (short ii = 0; ii < cntNameRecord; ii++) {
        if (!(nameList[ii] = new char[listOfNameRecords[ii].length + 1])) {
            fprintf(stdout, "main(): Memory Allocation error for nameList(%d/%d)\n", ii, cntNameRecord); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
        }
        uint32_t seekOffset = listOfTables[idxNameTable].offset + nameTable.stringOffset + listOfNameRecords[ii].offset;     // find out seek offset.
        if (fseek(fttf, seekOffset, SEEK_SET)) {
            fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxNameTable].offset); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
        }
        fread(nameList[ii], listOfNameRecords[ii].length, 1, fttf);
        if (errno) {
            fprintf(stdout, " main(): File error while reading object nameList (%d/%d). Error(%d)\n", ii, cntNameRecord, errno); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
        }
        // Names are stored in two bytes
        for (short jj = 0, kk=0; jj <= listOfNameRecords[ii].length/2; jj++, kk+=2) {       // length will be always an even number
            nameList[ii][jj] = (uint16_t)nameList[ii][kk + 1] > 125 ? ' ' : nameList[ii][kk + 1];     // don't print extended character on screen
        }
        nameList[ii][listOfNameRecords[ii].length/2] = 0;         // terminate with NULL.
        //printf("%2d) seekOffset=%d length=%d %s.........\n", ii, seekOffset, listOfNameRecords[ii].length, nameList[ii]);
    }

    // Head table
    const short idxHeadTable = getTable(listOfTables, numOfTables, "head");       // collect index corresponding to 'head' table.
    if (idxHeadTable == -1) {
        fprintf(stdout, " main(): the 'head' Table is not present: Total Tables: %d.\n", numOfTables); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    if (fseek(fttf, listOfTables[idxHeadTable].offset, SEEK_SET)) {
        fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxHeadTable].offset); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    STTFHeadTable headTable;
    fread(&headTable, sizeof(STTFHeadTable), 1, fttf);
    if (errno) {
        fprintf(stdout, " main(): File error while reading object STTFHeadTable. Error(%d)\n", errno); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }

    /**
     char* bswap = (char*)&headTable.version;
   printf("\nBefore Swap...\n");
    printf("   whole: 0x%04x (%d), Fract: 0x%04x (%d)\n", headTable.version.whole, headTable.version.whole, headTable.version.frac, headTable.version.frac);
    printf("   whole: 0x%04x (%d), Fract: 0x%04x (%d)\n", headTable.fontRevision.whole, headTable.fontRevision.whole, headTable.fontRevision.frac, headTable.fontRevision.frac);
    printf("minX=0x%04x minY=0x%04x, maxX=0x%04x maxY=0x%04x\n", headTable.xMin, headTable.yMin, headTable.xMax, headTable.yMax);
    printf("head offset=%lu\n\n", listOfTables[idxHeadTable].offset);
    */

    headTable.version.whole = SWAPWORD(headTable.version.whole);            // Version (whole part). 0x00010000 if (version 1.0)
    headTable.version.frac = SWAPWORD(headTable.version.frac);              // Version (fractional part)
    headTable.fontRevision.whole = SWAPWORD(headTable.fontRevision.whole);  // Version (whole part). set by font manufacturer
    headTable.fontRevision.frac = SWAPWORD(headTable.fontRevision.frac);    // Version (fractional part). set by font manufacturer
    headTable.checkSumAdjustment = SWAPLONG(headTable.checkSumAdjustment);  // To compute: set it to 0, calculate the checksum for the 'head' table and put it in the table directory, sum the entire font as a uint32_t, then store 0xB1B0AFBA - sum. (The checksum for the 'head' table will be wrong as a result. That is OK; do not reset it.)
    headTable.magicNumber = SWAPLONG(headTable.magicNumber);                // set to 0x5F0F3CF5
    headTable.flags = SWAPWORD(headTable.flags);                            // Name string length in bytes.
    headTable.unitsPerEm = SWAPWORD(headTable.unitsPerEm);                  // range from 64 to 16384
    headTable.created = SWAPLONGLONG(headTable.created); ;                  // international date
    headTable.modified = SWAPLONGLONG(headTable.modified); ;                // international date
    headTable.xMin = SWAPWORD(headTable.xMin);                              // for all glyph bounding boxes.
    headTable.yMin = SWAPWORD(headTable.yMin);                              // for all glyph bounding boxes.
    headTable.xMax = SWAPWORD(headTable.xMax);                              // for all glyph bounding boxes.
    headTable.yMax = SWAPWORD(headTable.yMax);                              // for all glyph bounding boxes.
    headTable.macStyle = SWAPWORD(headTable.macStyle);                      // bit 0 bold; bit 1 italic; bit 2 underline;  bit 3 outline; bit 4 shadow, bit 5 condensed (narrow) and bit 6 extended.
    headTable.lowestRecPPEM = SWAPWORD(headTable.lowestRecPPEM);            // smallest readable size in pixels.
    headTable.fontDirectionHint = SWAPWORD(headTable.fontDirectionHint);    // {0 Mixed directional glyphs, 1 Only strongly left to right glyphs, 2 Like 1 but also contains neutrals, -1 Only strongly right to left glyphs, -2 Like -1 but also contains neutrals}
    headTable.indexToLocFormat = SWAPWORD(headTable.indexToLocFormat);      // {0 for short offsets, 1 for long}
    headTable.glyphDataFormat = SWAPWORD(headTable.glyphDataFormat);        // {0 for current format}

    /**
    printf("After Swap...\n");
    printf("   whole: 0x%04x (%d), Fract: 0x%04x (%d)\n", headTable.version.whole, headTable.version.whole, headTable.version.frac, headTable.version.frac);
    printf("   whole: 0x%04x (%d), Fract: 0x%04x (%d)\n", headTable.fontRevision.whole, headTable.fontRevision.whole, headTable.fontRevision.frac, headTable.fontRevision.frac);
    printf("minX=0x%04x minY=0x%04x, maxX=0x%04x maxY=0x%04x\n", headTable.xMin, headTable.yMin, headTable.xMax, headTable.yMax);
    printf("minX=%d minY=%d, maxX=%d maxY=%d\n", headTable.xMin, headTable.yMin, headTable.xMax, headTable.yMax); getchar();
    */

    // Hhea table
    const short idxHheaTable = getTable(listOfTables, numOfTables, "hhea");    // collect index corresponding to 'hhea' table.
    if (idxHheaTable == -1) {
        fprintf(stdout, " main(): The 'hhea' Table is not present: Total Tables: %d.\n", numOfTables); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    if (fseek(fttf, listOfTables[idxHheaTable].offset, SEEK_SET)) {
        fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxHheaTable].offset); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    STTFHheaTable hheaTable;
    fread(&hheaTable, sizeof(STTFHheaTable), 1, fttf);
    if (errno) {
        fprintf(stdout, " main(): File error while reading object STTFHheaTable. Error(%d)\n", errno); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    hheaTable.version.whole = SWAPWORD(hheaTable.version.whole);                // Version (whole part). 0x00010000 if (version 1.0)
    hheaTable.version.frac = SWAPWORD(hheaTable.version.frac);                  // Version (fractional part)
    hheaTable.ascent = SWAPWORD(hheaTable.ascent);                              // Distance from baseline of highest ascender.
    hheaTable.descent = SWAPWORD(hheaTable.descent);                            // Distance from baseline of lowest descender.
    hheaTable.lineGap = SWAPWORD(hheaTable.lineGap);                            // typographic line gap. Negative LineGap values are treated as zero in some legacy platform implementations
    hheaTable.advanceWidthMax = SWAPWORD(hheaTable.advanceWidthMax);            // must be consistent with horizontal metrics.
    hheaTable.minLeftSideBearing = SWAPWORD(hheaTable.minLeftSideBearing);      // must be consistent with horizontal metrics.
    hheaTable.minRightSideBearing = SWAPWORD(hheaTable.minRightSideBearing);    // must be consistent with horizontal metrics.
    hheaTable.xMaxExtent = SWAPWORD(hheaTable.xMaxExtent); ;                    // max(lsb + (xMax-xMin)).
    hheaTable.caretSlopeRise = SWAPWORD(hheaTable.caretSlopeRise);              // used to calculate the slope of the caret (rise/run) set to 1 for vertical caret.
    hheaTable.caretSlopeRun = SWAPWORD(hheaTable.caretSlopeRun);                // 0 for vertical.
    hheaTable.caretOffset = SWAPWORD(hheaTable.caretOffset);                    // The amount by which a slanted highlight on a glyph needs to be shifted to produce the best appearance. Set to 0 for non-slanted fonts.
    hheaTable.reserved_1 = SWAPWORD(hheaTable.reserved_1);                      // set value to 0.
    hheaTable.reserved_2 = SWAPWORD(hheaTable.reserved_3);                      // set value to 0.
    hheaTable.reserved_3 = SWAPWORD(hheaTable.reserved_3);                      // set value to 0.
    hheaTable.reserved_4 = SWAPWORD(hheaTable.reserved_4);                      // set value to 0.
    hheaTable.metricDataFormat = SWAPWORD(hheaTable.metricDataFormat);          // 0 for current format
    hheaTable.numOfLongHorMetrics = SWAPWORD(hheaTable.numOfLongHorMetrics);    // Number of hMetric entries in 'hmtx' table


    // Cmap Table
    const short idxCmapTable = getTable(listOfTables, numOfTables, "cmap");        // collect index corresponding to 'cmap' table.   
    if (idxCmapTable == -1) {
        fprintf(stdout, " main(): The 'cmap' Table is not present: Total Tables: %d.\n", numOfTables); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    if (fseek(fttf, listOfTables[idxCmapTable].offset, SEEK_SET)) {
        fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxCmapTable].offset); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    STTFCMapTable_Header cmapTable;
    fread(&cmapTable, sizeof(STTFCMapTable_Header), 1, fttf);
    if (errno) {
        fprintf(stdout, " main(): File error while reading object STTFCMapTable_Header. Error(%d)\n", errno); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    cmapTable.version = SWAPWORD(cmapTable.version);                // Table version number (0).
    cmapTable.numTables = SWAPWORD(cmapTable.numTables);            // Number of encoding tables that follow. Note that only one of these encoding subtables is used at a time.
    const uint16_t cmapEncodingRecord = cmapTable.numTables;      // Total number cmap encoding records.

    STTFCmapTable_Encoding* cmapSubTableList = new STTFCmapTable_Encoding[cmapEncodingRecord];
    if (!cmapSubTableList) {
        fprintf(stdout, " main(): Memory Allocation error of Cmap. Total Encoding SubTables: %d.\n", cmapEncodingRecord); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    for (short ii = 0; ii < cmapEncodingRecord; ii++) {
        fread(cmapSubTableList+ii, sizeof(STTFCmapTable_Encoding), 1, fttf);
        if (errno) {
            fprintf(stdout, " main(): File error while reading object cmapSubTableList (%d/%d). Error(%d)\n", ii, cmapEncodingRecord, errno); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
        }
        cmapSubTableList[ii].platformID = SWAPWORD(cmapSubTableList[ii].platformID);                    // Platform identifier. 0=Unicode, 1=Macintosh, 2=reserved and 3=Microsoft
        cmapSubTableList[ii].platformSpecificID = SWAPWORD(cmapSubTableList[ii].platformSpecificID);    // Platform-specific encoding ID.
        cmapSubTableList[ii].offset = SWAPLONG(cmapSubTableList[ii].offset);                            // Byte offset from beginning of table to the subtable for this encoding.
    }

    STTFCmapTable_Format_4 cmapFormat_4;
    STTFCmapTable_Format_12 cmapFormat_12;
    STTFCmapTable_Format_4_Segment_Array segArray = {NULL, 0, NULL, NULL, NULL};
    STTFCmapTable_SequentialMapGroup_Record *groupRecord=NULL;
    uint16_t segcount;
    for (short ii = 0; ii < cmapEncodingRecord; ii++) {
        if ((cmapSubTableList[ii].platformID == 0 && cmapSubTableList[ii].platformSpecificID == 3) ||    // Unicode: Unicode BMP-only.
            (cmapSubTableList[ii].platformID == 3 && cmapSubTableList[ii].platformSpecificID == 1))      // Microsoft: Unicode BMP-only.
        {
            if (cmapSubTableList[ii].platformID == 3) continue;     // Unicode and Microsoft have same data in STTFCmapTable_Format_4 structure. 
            uint32_t seekOffset = listOfTables[idxCmapTable].offset + cmapSubTableList[ii].offset;      // find out seek offset.
            if (fseek(fttf, seekOffset, SEEK_SET)) {
                fprintf(stdout, " main(): File seek error in 'cmap` table. Offset: %u\n", seekOffset); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
            }
            fread(&cmapFormat_4, sizeof(STTFCmapTable_Format_4), 1, fttf);
            if (errno) {
                fprintf(stdout, " main(): File error while reading object STTFCmapTable_Format_4. Error(%d)\n", errno); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
            }
            cmapFormat_4.format = SWAPWORD(cmapFormat_4.format);                    // Subtable format; set to 4.
            cmapFormat_4.length = SWAPWORD(cmapFormat_4.length);                    // This is the length in bytes of the subtable.
            cmapFormat_4.language = SWAPWORD(cmapFormat_4.language);                // For requirements on use of the language field.
            cmapFormat_4.segCountX2 = SWAPWORD(cmapFormat_4.segCountX2);            // 2 × segCount.
            cmapFormat_4.searchRange = SWAPWORD(cmapFormat_4.searchRange);          // Maximum power of 2 less than or equal to segCount, times 2. i.e. ((2**floor(log2(segCount))) * 2, where “**” is an exponentiation operator)
            cmapFormat_4.entrySelector = SWAPWORD(cmapFormat_4.entrySelector);      // Log2 of the maximum power of 2 less than or equal to numTables (log2(searchRange/2), which is equal to floor(log2(segCount)))
            cmapFormat_4.rangeShift = SWAPWORD(cmapFormat_4.rangeShift);            // segCount times 2, minus searchRange ((segCount * 2) - searchRange)
            segcount = cmapFormat_4.segCountX2 / 2;                                 // segment count;
            segArray.endCode = new uint16_t[segcount];
            if (!segArray.endCode) {
                fprintf(stdout, " main(): Memory Allocation error of Cmap::endCode: segcount: %d.\n", segcount); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
            }
            segArray.startCode = new uint16_t[segcount];
            if (!segArray.startCode) {
                fprintf(stdout, " main(): Memory Allocation error of Cmap::startCode: segcount: %d.\n", segcount); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
            }
            segArray.idDelta = new uint16_t[segcount];
            if (!segArray.idDelta) {
                fprintf(stdout, " main(): Memory Allocation error of Cmap::idDelta: segcount: %d.\n", segcount); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
            }
            segArray.idRangeOffset = new uint16_t[segcount];
            if (!segArray.idRangeOffset) {
                fprintf(stdout, " main(): Memory Allocation error of Cmap::idRangeOffset: segcount: %d.\n", segcount); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
            }
            // Segment Array endcode
            fread(segArray.endCode, sizeof(uint16_t), segcount, fttf);
            if (errno) {
                fprintf(stdout, " main(): File error while reading object Segment Array endCode. segcount:%d, Error(%d)\n", segcount, errno); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
            }
            // Segment Array reservedPad
            fread(&segArray.reservedPad, sizeof(uint16_t), 1, fttf);
            if (errno) {
                fprintf(stdout, " main(): File error while reading object Segment Array reservedPad. Error(%d)\n", errno); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
            }
            // Segment Array startCode
            fread(segArray.startCode, sizeof(uint16_t), segcount, fttf);
            if (errno) {
                fprintf(stdout, " main(): File error while reading object Segment Array startCode. segcount:%d, Error(%d)\n", segcount, errno); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
            }
            // Segment Array idDelta
            fread(segArray.idDelta, sizeof(uint16_t), segcount, fttf);
            if (errno) {
                fprintf(stdout, " main(): File error while reading object Segment Array idDelta. segcount:%d, Error(%d)\n", segcount, errno); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
            }
            // Segment Array idRangeOffset
            fread(segArray.idRangeOffset, sizeof(uint16_t), segcount, fttf);
            if (errno) {
                fprintf(stdout, " main(): File error while reading object Segment Array idRangeOffset. segcount:%d, Error(%d)\n", segcount, errno); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
            }
            for (uint16_t jj = 0; jj < segcount; jj++) {
                segArray.endCode[jj] = SWAPWORD(segArray.endCode[jj]);              // convert from beg Endian to little Endian
                segArray.startCode[jj] = SWAPWORD(segArray.startCode[jj]);          // convert from beg Endian to little Endian
                segArray.idDelta[jj] = SWAPWORD(segArray.idDelta[jj]);              // convert from beg Endian to little Endian
                segArray.idRangeOffset[jj] = SWAPWORD(segArray.idRangeOffset[jj]);  // convert from beg Endian to little Endian
            }
        }
        else if ((cmapSubTableList[ii].platformID == 0 && cmapSubTableList[ii].platformSpecificID == 4) ||    // Unicode: Unicode non-BMP characters allowed
                (cmapSubTableList[ii].platformID == 3 && cmapSubTableList[ii].platformSpecificID == 10)) {        // Microsoft: Unicode UCS-4.
            if (cmapSubTableList[ii].platformID == 3) continue;     // Unicode and Microsoft have same data in STTFCmapTable_Format_12 structure. 
            uint32_t seekOffset = listOfTables[idxCmapTable].offset + cmapSubTableList[ii].offset;      // find out seek offset.
            if (fseek(fttf, seekOffset, SEEK_SET)) {
                fprintf(stdout, " main(): File seek error in 'cmap` table. Offset: %u\n", seekOffset); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
            }
            fread(&cmapFormat_12, sizeof(STTFCmapTable_Format_12), 1, fttf);
            if (errno) {
                fprintf(stdout, " main(): File error while reading object STTFCmapTable_Format_12. Error(%d)\n", errno); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
            }
            cmapFormat_12.format = SWAPWORD(cmapFormat_12.format);              // Subtable format; set to 12.
            cmapFormat_12.reserved = SWAPWORD(cmapFormat_12.reserved);          // Reserved; set to 0.
            cmapFormat_12.length = SWAPLONG(cmapFormat_12.length);              // Byte length of this subtable (including the header).
            cmapFormat_12.language = SWAPLONG(cmapFormat_12.language);          // For requirements on use of the language field.
            cmapFormat_12.numGroups = SWAPLONG(cmapFormat_12.numGroups);        // Number of groupings which follow.
            groupRecord = new STTFCmapTable_SequentialMapGroup_Record[cmapFormat_12.numGroups];
            if (!groupRecord) {
                fprintf(stdout, " main(): Memory Allocation error of Cmap. SequentialMapGroup_Record: %d.\n", cmapFormat_12.numGroups); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
            }
            fread(groupRecord, sizeof(STTFCmapTable_SequentialMapGroup_Record), cmapFormat_12.numGroups, fttf);
            if (errno) {
                fprintf(stdout, " main(): File error while reading object STTFCmapTable_SequentialMapGroup_Record. numGroups:%d, Error(%d)\n", cmapFormat_12.numGroups, errno); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
            }
            for (uint32_t jj = 0; jj < cmapFormat_12.numGroups; jj++) {
                groupRecord[jj].startCharCode = SWAPLONG(groupRecord[jj].startCharCode);    // First character code in this group.
                groupRecord[jj].endCharCode = SWAPLONG(groupRecord[jj].endCharCode);        // Last character code in this group.
                groupRecord[jj].startGlyphID = SWAPLONG(groupRecord[jj].startGlyphID);      // Glyph index corresponding to the starting character code. subsequent charcters are mapped to sequential glyphs
            }
        }
        else continue;
    }

    // Maxp table
    const short idxMaxpTable = getTable(listOfTables, numOfTables, "maxp");     // collect index corresponding to 'maxp' table.
    if (idxMaxpTable == -1) {
        fprintf(stdout, " main(): Maxp Table is not present: Total Tables: %d.\n", numOfTables); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    if (fseek(fttf, listOfTables[idxMaxpTable].offset, SEEK_SET)) {
        fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxMaxpTable].offset); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    STTFMaxpTable maxpTable;         // Maximum Profile table
    fread(&maxpTable, sizeof(STTFMaxpTable), 1, fttf);
    if (errno) {
        fprintf(stdout, " main(): File error while reading object STTFMaxpTable. Error(%d)\n", errno); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    maxpTable.version.whole = SWAPWORD(maxpTable.version.whole);                    // Version (whole part). 0x00010000 if (version 1.0)
    maxpTable.version.frac = SWAPWORD(maxpTable.version.frac);                      // Version (fractional part)
    maxpTable.numGlyphs = SWAPWORD(maxpTable.numGlyphs);                            // the number of glyphs in the font
    maxpTable.maxPoints = SWAPWORD(maxpTable.maxPoints);                            // points in non-compound glyph
    maxpTable.maxContours = SWAPWORD(maxpTable.maxContours);                        // contours in non-compound glyph
    maxpTable.maxComponentPoints = SWAPWORD(maxpTable.maxComponentPoints);          // points in compound glyph
    maxpTable.maxComponentContours = SWAPWORD(maxpTable.maxComponentContours);      // contours in compound glyph
    maxpTable.maxZones = SWAPWORD(maxpTable.maxZones); ;                            // set to 2
    maxpTable.maxTwilightPoints = SWAPWORD(maxpTable.maxTwilightPoints);            // points used in Twilight Zone (Z0)
    maxpTable.maxStorage = SWAPWORD(maxpTable.maxStorage);                          // number of Storage Area locations
    maxpTable.maxFunctionDefs = SWAPWORD(maxpTable.maxFunctionDefs);                // number of FDEFs
    maxpTable.maxInstructionDefs = SWAPWORD(maxpTable.maxInstructionDefs);          // number of IDEFs
    maxpTable.maxStackElements = SWAPWORD(maxpTable.maxStackElements);              // maximum stack depth
    maxpTable.maxSizeOfInstructions = SWAPWORD(maxpTable.maxSizeOfInstructions);    // byte count for glyph instructions
    maxpTable.maxComponentElements = SWAPWORD(maxpTable.maxComponentElements);      // number of glyphs referenced at top level
    maxpTable.maxComponentDepth = SWAPWORD(maxpTable.maxComponentDepth);            // levels of recursion, set to 0 if font has only simple glyphs

    // Post table
    const short idxPostTable = getTable(listOfTables, numOfTables, "post");     // collect index corresponding to 'maxp' table.
    if (idxPostTable == -1) {
        fprintf(stdout, " main(): Post Table is not present: Total Tables: %d.\n", numOfTables); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    if (fseek(fttf, listOfTables[idxPostTable].offset, SEEK_SET)) {
        fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxPostTable].offset); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    STTFPostTable postTable;         // Maximum Profile table
    fread(&postTable, sizeof(STTFPostTable), 1, fttf);
    if (errno) {
        fprintf(stdout, " main(): File error while reading object STTFPostTable. Error(%d)\n", errno); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    postTable.format.whole = SWAPWORD(postTable.format.whole);              // Format of this table: {0x00010000 for version 1.0, 0x00020000 for version 2.0, 0x00025000 for version 2.5, 0x00030000 for version 3.0}
    postTable.format.frac = SWAPWORD(postTable.format.frac);                // Format of this table (fractional part).
    postTable.italicAngle.whole = SWAPWORD(postTable.italicAngle.whole);    // italicAngle (whole part). Italic angle in counter-clockwise degrees from the vertical. Zero for upright text, negative for text that leans to the right (forward).
    postTable.italicAngle.frac = SWAPWORD(postTable.italicAngle.frac);      // italicAngle (fractional part). Italic angle in counter-clockwise degrees from the vertical. Zero for upright text, negative for text that leans to the right (forward).
    postTable.underlinePosition = SWAPWORD(postTable.underlinePosition);    // This is the suggested distance of the top of the underline from the baseline (negative values indicate below baseline).
    postTable.underlineThickness = SWAPWORD(postTable.underlineThickness);  // Suggested values for the underline thickness. In general, the underline thickness should match the thickness of the underscore character (U+005F LOW LINE), and should also match the strikeout thickness, which is specified in the OS/2 tabl
    postTable.isFixedPitch = SWAPLONG(postTable.isFixedPitch);              // Set to 0 if the font is proportionally spaced, non-zero if the font is not proportionally spaced (i.e. monospaced).
    postTable.minMemType42 = SWAPLONG(postTable.minMemType42);              // Minimum memory usage when a TrueType font is downloaded as a Type 42 font
    postTable.maxMemType42 = SWAPLONG(postTable.maxMemType42);              // Maximum memory usage when a TrueType font is downloaded as a Type 42 font
    postTable.minMemType1 = SWAPLONG(postTable.minMemType1);                // Minimum memory usage when a TrueType font is downloaded as a Type 1 font
    postTable.maxMemType1 = SWAPLONG(postTable.maxMemType1);                // Maximum memory usage when a TrueType font is downloaded as a Type 1 font


    // OS/2 table
    const short idxOS2Table = getTable(listOfTables, numOfTables, "os/2");     // collect index corresponding to 'maxp' table.
    if (idxOS2Table == -1) {
        fprintf(stdout, " main(): OS/2 Table is not present: Total Tables: %d.\n", numOfTables); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    if (fseek(fttf, listOfTables[idxOS2Table].offset, SEEK_SET)) {
        fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxOS2Table].offset); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    STTFOS2Table os2Table;         // Maximum Profile table
    fread(&os2Table, sizeof(STTFOS2Table), 1, fttf);
    if (errno) {
        fprintf(stdout, " main(): File error while reading object STTFOS2Table. Error(%d)\n", errno); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    os2Table.version = SWAPWORD(os2Table.version);                          // 0x0005
    os2Table.xAvgCharWidth = SWAPWORD(os2Table.xAvgCharWidth);              // average weighted advance width of lower case letters and space.
    os2Table.usWeightClass = SWAPWORD(os2Table.usWeightClass);              // visual weight (degree of blackness or thickness) of stroke in glyphs.
    os2Table.usWidthClass = SWAPWORD(os2Table.usWidthClass);                // relative change from the normal aspect ratio (width to height ratio) as specified by a font designer for the glyphs in the font.
    os2Table.fsType = SWAPWORD(os2Table.fsType);                            // characteristics and properties of this font (set undefined bits to zero).
    os2Table.ySubscriptXSize = SWAPWORD(os2Table.ySubscriptXSize);          // recommended horizontal size in pixels for subscripts.
    os2Table.ySubscriptYSize = SWAPWORD(os2Table.ySubscriptYSize); ;        // recommended vertical size in pixels for subscripts.
    os2Table.ySubscriptXOffset = SWAPWORD(os2Table.ySubscriptXOffset);      // recommended horizontal offset for subscripts
    os2Table.ySubscriptYOffset = SWAPWORD(os2Table.ySubscriptYOffset);      // recommended vertical offset form the baseline for subscripts
    os2Table.ySuperscriptXSize = SWAPWORD(os2Table.ySuperscriptXSize);      // recommended horizontal size in pixels for superscripts
    os2Table.ySuperscriptYSize = SWAPWORD(os2Table.ySuperscriptYSize);      // recommended vertical size in pixels for superscripts
    os2Table.ySuperscriptXOffset = SWAPWORD(os2Table.ySuperscriptXOffset);  // recommended horizontal offset for superscripts
    os2Table.ySuperscriptYOffset = SWAPWORD(os2Table.ySuperscriptYOffset);  // recommended vertical offset from the baseline for superscripts
    os2Table.yStrikeoutSize = SWAPWORD(os2Table.yStrikeoutSize);            // width of the strikeout stroke
    os2Table.yStrikeoutPosition = SWAPWORD(os2Table.yStrikeoutPosition);    // position of the strikeout stroke relative to the baseline
    os2Table.sFamilyClass = SWAPWORD(os2Table.sFamilyClass);                // classification of font-family design.
    os2Table.ulUnicodeRange1 = SWAPLONG(os2Table.ulUnicodeRange1);          // Bits 0–31
    os2Table.ulUnicodeRange2 = SWAPLONG(os2Table.ulUnicodeRange2);          // Bits 32–63
    os2Table.ulUnicodeRange3 = SWAPLONG(os2Table.ulUnicodeRange3);          // Bits 64–95
    os2Table.ulUnicodeRange4 = SWAPLONG(os2Table.ulUnicodeRange4);          // Bits 96–127
    os2Table.fsSelection = SWAPWORD(os2Table.fsSelection);                  // 2-byte bit field containing information concerning the nature of the font patterns
    os2Table.usFirstCharIndex = SWAPWORD(os2Table.usFirstCharIndex);        // The minimum Unicode index in this font.
    os2Table.usLastCharIndex = SWAPWORD(os2Table.usLastCharIndex);          // The maximum Unicode index in this font.
    os2Table.sTypoAscender = SWAPWORD(os2Table.sTypoAscender);              // The typographic ascender for this font. This is not necessarily the same as the ascender value in the 'hhea' table.
    os2Table.sTypoDescender = SWAPWORD(os2Table.sTypoDescender);            // The typographic descender for this font. This is not necessarily the same as the descender value in the 'hhea' table.
    os2Table.sTypoLineGap = SWAPWORD(os2Table.sTypoLineGap);                // The typographic line gap for this font. This is not necessarily the same as the line gap value in the 'hhea' table.
    os2Table.usWinAscent = SWAPWORD(os2Table.usWinAscent);                  // The ascender metric for Windows. usWinAscent is computed as the yMax for all characters in the Windows ANSI character set.
    os2Table.usWinDescent = SWAPWORD(os2Table.usWinDescent);                // The descender metric for Windows. usWinDescent is computed as the -yMin for all characters in the Windows ANSI character set.
    os2Table.ulCodePageRange1 = SWAPLONG(os2Table.ulCodePageRange1);        // Bits 0-31
    os2Table.ulCodePageRange2 = SWAPLONG(os2Table.ulCodePageRange2);        // Bits 32-63
    os2Table.sxHeight = SWAPWORD(os2Table.sxHeight);                        // The distance between the baseline and the approximate height of non-ascending lowercase letters measured in FUnits.
    os2Table.sCapHeight = SWAPWORD(os2Table.sCapHeight);                    // The distance between the baseline and the approximate height of uppercase letters measured in FUnits.
    os2Table.usDefaultChar = SWAPWORD(os2Table.usDefaultChar);              // The default character displayed by Windows to represent an unsupported character. (Typically this should be 0.)
    os2Table.usBreakChar = SWAPWORD(os2Table.usBreakChar);                  // The break character used by Windows.
    os2Table.usMaxContext = SWAPWORD(os2Table.usMaxContext);                // The maximum length of a target glyph OpenType context for any feature in this font.
    os2Table.usLowerOpticalPointSize = SWAPWORD(os2Table.usLowerOpticalPointSize);  // Proposed for version 5 The lowest size (in twentieths of a typographic point), at which the font starts to be used. This is an inclusive value.
    os2Table.usUpperOpticalPointSize = SWAPWORD(os2Table.usUpperOpticalPointSize);  // Proposed for version 5 The highest size (in twentieths of a typographic point), at which the font starts to be used. This is an exclusive value. Use 0xFFFFU to indicate no upper limit.


    // Hmtx table
    const short idxHmtxTable = getTable(listOfTables, numOfTables, "hmtx");     // collect index corresponding to 'hmtx' table.
    if (idxHmtxTable == -1) {
        fprintf(stdout, " main(): The 'hmtx' Table is not present: Total Tables: %d.\n", numOfTables); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    if (fseek(fttf, listOfTables[idxHmtxTable].offset, SEEK_SET)) {
        fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxHmtxTable].offset); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }

    const uint16_t numOfGlyphs = maxpTable.numGlyphs;                                               // the number of glyphs in the font
    const uint16_t numberOfHMetrics = hheaTable.numOfLongHorMetrics;                                // Number of hMetric entries in 'hmtx' table
    STTFHmtxTable_LeftSideBearings* listOfTableHmtx_LSBearings = NULL;                              // Initialize with NULL.
    STTFHmtxTable_HMetrix* listOfTableHmtx_Hmetrix = new STTFHmtxTable_HMetrix[numberOfHMetrics];   // Allocate memeory
    if (!listOfTableHmtx_Hmetrix) {
        fprintf(stdout, " main(): Memory Allocation error for object STTFHmtxTable_HMetrix. Total Metrix: %d.\n", numberOfHMetrics); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    for (uint16_t ii = 0; ii < numberOfHMetrics; ii++) {
        fread(listOfTableHmtx_Hmetrix + ii, sizeof(STTFHmtxTable_HMetrix), 1, fttf);
        if (errno) {
            fprintf(stdout, " main(): File error while reading object STTFHmtxTable_HMetrix(%d/%d). Error(%d)\n", ii, numberOfHMetrics, errno); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
        }
        listOfTableHmtx_Hmetrix[ii].advanceWidth = SWAPWORD(listOfTableHmtx_Hmetrix[ii].advanceWidth);  // Advance width, in font design units.
        listOfTableHmtx_Hmetrix[ii].lsb = SWAPWORD(listOfTableHmtx_Hmetrix[ii].lsb);                    // Glyph left side bearing, in font design units.
    }
    if (numberOfHMetrics < numOfGlyphs) {
        const uint16_t remainingGlyphs = numOfGlyphs - numberOfHMetrics;                                    // remaining.
        listOfTableHmtx_LSBearings = new STTFHmtxTable_LeftSideBearings[remainingGlyphs];                   // Allocate memeory
        if (!listOfTableHmtx_LSBearings) {
            fprintf(stdout, " main(): Memory Allocation error for object STTFHmtxTable_LeftSideBearings. Reaminig Glyphs: %d.\n", remainingGlyphs); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
        }
        for (uint16_t ii = 0; ii < remainingGlyphs; ii++) {
            fread(listOfTableHmtx_LSBearings + ii, sizeof(STTFHmtxTable_LeftSideBearings), 1, fttf);
            if (errno) {
                fprintf(stdout, " main(): File error while reading object STTFHmtxTable_LeftSideBearings(%d/%d). Error(%d)\n", ii, remainingGlyphs, errno); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
            }
            listOfTableHmtx_LSBearings[ii].leftSideBearings = SWAPWORD(listOfTableHmtx_LSBearings[ii].leftSideBearings);    // Here the advanceWidth is assumed to be the same as the advanceWidth for the last entry above structure.
        }
    }


    // Loca table
    const short idxLocaTable = getTable(listOfTables, numOfTables, "loca");     // collect index corresponding to 'loca' table.
    if (idxLocaTable == -1) {
        fprintf(stdout, " main(): The 'loca' Table is not present: Total Tables: %d.\n", numOfTables); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    if (fseek(fttf, listOfTables[idxLocaTable].offset, SEEK_SET)) {
        fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxLocaTable].offset); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    const uint16_t numOfLocaOffsets = maxpTable.numGlyphs + 1;              // Total number of offsets of table 'loca'.
    STTFLocaTable_Short* listOfLocaOffsets_short = NULL;                    // Initialize with NULL.
    STTFLocaTable_Long* listOfLocaOffsets_long = NULL;                      // Initialize with NULL.
    if (headTable.indexToLocFormat) {     // {0 for short offsets, 1 for long}
        listOfLocaOffsets_long = new STTFLocaTable_Long[numOfLocaOffsets];   // Allocate memeory
        if (!listOfLocaOffsets_long) {
            fprintf(stdout, " main(): Memory Allocation error for object STTFLocaTable_long. Total Offsets: %d.\n", numOfLocaOffsets); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
        }
        for (uint16_t ii = 0; ii < numOfLocaOffsets; ii++) {
            fread(listOfLocaOffsets_long + ii, sizeof(STTFLocaTable_Long), 1, fttf);
            if (errno) {
                fprintf(stdout, " main(): File error while reading object STTFLocaTable_Long(%d/%d). Error(%d)\n", ii, numOfLocaOffsets, errno); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
            }
            listOfLocaOffsets_long[ii].offsets = SWAPLONG(listOfLocaOffsets_long[ii].offsets);  // 'loca' Table: long version.
        }
    }
    else {// short
        listOfLocaOffsets_short = new STTFLocaTable_Short[numOfLocaOffsets];   // Allocate memeory
        if (!listOfLocaOffsets_short) {
            fprintf(stdout, " main(): Memory Allocation error for object STTFLocaTable_Short. Total Offsets: %d.\n", numOfLocaOffsets); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
        }
        for (uint16_t ii = 0; ii < numOfLocaOffsets; ii++) {
            fread(listOfLocaOffsets_short + ii, sizeof(STTFLocaTable_Short), 1, fttf);
            if (errno) {
                fprintf(stdout, " main(): File error while reading object STTFLocaTable_Short(%d/%d). Error(%d)\n", ii, numOfLocaOffsets, errno); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
            }
            listOfLocaOffsets_short[ii].offsets = SWAPWORD(listOfLocaOffsets_short[ii].offsets);  // 'loca' Table: short version.
        }
    }


    // Prep table
    const short idxPrepTable = getTable(listOfTables, numOfTables, "prep");     // collect index corresponding to 'prep' table.
    if (idxPrepTable == -1) {
        fprintf(stdout, " main(): The 'prep' Table is not present: Total Tables: %d.\n", numOfTables); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    if (fseek(fttf, listOfTables[idxPrepTable].offset, SEEK_SET)) {
        fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxPrepTable].offset); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    uint32_t lengthOfPrepTable = listOfTables[idxPrepTable].length;                 // length of the 'prep' table in bytes
    STTFPrepTable* listOfPrep = new STTFPrepTable[lengthOfPrepTable];               // Allocate memeory
    if (!listOfPrep) {
        fprintf(stdout, " main(): Memory Allocation error for object STTFPrepTable. Total Offsets: %d.\n", lengthOfPrepTable); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
    }
    fread(listOfPrep, sizeof(STTFPrepTable), lengthOfPrepTable, fttf);
    if (errno) {
        fprintf(stdout, " main(): File error while reading object STTFPrepTable(%d). Error(%d)\n", lengthOfPrepTable, errno); // error
        fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
    }


    // Fpgm table: optional
    STTFFpgmTable* listOfFpgm= NULL;                                            // Initialize with NULL.
    const short idxFpgmTable = getTable(listOfTables, numOfTables, "fpgm");     // collect index corresponding to 'fpgm' table.
    if (idxFpgmTable != -1) {
        if (fseek(fttf, listOfTables[idxFpgmTable].offset, SEEK_SET)) {
            fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxFpgmTable].offset); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
        }
        uint32_t lengthOfFpgmTable = listOfTables[idxFpgmTable].length;                 // length of the 'fpgm' table in bytes
        if (!(listOfFpgm = new STTFFpgmTable[lengthOfFpgmTable])) {                      // Allocate memeory
            fprintf(stdout, " main(): Memory Allocation error for object STTFFpgmTable. Total Offsets: %d.\n", lengthOfFpgmTable); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
        }
        fread(listOfFpgm, sizeof(STTFFpgmTable), lengthOfFpgmTable, fttf);
        if (errno) {
            fprintf(stdout, " main(): File error while reading object STTFFpgmTable(%d). Error(%d)\n", lengthOfFpgmTable, errno); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
        }
    }

    // Cvt table: optional
    STTFCvtTable* listOfCvt = NULL;                                            // Initialize with NULL.
    const short idxCvtTable = getTable(listOfTables, numOfTables, "cvt");     // collect index corresponding to 'cvt' table.
    if (idxCvtTable != -1) {
        if (fseek(fttf, listOfTables[idxCvtTable].offset, SEEK_SET)) {
            fprintf(stdout, " main(): File seek error. Offset: %u\n", listOfTables[idxCvtTable].offset); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
        }
        uint32_t lengthOfCvtTable = listOfTables[idxCvtTable].length;                 // length of the 'cvt' table in bytes
        printf("lengthOfCvtTable=%d\n", lengthOfCvtTable);
        if (!(listOfCvt = new STTFCvtTable[lengthOfCvtTable])) {               // Allocate memeory
            fprintf(stdout, " main(): Memory Allocation error for object STTFCvtTable. Total Offsets: %u.\n", lengthOfCvtTable); // error
            fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 		exit(1);
        }
        for (uint16_t ii = 0; ii < lengthOfCvtTable; ii++) {
            fread(listOfCvt + ii, sizeof(STTFCvtTable), 1, fttf);
            if (errno) {
                fprintf(stdout, " main(): File error while reading object STTFCvtTable(%d/%d). Error(%d)\n", ii, lengthOfCvtTable, errno); // error
                fprintf(stdout, "\nHit any key to exit.......\n");		getchar(); 	exit(1);
            }
            listOfCvt[ii].controlValues = SWAPWORD(listOfCvt[ii].controlValues);  // swap word.
        }
    }

    //printf("NumOfGlyphs=%d numberOfHMetrics = %u cntNameRecord=%d", numOfGlyphs, numberOfHMetrics, cntNameRecord); getchar();

    const double emUnit = 1000.0 / static_cast<double>(headTable.unitsPerEm);   // The space required in termos of 1000 units for character 'M'.
    const char* strCopyRight = nameList[0];                                     // code 0 represents copy right.
    const char* strFontFamily = nameList[1];                                    // code 1 represents font family name.
    const char* strFontStyle = nameList[2];                                     // code 2 represents font Style.
    const char* strFontFullName = nameList[4];                                  // code 4 represents font full name.
    const char *strPSFontName = nameList[6];                                    // code 6 represents name of Postscript font.
    const char* strTradeMark = cntNameRecord < 8 ? "" : nameList[7];            // code 7 represents Trademark.
    const char* strFontWeight = getWeight(os2Table.usWeightClass);              // Font Weight Class.

    /**
    printf("panose familykind=%d weight=%d %d\n", os2Table.panose[0], os2Table.panose[2], os2Table.usWeightClass);
    printf("emUnit=%g\n", emUnit);
    printf("Before:   whole: 0x%04x (%d), Fract: 0x%04x (%d)\n", headTable.version.whole, headTable.version.whole, headTable.version.frac, headTable.version.frac);
    printf("Before:   whole: 0x%04x (%d), Fract: 0x%04x (%d)\n", headTable.fontRevision.whole, headTable.fontRevision.whole, headTable.fontRevision.frac, headTable.fontRevision.frac);
    */
    fprintf(fcid, "%%!PS-TrueTypeFont-%d-%d\n", headTable.version.whole, headTable.fontRevision.whole);   // %!PS-TrueTypeFont is required for disk-based Type 42 fonts on any interpreter that supports type 42 font.
    fprintf(fcid, "%%%%VMusage: %u %u\n", postTable.minMemType42, postTable.maxMemType42);  // This comment is not used by Postscript interpreter but is useful for application programs. Operator resourcestatus can be used to obtain VM requirement for a font resource.
    fprintf(fcid, "\n%% --------------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(fcid, "%% CIDInit is required for i) Building a Type 0 CIDFont or ii) Creating CMap dictionary.\n");
    fprintf(fcid, "%% A CMap dictionary is created with the assistance of operators defined in the CIDInit procedure set.\n");
    fprintf(fcid, "%% CIDInit procedure set is an instance of the ProcSet resource category.\n");  // 
    fprintf(fcid, "%% The use of these operators to construct a CMap for a CIDkeyed font has been discussed in Section 5.11.5, 'FMapType 9 Composite Fonts.'\n");  // 
    fprintf(fcid, "%% --------------------------------------------------------------------------------------------------------------------------------\n\n");
    fprintf(fcid, "/CIDInit /ProcSet findresource begin\n");        // Building CMap dictionary. The operators needed to construct a CMap dictionary are contained in the CIDInit procedure set (an instance of the ProcSet resource category).
    fprintf(fcid, "  6 dict begin\n");                              // Dictionary construction begins with space for 10 key-value pairs.
    fprintf(fcid, "     begincmap\n");                              // This is must
    fprintf(fcid, "       /CMapType 1 def\n");                      // The CMap type, indicating how the CMap is organized. The only defined values are 0 and 1, which are equivalent; 1 is recommended.
    fprintf(fcid, "       /CMapName /Identity-H def\n");            // The name of the CMap. Ordinarily, it is the same as the key given to defineresource when defining the dictionary as an instance of the CMap resource category
    fprintf(fcid, "       /CIDSystemInfo 3 dict dup begin\n");      // A dictionary or array identifying the character collection for each associated font or CIDFont.
    fprintf(fcid, "          /Registry (Adobe) def\n");             // A string identifying an issuer of character collections—for example 'Adobe'.
    fprintf(fcid, "          /Ordering (Identity) def\n");          // A string that uniquely names a character collection issued by a specific registry. Character collections whose Registry and Ordering values are the same are compatible
    fprintf(fcid, "          /Supplement 0 def\n");                 // An original character collection has a supplement number of 0. Whenever additionalCIDs are assigned in a character collection, the supplement number is increased.
    fprintf(fcid, "       end readonly def\n");                     // CIDSystemInfo dictionary is readonly.
    fprintf(fcid, "       1 begincodespacerange\n");                // Codespace ranges (the valid input character code range)by specifying a pair of codes of some particular length giving the lower and upper bounds of each range.
    fprintf(fcid, "          <0000> <ffff>\n");                     // Right now there is only one range from 0 to 65535.
    fprintf(fcid, "       endcodespacerange\n");                    // End of Codespace range.
    fprintf(fcid, "       0 usefont\n");                            // Specifies the font number of the font or CIDFont to which the subsequent CMap mapping operations apply.
    fprintf(fcid, "       1 begincidrange\n");                      // define mappings of individual input character codes to CIDs in the associated CIDFont involving ranges of input codes.
    fprintf(fcid, "          <0000> <ffff> 0\n");                   // Right now there is only one range from 0 to 65535.
    fprintf(fcid, "       endcidrange\n");                          // End of CIDspace range.
    fprintf(fcid, "     endcmap\n");                                // End of CMap.
    fprintf(fcid, "     currentdict CMapName exch /CMap defineresource pop\n");  // Operator defineresource operates on the CMap category which associates a resource instance with a resource name in a specified category. The 'CMapName' is resource name. The 'currentdict' is resource instance. And the 'CMap' is resource category.
    fprintf(fcid, "  end\n");                                       // The 'cmap' Dictionary has been defined.
    fprintf(fcid, "end\n");                                         // End of CIDInit.
    fprintf(fcid, "\n%% -------------------------------\n");
    fprintf(fcid, "%% Construct Mycidfont Dictionary.\n");
    fprintf(fcid, "%% -------------------------------\n\n");
    fprintf(fcid, "/Mycidfont 14 dict dup begin\n");                // Construction of Mycidfont dictionary begins with space for 14 key-value pairs.
    fprintf(fcid, "   /CIDFontName /%s def\n", strPSFontName);      // Postscript font name.
    fprintf(fcid, "   /CIDFontType 2 def\n");                       // CID fonttype 2.
    fprintf(fcid, "   /FontType 42 def\n");                         // This will be replaced with 11, subsequently by definefont which is TrueType glyph descriptions, similar to Type 42 base fonts.
    fprintf(fcid, "   /CIDSystemInfo 3 dict dup begin\n");          // A dictionary or array identifying the character collection for each associated font or CIDFont.
    fprintf(fcid, "      /Registry (Adobe) def\n");                 // A string identifying an issuer of character collections—for example 'Adobe'.
    fprintf(fcid, "      /Ordering (Identity) def\n");              // A string that uniquely names a character collection issued by a specific registry. Character collections whose Registry and Ordering values are the same are compatible
    fprintf(fcid, "      /Supplement 0 def\n");                     // An original character collection has a supplement number of 0. Whenever additionalCIDs are assigned in a character collection, the supplement number is increased.
    fprintf(fcid, "   end readonly def\n");                         // CIDSystemInfo dictionary is readonly.
    fprintf(fcid, "   /GDBytes %d def\n", numOfGlyphs > 255 ? 2:1); // The length in bytes of the TrueType glyph index in the CIDMap table. If the length is greater than 1, the bytes comprising a glyph index are interpreted high-order byte first. In most cases GDBytes will be 2,allowing glyph indices in the range 0 to 65,535.
    fprintf(fcid, "   /CIDCount %d def\n", numOfGlyphs);            // The number of valid CIDs in the CIDFont. Valid CIDs range from 0 to (CIDCount - 1); CIDs outside this range are treated as undefined glyphs.
    fprintf(fcid, "   /CIDMap %d string\n", 2 * numOfGlyphs);       // A table containing the glyph index for each CID. The table must be 'CIDCount * GDBytes' long. It may be represented as a single string or as an array of strings in which each element is a multiple of GDBytes long.
    fprintf(fcid, "      0 1 %d {\n", numOfGlyphs - 1);                         // Iterate total glyphs times for loop to fill CID values from 0 to (numOfGlyphs-1) in the CMap string. Note that CIDmap can be implemented as a dictionary or an array or a string or an integer.
    fprintf(fcid, "         2 copy dup 2 mul exch -8 bitshift put\n");          // Store low order byte of cid whose value is between 0 and numOfGlyphs-1.
    fprintf(fcid, "         1 index exch dup 2 mul 1 add exch 255 and put\n");  // Store high order byte of cid whose value is between 0 and numOfGlyphs-1.
    fprintf(fcid, "      } for\n");                                             // End of for loop.
    fprintf(fcid, "   def\n");                                                  // CIDMap string has been defined.
    fprintf(fcid, "   /FontInfo 16 dict dup begin\n");                          // (Optional) A dictionary containing font information that is not accessed by the PostScript interpreter.
    fprintf(fcid, "      /version (%d.%d) readonly def\n", maxpTable.version.whole, maxpTable.version.frac); // Set to 0 if the font is proportionally spaced, non-zero if the font is not proportionally spaced (i.e. monospaced).
    fprintf(fcid, "      /Notice (%s) readonly def\n", strTradeMark);                           // Notice that displays trade mark of the font.
    fprintf(fcid, "      /Copyright (%s) readonly def\n", strCopyRight);                        // Copy Right.
    fprintf(fcid, "      /FamilyName (%s) readonly def\n", strFontFamily);                      // Font Family name.
    fprintf(fcid, "      /FullName (%s) readonly def\n", strFontFullName);                      // Font Full name.
    fprintf(fcid, "      /Weight (%s) readonly def\n", strFontWeight);                          // Font Weight class.
    fprintf(fcid, "      /Style (%s) readonly def\n", strFontStyle);                            // Font Style
    fprintf(fcid, "      /isFixedPitch %s def\n", postTable.isFixedPitch ? "true" : "false");   // Set to 0 if the font is proportionally spaced, non-zero if the font is not proportionally spaced (i.e. monospaced).
    fprintf(fcid, "      /isSymbolFont %s def\n", os2Table.panose[0] == 5 ? "true" : "false");  // Some applications will use Family Kind = 5 (Latin Symbol) to identify symbol fonts, which might affect font selection or fallback behaviors. There are no requirements for how applications should use the panose values.
    fprintf(fcid, "      /ItalicAngle %d.%d def\n", postTable.italicAngle.whole, postTable.italicAngle.frac);   // Italic angle in counter-clockwise degrees from the vertical. Zero for upright text, negative for text that leans to the right (forward).
    fprintf(fcid, "      /Ascender %g def\n", emUnit* static_cast<double>(os2Table.sTypoAscender));     // Distance from baseline of highest ascender. This is not necessarily the same as the ascender value in the 'hhea' table.
    fprintf(fcid, "      /Descender %g def\n", emUnit* static_cast<double>(os2Table.sTypoDescender));   // Distance from baseline of lowest descender. This is not necessarily the same as the descender value in the 'hhea' table.
    fprintf(fcid, "      /LineSpacing  %u def\n", os2Table.sTypoLineGap);                               // The typographic line gap for this font. This is not necessarily the same as the line gap value in the 'hhea' table.
    fprintf(fcid, "      /UnderlinePosition %g def\n", emUnit * static_cast<double>(postTable.underlinePosition));   // This is the suggested distance of the top of the underline from the baseline (negative values indicate below baseline).
    fprintf(fcid, "      /UnderlineThickness %g def\n", emUnit* static_cast<double>(postTable.underlineThickness));  // Suggested values for the underline thickness. In general, the underline thickness should match the thickness of the underscore character (U+005F LOW LINE), and should also match the strikeout thickness, which is specified in the OS/2 tabl
    fprintf(fcid, "      /unitsPerEm %u def\n", headTable.unitsPerEm);  // range from 64 to 16384. Any value in this range is valid. In fonts that have TrueType outlines, a power of 2 is recommended as this allows performance optimizations in some rasterizers.
    fprintf(fcid, "   end readonly def\n");                             // CIDSystemInfo dictionary is readonly.
    fprintf(fcid, "   /FontMatrix [1 0 0 1 0 0] def\n");                // Type 42 fonts are usually defined in terms of an identity transform, so the value of FontMatrix (which transforms the glyph coordinate system into the user coordinate system) should be [1 0 0 1 0 0] in a Type 42 font dictionary. This is in contrast to a Type 1 font, whose glyph coordinate system is typically defined at a 1000-unit scale relative to user space.
    fprintf(fcid, "   /FontBBox [%g %g %g %g] readonly def\n", emUnit * static_cast<double>(headTable.xMin), emUnit * static_cast<double>(headTable.yMin), emUnit * static_cast<double>(headTable.xMax), emUnit * static_cast<double>(headTable.yMax));   // FontBBox is a required key that defines space of 1000/em a box largwe enough to enclose any of the characters in the CIDfont. An array of four numbers in the glyph coordinate system giving the left, bottom, right, and top coordinates, respectively, of the font bounding box.
    fprintf(fcid, "   /PaintType 0 def\n");                                     // A code indicating how the glyphs of the CIDFont are to be painted {0 = Glyph outlines are filled and 2 = Glyph outlines (designed to be filled) are stroked}. Default valueis 0.
    fprintf(fcid, "   /Encoding [] readonly def\n");                            // An encoding array usually contains 256 names, permitting it to be indexed by any 8-bit character code. An encoding array for use with composite fonts contains integers instead of names, and can be of any length.
    fprintf(fcid, "   /CharStrings 1 dict dup begin\n");                        // A dictionary associating character names (the keys in the dictionary) with glyph descriptions. The value can also be a PostScript procedure. This dictionary must have an entry whose key is .notdef.
    fprintf(fcid, "      /.notdef 0 def\n");                                    // Define an entry whose key is .notdef.
    fprintf(fcid, "   end readonly def\n");                                     // The CharStrings dictionary is readonly.
    fprintf(fcid, "   /sfnts [] def\n");                                        // sfnts array with no entries.
    fprintf(fcid, "end def\n");                                                 // The 'cidfont' Dictionary has been defined.
    fprintf(fcid, "\n%% ---------------------------------------------------------------------------------------------\n");
    fprintf(fcid, "%% Construct sfnts array which is a set of hex strings representing entire ttf file binary data.\n");
    fprintf(fcid, "%% ---------------------------------------------------------------------------------------------\n\n");
    fprintf(fcid, "Mycidfont /sfnts [\n");                                                  // sfnts array definition begins. This is a set of hex string representing entire ttf file binary data.
    embedTablesAsHexStrings_sfnts(fttf, fcid, ttfSubTable, listOfTables, numOfTables);      // Embed tables as Hex String delimted by angle brackets.
    fprintf(fcid, "] readonly put\n");                                                      // sfnts array Definition ends. This is a read-only array object.
    fprintf(fcid, "\n%% -------------------------------------------------------------------------------------------------------------\n");
    fprintf(fcid, "%% i)  Operator defineresource registers instance of Mycidfont with /CIDFont resource category.\n");
    fprintf(fcid, "%% ii) Operator composefont creates a Type 0 font with the name CIDFontName.\n");
    fprintf(fcid, "%%     Note that a Type 0 font with FMapType 9 require a CMap entry in the font dictionary which is /Identity-H.\n");
    fprintf(fcid, "%% -------------------------------------------------------------------------------------------------------------\n\n");
    fprintf(fcid, "Mycidfont dup /CIDFontName get exch /CIDFont defineresource\n");  // Register instance of Mycidfont with /CIDFont resource category.
    fprintf(fcid, "/CIDFontName get /Identity-H [2 index] composefont pop\n");       // Type 0 Composite font. Operator composefont creates a Type 0 font with the name CIDFontName. Note that Type 0 fonts with FMapType 9 require a CMap entry in the font dictionary which is /Identity-H.
    fclose(fttf);                                           // close ttf file handle.
    fclose(fcid);                                           // close cid file handle.
    printAlphabet_T42(fps, numOfGlyphs, strPSFontName);     // Print Glyphs along with corresponding CIDs
    fclose(fps);                                            // close Postscript file handle.
    if (isdisplay) {
        //debugString();
        debugSubTables(ttfSubTable, strTrueTypeFontFile);                                                   // debug Sub table
        debugListOfTables(listOfTables, numOfTables, numOfGlyphs, strPSFontName, strTrueTypeFontFile);      // Debug List of Tables
        debugListOfCMapEncodingRecords(cmapSubTableList, cmapEncodingRecord, strTrueTypeFontFile);        // Debug List of Cmap Encoding Subtables.
        for (short ii = 0; ii < cmapEncodingRecord; ii++) {
            if ((cmapSubTableList[ii].platformID == 0 && cmapSubTableList[ii].platformSpecificID == 3) ||    // Unicode: Unicode BMP-only.
                (cmapSubTableList[ii].platformID == 3 && cmapSubTableList[ii].platformSpecificID == 1))      // Microsoft: Unicode BMP-only.
            {
                if (cmapSubTableList[ii].platformID == 3) continue;     // Unicode and Microsoft have same data in STTFCmapTable_Format_4 structure. 
                debugCMapSegmeentArrayFormat_4(cmapFormat_4, segArray, segcount, cmapSubTableList[ii].platformID, cmapSubTableList[ii].platformSpecificID, strTrueTypeFontFile);
            }
            else if ((cmapSubTableList[ii].platformID == 0 && cmapSubTableList[ii].platformSpecificID == 4) ||    // Unicode: Unicode non-BMP characters allowed
                (cmapSubTableList[ii].platformID == 3 && cmapSubTableList[ii].platformSpecificID == 10)) {        // Microsoft: Unicode UCS-4.
                if (cmapSubTableList[ii].platformID == 3) continue;     // Unicode and Microsoft have same data in STTFCmapTable_Format_12 structure. 
                debugCMapSegmeentArrayFormat_12(cmapFormat_12, groupRecord, cmapSubTableList[ii].platformID, cmapSubTableList[ii].platformSpecificID, strTrueTypeFontFile);
            }
            else continue;
        }
        debugListOfNameRecords(listOfNameRecords, nameList, cntNameRecord, strTrueTypeFontFile);            // Debug List of Name Records.
    }

    if (listOfTables) delete[]listOfTables;                                 // release allocated memory
    if (listOfNameRecords) delete[]listOfNameRecords;                       // release allocated memory
    if (cntNameRecord) {
        for (int ii = 0; ii < cntNameRecord; ii++) {
            if (nameList[ii]) delete nameList[ii];                          // release memeory allocated for individual name.
        }
        delete[]nameList;				                                    // release allocated memory for 'name' list.
    }
    if (cmapSubTableList) delete[]cmapSubTableList;                         // release memeory allocated for Encoding subtables.
    if (groupRecord) delete[]groupRecord;                                   // release memeory allocated for 'cmap' format 12 SequentialMapGroup_Record
    if (segArray.endCode) delete[]segArray.endCode;                         // release memeory allocated for 'cmap' format 4 Segment Array endCode.
    if (segArray.startCode) delete[]segArray.startCode;                     // release memeory allocated for 'cmap' format 4 Segment Array startCode.
    if (segArray.idDelta) delete[]segArray.idDelta;                         // release memeory allocated for 'cmap' format 4 Segment Array idDelta.
    if (segArray.idRangeOffset) delete[]segArray.idRangeOffset;             // release memeory allocated for 'cmap' format 4 Segment Array idRangeOffset.
    if (listOfTableHmtx_LSBearings) delete[]listOfTableHmtx_LSBearings;     // release allocated memory LeftSideBearings of 'hmtx' table.
    if (listOfTableHmtx_Hmetrix) delete[]listOfTableHmtx_Hmetrix;           // release allocated memory for HMetrix of 'hmtx' table.
    if (listOfLocaOffsets_short) delete[]listOfLocaOffsets_short;           // release allocated memory for 'loca' table short offset.
    if (listOfLocaOffsets_long) delete[]listOfLocaOffsets_long;             // release allocated memory for 'loca' table long offset.
    if (listOfPrep) delete[]listOfPrep;                                     // release allocated memory for 'prep' table.
    if (listOfFpgm) delete[]listOfFpgm;                                     // release allocated memory for 'fpgm' table.
    if (listOfCvt) delete[]listOfCvt;                                       // release allocated memory for 'cvt' table.

    // completed successfully
    printf("\nConversion has been successfully completed and the following two files have been generated.\n");
    printf("  1. %s is the converted file corresponding to the TrueType font %s.\n", t42Filenamet, strTrueTypeFontFile);
#if _MSC_VER			// Visual Studio
    printf("  2. %s is a postscript program file executable either through Ghostscript (gswin64c.exe) or through GSView (gsview64.exe).\n", psFilename);
#elif __GNUC__	|| __CYGWIN__		// gcc
    printf("  2. %s is a postscript program file executable either through Ghostscript (gs) or through GSView (gv).\n", psFilename);
#endif
    printf("     This postscript program displays %u Glyphs present in the character set along with the corresponding CIDs in %d pages.\n", numOfGlyphs, numOfGlyphs/128 + ((numOfGlyphs % 128) > 0));
    printf("     Note: Before executing postscript program, make sure that CIDfont file %s is accessible to Ghostscript.\n\n", t42Filenamet);
    exit(0);
}
