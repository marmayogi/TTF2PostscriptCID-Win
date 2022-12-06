# TTF2PostscriptCID
----------------------------------------------------------------------------------------
##### _TTF to PostScript Type 2 CIDFont (Type 42 Base Font) Converter_ 

##### Microsoft Windows 10.
----------------------------------------------------------------------------------------
##### Description
This standalone utility, written in C++, converts `TrueType` fonts to `Type 0` Composite fonts that can be accessible from a postscript program. 

**Adobe** supports 3 approaches for conversion of `ttf` fonts to `Type 42` base font.
1. If `ttf` font contains up to 256 glyphs, then conversion involves wrapping up of TrueType font into a PostScript Type 42 font. The Postscript **show** operator will extract a `single byte` every time.
2. If `ttf` font contains more than 256 glyphs, then Type 0 composite font conversion is required having Type 42 as base font. The TrueType font will be split into a hierarchy of multiple Type 42 fonts supporting access upto 256 glyphs at each level. i.e. At the first level, a Type 42 font can access first 256 glyphs (0 to 255), the second one will access the next 256 glyphs (256 to 511) and so on. The bottom most one will access the remaining glyps in the character set which should never exceed 256. This is called `8/8` mapping whose FMapType is `2`. The Postscript **show** operator will extract `two bytes` every time. First byte is `font number` and the 2nd one will be `character code`.
3. If TrueType font character set involves more than 256 glyphs, then a TrueType font can be converted into a CIDFont with TrueType outlines. This is called `CMap` mapping whose FMapType is `9`. The CMap resource (maps character code to CIDs) can be constructed so the mapping in `CIDMap` (CIDs to glyph indices) is reduced to an identity mapping. i.e **Glyph index = CID**. A CIDMap could be either a string or an array of strings or a dictionary. In this software, CIDMap is implemented as a string and these fonts are called as `Type 2 CIDfonts` with Type 42 as base font. In this  composite font (`Type 0`), the hierarchy involved is only one level which is the CIDfont itself having the capability of accessing 65535 glyphs! Bear in mind that the Postscript **show** operator will extract `two bytes` every time. This 16 bit value will be the `character code` that maps to `glyph index` through `CIDMap`.

Since number of glyphs exceed 256, conversion utility should be developed based on either 2nd approach or 3rd approach. 
- The 3rd approach is not only flexible over 2nd approach but also support for unicode is implementable.
- In the 3rd approach the hierarchy of composite fonts will never exceed 1 whereas in the case of 2nd approach the depth will be atleast 2 and number of Glyphs in the character set will decide the hierarchy's actual depth. i.e. `Depth of Hierarchy  = numGlyphs/256 + ((numGlyphs % 256) > 0)`. i.e. add 1 with quotient if remainder is non-zero.

This conversion utility has been developed based on `3rd approach`. 


##### Technical Features
- This utility is a console application developed on Microsoft Visual Studio Community 2022 (64-bit) Edition- Version 17.4.2 under Windows 10.
- This utility is available in [Linux platform](https://github.com/marmayogi/TTF2PostscriptCID-Linux) too.
- Note that this program is 100% portable across Windows and Linux. i.e. The source files (`main.ccp` and `ttf.h`) are same and identical across platforms.
- Ghostscript version is [10.0.0 (64 bit)](https://www.ghostscript.com/releases/gsdnld.html) and GSView version is [5.0 (64 bit)](http://ghostgum.com.au/software/gsview.htm).


##### References
This program has been developed based on the following documents.
1. [Microsoft OpenType® Specification Version 1.9](https://learn.microsoft.com/en-us/typography/opentype/spec/)
2. [Fonts - TrueType Reference Manual - Apple Developer](https://developer.apple.com/fonts/TrueType-Reference-Manual/)
3. [PostScript Language Reference, third edition - Adobe Corporation (912 pages PDF, 7410K) Feb/1999](https://www.adobe.com/jp/print/postscript/pdfs/PLRM.pdf)
4. [The Type 42 Font Format Specification #5012 (28 pages PDF, 159k) 31/Jul/1998](https://adobe-type-tools.github.io/font-tech-notes/pdfs/5012.Type42_Spec.pdf)
5. [Adobe CMap and CID Font Files Specification #5014 (102 pages PDF, 541k) 11/Jun/1996 Version 1.0](https://adobe-type-tools.github.io/font-tech-notes/pdfs/5014.CIDFont_Spec.pdf)

##### Usage
Create a folder `D:\cidfonts` in **D** drive (or any other drive) and store `ttf` file which should be convered. Now issue the following command:
- `ttf2postscriptcid.exe -d "D:\cidfonts\filename.ttf"`

If option **-d** is specified, then `ttf` table data will be displayed during execution.
This utility generates the following two files as output:
1. **filename.t42** is the required converted file with extension Type 42.
2. **filename.ps** is a postscript program file whose execution displays the glyphs present in the character set along with CIDs and Unicode Points. Since only around 12% glyphs of Indian Languages are alloted `Code Space` (unicode points), the Glyphs with no unicode point will have `none` printed.
3. Invoke Ghostscript to execute a postscript program in order to display glyphs as follows:
    - `gswin64c.exe "D:\cidfonts\filename.t42" "D:\cidfonts\filename.ps"`

##### Example
**Google** has developed a family of Tamil fonts called [Noto Sans Tamil](https://fonts.google.com/noto/specimen/Noto+Sans+Tamil).
In order to convert **NotoSansTamil-Regular.ttf** font to Type 42 CID-Keyed font, issue the following command:
- `ttf2postscriptcid.exe -d "D:\cidfonts\NotoSansTamil-Regular.ttf"`

Two files are generated by the command which are **NotoSansTamil-Regular.t42** and **NotoSansTamil-Regular.ps**. To view Glyphs for entire character set along with CIDs and Unicode Points, issue the following command:

- `gswin64c.exe "D:\cidfonts\NotoSansTamil-Regular.t42" "D:\cidfonts\NotoSansTamil-Regular.ps"`.

There are 534 `Tamil` and `Latin` Glyphs in the character set which will be displayed in 5 pages by Ghostscript.

#### Testing t42 Font.

In order to test **NotoSansTamil-Regular.t42** CID-Keyed font file, edit a file **tamil.ps** using nodepad in the folder `D:\cidfonts\` with the following postscript code:
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

This will display two strings `தமிழ் தங்களை வரவேற்கிறது!` and `Tamil Welcomes You!` respectively in subsequent rows.

Note that the strings for `show` operator are in Hexadecimal format embeded within angular brackets. Operator `show` extracts 2 bytes at a time and maps this CID (16 bit value) to a Glyph.
For example, the first 4 Hex digits in the 1st string is `0019` whose decimal equivalent is `25`. This maps to glyph `த`.

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

##### Apple's Truetype fonts vs Adobe's fonts.

- The first difference is that PostScript supports `cubic Bézier` curves, where each arc of each glyph is described by `four control` points. TrueType uses `quadratic splines` instead of `cubic`, with each arc having only `three control` points. This offers less control over the shape of the curve.
-  The second difference is the way they perform `hinting`. Since TrueType was originally targeted to low resolution screen rendering, its hinting system works by adjusting the curves to fit nicely on pixel lattice points, using a fairly elaborate `bytecode mechanism`. To edit the hints of a TrueType font, you must learn ttf's low-level programming language which is daunting task for the typical font designer. PostScript fonts were intended for higher resolution paper prints, and used guidelines to snap curves to right angles at appropriate places. For an Adobe font designer, these hints are pretty easy to understand and declare.

##### UTF-8 Support for t42 fonts.

For a Truetype font to be recognized by a Postscript interpreter, it must be enclosed in a Postscript font dictionary as a CID font with CIDFontType 2 and FontType 42. CIDMap maps CID(Character Indetitifer) to Glyph index and enforces Identity mapping as follows:
```
Character code 0 maps to Glyph index 0
Character code 1 maps to Glyph index 1
Character code 2 maps to Glyph index 2
......
......
Character code NumGlyphs-1 maps to Glyph index NumGlyphs-1
```
This `t42` fonts, converted font from `ttf`, are futile unless renders support to Unicode (UTF-8). 
This has been demonstrated by `Tamil.ps` postscript script in which hexadecimal strings in angular brackets are supplied as operands to postscript's `show` operator. Of course this hex string definitely must represent some UTF-8 encoded string which should have been supplied by an application software (written either in C, C++, Postscript or any other language) that uses the `t42` font.
To get an answer for this important question read the post [How to implement Unicode (UTF-8) support for a CID-keyed font (Adobe's Type 0 Composite font) converted from ttf?](https://stackoverflow.com/questions/74318928/how-to-implement-unicode-utf-8-support-for-a-cid-keyed-font-adobes-type-0-co).


