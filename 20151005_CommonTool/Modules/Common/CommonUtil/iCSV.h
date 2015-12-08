

class iCSV
{
public:
    /************************************************************************/
    // GetCSVLineByKey
    // - Parameter:
    //   - szFilename: INPUT,  Filename.
    //   - szKey     : INPUT,  Keyword to search.
    //   - iKeyIndex : INPUT,  Keyword column to search.
    //   - szLine    : OUTPUT, Search Result, return entire row.
    // - Return:
    //   - true      : Find the target.
    //   - false     : Can't find the target.
    /************************************************************************/
    bool GetCSVLineByKey (const TCHAR* szFilename, const TCHAR* szKey, const int iKeyIndex, TCHAR* szLine) ;

    /************************************************************************/
    // GetCSVItemByKey
    // - Parameter:
    //   - szFilename: INPUT,  Filename.
    //   - szKey     : INPUT,  Keyword to search.
    //   - iKeyIndex : INPUT,  Keyword column to search.
    //   - szResult  : OUTPUT, Search Result.
    //   - iTargetKey: INPUT,  Setup which column to return.
    // - Return:
    //   - true      : Find the target.
    //   - false     : Can't find the target.
    /************************************************************************/
    bool GetCSVItemByKey (const TCHAR* szFilename, const TCHAR* szKey, const int iKeyIndex, TCHAR* szResult, const int iTargetKey) ;
} ;
