# TTF2PostscriptCID
----------------------------------------------------------------------------------------
##### _TTF to PostScript Type 2 CIDFont (Type 42 Base Font) Converter_ 

##### Microsoft Windows 10.
----------------------------------------------------------------------------------------
##### Description
This standalone utility, written in C++, converts TrueType fonts to Type 0 Composite font that can be accessible from a postscript program. 

Adobe supports 3 approaches for conversion of ttf fonts to Type 42 base font.
1. If ttf font contains up to 255 glyphs, then conversion involves wrapping up of TrueType font into a PostScript Type 42 font. The Postscript **show** operator will extract a single byte every time.
2. If ttf font contains more than 255 glyphs, then Type 0 composite font conversion is required having Type 42 as base font. The TrueType font will be split into a hierarchy of multiple Type 42 fonts supporting access upto 255 fonts at each level. i.e. At the first level, a Type 42 font can access first 255 glyphs, the second one will access the next 255 glyphs and so on. The bottom most one will access the remaining glyps in the character set which should be less than 255. This is called 8/8 mapping whose FMapType is 2. The Postscript **show** operator will extract two bytes every time. First byte is font number and the 2nd one will be character code.
3. If TrueType font character set involves more than 255 glyphs, then a TrueType font can be converted into a CIDFont with TrueType outlines. This is called CMap mapping whose FMapType is 9. The CMap resource (maps character code to CIDs) can be constructed so the mapping in CIDMap (CIDs to glyph indices) is reduced to an identity mapping. i.e **Glyph index = CID**. A CIDMap could be either a string or an array of strings or a dictionary. In this software, CIDMap is implemented as a string and these fonts are called as Type 2 CIDfonts with Type 42 as base font. In this  composite font (Type 0), the hierarchy involved is only one level which is the CIDfont itself having the capability of accessing 65535 glyphs! Bear in mind that the Postscript **show** operator will extract two bytes every time.

A conversion utility can be developed using 2nd and 3rd approaches. The 3rd approach is not only flexible over 2nd approach but also unicode support is implementable.
This conversion utility supports only 3rd approach without any unicode support. 


##### Technical Features
- This utility is a console application developed on Microsoft Visual Studio Community 2022 (64-bit) Edition- Version 17.3.5 under Windows 10.
- This utility is also available in Linux platform.
- Note that this program is 100% portable across Windows-10 and Linux. i.e. The source files (main.ccp and ttf.h) are same and identical across platforms.
- Ghostsciprt version is 9.52 (64 bit) and GSView version is 5.0 (64 bit).


##### References
This program has been developed based on the following documents.
1. [Microsoft OpenType® Specification Version 1.9](https://learn.microsoft.com/en-us/typography/opentype/spec/)
2. [Fonts - TrueType Reference Manual - Apple Developer](https://developer.apple.com/fonts/TrueType-Reference-Manual/)
3. [PostScript Language Reference, third edition - Adobe Corporation (912 pages PDF, 7410K) Feb/1999](https://www.adobe.com/jp/print/postscript/pdfs/PLRM.pdf)
4. [The Type 42 Font Format Specification #5012 (28 pages PDF, 159k) 31/Jul/1998](https://adobe-type-tools.github.io/font-tech-notes/pdfs/5012.Type42_Spec.pdf)
5. [Adobe CMap and CID Font Files Specification #5014 (102 pages PDF, 541k) 11/Jun/1996 Version 1.0](https://adobe-type-tools.github.io/font-tech-notes/pdfs/5014.CIDFont_Spec.pdf)

##### Usage
Create a folder cidfonts in **D** drive and store ttf file which should be convered. Now issue the following command:
- `ttf2Postscriptcid.exe -d "D:\cidfonts\filename.ttf"`

If option **-d** is specified, then ttf table data will be displayed during execution.
This utility generates the following two files as output:
1. **filename.t42** is the required converted file with extension Type 42.
2. **filename.ps** is a postscript program file whose execution displays the glyphs present in the character set along with CIDs.
3. Invoke Ghostscript to execute a postscript program as follows:
    - `gswin64c.exe "D:\cidfonts\filename.t42" "D:\cidfonts\filename.ps"`

##### Example
[Google](https://fonts.google.com/noto/specimen/Noto+Sans+Tamil) has developed a family of Tamil fonts called **Noto Sans Tamil**.
In order to convert **NotoSansTamil-Regular.ttf"** font to Type 42 CIDfont, issue the following command:
- `ttf2Postscriptcid.exe -d "D:\cidfonts\NotoSansTamil-Regular.ttf"`

Two files are generated by the command which are **NotoSansTamil-Regular.t42** and **NotoSansTamil-Regular.ps**
To view Glyphs for entire character set along with CIDs, issue the following command:

- `gswin64c.exe "D:\cidfonts\NotoSansTamil-Regular.t42" "D:\cidfonts\NotoSansTamil-Regular.ps"`.

In order to test **NotoSansTamil-Regular.t42** CIDfont file, edit a file **tamil.ps** using nodepad in the folder "D:\cidfonts\" with the following postscript code:
```
%!PS-Adobe-3.0
/myNoTo {/NotoSansTamil-Regular findfont exch scalefont setfont} bind def
13 myNoTo
100 600 moveto 
% தமிழ் தங்களை வரவேற்கிறது!
<0019001d002a005e00030019004e00120030002200030024001f002f0024005b0012002a0020007a00aa> show
100 550 moveto 
% Tamil Welcomes You!
<0155017201aa019801a500030163018801a5017f01b101aa018801c20003016901b101cb00aa00b5> show
showpage
```

Issue the following Ghostscript command  to execute the `tamil.ps` postscript program. 
- `gswin64c.exe "D:\cidfonts\NotoSansTamil-Regular.t42" "D:\cidfonts\tamil.ps`

This will display strings `தமிழ் தங்களை வரவேற்கிறது!` and `Tamil Welcomes You!`.

Note that the strings for `show` operator are in Hexadecimal format embeded within angular brackets. Operator `show` extracts 2 bytes at a time and maps this CID (16 bit value) to a Glyph.
The first 4 Hex digits in the 1st string is `0019` whose decimal equivalent is `25`. This maps to glyph `த`.

##### [Other Indian Languages](https://fonts.google.com/noto/fonts)
This utility has been tested for the following languages successfully.
1. [Malayalam](https://fonts.google.com/noto/specimen/Noto+Sans+Malayalam)
2. [Telugu](https://fonts.google.com/noto/specimen/Noto+Sans+Telugu)
3. [Kannada](https://fonts.google.com/noto/specimen/Noto+Sans+Kannada)
4. [Gujarati](https://fonts.google.com/noto/specimen/Noto+Sans+Gujarati)
5. [Gurmukhi](https://fonts.google.com/noto/specimen/Noto+Sans+Gurmukhi) (Punjabi)
6. [Oriya](https://fonts.google.com/noto/specimen/Noto+Sans+Oriya)
7. [Bengali](https://fonts.google.com/noto/specimen/Noto+Sans+Bengali) (assamese)
8. [Devanagari](https://fonts.google.com/noto/specimen/Noto+Sans+Devanagari) (Sanskrit, Hindi and Marathi)