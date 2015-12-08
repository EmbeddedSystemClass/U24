// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// DocObjectProperty wrapper class
#if !defined(LPP2_H)
#define LPP2_H


class DocObjectProperty : public COleDispatchDriver
{
public:
	DocObjectProperty() {}		// Calls COleDispatchDriver default constructor
	DocObjectProperty(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DocObjectProperty(const DocObjectProperty& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CString Get_Name();
	void Set_Name(LPCTSTR);
	long GetDataType();
	void SetDataType(long);
	VARIANT GetValue();
	void SetValue(const VARIANT&);
	long GetId();
	void SetId(long);
	CString GetEntries();
	void SetEntries(LPCTSTR);
	BOOL GetLocked();
	void SetLocked(BOOL);
	VARIANT GetTag();
	void SetTag(const VARIANT&);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// IApplication wrapper class

class IApplication : public COleDispatchDriver
{
public:
	IApplication() {}		// Calls COleDispatchDriver default constructor
	IApplication(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IApplication(const IApplication& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	BOOL GetVisible();
	void SetVisible(BOOL);
	BOOL GetUserControl();
	void SetUserControl(BOOL);
	LPDISPATCH GetDocuments();
	void SetDocuments(LPDISPATCH);
	LPDISPATCH GetDialogs();
	void SetDialogs(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	CString GetCaption();
	void SetCaption(LPCTSTR);
	CString GetDefaultFilePath();
	void SetDefaultFilePath(LPCTSTR);
	CString GetFullName();
	void SetFullName(LPCTSTR);
	CString GetName();
	void SetName(LPCTSTR);
	CString Get_Name();
	void Set_Name(LPCTSTR);
	CString GetPath();
	void SetPath(LPCTSTR);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetOptions();
	void SetOptions(LPDISPATCH);
	CString GetVersion();
	void SetVersion(LPCTSTR);
	long GetWidth();
	void SetWidth(long);
	long GetHeight();
	void SetHeight(long);
	long GetLeft();
	void SetLeft(long);
	long GetTop();
	void SetTop(long);
	LPDISPATCH GetActiveDocument();
	void SetActiveDocument(LPDISPATCH);
	BOOL GetLocked();
	void SetLocked(BOOL);
	LPDISPATCH GetRecentFiles();
	void SetRecentFiles(LPDISPATCH);
	BOOL GetEnableEvents();
	void SetEnableEvents(BOOL);
	CString GetActivePrinterName();
	void SetActivePrinterName(LPCTSTR);
	LPDISPATCH GetSharedOwner();
	void SetSharedOwner(LPDISPATCH);
	long GetMainWndHandle();
	void SetMainWndHandle(long);
	BOOL GetIsEval();
	void SetIsEval(BOOL);
	long GetPid();
	void SetPid(long);
	long GetProgramCode();
	void SetProgramCode(long);
	long GetPrintMode();
	void SetPrintMode(long);

// Operations
public:
	void Quit();
	LPDISPATCH PrinterSystem();
	void Resize(long Width, long Height);
	void Move(long Left, long Top);
	void ShowHelp(LPCTSTR strHelpFile, long HelpContextID);
	short GetLastError();
	CString ErrorMessage(short ErrorCode);
};
/////////////////////////////////////////////////////////////////////////////
// Documents wrapper class

class Documents : public COleDispatchDriver
{
public:
	Documents() {}		// Calls COleDispatchDriver default constructor
	Documents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Documents(const Documents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);
	CString GetDefaultExt();
	void SetDefaultExt(LPCTSTR);

// Operations
public:
	LPDISPATCH Add(LPCTSTR Key);
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Open(LPCTSTR strDocName, BOOL ReadOnly);
	void SaveAll(BOOL AlwaysPrompt);
	void CloseAll(BOOL Save);
	LPDISPATCH Import(LPCTSTR strDocName, LPCTSTR strFileExt);
};
/////////////////////////////////////////////////////////////////////////////
// IDocument wrapper class

class IDocument : public COleDispatchDriver
{
public:
	IDocument() {}		// Calls COleDispatchDriver default constructor
	IDocument(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDocument(const IDocument& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetVariables();
	void SetVariables(LPDISPATCH);
	CString GetName();
	void SetName(LPCTSTR);
	CString Get_Name();
	void Set_Name(LPCTSTR);
	LPDISPATCH GetDocObjects();
	void SetDocObjects(LPDISPATCH);
	LPDISPATCH GetPrinter();
	void SetPrinter(LPDISPATCH);
	LPDISPATCH GetFormat();
	void SetFormat(LPDISPATCH);
	LPDISPATCH GetDatabase();
	void SetDatabase(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	CString GetFullName();
	void SetFullName(LPCTSTR);
	BOOL GetReadOnly();
	void SetReadOnly(BOOL);
	long GetViewMode();
	void SetViewMode(long);
	long GetWindowState();
	void SetWindowState(long);
	LPDISPATCH GetBuiltInDocumentProperties();
	void SetBuiltInDocumentProperties(LPDISPATCH);
	long GetTriggerForm();
	void SetTriggerForm(long);
	long GetViewOrientation();
	void SetViewOrientation(long);
	BOOL GetIsModified();
	void SetIsModified(BOOL);
	LPDISPATCH GetSelectedDocObjects();
	void SetSelectedDocObjects(LPDISPATCH);
	BOOL GetAutoPageIncrement();
	void SetAutoPageIncrement(BOOL);
	long GetHorzPrintOffset();
	void SetHorzPrintOffset(long);
	long GetVertPrintOffset();
	void SetVertPrintOffset(long);
	CString GetRFTagList();
	void SetRFTagList(LPCTSTR);

// Operations
public:
	short Save();
	short SaveAs(LPCTSTR strPathName);
	short PrintDocument(long Quantity);
	short PrintLabel(long Quantity, long LabelCopy, long InterCut, long PageCopy, long NoFrom, LPCTSTR FileName);
	short GeneratePOF(LPCTSTR DestFileName, LPCTSTR ModeleFileName);
	short Merge(long Quantity, long LabelCopy, long InterCut, long PageCopy, long NoFrom, LPCTSTR FileName);
	short Insert(LPCTSTR strPathName);
	BOOL CopyToClipboard();
	short FormFeed();
	void Close(BOOL Save);
	void Activate();
	CString CopyImageToFile(short Colors, LPCTSTR Extension, short Rotation, short Percent, LPCTSTR FileName);
	short WriteVariables(const VARIANT& varArray);
	VARIANT ReadVariables(long iType);
	void Passwords(LPCTSTR strOwnerPassword, LPCTSTR strPrintPassword);
	BOOL NewRFTag(long IDRfTag, long Reserved);
	LPDISPATCH RFTag(long Reserved);
	BOOL SelectDatabase(LPCTSTR strName);
	BOOL CopyToClipboardEx(BOOL Border, short nMethod, short Colors, short Rotation, short Percent);
};
/////////////////////////////////////////////////////////////////////////////
// Variables wrapper class

class Variables : public COleDispatchDriver
{
public:
	Variables() {}		// Calls COleDispatchDriver default constructor
	Variables(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Variables(const Variables& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetFreeVariables();
	void SetFreeVariables(LPDISPATCH);
	LPDISPATCH GetFormVariables();
	void SetFormVariables(LPDISPATCH);
	LPDISPATCH GetDatabaseVariables();
	void SetDatabaseVariables(LPDISPATCH);
	LPDISPATCH GetCounters();
	void SetCounters(LPDISPATCH);
	LPDISPATCH GetFormulas();
	void SetFormulas(LPDISPATCH);
	LPDISPATCH GetTableLookups();
	void SetTableLookups(LPDISPATCH);
	LPDISPATCH GetDates();
	void SetDates(LPDISPATCH);
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Item1, const VARIANT& Item2);
	void Remove(const VARIANT& Key);
	void Refresh();
};
/////////////////////////////////////////////////////////////////////////////
// FreeVariables wrapper class

class FreeVariables : public COleDispatchDriver
{
public:
	FreeVariables() {}		// Calls COleDispatchDriver default constructor
	FreeVariables(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	FreeVariables(const FreeVariables& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// Free wrapper class

class Free : public COleDispatchDriver
{
public:
	Free() {}		// Calls COleDispatchDriver default constructor
	Free(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Free(const Free& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);
	CString Get_Value();
	void Set_Value(LPCTSTR);
	long GetDataSource();
	void SetDataSource(long);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	CString GetDisplayValue();
	void SetDisplayValue(LPCTSTR);
	VARIANT GetIncrement();
	void SetIncrement(const VARIANT&);
	long GetIso();
	void SetIso(long);
	long GetBaseType();
	void SetBaseType(long);
	long GetTriggerMode();
	void SetTriggerMode(long);
	VARIANT GetTriggerParameter();
	void SetTriggerParameter(const VARIANT&);
	CString GetCustomSet();
	void SetCustomSet(LPCTSTR);
	VARIANT GetMaxValue();
	void SetMaxValue(const VARIANT&);
	VARIANT GetResetToValue();
	void SetResetToValue(const VARIANT&);
	CString GetPrefix();
	void SetPrefix(LPCTSTR);
	CString GetSuffix();
	void SetSuffix(LPCTSTR);
	CString GetPadCharacter();
	void SetPadCharacter(LPCTSTR);
	BOOL GetDecimalUse();
	void SetDecimalUse(BOOL);
	short GetNumberOfDecimals();
	void SetNumberOfDecimals(short);
	CString GetDecimalSeparator();
	void SetDecimalSeparator(LPCTSTR);
	CString GetThousandSeparator();
	void SetThousandSeparator(LPCTSTR);
	long GetLabelCopies();
	void SetLabelCopies(long);
	CString GetOutputMask();
	void SetOutputMask(LPCTSTR);
	CString GetAccumulatorFile();
	void SetAccumulatorFile(LPCTSTR);
	long GetIncludeInLogFile();
	void SetIncludeInLogFile(long);
	long GetSharedCounter();
	void SetSharedCounter(long);
	long GetCounterUse();
	void SetCounterUse(long);
	short GetFormOrder();
	void SetFormOrder(short);
	CString GetFormPrompt();
	void SetFormPrompt(LPCTSTR);
	BOOL GetDisplayInForm();
	void SetDisplayInForm(BOOL);
	CString GetInputMask();
	void SetInputMask(LPCTSTR);
	BOOL GetShared();
	void SetShared(BOOL);
	long GetLength();
	void SetLength(long);
	short GetPadLength();
	void SetPadLength(short);
	BOOL GetFromAFile();
	void SetFromAFile(BOOL);
	CString GetFileName();
	void SetFileName(LPCTSTR);
	BOOL GetClearAfterPrint();
	void SetClearAfterPrint(BOOL);
	BOOL GetRestoreAfterPrinting();
	void SetRestoreAfterPrinting(BOOL);
	long GetUpdateAfterPrinting();
	void SetUpdateAfterPrinting(long);
	short GetInputMiniLength();
	void SetInputMiniLength(short);
	long GetRequired();
	void SetRequired(long);
	BOOL GetPickListUse();
	void SetPickListUse(BOOL);
	BOOL GetValueFromPickList();
	void SetValueFromPickList(BOOL);
	BOOL GetSkipSpaces();
	void SetSkipSpaces(BOOL);
	VARIANT GetRestoreValue();
	void SetRestoreValue(const VARIANT&);

// Operations
public:
	long ShowDialog(long hParent);
	LPDISPATCH GetPickList();
};
/////////////////////////////////////////////////////////////////////////////
// FormVariables wrapper class

class FormVariables : public COleDispatchDriver
{
public:
	FormVariables() {}		// Calls COleDispatchDriver default constructor
	FormVariables(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	FormVariables(const FormVariables& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// DatabaseVariables wrapper class

class DatabaseVariables : public COleDispatchDriver
{
public:
	DatabaseVariables() {}		// Calls COleDispatchDriver default constructor
	DatabaseVariables(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DatabaseVariables(const DatabaseVariables& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// Counters wrapper class

class Counters : public COleDispatchDriver
{
public:
	Counters() {}		// Calls COleDispatchDriver default constructor
	Counters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Counters(const Counters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// Counter wrapper class

class Counter : public COleDispatchDriver
{
public:
	Counter() {}		// Calls COleDispatchDriver default constructor
	Counter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Counter(const Counter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);
	CString Get_Value();
	void Set_Value(LPCTSTR);
	long GetDataSource();
	void SetDataSource(long);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	CString GetDisplayValue();
	void SetDisplayValue(LPCTSTR);
	VARIANT GetIncrement();
	void SetIncrement(const VARIANT&);
	long GetIso();
	void SetIso(long);
	long GetBaseType();
	void SetBaseType(long);
	long GetTriggerMode();
	void SetTriggerMode(long);
	VARIANT GetTriggerParameter();
	void SetTriggerParameter(const VARIANT&);
	CString GetCustomSet();
	void SetCustomSet(LPCTSTR);
	VARIANT GetMaxValue();
	void SetMaxValue(const VARIANT&);
	VARIANT GetResetToValue();
	void SetResetToValue(const VARIANT&);
	CString GetPrefix();
	void SetPrefix(LPCTSTR);
	CString GetSuffix();
	void SetSuffix(LPCTSTR);
	CString GetPadCharacter();
	void SetPadCharacter(LPCTSTR);
	BOOL GetDecimalUse();
	void SetDecimalUse(BOOL);
	short GetNumberOfDecimals();
	void SetNumberOfDecimals(short);
	CString GetDecimalSeparator();
	void SetDecimalSeparator(LPCTSTR);
	CString GetThousandSeparator();
	void SetThousandSeparator(LPCTSTR);
	long GetLabelCopies();
	void SetLabelCopies(long);
	CString GetOutputMask();
	void SetOutputMask(LPCTSTR);
	CString GetAccumulatorFile();
	void SetAccumulatorFile(LPCTSTR);
	long GetIncludeInLogFile();
	void SetIncludeInLogFile(long);
	long GetSharedCounter();
	void SetSharedCounter(long);

// Operations
public:
	long ShowDialog(long hParent);
};
/////////////////////////////////////////////////////////////////////////////
// Formulas wrapper class

class Formulas : public COleDispatchDriver
{
public:
	Formulas() {}		// Calls COleDispatchDriver default constructor
	Formulas(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Formulas(const Formulas& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// Formula wrapper class

class Formula : public COleDispatchDriver
{
public:
	Formula() {}		// Calls COleDispatchDriver default constructor
	Formula(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Formula(const Formula& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);
	CString Get_Value();
	void Set_Value(LPCTSTR);
	long GetDataSource();
	void SetDataSource(long);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	CString GetDisplayValue();
	void SetDisplayValue(LPCTSTR);
	VARIANT GetIncrement();
	void SetIncrement(const VARIANT&);
	long GetIso();
	void SetIso(long);
	long GetBaseType();
	void SetBaseType(long);
	long GetTriggerMode();
	void SetTriggerMode(long);
	VARIANT GetTriggerParameter();
	void SetTriggerParameter(const VARIANT&);
	CString GetCustomSet();
	void SetCustomSet(LPCTSTR);
	VARIANT GetMaxValue();
	void SetMaxValue(const VARIANT&);
	VARIANT GetResetToValue();
	void SetResetToValue(const VARIANT&);
	CString GetPrefix();
	void SetPrefix(LPCTSTR);
	CString GetSuffix();
	void SetSuffix(LPCTSTR);
	CString GetPadCharacter();
	void SetPadCharacter(LPCTSTR);
	BOOL GetDecimalUse();
	void SetDecimalUse(BOOL);
	short GetNumberOfDecimals();
	void SetNumberOfDecimals(short);
	CString GetDecimalSeparator();
	void SetDecimalSeparator(LPCTSTR);
	CString GetThousandSeparator();
	void SetThousandSeparator(LPCTSTR);
	long GetLabelCopies();
	void SetLabelCopies(long);
	CString GetOutputMask();
	void SetOutputMask(LPCTSTR);
	CString GetAccumulatorFile();
	void SetAccumulatorFile(LPCTSTR);
	long GetIncludeInLogFile();
	void SetIncludeInLogFile(long);
	long GetSharedCounter();
	void SetSharedCounter(long);
	long GetCounterUse();
	void SetCounterUse(long);
	CString GetExpression();
	void SetExpression(LPCTSTR);
	long GetLength();
	void SetLength(long);
	short GetPadLength();
	void SetPadLength(short);

// Operations
public:
	long ShowDialog(long hParent);
	BOOL Test();
};
/////////////////////////////////////////////////////////////////////////////
// TableLookups wrapper class

class TableLookups : public COleDispatchDriver
{
public:
	TableLookups() {}		// Calls COleDispatchDriver default constructor
	TableLookups(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	TableLookups(const TableLookups& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// TableLookup wrapper class

class TableLookup : public COleDispatchDriver
{
public:
	TableLookup() {}		// Calls COleDispatchDriver default constructor
	TableLookup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	TableLookup(const TableLookup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);
	CString Get_Value();
	void Set_Value(LPCTSTR);
	long GetDataSource();
	void SetDataSource(long);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	CString GetDisplayValue();
	void SetDisplayValue(LPCTSTR);
	VARIANT GetIncrement();
	void SetIncrement(const VARIANT&);
	long GetIso();
	void SetIso(long);
	long GetBaseType();
	void SetBaseType(long);
	long GetTriggerMode();
	void SetTriggerMode(long);
	VARIANT GetTriggerParameter();
	void SetTriggerParameter(const VARIANT&);
	CString GetCustomSet();
	void SetCustomSet(LPCTSTR);
	VARIANT GetMaxValue();
	void SetMaxValue(const VARIANT&);
	VARIANT GetResetToValue();
	void SetResetToValue(const VARIANT&);
	CString GetPrefix();
	void SetPrefix(LPCTSTR);
	CString GetSuffix();
	void SetSuffix(LPCTSTR);
	CString GetPadCharacter();
	void SetPadCharacter(LPCTSTR);
	BOOL GetDecimalUse();
	void SetDecimalUse(BOOL);
	short GetNumberOfDecimals();
	void SetNumberOfDecimals(short);
	CString GetDecimalSeparator();
	void SetDecimalSeparator(LPCTSTR);
	CString GetThousandSeparator();
	void SetThousandSeparator(LPCTSTR);
	long GetLabelCopies();
	void SetLabelCopies(long);
	CString GetOutputMask();
	void SetOutputMask(LPCTSTR);
	CString GetAccumulatorFile();
	void SetAccumulatorFile(LPCTSTR);
	long GetIncludeInLogFile();
	void SetIncludeInLogFile(long);
	long GetSharedCounter();
	void SetSharedCounter(long);
	long GetCounterUse();
	void SetCounterUse(long);
	CString GetDatabaseSource();
	void SetDatabaseSource(LPCTSTR);
	CString GetTableName();
	void SetTableName(LPCTSTR);
	CString GetResultField();
	void SetResultField(LPCTSTR);
	CString GetKeys();
	void SetKeys(LPCTSTR);
	long GetLength();
	void SetLength(long);
	short GetPadLength();
	void SetPadLength(short);
	long GetDatabaseType();
	void SetDatabaseType(long);

// Operations
public:
	long ShowDialog(long hParent);
	void AddKey(LPCTSTR strField, LPCTSTR strVariableName);
	void DeleteKey(LPCTSTR strField);
	long Connect();
	long Disconnect();
};
/////////////////////////////////////////////////////////////////////////////
// Dates wrapper class

class Dates : public COleDispatchDriver
{
public:
	Dates() {}		// Calls COleDispatchDriver default constructor
	Dates(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Dates(const Dates& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// DateTime wrapper class

class DateTime : public COleDispatchDriver
{
public:
	DateTime() {}		// Calls COleDispatchDriver default constructor
	DateTime(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DateTime(const DateTime& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);
	CString Get_Value();
	void Set_Value(LPCTSTR);
	long GetDataSource();
	void SetDataSource(long);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	CString GetDisplayValue();
	void SetDisplayValue(LPCTSTR);
	long GetDevice();
	void SetDevice(long);
	CString GetFormat();
	void SetFormat(LPCTSTR);
	long GetIncludeInLogFile();
	void SetIncludeInLogFile(long);
	long GetOffsetValue();
	void SetOffsetValue(long);
	short GetOffsetUnit();
	void SetOffsetUnit(short);
	CString GetOffsetVariableName();
	void SetOffsetVariableName(LPCTSTR);
	LPDISPATCH GetOffsetVariableObject();
	void SetOffsetVariableObject(LPDISPATCH);

// Operations
public:
	long ShowDialog(long hParent);
};
/////////////////////////////////////////////////////////////////////////////
// Variable wrapper class

class Variable : public COleDispatchDriver
{
public:
	Variable() {}		// Calls COleDispatchDriver default constructor
	Variable(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Variable(const Variable& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);
	CString Get_Value();
	void Set_Value(LPCTSTR);
	long GetDataSource();
	void SetDataSource(long);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	CString GetDisplayValue();
	void SetDisplayValue(LPCTSTR);
	long GetIncludeInLogFile();
	void SetIncludeInLogFile(long);
	long GetLength();
	void SetLength(long);

// Operations
public:
	long ShowDialog(long hParent);
};
/////////////////////////////////////////////////////////////////////////////
// DocObjects wrapper class

class DocObjects : public COleDispatchDriver
{
public:
	DocObjects() {}		// Calls COleDispatchDriver default constructor
	DocObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DocObjects(const DocObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);
	LPDISPATCH GetTexts();
	void SetTexts(LPDISPATCH);
	LPDISPATCH GetBarcodes();
	void SetBarcodes(LPDISPATCH);
	LPDISPATCH GetImages();
	void SetImages(LPDISPATCH);
	LPDISPATCH GetShapes();
	void SetShapes(LPDISPATCH);
	LPDISPATCH GetOLEObjects();
	void SetOLEObjects(LPDISPATCH);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(long Type, const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// Texts wrapper class

class Texts : public COleDispatchDriver
{
public:
	Texts() {}		// Calls COleDispatchDriver default constructor
	Texts(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Texts(const Texts& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// Text wrapper class

class Text : public COleDispatchDriver
{
public:
	Text() {}		// Calls COleDispatchDriver default constructor
	Text(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Text(const Text& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetLeft();
	void SetLeft(long);
	long GetTop();
	void SetTop(long);
	long GetWidth();
	void SetWidth(long);
	long GetHeight();
	void SetHeight(long);
	CString GetName();
	void SetName(LPCTSTR);
	CString Get_Name();
	void Set_Name(LPCTSTR);
	long GetType();
	void SetType(long);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	long GetLocked();
	void SetLocked(long);
	long GetPrintable();
	void SetPrintable(long);
	long GetAnchorPoint();
	void SetAnchorPoint(long);
	long GetRotation();
	void SetRotation(long);
	long GetBackColor();
	void SetBackColor(long);
	long GetForeColor();
	void SetForeColor(long);
	long GetMarginLeft();
	void SetMarginLeft(long);
	long GetMarginRight();
	void SetMarginRight(long);
	long GetMarginTop();
	void SetMarginTop(long);
	long GetMarginBottom();
	void SetMarginBottom(long);
	long GetPropertyCount();
	void SetPropertyCount(long);
	CString GetData();
	void SetData(LPCTSTR);
	CString GetDataSourceNames();
	void SetDataSourceNames(LPCTSTR);
	LPDISPATCH GetFont();
	void SetFont(LPDISPATCH);
	CString GetValue();
	void SetValue(LPCTSTR);
	long GetWordWrap();
	void SetWordWrap(long);
	long GetWordHyphenation();
	void SetWordHyphenation(long);
	long GetFitToFrame();
	void SetFitToFrame(long);
	long GetAlignment();
	void SetAlignment(long);
	LPDISPATCH GetVariableObject();
	void SetVariableObject(LPDISPATCH);
	CString GetVariableName();
	void SetVariableName(LPCTSTR);
	LPDISPATCH GetSelText();
	void SetSelText(LPDISPATCH);
	long GetLineSpacing();
	void SetLineSpacing(long);
	LPDISPATCH GetStrings();
	void SetStrings(LPDISPATCH);

// Operations
public:
	void Move(long X, long Y);
	void Bound(long Left, long Top, long Right, long Bottom);
	long ShowDialog(long hParent);
	LPDISPATCH InsertString(LPCTSTR strString, const VARIANT& Position, LPDISPATCH Font);
	void InsertTextObject(LPDISPATCH TextObject, const VARIANT& Position);
	void InsertVariable(LPDISPATCH Var, const VARIANT& Position, LPDISPATCH Font);
	void InsertCRLF(const VARIANT& Position, LPDISPATCH Font);
	LPDISPATCH AppendString(LPCTSTR strString, const VARIANT& Position, LPDISPATCH Font);
	void AppendTextObject(LPDISPATCH TextObject);
	void AppendVariable(LPDISPATCH Var, LPDISPATCH Font);
	void AppendCRLF(LPDISPATCH Font);
	void Copy();
	void Paste(const VARIANT& Position);
	LPDISPATCH GetProperty_(long Index);
	LPDISPATCH GetPropertyID(long ID);
	void Refresh();
};
/////////////////////////////////////////////////////////////////////////////
// IFontDisp2 wrapper class

class IFontDisp2 : public COleDispatchDriver
{
public:
	IFontDisp2() {}		// Calls COleDispatchDriver default constructor
	IFontDisp2(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IFontDisp2(const IFontDisp2& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	CY GetSize();
	void SetSize(const CY&);
	BOOL GetBold();
	void SetBold(BOOL);
	BOOL GetItalic();
	void SetItalic(BOOL);
	BOOL GetUnderline();
	void SetUnderline(BOOL);
	BOOL GetStrikethrough();
	void SetStrikethrough(BOOL);
	short GetWeight();
	void SetWeight(short);
	short GetCharset();
	void SetCharset(short);
	short GetOutPrecision();
	void SetOutPrecision(short);
	short GetPitchAndFamily();
	void SetPitchAndFamily(short);
	long GetWidthRatio();
	void SetWidthRatio(long);
	long GetHfont();
	void SetHfont(long);
	long GetHeight();
	void SetHeight(long);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// TextSelection wrapper class

class TextSelection : public COleDispatchDriver
{
public:
	TextSelection() {}		// Calls COleDispatchDriver default constructor
	TextSelection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	TextSelection(const TextSelection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetFont();
	void SetFont(LPDISPATCH);
	CString GetValue();
	void SetValue(LPCTSTR);
	long GetBackColor();
	void SetBackColor(long);
	long GetForeColor();
	void SetForeColor(long);
	BOOL GetIsEmpty();
	void SetIsEmpty(BOOL);

// Operations
public:
	void Select(const VARIANT& FirstPosition, const VARIANT& LastPosition);
	void Cut();
	void Copy();
	void Paste();
};
/////////////////////////////////////////////////////////////////////////////
// TextStrings wrapper class

class TextStrings : public COleDispatchDriver
{
public:
	TextStrings() {}		// Calls COleDispatchDriver default constructor
	TextStrings(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	TextStrings(const TextStrings& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// TextString wrapper class

class TextString : public COleDispatchDriver
{
public:
	TextString() {}		// Calls COleDispatchDriver default constructor
	TextString(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	TextString(const TextString& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetFont();
	void SetFont(LPDISPATCH);
	long GetBackColor();
	void SetBackColor(long);
	long GetForeColor();
	void SetForeColor(long);
	CString GetData();
	void SetData(LPCTSTR);
	CString GetVariableName();
	void SetVariableName(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// Barcodes wrapper class

class Barcodes : public COleDispatchDriver
{
public:
	Barcodes() {}		// Calls COleDispatchDriver default constructor
	Barcodes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Barcodes(const Barcodes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// Barcode wrapper class

class Barcode : public COleDispatchDriver
{
public:
	Barcode() {}		// Calls COleDispatchDriver default constructor
	Barcode(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Barcode(const Barcode& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetLeft();
	void SetLeft(long);
	long GetTop();
	void SetTop(long);
	long GetWidth();
	void SetWidth(long);
	long GetHeight();
	void SetHeight(long);
	CString GetName();
	void SetName(LPCTSTR);
	CString Get_Name();
	void Set_Name(LPCTSTR);
	long GetType();
	void SetType(long);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	long GetLocked();
	void SetLocked(long);
	long GetPrintable();
	void SetPrintable(long);
	long GetAnchorPoint();
	void SetAnchorPoint(long);
	long GetRotation();
	void SetRotation(long);
	long GetBackColor();
	void SetBackColor(long);
	long GetForeColor();
	void SetForeColor(long);
	long GetMarginLeft();
	void SetMarginLeft(long);
	long GetMarginRight();
	void SetMarginRight(long);
	long GetMarginTop();
	void SetMarginTop(long);
	long GetMarginBottom();
	void SetMarginBottom(long);
	long GetPropertyCount();
	void SetPropertyCount(long);
	CString GetData();
	void SetData(LPCTSTR);
	CString GetDataSourceNames();
	void SetDataSourceNames(LPCTSTR);
	long GetBarHeight();
	void SetBarHeight(long);
	long GetNarrowBarWidth();
	void SetNarrowBarWidth(long);
	short GetRatio();
	void SetRatio(short);
	long GetCheckMode();
	void SetCheckMode(long);
	long GetDevice();
	void SetDevice(long);
	LPDISPATCH GetCode2D();
	void SetCode2D(LPDISPATCH);
	CString GetValue();
	void SetValue(LPCTSTR);
	CString GetVariableName();
	void SetVariableName(LPCTSTR);
	long GetSymbology();
	void SetSymbology(long);
	LPDISPATCH GetVariableObject();
	void SetVariableObject(LPDISPATCH);
	long GetHRGap();
	void SetHRGap(long);
	long GetHRAlignment();
	void SetHRAlignment(long);
	long GetHRPosition();
	void SetHRPosition(long);
	long GetHRCheckCharacter();
	void SetHRCheckCharacter(long);
	LPDISPATCH GetHRFont();
	void SetHRFont(LPDISPATCH);
	LPDISPATCH GetHRFreeTextObject();
	void SetHRFreeTextObject(LPDISPATCH);
	BOOL GetHRDevice();
	void SetHRDevice(BOOL);
	CString GetCheckValue();
	void SetCheckValue(LPCTSTR);
	CString GetHROutputMask();
	void SetHROutputMask(LPCTSTR);
	long GetNarrowBarWidthDot();
	void SetNarrowBarWidthDot(long);
	long GetExtendedDescenders();
	void SetExtendedDescenders(long);
	long GetShape();
	void SetShape(long);
	long GetInnerRadius();
	void SetInnerRadius(long);
	long GetBeginAngle();
	void SetBeginAngle(long);
	long GetAngle();
	void SetAngle(long);

// Operations
public:
	void Move(long X, long Y);
	void Bound(long Left, long Top, long Right, long Bottom);
	long ShowDialog(long hParent);
	long Is2D();
	LPDISPATCH GetProperty_(long Index);
	LPDISPATCH GetPropertyID(long ID);
	void Refresh();
};
/////////////////////////////////////////////////////////////////////////////
// Code2D wrapper class

class Code2D : public COleDispatchDriver
{
public:
	Code2D() {}		// Calls COleDispatchDriver default constructor
	Code2D(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Code2D(const Code2D& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetModuleX();
	void SetModuleX(long);
	long GetModuleY();
	void SetModuleY(long);
	long GetColumns();
	void SetColumns(long);
	long GetRows();
	void SetRows(long);
	CString GetEcc();
	void SetEcc(LPCTSTR);
	long GetModuleXDot();
	void SetModuleXDot(long);
	long GetModuleYDot();
	void SetModuleYDot(long);
	LPDISPATCH GetOptionsAvailable();
	void SetOptionsAvailable(LPDISPATCH);

// Operations
public:
	void SetOption(LPCTSTR strOptionName, const VARIANT& varValue);
	VARIANT GetOption(LPCTSTR strOptionName);
};
/////////////////////////////////////////////////////////////////////////////
// Strings wrapper class

class Strings : public COleDispatchDriver
{
public:
	Strings() {}		// Calls COleDispatchDriver default constructor
	Strings(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Strings(const Strings& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	CString Item(long Key);
	CString _Item(long Key);
};
/////////////////////////////////////////////////////////////////////////////
// Images wrapper class

class Images : public COleDispatchDriver
{
public:
	Images() {}		// Calls COleDispatchDriver default constructor
	Images(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Images(const Images& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// Image wrapper class

class Image : public COleDispatchDriver
{
public:
	Image() {}		// Calls COleDispatchDriver default constructor
	Image(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Image(const Image& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetLeft();
	void SetLeft(long);
	long GetTop();
	void SetTop(long);
	long GetWidth();
	void SetWidth(long);
	long GetHeight();
	void SetHeight(long);
	CString GetName();
	void SetName(LPCTSTR);
	CString Get_Name();
	void Set_Name(LPCTSTR);
	long GetType();
	void SetType(long);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	long GetLocked();
	void SetLocked(long);
	long GetPrintable();
	void SetPrintable(long);
	long GetAnchorPoint();
	void SetAnchorPoint(long);
	long GetRotation();
	void SetRotation(long);
	long GetBackColor();
	void SetBackColor(long);
	long GetForeColor();
	void SetForeColor(long);
	long GetMarginLeft();
	void SetMarginLeft(long);
	long GetMarginRight();
	void SetMarginRight(long);
	long GetMarginTop();
	void SetMarginTop(long);
	long GetMarginBottom();
	void SetMarginBottom(long);
	long GetPropertyCount();
	void SetPropertyCount(long);
	CString GetData();
	void SetData(LPCTSTR);
	CString GetDataSourceNames();
	void SetDataSourceNames(LPCTSTR);
	CString GetFileName();
	void SetFileName(LPCTSTR);
	long GetVertFlip();
	void SetVertFlip(long);
	long GetHorzFlip();
	void SetHorzFlip(long);
	long GetNegative();
	void SetNegative(long);
	short GetBrightness();
	void SetBrightness(short);
	LPDISPATCH GetVariableObject();
	void SetVariableObject(LPDISPATCH);
	CString GetVariableName();
	void SetVariableName(LPCTSTR);
	short GetRatio();
	void SetRatio(short);
	LPDISPATCH GetPicture();
	void SetPicture(LPDISPATCH);
	short GetContrast();
	void SetContrast(short);
	long GetDefaultFolder();
	void SetDefaultFolder(long);
	CString GetFolderName();
	void SetFolderName(LPCTSTR);
	long GetNumColors();
	void SetNumColors(long);
	long GetMethod();
	void SetMethod(long);
	long GetNumMaxColors();
	void SetNumMaxColors(long);

// Operations
public:
	void Move(long X, long Y);
	void Bound(long Left, long Top, long Right, long Bottom);
	long ShowDialog(long hParent);
	LPDISPATCH GetProperty_(long Index);
	LPDISPATCH GetPropertyID(long ID);
	void Refresh();
};
/////////////////////////////////////////////////////////////////////////////
// Shapes wrapper class

class Shapes : public COleDispatchDriver
{
public:
	Shapes() {}		// Calls COleDispatchDriver default constructor
	Shapes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Shapes(const Shapes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	void Remove(const VARIANT& Key);
	LPDISPATCH AddLine(long Left, long Top, long Right, long Bottom);
	LPDISPATCH AddOblique(long Left, long Top, long Right, long Bottom);
	LPDISPATCH AddPolygon(const VARIANT& Key);
	LPDISPATCH AddRectangle(long Left, long Top, long Right, long Bottom);
	LPDISPATCH AddRoundRect(long Left, long Top, long Right, long Bottom, long Radius);
	LPDISPATCH AddEllipse(long Left, long Top, long Right, long Bottom);
};
/////////////////////////////////////////////////////////////////////////////
// Shape wrapper class

class Shape : public COleDispatchDriver
{
public:
	Shape() {}		// Calls COleDispatchDriver default constructor
	Shape(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Shape(const Shape& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetLeft();
	void SetLeft(long);
	long GetTop();
	void SetTop(long);
	long GetWidth();
	void SetWidth(long);
	long GetHeight();
	void SetHeight(long);
	CString GetName();
	void SetName(LPCTSTR);
	CString Get_Name();
	void Set_Name(LPCTSTR);
	long GetType();
	void SetType(long);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	long GetLocked();
	void SetLocked(long);
	long GetPrintable();
	void SetPrintable(long);
	long GetAnchorPoint();
	void SetAnchorPoint(long);
	long GetRotation();
	void SetRotation(long);
	long GetBackColor();
	void SetBackColor(long);
	long GetForeColor();
	void SetForeColor(long);
	long GetMarginLeft();
	void SetMarginLeft(long);
	long GetMarginRight();
	void SetMarginRight(long);
	long GetMarginTop();
	void SetMarginTop(long);
	long GetMarginBottom();
	void SetMarginBottom(long);
	long GetPropertyCount();
	void SetPropertyCount(long);
	CString GetData();
	void SetData(LPCTSTR);
	CString GetDataSourceNames();
	void SetDataSourceNames(LPCTSTR);
	long GetLineWidth();
	void SetLineWidth(long);
	long GetClosed();
	void SetClosed(long);
	long GetBackMode();
	void SetBackMode(long);

// Operations
public:
	void Move(long X, long Y);
	void Bound(long Left, long Top, long Right, long Bottom);
	long ShowDialog(long hParent);
	VARIANT GetPoints();
	void SetPoints(const VARIANT& pts);
	LPDISPATCH GetProperty_(long Index);
	LPDISPATCH GetPropertyID(long ID);
	void Refresh();
};
/////////////////////////////////////////////////////////////////////////////
// OLEObjects wrapper class

class OLEObjects : public COleDispatchDriver
{
public:
	OLEObjects() {}		// Calls COleDispatchDriver default constructor
	OLEObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	OLEObjects(const OLEObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	LPDISPATCH Add(const VARIANT& Key);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// OLEObject wrapper class

class OLEObject : public COleDispatchDriver
{
public:
	OLEObject() {}		// Calls COleDispatchDriver default constructor
	OLEObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	OLEObject(const OLEObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetLeft();
	void SetLeft(long);
	long GetTop();
	void SetTop(long);
	long GetWidth();
	void SetWidth(long);
	long GetHeight();
	void SetHeight(long);
	CString GetName();
	void SetName(LPCTSTR);
	CString Get_Name();
	void Set_Name(LPCTSTR);
	long GetType();
	void SetType(long);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	long GetLocked();
	void SetLocked(long);
	long GetPrintable();
	void SetPrintable(long);
	long GetAnchorPoint();
	void SetAnchorPoint(long);
	long GetRotation();
	void SetRotation(long);
	long GetBackColor();
	void SetBackColor(long);
	long GetForeColor();
	void SetForeColor(long);
	long GetMarginLeft();
	void SetMarginLeft(long);
	long GetMarginRight();
	void SetMarginRight(long);
	long GetMarginTop();
	void SetMarginTop(long);
	long GetMarginBottom();
	void SetMarginBottom(long);
	long GetPropertyCount();
	void SetPropertyCount(long);
	CString GetData();
	void SetData(LPCTSTR);
	CString GetDataSourceNames();
	void SetDataSourceNames(LPCTSTR);
	LPDISPATCH GetObject();
	void SetObject(LPDISPATCH);
	CString GetConnectedServer();
	void SetConnectedServer(LPCTSTR);
	CString GetLinkedFileName();
	void SetLinkedFileName(LPCTSTR);

// Operations
public:
	void Move(long X, long Y);
	void Bound(long Left, long Top, long Right, long Bottom);
	BOOL EmbedFile(LPCTSTR strFileName);
	BOOL LinkFile(LPCTSTR strLinkFileName);
	BOOL ConnectServer(LPCTSTR strCLSIDOrServerName);
	LPDISPATCH GetProperty_(long Index);
	LPDISPATCH GetPropertyID(long ID);
	void Refresh();
};
/////////////////////////////////////////////////////////////////////////////
// DocObject wrapper class

class DocObject : public COleDispatchDriver
{
public:
	DocObject() {}		// Calls COleDispatchDriver default constructor
	DocObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DocObject(const DocObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	long GetLeft();
	void SetLeft(long);
	long GetTop();
	void SetTop(long);
	long GetWidth();
	void SetWidth(long);
	long GetHeight();
	void SetHeight(long);
	CString GetName();
	void SetName(LPCTSTR);
	CString Get_Name();
	void Set_Name(LPCTSTR);
	long GetType();
	void SetType(long);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	long GetLocked();
	void SetLocked(long);
	long GetPrintable();
	void SetPrintable(long);
	long GetAnchorPoint();
	void SetAnchorPoint(long);
	long GetRotation();
	void SetRotation(long);
	long GetBackColor();
	void SetBackColor(long);
	long GetForeColor();
	void SetForeColor(long);
	long GetMarginLeft();
	void SetMarginLeft(long);
	long GetMarginRight();
	void SetMarginRight(long);
	long GetMarginTop();
	void SetMarginTop(long);
	long GetMarginBottom();
	void SetMarginBottom(long);
	long GetPropertyCount();
	void SetPropertyCount(long);
	CString GetData();
	void SetData(LPCTSTR);
	CString GetDataSourceNames();
	void SetDataSourceNames(LPCTSTR);

// Operations
public:
	void Move(long X, long Y);
	void Bound(long Left, long Top, long Right, long Bottom);
	long ShowDialog(long hParent);
	LPDISPATCH GetProperty_(long Index);
	LPDISPATCH GetPropertyID(long ID);
	void Refresh();
};
/////////////////////////////////////////////////////////////////////////////
// Printer wrapper class

class Printer : public COleDispatchDriver
{
public:
	Printer() {}		// Calls COleDispatchDriver default constructor
	Printer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Printer(const Printer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	CString GetName();
	void SetName(LPCTSTR);
	long GetXdpi();
	void SetXdpi(long);
	long GetYdpi();
	void SetYdpi(long);
	CString GetFullName();
	void SetFullName(LPCTSTR);
	CString Get_FullName();
	void Set_FullName(LPCTSTR);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetWindowsFontNames();
	void SetWindowsFontNames(LPDISPATCH);
	LPDISPATCH GetDeviceFontNames();
	void SetDeviceFontNames(LPDISPATCH);
	LPDISPATCH GetWindowsCodeNames();
	void SetWindowsCodeNames(LPDISPATCH);
	LPDISPATCH GetDeviceCodeNames();
	void SetDeviceCodeNames(LPDISPATCH);
	CString GetDriverName();
	void SetDriverName(LPCTSTR);
	CString GetModelName();
	void SetModelName(LPCTSTR);

// Operations
public:
	short ShowSetup();
	BOOL SetParameter(const VARIANT& Parameter, const VARIANT& Value);
	BOOL SwitchTo(LPCTSTR strPrinterName, LPCTSTR strPortName, BOOL DirectAccess);
	BOOL Send(LPCTSTR strEscapeSequence);
	BOOL IsDevice();
	long GetDevMode(VARIANT* DevMode);
	long SetDevMode(VARIANT* DevMode);
	BOOL SwitchToEx(LPCTSTR strPrinterName, LPCTSTR strPortName, BOOL DirectAccess, long Flags);
	BOOL GetParameter(const VARIANT& Parameter, VARIANT* Value);
};
/////////////////////////////////////////////////////////////////////////////
// Format wrapper class

class Format : public COleDispatchDriver
{
public:
	Format() {}		// Calls COleDispatchDriver default constructor
	Format(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Format(const Format& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	long GetPageHeight();
	void SetPageHeight(long);
	long GetPageWidth();
	void SetPageWidth(long);
	long GetLabelHeight();
	void SetLabelHeight(long);
	long GetLabelWidth();
	void SetLabelWidth(long);
	long GetMarginLeft();
	void SetMarginLeft(long);
	long GetMarginTop();
	void SetMarginTop(long);
	long GetCorner();
	void SetCorner(long);
	long GetHorizontalGap();
	void SetHorizontalGap(long);
	long GetVerticalGap();
	void SetVerticalGap(long);
	long GetRowCount();
	void SetRowCount(long);
	long GetColumnCount();
	void SetColumnCount(long);
	BOOL GetPortrait();
	void SetPortrait(BOOL);
	BOOL GetAutoSize();
	void SetAutoSize(BOOL);
	CString GetStockType();
	void SetStockType(LPCTSTR);
	CString GetStockName();
	void SetStockName(LPCTSTR);
	long GetSupportType();
	void SetSupportType(long);

// Operations
public:
	void SaveStock();
};
/////////////////////////////////////////////////////////////////////////////
// Database wrapper class

class Database : public COleDispatchDriver
{
public:
	Database() {}		// Calls COleDispatchDriver default constructor
	Database(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Database(const Database& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	BOOL GetEof();
	void SetEof(BOOL);
	BOOL GetBof();
	void SetBof(BOOL);
	BOOL GetCreatingVariables();
	void SetCreatingVariables(BOOL);
	BOOL GetIsOpen();
	void SetIsOpen(BOOL);
	CString GetConnectionString();
	void SetConnectionString(LPCTSTR);
	CString GetQueryString();
	void SetQueryString(LPCTSTR);
	CString GetQueryFileName();
	void SetQueryFileName(LPCTSTR);

// Operations
public:
	BOOL MoveNext();
	BOOL MovePrevious();
	BOOL MoveFirst();
	BOOL MoveLast();
	BOOL OpenASCII(LPCTSTR strFileName, LPCTSTR strDescriptorFileName);
	BOOL OpenQuery(LPCTSTR strQueryFileName);
	BOOL OpenODBC(LPCTSTR strDatasourceName, LPCTSTR strSQLQuery);
	void Close();
	BOOL Refresh();
	long Find(long lDatabaseFind, LPCTSTR strField, LPCTSTR strValue);
};
/////////////////////////////////////////////////////////////////////////////
// DocumentProperties wrapper class

class DocumentProperties : public COleDispatchDriver
{
public:
	DocumentProperties() {}		// Calls COleDispatchDriver default constructor
	DocumentProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DocumentProperties(const DocumentProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// DocumentProperty wrapper class

class DocumentProperty : public COleDispatchDriver
{
public:
	DocumentProperty() {}		// Calls COleDispatchDriver default constructor
	DocumentProperty(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	DocumentProperty(const DocumentProperty& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	VARIANT GetValue();
	void SetValue(const VARIANT&);
	VARIANT Get_Value();
	void Set_Value(const VARIANT&);
	long GetType();
	void SetType(long);
	CString GetName();
	void SetName(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// SelectedDocObjects wrapper class

class SelectedDocObjects : public COleDispatchDriver
{
public:
	SelectedDocObjects() {}		// Calls COleDispatchDriver default constructor
	SelectedDocObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SelectedDocObjects(const SelectedDocObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH _Item(const VARIANT& Key);
	void Add(LPDISPATCH pObject);
	void Remove(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// Dialogs wrapper class

class Dialogs : public COleDispatchDriver
{
public:
	Dialogs() {}		// Calls COleDispatchDriver default constructor
	Dialogs(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Dialogs(const Dialogs& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	short GetCount();
	void SetCount(short);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(long Key);
	LPDISPATCH _Item(long Key);
};
/////////////////////////////////////////////////////////////////////////////
// Dialog wrapper class

class Dialog : public COleDispatchDriver
{
public:
	Dialog() {}		// Calls COleDispatchDriver default constructor
	Dialog(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Dialog(const Dialog& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	long GetType();
	void SetType(long);

// Operations
public:
	short Show(const VARIANT& varParent);
};
/////////////////////////////////////////////////////////////////////////////
// Options wrapper class

class Options : public COleDispatchDriver
{
public:
	Options() {}		// Calls COleDispatchDriver default constructor
	Options(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	Options(const Options& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	BOOL GetLoadPrinterSetup();
	void SetLoadPrinterSetup(BOOL);
	BOOL GetLoadPrinter();
	void SetLoadPrinter(BOOL);
	BOOL GetOpenMergeDatabase();
	void SetOpenMergeDatabase(BOOL);
	BOOL GetCreateBackup();
	void SetCreateBackup(BOOL);
	BOOL GetOpenReadOnly();
	void SetOpenReadOnly(BOOL);
	long GetMeasureSystem();
	void SetMeasureSystem(long);
	CString GetDefaultImagePath();
	void SetDefaultImagePath(LPCTSTR);
	CString GetDefaultQueryPath();
	void SetDefaultQueryPath(LPCTSTR);
	CString GetDefaultDescriberPath();
	void SetDefaultDescriberPath(LPCTSTR);
	CString GetDefaultPrintOutPath();
	void SetDefaultPrintOutPath(LPCTSTR);
	CString GetDefaultSharedPath();
	void SetDefaultSharedPath(LPCTSTR);
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	BOOL GetTrayNotification();
	void SetTrayNotification(BOOL);
	long GetLanguage();
	void SetLanguage(long);
	float GetEuroConversionRate();
	void SetEuroConversionRate(float);
	CString GetDefaultUserSettingsPath();
	void SetDefaultUserSettingsPath(LPCTSTR);
	long GetSharedFileAccessTimeout();
	void SetSharedFileAccessTimeout(long);
	long GetSettingsMode();
	void SetSettingsMode(long);
	CString GetDefaultDataPath();
	void SetDefaultDataPath(LPCTSTR);
	BOOL GetDoNotDisplayWarning();
	void SetDoNotDisplayWarning(BOOL);
	long GetHfont();
	void SetHfont(long);
	LPDISPATCH GetFont();
	void SetFont(LPDISPATCH);
	long GetAutoSaveDelay();
	void SetAutoSaveDelay(long);
	CString GetDefaultDocumentPath();
	void SetDefaultDocumentPath(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// RecentFiles wrapper class

class RecentFiles : public COleDispatchDriver
{
public:
	RecentFiles() {}		// Calls COleDispatchDriver default constructor
	RecentFiles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	RecentFiles(const RecentFiles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	long GetMaximum();
	void SetMaximum(long);
	long GetCount();
	void SetCount(long);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Add(const VARIANT& Key, BOOL ReadOnly);
	LPDISPATCH Item(long Key);
	LPDISPATCH _Item(long Key);
	void Clear();
	void Remove(short Key);
};
/////////////////////////////////////////////////////////////////////////////
// RecentFile wrapper class

class RecentFile : public COleDispatchDriver
{
public:
	RecentFile() {}		// Calls COleDispatchDriver default constructor
	RecentFile(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	RecentFile(const RecentFile& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	CString GetPath();
	void SetPath(LPCTSTR);
	CString GetName();
	void SetName(LPCTSTR);

// Operations
public:
	LPDISPATCH Open();
};
/////////////////////////////////////////////////////////////////////////////
// SharedOwner wrapper class

class SharedOwner : public COleDispatchDriver
{
public:
	SharedOwner() {}		// Calls COleDispatchDriver default constructor
	SharedOwner(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SharedOwner(const SharedOwner& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetCounters();
	void SetCounters(LPDISPATCH);
	LPDISPATCH GetLists();
	void SetLists(LPDISPATCH);
	LPDISPATCH GetFrees();
	void SetFrees(LPDISPATCH);
	LPDISPATCH GetForms();
	void SetForms(LPDISPATCH);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// SharedCounters wrapper class

class SharedCounters : public COleDispatchDriver
{
public:
	SharedCounters() {}		// Calls COleDispatchDriver default constructor
	SharedCounters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SharedCounters(const SharedCounters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	short GetCount();
	void SetCount(short);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH Add(LPCTSTR strName);
	long Remove(const VARIANT& Key);
	long ShowDialog();
	void Refresh();
	long Write();
	LPDISPATCH _Item(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// SharedCounter wrapper class

class SharedCounter : public COleDispatchDriver
{
public:
	SharedCounter() {}		// Calls COleDispatchDriver default constructor
	SharedCounter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SharedCounter(const SharedCounter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	CString GetName();
	void SetName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// SharedListsCollection wrapper class

class SharedListsCollection : public COleDispatchDriver
{
public:
	SharedListsCollection() {}		// Calls COleDispatchDriver default constructor
	SharedListsCollection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SharedListsCollection(const SharedListsCollection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	short GetCount();
	void SetCount(short);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH Add(LPCTSTR strName);
	long Remove(const VARIANT& Key);
	void Refresh();
	long Write();
	LPDISPATCH _Item(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// SharedLists wrapper class

class SharedLists : public COleDispatchDriver
{
public:
	SharedLists() {}		// Calls COleDispatchDriver default constructor
	SharedLists(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SharedLists(const SharedLists& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	short GetCount();
	void SetCount(short);
	CString GetName();
	void SetName(LPCTSTR);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	long ShowDialog();
	void Refresh();
	long Write();
	LPDISPATCH AddValue(LPCTSTR sValue);
	long DeleteValue(LPCTSTR sValue);
	LPDISPATCH GetAt(long lValue);
	LPDISPATCH _GetAt(long lValue);
};
/////////////////////////////////////////////////////////////////////////////
// SharedList wrapper class

class SharedList : public COleDispatchDriver
{
public:
	SharedList() {}		// Calls COleDispatchDriver default constructor
	SharedList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SharedList(const SharedList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	CString GetValue();
	void SetValue(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// SharedFrees wrapper class

class SharedFrees : public COleDispatchDriver
{
public:
	SharedFrees() {}		// Calls COleDispatchDriver default constructor
	SharedFrees(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SharedFrees(const SharedFrees& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	short GetCount();
	void SetCount(short);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH Add(LPCTSTR strName);
	long Remove(const VARIANT& Key);
	long ShowDialog();
	void Refresh();
	long Write();
	LPDISPATCH _Item(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// SharedFree wrapper class

class SharedFree : public COleDispatchDriver
{
public:
	SharedFree() {}		// Calls COleDispatchDriver default constructor
	SharedFree(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SharedFree(const SharedFree& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	CString GetName();
	void SetName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// SharedForms wrapper class

class SharedForms : public COleDispatchDriver
{
public:
	SharedForms() {}		// Calls COleDispatchDriver default constructor
	SharedForms(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SharedForms(const SharedForms& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	short GetCount();
	void SetCount(short);
	LPUNKNOWN Get_NewEnum();
	void Set_NewEnum(LPUNKNOWN);

// Operations
public:
	LPDISPATCH Item(const VARIANT& Key);
	LPDISPATCH Add(LPCTSTR strName);
	long Remove(const VARIANT& Key);
	long ShowDialog();
	void Refresh();
	long Write();
	LPDISPATCH _Item(const VARIANT& Key);
};
/////////////////////////////////////////////////////////////////////////////
// SharedForm wrapper class

class SharedForm : public COleDispatchDriver
{
public:
	SharedForm() {}		// Calls COleDispatchDriver default constructor
	SharedForm(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	SharedForm(const SharedForm& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	CString GetName();
	void SetName(LPCTSTR);
	CString GetValue();
	void SetValue(LPCTSTR);

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// PrinterSystem wrapper class

class PrinterSystem : public COleDispatchDriver
{
public:
	PrinterSystem() {}		// Calls COleDispatchDriver default constructor
	PrinterSystem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	PrinterSystem(const PrinterSystem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH Families();
	LPDISPATCH Models(LPCTSTR Family);
	LPDISPATCH Printers(long KindOfPrinter);
	LPDISPATCH Ports();
	CString Add(LPCTSTR strPrinterName, LPCTSTR strPortName, BOOL DirectAccess);
	void Remove(LPCTSTR strPrinterName);
	void Rename(LPCTSTR strPrinterName, LPCTSTR strNewPrinterName);
	void ChangePort(LPCTSTR strPrinterName, LPCTSTR strPortName, BOOL DirectAccess);
};
/////////////////////////////////////////////////////////////////////////////
// INotifyApplicationEvent wrapper class

class INotifyApplicationEvent : public COleDispatchDriver
{
public:
	INotifyApplicationEvent() {}		// Calls COleDispatchDriver default constructor
	INotifyApplicationEvent(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	INotifyApplicationEvent(const INotifyApplicationEvent& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Quit();
	void Close();
	void DocumentClosed(LPCTSTR strDocTitle);
};
/////////////////////////////////////////////////////////////////////////////
// INotifyDocumentEvent wrapper class

class INotifyDocumentEvent : public COleDispatchDriver
{
public:
	INotifyDocumentEvent() {}		// Calls COleDispatchDriver default constructor
	INotifyDocumentEvent(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	INotifyDocumentEvent(const INotifyDocumentEvent& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Change();
	void BeginPrinting(LPCTSTR strDocName);
	void ProgressPrinting(short Percent, short* Cancel);
	void EndPrinting(long Reason);
	void PausedPrinting(long Reason, short* Cancel);
	void Close();
};
/////////////////////////////////////////////////////////////////////////////
// PickList wrapper class

class PickList : public COleDispatchDriver
{
public:
	PickList() {}		// Calls COleDispatchDriver default constructor
	PickList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	PickList(const PickList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:
	LPDISPATCH GetApplication();
	void SetApplication(LPDISPATCH);
	LPDISPATCH GetParent();
	void SetParent(LPDISPATCH);
	CString GetName();
	void SetName(LPCTSTR);
	long GetShared();
	void SetShared(long);
	short GetDefaultId();
	void SetDefaultId(short);

// Operations
public:
	CString GetValue(short iId);
	void SetValue(short iId, LPCTSTR sValue);
	long AddValue(LPCTSTR sValue);
	long DeleteValue(const VARIANT& vValue);
	short GetSize();
	long ShowDialog(long hParent);
};


#endif // !defined(LPP2_H)