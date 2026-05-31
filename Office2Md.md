# Office2Md

This project intended for parsing and converting Open-XML: 

- DocumentFormat.OpenXml.Packaging.WordprocessingDocument
- DocumentFormat.OpenXml.Packaging.SpreadsheetDocument
- DocumentFormat.OpenXml.Packaging.PresentationDocument

And Pdf files.

to the:

- Markdown documents 
- 11ty-aware documents divided by Headings with build hierarchy automatically (for DocumentFormat.OpenXml.Packaging.WordprocessingDocument)

It means this script **should work with all types of Microsoft Office files** (docx|dotm|dotx xlsx|xlsm|xltx|xltm pptx|pptm|potx|potm|ppsx), but tested only on **.docx**, **.pptx**, **.xlsx**. It works with **.pdf** documents as well.

Conversion based on pure PowerShell code using [Open-XML-SDK](https://github.com/OfficeDev/Open-XML-SDK) ([Nuget package](https://www.nuget.org/packages/DocumentFormat.OpenXml/)) for Office documents. And [iTextSharp](https://www.nuget.org/packages/iTextSharp/) libraries for .pdf. Document conversion make without any additional components/programs like Pandoc for example. No any external API/REST API used. Conversion occurs locally on your computer/VM.

**You can use it for free.**

Office2Md.ps1 provides just a wrapper for classes. You can use classes in the way you want without this wrapper. See **Direct usage of Classes** below.

I'm using [Open-XML-SDK](https://github.com/OfficeDev/Open-XML-SDK) v2.9.1 for this solution and [iTextSharp](https://www.nuget.org/packages/iTextSharp/) v5.5.13.2. But, actually, you don't need to worry about it, because script installs all dependencies itself. In case of any error, you could download those dlls, put them into directory you want and correct path for loading libraries.

## Requirements

Required files:

- Office2Md.ps1
- Office2Md directory with its content:
  - O2mCommon.psm1
  - Office.ps1
  - Pdf.ps1 - *in case you want to convert .pdf documents*
- Office2Md.json

In case you'd like to rename main script `Office2Md.ps1`, you should rename the Directory and .json file according to a new name also.

Log file will have the same name as a main script (Office2Md at the moment) and .log extension. It will be place in the same directory as a main script (Office2Md).

## Assumptions

- Office documents will be represented as hierarchy of PowerShell Classes. It means it can be used in PowerShell 5.0 and above, PowerShell Core.
- Script has been tested on both PowerShell 5.1 & PowerShell Core v6+
- Script was tested on Windows OS only.

## Additionally

In my environment script works in PowerShell Core much faster than on PowerShell 5.1. I don't know why is it, but it is so.

## Features

- Converts **.docx** documents using basic & [Github](https://guides.github.com/features/mastering-markdown/) flawored markdown syntax
  
  - Images
    - in Text
    - in Tables
    - With Links
  - Strings
    - Color (by names & codes: "red", "#aabbcc")
    - Links
    - Bold
    - Italic
    - Underline
    - Strikethrough
    - Highlight
  - Tables
    - with alignment: 
      - right
      - left
      - center
      - none
  - Lists (in text & tables)
    - Ordered (multilevel)
    - Unordered (multilevel)
    - support alpha and roman list item tags. 
    - Unsupported symbols replacement (defined in config file)

- Settings for text color  
  - Color replacement

- Settings for Tables with Merged Cells. Their normalization to the Tables with equivale cells count in each row in order to be available to show as Markdown Table

- Converts **.xlsx** documents using basic & [Github](https://guides.github.com/features/mastering-markdown/) flawored markdown syntax
  
  - Tables with:
    - Automatic detect sheet dimension and delete empty cells from left and top
    - Show **Date** and **Currency** in the settings of culture of the computer you make convertion

- Converts **.pptx** documents using basic & [Github](https://guides.github.com/features/mastering-markdown/) flawored markdown syntax  
  - Titles
  - Text
  - Tables
  - Images (partially)
  
- Converts images from/to 9 format types on the fly. It's widely useful for standardize (reduce) images types to one type in Markdown document. Also some image types could be shown improperly in some software/tools. Supported image types: `bmp`, `gif`, `jpg`, `jpeg`, `ico`, `png`, `tif`, `tiff`, `emf`, `wmf`, `exif`. See **Configurstion** section for details.
- Resize all images in order to standardize their Width or Height by MaxWidth or MaxHeight
- Supports **Plugins** for `ImgItem` class (just for this class yet). See **Plugins** section in this document

- Project has PowerShell script `o2m.ps1` that provides GUI and run `Office2Md.ps1` script under the hood. It work on Windows OS only.

---

So supported **list** items (currently for **.docx** only) are:

- Numbers
- Alpha
- Roman

of any nesting.

---

Examples:

1. Ordered ListItem 1
2. Ordered ListItem 2
   1. Ordered SubItem 1
      a. Alpha SubItem (a)
      b. Alpha subitem (b)
        i. Roman Subitem (i)
        ii. Romain subitem (ii)
- Unordered ListItem 1
- Unordered ListItem 2
  - Unordered subitem 1
  - Unordered subitem 2
    - Unordered sub-subitem 1

---

In OpenXml format list nesting level restricted to: **8**.

## Restrictions

- It is impossible to build a table with merged cells using Markdown syntax. Script analyze table for such things and can automatically normalize it or didn't convert it to markdown one. Instead of conversion, script will put the warning message in red color in the place where table would be. You can drive it by settings in configuration file. See configuration file description.
- Script doesn't work with Shapes, Word Art and anything like that. Only images supported (for **.docx** and **.pptx** files).

## Usage

Office2Md.ps1 script can convert multiple files at once. Also it supports the conversion multiple files into a single Markdown document.

```
.\Office2Md.ps1 [OfficeFilePaths] [OutputDirectory] [-GithubFlawor] [-IntoSingleOne]
```

or

```
.\Office2Md.ps1 -DocPath [OfficeFilePaths] -MdDir [OutputDirectory] [-GithubFlawor] [-IntoSingleOne]
```

- In case of usage without `-IntoSingleOne` option:
  - In [Output Directory] .md file with the same name as each [Office File] will be created and all images will be place into /images directory under the [OutputDirectory].
- In case of usage with `-IntoSingleOne` option:
  - In the [OutputDirectory] will be created a single file with name as first file in [OfficeFilePaths] and all images will be place into /images directory under the [OutputDirectory].

Examples:

```
.\Office2Md.ps1 c:\Documents\1.docx c:\Markdown
```

```
.\Office2Md.ps1 c:\Documents\1.docx,c:\Documents\my.xlsx,c:\Documents\any.pptx,c:\Documents\another.pptx,C:\Pdf\my.pdf c:\Markdown
```

```
.\Office2Md.ps1 c:\Documents\1.docx,c:\Documents\my.xlsx,c:\Documents\any.pptx,c:\Documents\another.pptx,C:\Pdf\my.pdf c:\Markdown -IntoSingleOne
```

## Direct usage of Classes

As I sayed previously, `Office2Md.ps1` just a wrapper for the PowerShell classes, it intended to hide everything from the customer in order to simplify conversion itself. It's ready-to-use solution with command line support for documents conversion.

But if you'd like to include document conversion ability to your own solution, you able to use just Classes.ps1 script that contains a classes for providing conversion. Also you could like to save document data to the disk for future usage (this is much faster than convert document again and again for example). For doing that you might have to know how how it's organized internally a bit.

### Classes hierarchy

Below you can see the classes hierarchy in order to clearly understand how it works.
And... it's pretty simple. :)

```
[BaseItem]
       |
       +------- [DocumentItem]
       |
       +------- [StringItem]
       |
       +------- [ImgItem]
       |
       +------- [ParagraphItem]
       |
       +------- [TableItem]
       |
       +------- [RowItem]
       |
       +------- [CellItem]

[PdfItem]       
```

As I told you ;)

### Call methods

You can use a provided classes in any way you want. In case you'd like' to create your own script using this classes, you must to remember, there are several <u>methods</u> you must to know and use:

| Method                                                                           | Explanation                                                                                                                                                      |
| -------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `[DocumentItem]::new($Path, $MdDir, $cfgFile)`                                   | Creates new instance of single .docx, .xlsx or .pptx document.                                                                                                   |
| `[PdfItem]::new($Path, $MdDir, $cfgFile)`                                        | Creates new instance of single .pdf document.                                                                                                                    |
| `[void] ProcessDocument();`                                                      | Converts .docx, .xlsx or .pptx document into internal one                                                                                                        |
| `[void] CloseDocument();`                                                        | Closes a source .docx, .xlsx or .pptx document                                                                                                                   |
| `[void] SaveToText([string] $FileName)`                                          | Saves .docx, .xlsx or .pptx to the text file. It is a totally silly method and implemented just for fun.                                                         |
| `[void] SaveToMarkdown([string] $FileName, [bool] $GithubFlawor)`                | Saves .docx, .xlsx or .pptx to markdown format                                                                                                                   |
| `[void] SaveToMarkdownFor11ty([string] $DirName, [bool] $GithubFlawor)`          | Save .docx, .xlsx or .pptx to markdown with splitting the document into Headings with creation hierarchy of documents like Content in Word with 11ty header each |
| `[void] SaveTo([string] $Format, [string] $FileOrDirName, [bool] $GithubFlawor)` | All in one. Saves in the 3 previous ways but the desired type passed in the **Format** variable. Available values are: **Text**, **Markdown**, **11ty**          |
| `[string] ToMarkdown()`                                                          | Converts processed document to Markdown and returns string contains Markdown document                                                                            |

$GithubFlawor - if true allows to use those text decorations:

- Strikethrough
- Underline
- Highlight

It is simple to realize additional feature for Githubflawor, but there is no clear comparison them to the **.docx**, **.xlxs** and **.pptx** elements.

If you'd like to use classes without O2mCommon.psm1 module, you must to know that classes use `toLog()` function from O2mCommon.psm1 extensively. You have to define it inside your .ps1 script or module.

Examples of methods usage:

```
$doc = [DocumentItem]::new($DocxPath, $MdDir, $cfgFile)
$doc.ProcessDocument();
$doc.CloseDocument();
$doc.SaveToText((Join-Path $DacDir '1.txt'))
$doc.SaveToMarkdown((Join-Path $DacDir '2.md'))
$doc.SaveToMarkdownFor11ty((Join-Path $DacDir '11ty'), $true)
```

```
$doc = [PdfItem]::new($PdfPath, $MdDir, $cfgFile)
$doc.ProcessDocument();
$doc.CloseDocument();
$doc.SaveToMarkdown((Join-Path $DacDir '2.md'))
```

Object's methods to save to Xml, for example, did not provided because they are don't needed. You can use pure PowerShell commandlets to do save/restore objects.

As you can see, you work just with `[DocumentItem]` or `[PdfItem]` objects. You don't have to know how all of that organized internally and interact between each over. Main methods are the same for `[DocumentItem]` and `[PdfItem]`.

In order to change object's behavior you'd just to inherit the objects and write the code for your needs only.

## Plugins

At the moment, plugins supported just for `[ImgItem]` class. All other classes will be support plugin also soon.
Plugins must be located under `Office2Md\Plugins` folder. Each plugin must have its own folder with unique name. In this folder there must be a `.psm1` module and any count of other files/folders/subfolders.

There are some requirements for the content of `.psm1` module:

- It is required that all functions there, those must be exposed from the module, start with the class name to which they will belong. At the moment: `ImgItem`.
- There are 4 types of plugin functions available for `ImgItem` and all of them are for `ConvertAndSave` method only. Functions names are: 
  - `ImgItem-BeforeConvertAndSave` - calling before any actions in this method. Parameters are:
    - [DocumentFormat.OpenXml.Packaging.ImagePart]$img
    - [System.IO.Stream]$stream
    - [string]$savePath
    - [object]$this.Document.Cfg.Image - points to the configuration part "Image"
  - `ImgItem-ReplaceConvertAndSave` - calling instead of the body of the default method `ConvertAndSave` of `ImgItem` class. But `ImgItem-BeforeConvertAndSave` and `ImgItem-AfterConvertAndSave` will be called if defined. Parameters are:
    - [DocumentFormat.OpenXml.Packaging.ImagePart]$img
    - [System.IO.Stream]$stream
    - [string]$savePath
    - [object]$this.Document.Cfg.Image - points to the configuration part "Image"
  - `ImgItem-ResizeImage` - calling instead of default resize code inside `ConvertAndSave` method of the `ImgItem` class. Won't be called if you defined `ImgItem-ReplaceConvertAndSave` because `ImgItem-ReplaceConvertAndSave` overload this code. Parameters are:
    - [System.IO.Stream]$stream
    - [object]$this.Document.Cfg.Image - points to the configuration part "Image"
  - `ImgItem-AfterConvertAndSave` - calling after any actions in this method. Parameters are:
    - [DocumentFormat.OpenXml.Packaging.ImagePart]$img
    - [System.IO.Stream]$stream
    - [string]$savePath
    - [object]$this.Document.Cfg.Image - points to the configuration part "Image"

You are not obliged to define all of the functions above. Just those you want to.

See `Office2Md\Plugins\ImageResize` for example.


## Configuration file description

| Key         | Subkey         | SubSubkey     | Type                   | Available values                                                                                          | Examples                                   | Meaning                                                                                                                                                                                                                                                                                                                                                                                                                                |
| ----------- | -------------- | ------------- | ---------------------- | --------------------------------------------------------------------------------------------------------- | ------------------------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Toc         |                |               | Object                 |                                                                                                           |                                            | Styles those will be considered as **Table of Content**                                                                                                                                                                                                                                                                                                                                                                                |
|             | TocHeading     |               | Array of strings       | Any strings                                                                                               | \["tocH1", "tocH2", "tocH3"\]              | Don't using at the moment                                                                                                                                                                                                                                                                                                                                                                                                              |
|             | Toc1           |               | Array of strings       | Any strings                                                                                               | \["toc1", "toc2", "toc3"\]                 | Don't using at the moment                                                                                                                                                                                                                                                                                                                                                                                                              |
|             | Toc2           |               | Array of strings       | Any strings                                                                                               | \["toc1", "toc2", "toc3"\]                 | Don't using at the moment                                                                                                                                                                                                                                                                                                                                                                                                              |
|             | Toc3           |               | Array of strings       | Any strings                                                                                               | \["toc1", "toc2", "toc3"\]                 | Don't using at the moment                                                                                                                                                                                                                                                                                                                                                                                                              |
|             | Toc4           |               | Array of strings       | Any strings                                                                                               | \["toc1", "toc2", "toc3"\]                 | Don't using at the moment                                                                                                                                                                                                                                                                                                                                                                                                              |
| Head        |                |               | Object                 |                                                                                                           |                                            | Styles those will be considered as **Headings**                                                                                                                                                                                                                                                                                                                                                                                        |
|             | Head1          |               | Array of strings       | Any strings                                                                                               | \["head1", "otherHead", "overhead"\]       |                                                                                                                                                                                                                                                                                                                                                                                                                                        |
|             | Head2          |               | Array of strings       | Any strings                                                                                               | \["head1", "otherHead", "overhead"\]       |                                                                                                                                                                                                                                                                                                                                                                                                                                        |
|             | Head3          |               | Array of strings       | Any strings                                                                                               | \["head1", "otherHead", "overhead"\]       |                                                                                                                                                                                                                                                                                                                                                                                                                                        |
|             | Head4          |               | Array of strings       | Any strings                                                                                               | \["head1", "otherHead", "overhead"\]       |                                                                                                                                                                                                                                                                                                                                                                                                                                        |
|             | Head5          |               | Array of strings       | Any strings                                                                                               | \["head1", "otherHead", "overhead"\]       |                                                                                                                                                                                                                                                                                                                                                                                                                                        |
|             | Head6          |               | Array of strings       | Any strings                                                                                               | \["head1", "otherHead", "overhead"\]       |                                                                                                                                                                                                                                                                                                                                                                                                                                        |
| List        |                |               | Array of strings       | Any strings                                                                                               | \["ListParagraph", "Bullet1", null\]       | Styles those will be considered as **Lists**                                                                                                                                                                                                                                                                                                                                                                                           |
| Common      |                |               | Array of strings       | Any strings                                                                                               | \["Normal", "Caption", "Hyperlynk", null\] | Styles those will be considered as **Common Text** or **Links**                                                                                                                                                                                                                                                                                                                                                                        |
| Image       |                |               |                        |                                                                                                           |                                            | Options for working with Images                                                                                                                                                                                                                                                                                                                                                                                                        |
|             | Prefix         |               | String                 | Any strings                                                                                               |                                            | Prefix for every image that will be put before /images/ImageName\.ext\. Very useful for creating content for 11ty for example                                                                                                                                                                                                                                                                                                          |
|             | UsePrefix      |               | Boolean                | true, false                                                                                               |                                            | Use the prefix above or not                                                                                                                                                                                                                                                                                                                                                                                                            |
|             | Embed          |               | Boolean                | true, false                                                                                               |                                            | Embed images directly to the Markdown document in order to have a single document without links to the images it contains                                                                                                                                                                                                                                                                                                              |
|             | Convert        |               | Object                 |                                                                                                           |                                            | Allow to convert images from one format to another                                                                                                                                                                                                                                                                                                                                                                                     |
|             |                | UseConversion | Boolean                |                                                                                                           |                                            | If **true**, then script will try to convert all images according Replace rules\. If there is no such extension defined in rules, then image will be saved as\-is with its default extension                                                                                                                                                                                                                                           |
|             |                | Replace       | Array of objects       |                                                                                                           |                                            | Contains simple from \- to pairs\. And allows to convert images with extension "From" to type with extension "To"\. Example: \`"Replace" : \[\{"From" : "\.emf", "To" : "\.jpg"\},<br><br>\{"From" : "\.wmf", "To" : "\.jpg"\}\]\`\. You can use the next extensions: \`bmp\`, \`gif\`, \`jpg\`,\` jpeg\`, \`ico\`, \`png\`, \`tif\`, \`tiff\`, \`emf\`, \`wmf\`, \`exif\` with or without leading "\."\. All others will be ignored\. |
|             |                | Resize        |                        | Object                                                                                                    |                                            | Allows to resize images in order to make them the same by Width or Height                                                                                                                                                                                                                                                                                                                                                              |
|             |                |               | UseResize              | Boolean                                                                                                   |                                            | If **True** all images will be resized according to the rules                                                                                                                                                                                                                                                                                                                                                                          |
|             |                |               | MaxWidth               | Unsigned int                                                                                              |                                            | If above the 0, then Width of all images will be resized to this value\. **MaxWith** have apriority on **MaxHeight**\.                                                                                                                                                                                                                                                                                                                 |
|             |                |               | MaxHeight              | Unsigned int                                                                                              |                                            | If above the 0, then Height of all images will be resized to this value\. **MaxWith** have apriority on **MaxHeight**\.                                                                                                                                                                                                                                                                                                                |
|             |                |               | DoNotResizeLessThanMax | Boolean                                                                                                   |                                            | If **true**, then all images those have Width or Height less than MaxHeight or MaxWidth won't be resized                                                                                                                                                                                                                                                                                                                               |
|             |                |               | InterpolationMode      | String                                                                                                    |                                            | Resize algorythm\. Can be one of:**HighQualityBicubicorHighQualityBilinear**                                                                                                                                                                                                                                                                                                                                                           |
| ReplaceThis |                |               | Array of objects       |                                                                                                           |                                            |                                                                                                                                                                                                                                                                                                                                                                                                                                        |
|             | From           |               | String                 | Any string \(including Unicode strings or their definition for PowerShell\)                               | Kb\.                                       | \{"From" : "Kb\.", "To" : "Kilobytes"\}                                                                                                                                                                                                                                                                                                                                                                                                |
|             | To             |               | String                 | Any string                                                                                                | Kilobytes                                  | \{"From" : "\u2122", "To" : "&trade;"\}                                                                                                                                                                                                                                                                                                                                                                                                |
| Table       |                |               |                        |                                                                                                           |                                            | Options for working with tables                                                                                                                                                                                                                                                                                                                                                                                                        |
|             | Standardize    |               | Boolean                | true, false                                                                                               |                                            | If true, checks and makes \(if required\) tables with different cells count in rows to the same count in row\. It is required for table can be represented as Markdown                                                                                                                                                                                                                                                                 |
| Colors      |                |               |                        |                                                                                                           |                                            | Options for working with colors of text                                                                                                                                                                                                                                                                                                                                                                                                |
|             | UseColors      |               | Boolean                | true, false                                                                                               |                                            | Use ability to color the text or left it uncolored                                                                                                                                                                                                                                                                                                                                                                                     |
|             | Replace        |               | Object                 |                                                                                                           | \{"From" : "\#FFFFFF", "To" : "\#000000"\} | Replaces one color definition to another one\. Useful when color of text set as **white** or **\#FFFFFF**                                                                                                                                                                                                                                                                                                                              |
|             |                | From          | String                 | Any string representing a color \(their name or RGB\)                                                     | \{"From" : "\#FFFFFF", "To" : "\#000000"\} |                                                                                                                                                                                                                                                                                                                                                                                                                                        |
|             |                | To            | String                 | Any string representing a color \(their name or RGB\)                                                     | \{"From" : "white", "To" : "black"\}       |                                                                                                                                                                                                                                                                                                                                                                                                                                        |
| Workbook    |                |               | Object                 |                                                                                                           |                                            | Options available for the Excel Workbooks \(\.xlsx files\)                                                                                                                                                                                                                                                                                                                                                                             |
|             | SheetHeading   |               | Object                 |                                                                                                           |                                            | Object for combining purposes related to the Tab names definition                                                                                                                                                                                                                                                                                                                                                                      |
|             |                | UseTabName    | Boolean                | true, false                                                                                               |                                            | Use the tab name as a prefix for the tab content or not                                                                                                                                                                                                                                                                                                                                                                                |
|             |                | Template      | String                 | Any string\. Use special placeholder \{TabName\}\. It will be replaced by the name of a current Tab       | "\#\#\# \{TabName\}"                       | Will be used if **UseTabName** is set to **true**                                                                                                                                                                                                                                                                                                                                                                                      |
| Pdf         |                |               | Object                 |                                                                                                           |                                            | Options we have to convert the PDF documents                                                                                                                                                                                                                                                                                                                                                                                           |
|             | UsePageNumbers |               | Boolean                | true, false                                                                                               |                                            | Put the page number to the resulting Markdown                                                                                                                                                                                                                                                                                                                                                                                          |
|             | Position       |               | string                 | "header" or "footer"                                                                                      | "footer"                                   | Case insensitive                                                                                                                                                                                                                                                                                                                                                                                                                       |
|             | Template       |               |                        | Any string\. Use special placeholder \{PageNumber\}\. It will be replaced by the number of a current page | "\#\#\#\# \{PageNumber\}"                  | Will be used if **UsePageNumber** is set to **true**                                                                                                                                                                                                                                                                                                                                                                                   |

## Known bugs/flaws/issues

- Complex/Distributed Lists could be shown improperly (I don't know how to separate one list to another one using OpenDoc format. Algorithm I've implemented is not work properly in some cases)
- Table of Content didn't implemented
- For **.pptx**, **.xlsx** and **.pdf** files didn't implemented any styles, colors etc, I mean there is no any decoration for those types of files
- On some environments (depends on Windows and/or PowerShell configuration) `Office2Md.ps1` script can't automatically download and install required packages [OpenXml](https://www.nuget.org/packages/DocumentFormat.OpenXml/) and [iTextSharp](https://www.nuget.org/packages/iTextSharp/). In this case You can do it manually by choosing one of two ways below:
  - Install them by [Install-Package](https://docs.microsoft.com/en-us/powershell/module/packagemanagement/install-package?view=powershell-7.1) CommandLet. For example:
    - `Install-Package iTextSharp -MinimumVersion 5.5.13.2`
    - `Install-Package DocumentFormat.OpenXml -MinimumVersion 2.9.1`
  - Download `.nuget` packages from the URLs above and unzip them into: `C:\Users\<LoginName>\AppData\Local\PackageManagement\Nuget\Packages` to the personal folder for each. For example into: `C:\Users\<LoginName>\AppData\Local\PackageManagement\NuGet\Packages\iTextSharp.5.5.13.2` and `C:\Users\<LoginName>\AppData\Local\PackageManagement\NuGet\Packages\Open-XML-SDK.2.9.1`

## ToDo

- Pipeline (Best Practices compliance, Digital signature, Tests)
- Versioning
- Releases
- Decorations for **.pptx**, **.xlsx** and **.pdf** files
- ...

## Author

Contact: 

- Email: [Alexander Ivanov](mailto:Alexander.Ivan0v@outlook.com)
- Skype **VeryComplexName**

## Example

In the directory **TestOffice** in the repository you can find a **Text.docx**, **Test1.pptx**, **Test2.xlsx** and **Test3.pdf** documents with a different styles, list, tables and they overlapping.
Below you can see the conversion result (you could compare result with original **Text.docx**, **Test1.pptx**, **Test2.xlsx** and **Test3.pdf** in the repository). Gitlab doesn't support many of extended features, but you could try to see it in Markdown editors/viewers ([Mark Text](https://marktext.app/)) for example or using Github or in an amount of plugins for Web browsers.
In **TestOffice** directory you can also find the resultant document **Test.md**.

Below the convertion result of 3 Office documents **Text.docx**, **Test1.pptx**, **Test2.xlsx** and **Test3.pdf** for conversion `-IntoSingleOne` option has been used. So, all of mentioned files were merged into a single one in order they are provided in the command line for script. Command for get this result you can find in `TestOffice/IntoSingle.cmd`

Below you can see how it looks in Gitlab. *Remember: Gitlab doesn't use a full power of Markdown documents. Use separate programs or browser plugins for see all the things available*. You can see resultant file **Test.md** in the **TestOffice** folder also.

---

This is simple string

![LogoDescription automatically generated](TestOffice/images/db96b366-8ac8-4eb4-bc23-0f13ca72f08e.png)

This is complex string with **Bold text**, *Italic text*, ***Bold and Italic text***, ~~Strike through text~~, <u>Underline text</u> and <span style="color:#FF0000">Colored text</span>\.

<u>Below is Unordered list</u> <u>\(and this line is underscored\)</u><u>:</u>

- **Bold** and ~~Strikethrough~~ text
- *Italic* <span style="color:#538135">Colored</span> and <u>*Underline*</u> <u>*Italic*</u>
  - <span style="color:#C45911">**Different color and Bold**</span>

<u>Below</u> <u>O</u><u>rdered list \(and this line is underscored\)</u><u>:</u>

1. <span style="color:#FF0000">**Bold Red**</span>
2. ~~*Italic and Strikethrough*~~
   b. Simple text

<u>Below ordered list tagged by Roman symbols \(and this line is underscored\):</u>

i. Roman1
ii. <span style="color:#00B050">Roman2 Colored</span>

<u>Below ordered list tagged by Alpha symbols \(and this line is underscored\):</u>

b. <span style="color:#C00000">*Alpha Colored and Italic*</span>
c. Alpha2 Common

| Left aligned column                                                                                                                                                                      | Centered column                                                                                                                                                  | Right aligned column                                                                                   |
| ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |:----------------------------------------------------------------------------------------------------------------------------------------------------------------:| ------------------------------------------------------------------------------------------------------:|
| <br>- <span style="color:#2F5496">Colored</span><br>- <span style="color:#2F5496">*Same color and Italic*</span><br>- Common **Bold** <span style="color:#BF8F00">Different color</span> | **Bold**                                                                                                                                                         | 77676767                                                                                               |
| <u>~~***Bold Italic Underline***~~</u>                                                                                                                                                   | <mark>Common</mark> <mark>Highlighted</mark>below you can see the image with link                                                                                | <br>1. Common<br>2. <span style="color:#C9C9C9">*Grayed text*</span><br>3. ***Italic***                |
| ![ShapeDescription automatically generated with low confidence](TestOffice/images/e6b50829-5484-4663-ac60-6074ad16a8f9.png)                                                              | [![IconDescription automatically generated](TestOffice/images/373f4a1e-5e04-4d97-9719-7f8ffd22365d.png)](https://guides.github.com/features/mastering-markdown/) | ![IconDescription automatically generated](TestOffice/images/72dd52e5-dd1b-4974-88c6-5600b03d456f.png) |

-----

This is a title

| 1   | 2    | 3   |
| --- | ---- | --- |
| dfd | dfdf | dfd |
| dfd | dfd  | dfd |

| www | eee | rrr | ttt |
| --- | --- | --- | --- |
|     |     |     |     |

Textbox

![](TestOffice/images/bcbf6993-8815-4d33-abb4-d2f95f8db23d.png)

![IconDescription automatically generated](TestOffice/images/86fdf7cd-6aa0-4859-a566-32a6c60acc3a.png)

Second slide title

| Title Left aligned                                                                            | Title Center aligned | Title Right Aligned                             |
| --------------------------------------------------------------------------------------------- | -------------------- | ----------------------------------------------- |
| qqq                                                                                           | rrrrrrr              | 66666666666                                     |
| W w w wwww www<br><br>Dfdf d fddf dfd<br><br>Dfdf df dfd<br><br>Gfgfgfgfgf<br><br>fgfgfgfgfgf | rrrrrrrrrrrrrrrrr    | Ggggg<br><br>Gfgfgffgfg<br><br>fgfhvnghjhjggggh |

![](TestOffice/images/49759c5a-4f3b-4e8a-afd1-ec2975cce4a2.png)

-----

### Sheet3

|      |              |     | 1   | 2   | 3   | 4   | 5   |
| ---- | ------------ | --- | --- | --- | --- | --- | --- |
|      |              |     | 6   | 7   | 8   | 9   | 10  |
|      |              |     | 11  | 12  | 13  | 14  | 15  |
|      |              |     |     |     |     |     |     |
| 4,35 |              |     |     |     |     |     |     |
|      |              |     |     |     |     |     |     |
|      | www          |     |     |     |     |     |     |
|      |              |     |     |     |     |     |     |
|      |              |     |     |     |     |     |     |
|      | 22\.11\.2021 |     |     |     |     |     |     |

### Sheet2

| 22  |              |       |
| --- | ------------ | ----- |
|     | 22\.11\.2021 |       |
|     |              | 44\.6 |

### Sheet1

| 12\.11\.2021 | 2          | 3   |
| ------------ | ---------- | --- |
| 4,00         | FALSE      | 6   |
| 789\.6789    | 394\.83945 | 9   |

-----

Using the LIFEBOOK Application Panel
Launching Applications with the LIFEBOOK Application Panel
The application panel enables you to launch applications with the touch of a button when your system is on. Pressing 
any of the buttons will launch a user-defined application. Your notebook is pre-installed with software utilities that 
let you operate and configure your LIFEBOOK Application Panel.
Configuring your LIFEBOOK Application Panel  
When you start Windows, the LIFEBOOK Application Panel is automatically activated. As an application launcher, the 
LIFEBOOK Application Panel is very flexible, giving you a variety of options. To set up the panel to best suit your needs, 
we have provided the Application Panel Setup utility that quickly and easily helps you make the most of this valuable 
feature.
To configure your LIFEBOOK Application Panel with Application Panel Setup:  
1 Right-click on the Start icon. Click All Apps.
2 Click on Setup of LIFEBOOK Application Panel. The Button Setting utility will appear. The tabs that appear in the 
utility window correspond to the application buttons on the LIFEBOOK Application Panel. When you receive your 
notebook, these buttons are pre-configured to launch specific programs.
To change an application associated with the Application buttons:  
1 Click on the tab for the button you would like to reconfigure – for example, Application A. 
2 Click on the Enable this button check box if it is unchecked, then click on the [Application Registration] button. 
3 Click the Start Program check box, then click [Next]
4 On the screen that appears, click Select from Apps and scroll through the list to select the application you want 
to start with the Application A button.
5 Click [Next] -> [Next] -> [Finish]. On the Button Setting window, click [Apply], then click [OK]. The button will now 
launch the new application.
1 - 

#### 1

The Internet tab is a little bit different. It comes set to launch your default Internet browser (Internet Explorer), unless 
you have changed it in Windows. To reconfigure it to launch another program, follow these steps:
1 Click on the Internet tab. 
2 Click on the Enable this button check box if it is unchecked, then click on the [Application Registration] button. 
3 Click the Start program check box, then click [Next].
4 On the screen that appears, click the Select from Apps check box and scroll through the list to select the 
application you want to start with the Internet button.
5 Click [Next] -> [Next] -> [Finish]. On the Button Setting window, click [Apply], then click [OK]. 
The button will now launch the new application. If you want to return to launching your Windows default Internet 
browser with this button, click on the Start Browser check box instead of the Start program check box. Be aware that 
you will erase the settings for the “other application”. If you wish to go back to launching the “other application” from 
this button, you will need to reconfigure it as described above.
When you have finished with button settings, click [OK], and the new settings will take effect. You can reconfigure 
your LIFEBOOK Application Panel as often as you like. 
IF ONE OF THE APPLICATION LAUNCHER BUTTONS IS IDENTIFIED AS AN INTERNET LAUNCHER, THE BUTTON CAN STILL BE CONFIGURED TO LAUNCH 
ANY APPLICATION YOU WISH; IT IS NOT LIMITED TO BEING AN INTERNET BROWSER.
2 - 

#### 2

-----
