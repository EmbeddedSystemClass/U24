// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "lppx2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// DocObjectProperty properties

CString DocObjectProperty::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void DocObjectProperty::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString DocObjectProperty::Get_Name()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void DocObjectProperty::Set_Name(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long DocObjectProperty::GetDataType()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void DocObjectProperty::SetDataType(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

VARIANT DocObjectProperty::GetValue()
{
	VARIANT result;
	GetProperty(0x3, VT_VARIANT, (void*)&result);
	return result;
}

void DocObjectProperty::SetValue(const VARIANT& propVal)
{
	SetProperty(0x3, VT_VARIANT, &propVal);
}

long DocObjectProperty::GetId()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void DocObjectProperty::SetId(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

CString DocObjectProperty::GetEntries()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void DocObjectProperty::SetEntries(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

BOOL DocObjectProperty::GetLocked()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}

void DocObjectProperty::SetLocked(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}

VARIANT DocObjectProperty::GetTag()
{
	VARIANT result;
	GetProperty(0x7, VT_VARIANT, (void*)&result);
	return result;
}

void DocObjectProperty::SetTag(const VARIANT& propVal)
{
	SetProperty(0x7, VT_VARIANT, &propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DocObjectProperty operations


/////////////////////////////////////////////////////////////////////////////
// IApplication properties

BOOL IApplication::GetVisible()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}

void IApplication::SetVisible(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}

BOOL IApplication::GetUserControl()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void IApplication::SetUserControl(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

LPDISPATCH IApplication::GetDocuments()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void IApplication::SetDocuments(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPDISPATCH IApplication::GetDialogs()
{
	LPDISPATCH result;
	GetProperty(0x4, VT_DISPATCH, (void*)&result);
	return result;
}

void IApplication::SetDialogs(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

LPDISPATCH IApplication::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void IApplication::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

CString IApplication::GetCaption()
{
	CString result;
	GetProperty(0x6, VT_BSTR, (void*)&result);
	return result;
}

void IApplication::SetCaption(LPCTSTR propVal)
{
	SetProperty(0x6, VT_BSTR, propVal);
}

CString IApplication::GetDefaultFilePath()
{
	CString result;
	GetProperty(0x7, VT_BSTR, (void*)&result);
	return result;
}

void IApplication::SetDefaultFilePath(LPCTSTR propVal)
{
	SetProperty(0x7, VT_BSTR, propVal);
}

CString IApplication::GetFullName()
{
	CString result;
	GetProperty(0x8, VT_BSTR, (void*)&result);
	return result;
}

void IApplication::SetFullName(LPCTSTR propVal)
{
	SetProperty(0x8, VT_BSTR, propVal);
}

CString IApplication::GetName()
{
	CString result;
	GetProperty(0x9, VT_BSTR, (void*)&result);
	return result;
}

void IApplication::SetName(LPCTSTR propVal)
{
	SetProperty(0x9, VT_BSTR, propVal);
}

CString IApplication::Get_Name()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void IApplication::Set_Name(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

CString IApplication::GetPath()
{
	CString result;
	GetProperty(0xa, VT_BSTR, (void*)&result);
	return result;
}

void IApplication::SetPath(LPCTSTR propVal)
{
	SetProperty(0xa, VT_BSTR, propVal);
}

LPDISPATCH IApplication::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0xb, VT_DISPATCH, (void*)&result);
	return result;
}

void IApplication::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0xb, VT_DISPATCH, propVal);
}

LPDISPATCH IApplication::GetOptions()
{
	LPDISPATCH result;
	GetProperty(0xc, VT_DISPATCH, (void*)&result);
	return result;
}

void IApplication::SetOptions(LPDISPATCH propVal)
{
	SetProperty(0xc, VT_DISPATCH, propVal);
}

CString IApplication::GetVersion()
{
	CString result;
	GetProperty(0xd, VT_BSTR, (void*)&result);
	return result;
}

void IApplication::SetVersion(LPCTSTR propVal)
{
	SetProperty(0xd, VT_BSTR, propVal);
}

long IApplication::GetWidth()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void IApplication::SetWidth(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

long IApplication::GetHeight()
{
	long result;
	GetProperty(0xf, VT_I4, (void*)&result);
	return result;
}

void IApplication::SetHeight(long propVal)
{
	SetProperty(0xf, VT_I4, propVal);
}

long IApplication::GetLeft()
{
	long result;
	GetProperty(0x10, VT_I4, (void*)&result);
	return result;
}

void IApplication::SetLeft(long propVal)
{
	SetProperty(0x10, VT_I4, propVal);
}

long IApplication::GetTop()
{
	long result;
	GetProperty(0x11, VT_I4, (void*)&result);
	return result;
}

void IApplication::SetTop(long propVal)
{
	SetProperty(0x11, VT_I4, propVal);
}

LPDISPATCH IApplication::GetActiveDocument()
{
	LPDISPATCH result;
	GetProperty(0x12, VT_DISPATCH, (void*)&result);
	return result;
}

void IApplication::SetActiveDocument(LPDISPATCH propVal)
{
	SetProperty(0x12, VT_DISPATCH, propVal);
}

BOOL IApplication::GetLocked()
{
	BOOL result;
	GetProperty(0x13, VT_BOOL, (void*)&result);
	return result;
}

void IApplication::SetLocked(BOOL propVal)
{
	SetProperty(0x13, VT_BOOL, propVal);
}

LPDISPATCH IApplication::GetRecentFiles()
{
	LPDISPATCH result;
	GetProperty(0x14, VT_DISPATCH, (void*)&result);
	return result;
}

void IApplication::SetRecentFiles(LPDISPATCH propVal)
{
	SetProperty(0x14, VT_DISPATCH, propVal);
}

BOOL IApplication::GetEnableEvents()
{
	BOOL result;
	GetProperty(0x15, VT_BOOL, (void*)&result);
	return result;
}

void IApplication::SetEnableEvents(BOOL propVal)
{
	SetProperty(0x15, VT_BOOL, propVal);
}

CString IApplication::GetActivePrinterName()
{
	CString result;
	GetProperty(0x16, VT_BSTR, (void*)&result);
	return result;
}

void IApplication::SetActivePrinterName(LPCTSTR propVal)
{
	SetProperty(0x16, VT_BSTR, propVal);
}

LPDISPATCH IApplication::GetSharedOwner()
{
	LPDISPATCH result;
	GetProperty(0x1e, VT_DISPATCH, (void*)&result);
	return result;
}

void IApplication::SetSharedOwner(LPDISPATCH propVal)
{
	SetProperty(0x1e, VT_DISPATCH, propVal);
}

long IApplication::GetMainWndHandle()
{
	long result;
	GetProperty(0x1f, VT_I4, (void*)&result);
	return result;
}

void IApplication::SetMainWndHandle(long propVal)
{
	SetProperty(0x1f, VT_I4, propVal);
}

BOOL IApplication::GetIsEval()
{
	BOOL result;
	GetProperty(0x20, VT_BOOL, (void*)&result);
	return result;
}

void IApplication::SetIsEval(BOOL propVal)
{
	SetProperty(0x20, VT_BOOL, propVal);
}

long IApplication::GetPid()
{
	long result;
	GetProperty(0x21, VT_I4, (void*)&result);
	return result;
}

void IApplication::SetPid(long propVal)
{
	SetProperty(0x21, VT_I4, propVal);
}

long IApplication::GetProgramCode()
{
	long result;
	GetProperty(0x22, VT_I4, (void*)&result);
	return result;
}

void IApplication::SetProgramCode(long propVal)
{
	SetProperty(0x22, VT_I4, propVal);
}

long IApplication::GetPrintMode()
{
	long result;
	GetProperty(0x23, VT_I4, (void*)&result);
	return result;
}

void IApplication::SetPrintMode(long propVal)
{
	SetProperty(0x23, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IApplication operations

void IApplication::Quit()
{
	InvokeHelper(0x17, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

LPDISPATCH IApplication::PrinterSystem()
{
	LPDISPATCH result;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void IApplication::Resize(long Width, long Height)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Width, Height);
}

void IApplication::Move(long Left, long Top)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top);
}

void IApplication::ShowHelp(LPCTSTR strHelpFile, long HelpContextID)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strHelpFile, HelpContextID);
}

short IApplication::GetLastError()
{
	short result;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

CString IApplication::ErrorMessage(short ErrorCode)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x1d, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		ErrorCode);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Documents properties

short Documents::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void Documents::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH Documents::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Documents::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH Documents::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void Documents::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN Documents::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void Documents::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

CString Documents::GetDefaultExt()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}

void Documents::SetDefaultExt(LPCTSTR propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Documents operations

LPDISPATCH Documents::Add(LPCTSTR Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Key);
	return result;
}

LPDISPATCH Documents::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Documents::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Documents::Open(LPCTSTR strDocName, BOOL ReadOnly)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		strDocName, ReadOnly);
	return result;
}

void Documents::SaveAll(BOOL AlwaysPrompt)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 AlwaysPrompt);
}

void Documents::CloseAll(BOOL Save)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Save);
}

LPDISPATCH Documents::Import(LPCTSTR strDocName, LPCTSTR strFileExt)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		strDocName, strFileExt);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IDocument properties

LPDISPATCH IDocument::GetVariables()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void IDocument::SetVariables(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

CString IDocument::GetName()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void IDocument::SetName(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString IDocument::Get_Name()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void IDocument::Set_Name(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

LPDISPATCH IDocument::GetDocObjects()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void IDocument::SetDocObjects(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPDISPATCH IDocument::GetPrinter()
{
	LPDISPATCH result;
	GetProperty(0x4, VT_DISPATCH, (void*)&result);
	return result;
}

void IDocument::SetPrinter(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

LPDISPATCH IDocument::GetFormat()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void IDocument::SetFormat(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

LPDISPATCH IDocument::GetDatabase()
{
	LPDISPATCH result;
	GetProperty(0x6, VT_DISPATCH, (void*)&result);
	return result;
}

void IDocument::SetDatabase(LPDISPATCH propVal)
{
	SetProperty(0x6, VT_DISPATCH, propVal);
}

LPDISPATCH IDocument::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x7, VT_DISPATCH, (void*)&result);
	return result;
}

void IDocument::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x7, VT_DISPATCH, propVal);
}

LPDISPATCH IDocument::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x8, VT_DISPATCH, (void*)&result);
	return result;
}

void IDocument::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x8, VT_DISPATCH, propVal);
}

CString IDocument::GetFullName()
{
	CString result;
	GetProperty(0x9, VT_BSTR, (void*)&result);
	return result;
}

void IDocument::SetFullName(LPCTSTR propVal)
{
	SetProperty(0x9, VT_BSTR, propVal);
}

BOOL IDocument::GetReadOnly()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}

void IDocument::SetReadOnly(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}

long IDocument::GetViewMode()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void IDocument::SetViewMode(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

long IDocument::GetWindowState()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}

void IDocument::SetWindowState(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}

LPDISPATCH IDocument::GetBuiltInDocumentProperties()
{
	LPDISPATCH result;
	GetProperty(0xd, VT_DISPATCH, (void*)&result);
	return result;
}

void IDocument::SetBuiltInDocumentProperties(LPDISPATCH propVal)
{
	SetProperty(0xd, VT_DISPATCH, propVal);
}

long IDocument::GetTriggerForm()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void IDocument::SetTriggerForm(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

long IDocument::GetViewOrientation()
{
	long result;
	GetProperty(0xf, VT_I4, (void*)&result);
	return result;
}

void IDocument::SetViewOrientation(long propVal)
{
	SetProperty(0xf, VT_I4, propVal);
}

BOOL IDocument::GetIsModified()
{
	BOOL result;
	GetProperty(0x10, VT_BOOL, (void*)&result);
	return result;
}

void IDocument::SetIsModified(BOOL propVal)
{
	SetProperty(0x10, VT_BOOL, propVal);
}

LPDISPATCH IDocument::GetSelectedDocObjects()
{
	LPDISPATCH result;
	GetProperty(0x1d, VT_DISPATCH, (void*)&result);
	return result;
}

void IDocument::SetSelectedDocObjects(LPDISPATCH propVal)
{
	SetProperty(0x1d, VT_DISPATCH, propVal);
}

BOOL IDocument::GetAutoPageIncrement()
{
	BOOL result;
	GetProperty(0x1e, VT_BOOL, (void*)&result);
	return result;
}

void IDocument::SetAutoPageIncrement(BOOL propVal)
{
	SetProperty(0x1e, VT_BOOL, propVal);
}

long IDocument::GetHorzPrintOffset()
{
	long result;
	GetProperty(0x22, VT_I4, (void*)&result);
	return result;
}

void IDocument::SetHorzPrintOffset(long propVal)
{
	SetProperty(0x22, VT_I4, propVal);
}

long IDocument::GetVertPrintOffset()
{
	long result;
	GetProperty(0x23, VT_I4, (void*)&result);
	return result;
}

void IDocument::SetVertPrintOffset(long propVal)
{
	SetProperty(0x23, VT_I4, propVal);
}

CString IDocument::GetRFTagList()
{
	CString result;
	GetProperty(0x24, VT_BSTR, (void*)&result);
	return result;
}

void IDocument::SetRFTagList(LPCTSTR propVal)
{
	SetProperty(0x24, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IDocument operations

short IDocument::Save()
{
	short result;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

short IDocument::SaveAs(LPCTSTR strPathName)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		strPathName);
	return result;
}

short IDocument::PrintDocument(long Quantity)
{
	short result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Quantity);
	return result;
}

short IDocument::PrintLabel(long Quantity, long LabelCopy, long InterCut, long PageCopy, long NoFrom, LPCTSTR FileName)
{
	short result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BSTR;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Quantity, LabelCopy, InterCut, PageCopy, NoFrom, FileName);
	return result;
}

short IDocument::GeneratePOF(LPCTSTR DestFileName, LPCTSTR ModeleFileName)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		DestFileName, ModeleFileName);
	return result;
}

short IDocument::Merge(long Quantity, long LabelCopy, long InterCut, long PageCopy, long NoFrom, LPCTSTR FileName)
{
	short result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_BSTR;
	InvokeHelper(0x16, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		Quantity, LabelCopy, InterCut, PageCopy, NoFrom, FileName);
	return result;
}

short IDocument::Insert(LPCTSTR strPathName)
{
	short result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x17, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		strPathName);
	return result;
}

BOOL IDocument::CopyToClipboard()
{
	BOOL result;
	InvokeHelper(0x18, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

short IDocument::FormFeed()
{
	short result;
	InvokeHelper(0x19, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

void IDocument::Close(BOOL Save)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x1a, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Save);
}

void IDocument::Activate()
{
	InvokeHelper(0x1b, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString IDocument::CopyImageToFile(short Colors, LPCTSTR Extension, short Rotation, short Percent, LPCTSTR FileName)
{
	CString result;
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_I2 VTS_I2 VTS_BSTR;
	InvokeHelper(0x1c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Colors, Extension, Rotation, Percent, FileName);
	return result;
}

short IDocument::WriteVariables(const VARIANT& varArray)
{
	short result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x1f, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		&varArray);
	return result;
}

VARIANT IDocument::ReadVariables(long iType)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x20, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		iType);
	return result;
}

void IDocument::Passwords(LPCTSTR strOwnerPassword, LPCTSTR strPrintPassword)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x21, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strOwnerPassword, strPrintPassword);
}

BOOL IDocument::NewRFTag(long IDRfTag, long Reserved)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x25, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		IDRfTag, Reserved);
	return result;
}

LPDISPATCH IDocument::RFTag(long Reserved)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x26, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Reserved);
	return result;
}

BOOL IDocument::SelectDatabase(LPCTSTR strName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x27, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strName);
	return result;
}

BOOL IDocument::CopyToClipboardEx(BOOL Border, short nMethod, short Colors, short Rotation, short Percent)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BOOL VTS_I2 VTS_I2 VTS_I2 VTS_I2;
	InvokeHelper(0x28, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Border, nMethod, Colors, Rotation, Percent);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Variables properties

LPDISPATCH Variables::GetFreeVariables()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void Variables::SetFreeVariables(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

LPDISPATCH Variables::GetFormVariables()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Variables::SetFormVariables(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH Variables::GetDatabaseVariables()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void Variables::SetDatabaseVariables(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPDISPATCH Variables::GetCounters()
{
	LPDISPATCH result;
	GetProperty(0x4, VT_DISPATCH, (void*)&result);
	return result;
}

void Variables::SetCounters(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

LPDISPATCH Variables::GetFormulas()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void Variables::SetFormulas(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

LPDISPATCH Variables::GetTableLookups()
{
	LPDISPATCH result;
	GetProperty(0x6, VT_DISPATCH, (void*)&result);
	return result;
}

void Variables::SetTableLookups(LPDISPATCH propVal)
{
	SetProperty(0x6, VT_DISPATCH, propVal);
}

LPDISPATCH Variables::GetDates()
{
	LPDISPATCH result;
	GetProperty(0x7, VT_DISPATCH, (void*)&result);
	return result;
}

void Variables::SetDates(LPDISPATCH propVal)
{
	SetProperty(0x7, VT_DISPATCH, propVal);
}

short Variables::GetCount()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}

void Variables::SetCount(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}

LPDISPATCH Variables::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x9, VT_DISPATCH, (void*)&result);
	return result;
}

void Variables::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x9, VT_DISPATCH, propVal);
}

LPDISPATCH Variables::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0xa, VT_DISPATCH, (void*)&result);
	return result;
}

void Variables::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0xa, VT_DISPATCH, propVal);
}

LPUNKNOWN Variables::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void Variables::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Variables operations

LPDISPATCH Variables::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Variables::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Variables::Add(const VARIANT& Item1, const VARIANT& Item2)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Item1, &Item2);
	return result;
}

void Variables::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}

void Variables::Refresh()
{
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// FreeVariables properties

short FreeVariables::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void FreeVariables::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH FreeVariables::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void FreeVariables::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH FreeVariables::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void FreeVariables::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN FreeVariables::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void FreeVariables::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// FreeVariables operations

LPDISPATCH FreeVariables::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH FreeVariables::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH FreeVariables::Add(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void FreeVariables::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// Free properties

CString Free::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString Free::GetValue()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetValue(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString Free::Get_Value()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void Free::Set_Value(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long Free::GetDataSource()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void Free::SetDataSource(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

LPDISPATCH Free::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x4, VT_DISPATCH, (void*)&result);
	return result;
}

void Free::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

LPDISPATCH Free::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void Free::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

CString Free::GetDisplayValue()
{
	CString result;
	GetProperty(0x7, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetDisplayValue(LPCTSTR propVal)
{
	SetProperty(0x7, VT_BSTR, propVal);
}

VARIANT Free::GetIncrement()
{
	VARIANT result;
	GetProperty(0x101, VT_VARIANT, (void*)&result);
	return result;
}

void Free::SetIncrement(const VARIANT& propVal)
{
	SetProperty(0x101, VT_VARIANT, &propVal);
}

long Free::GetIso()
{
	long result;
	GetProperty(0x102, VT_I4, (void*)&result);
	return result;
}

void Free::SetIso(long propVal)
{
	SetProperty(0x102, VT_I4, propVal);
}

long Free::GetBaseType()
{
	long result;
	GetProperty(0x103, VT_I4, (void*)&result);
	return result;
}

void Free::SetBaseType(long propVal)
{
	SetProperty(0x103, VT_I4, propVal);
}

long Free::GetTriggerMode()
{
	long result;
	GetProperty(0x104, VT_I4, (void*)&result);
	return result;
}

void Free::SetTriggerMode(long propVal)
{
	SetProperty(0x104, VT_I4, propVal);
}

VARIANT Free::GetTriggerParameter()
{
	VARIANT result;
	GetProperty(0x105, VT_VARIANT, (void*)&result);
	return result;
}

void Free::SetTriggerParameter(const VARIANT& propVal)
{
	SetProperty(0x105, VT_VARIANT, &propVal);
}

CString Free::GetCustomSet()
{
	CString result;
	GetProperty(0x106, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetCustomSet(LPCTSTR propVal)
{
	SetProperty(0x106, VT_BSTR, propVal);
}

VARIANT Free::GetMaxValue()
{
	VARIANT result;
	GetProperty(0x107, VT_VARIANT, (void*)&result);
	return result;
}

void Free::SetMaxValue(const VARIANT& propVal)
{
	SetProperty(0x107, VT_VARIANT, &propVal);
}

VARIANT Free::GetResetToValue()
{
	VARIANT result;
	GetProperty(0x108, VT_VARIANT, (void*)&result);
	return result;
}

void Free::SetResetToValue(const VARIANT& propVal)
{
	SetProperty(0x108, VT_VARIANT, &propVal);
}

CString Free::GetPrefix()
{
	CString result;
	GetProperty(0x109, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetPrefix(LPCTSTR propVal)
{
	SetProperty(0x109, VT_BSTR, propVal);
}

CString Free::GetSuffix()
{
	CString result;
	GetProperty(0x10a, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetSuffix(LPCTSTR propVal)
{
	SetProperty(0x10a, VT_BSTR, propVal);
}

CString Free::GetPadCharacter()
{
	CString result;
	GetProperty(0x10b, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetPadCharacter(LPCTSTR propVal)
{
	SetProperty(0x10b, VT_BSTR, propVal);
}

BOOL Free::GetDecimalUse()
{
	BOOL result;
	GetProperty(0x10c, VT_BOOL, (void*)&result);
	return result;
}

void Free::SetDecimalUse(BOOL propVal)
{
	SetProperty(0x10c, VT_BOOL, propVal);
}

short Free::GetNumberOfDecimals()
{
	short result;
	GetProperty(0x10d, VT_I2, (void*)&result);
	return result;
}

void Free::SetNumberOfDecimals(short propVal)
{
	SetProperty(0x10d, VT_I2, propVal);
}

CString Free::GetDecimalSeparator()
{
	CString result;
	GetProperty(0x10e, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetDecimalSeparator(LPCTSTR propVal)
{
	SetProperty(0x10e, VT_BSTR, propVal);
}

CString Free::GetThousandSeparator()
{
	CString result;
	GetProperty(0x10f, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetThousandSeparator(LPCTSTR propVal)
{
	SetProperty(0x10f, VT_BSTR, propVal);
}

long Free::GetLabelCopies()
{
	long result;
	GetProperty(0x110, VT_I4, (void*)&result);
	return result;
}

void Free::SetLabelCopies(long propVal)
{
	SetProperty(0x110, VT_I4, propVal);
}

CString Free::GetOutputMask()
{
	CString result;
	GetProperty(0x111, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetOutputMask(LPCTSTR propVal)
{
	SetProperty(0x111, VT_BSTR, propVal);
}

CString Free::GetAccumulatorFile()
{
	CString result;
	GetProperty(0x112, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetAccumulatorFile(LPCTSTR propVal)
{
	SetProperty(0x112, VT_BSTR, propVal);
}

long Free::GetIncludeInLogFile()
{
	long result;
	GetProperty(0x113, VT_I4, (void*)&result);
	return result;
}

void Free::SetIncludeInLogFile(long propVal)
{
	SetProperty(0x113, VT_I4, propVal);
}

long Free::GetSharedCounter()
{
	long result;
	GetProperty(0x114, VT_I4, (void*)&result);
	return result;
}

void Free::SetSharedCounter(long propVal)
{
	SetProperty(0x114, VT_I4, propVal);
}

long Free::GetCounterUse()
{
	long result;
	GetProperty(0x201, VT_I4, (void*)&result);
	return result;
}

void Free::SetCounterUse(long propVal)
{
	SetProperty(0x201, VT_I4, propVal);
}

short Free::GetFormOrder()
{
	short result;
	GetProperty(0x202, VT_I2, (void*)&result);
	return result;
}

void Free::SetFormOrder(short propVal)
{
	SetProperty(0x202, VT_I2, propVal);
}

CString Free::GetFormPrompt()
{
	CString result;
	GetProperty(0x203, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetFormPrompt(LPCTSTR propVal)
{
	SetProperty(0x203, VT_BSTR, propVal);
}

BOOL Free::GetDisplayInForm()
{
	BOOL result;
	GetProperty(0x204, VT_BOOL, (void*)&result);
	return result;
}

void Free::SetDisplayInForm(BOOL propVal)
{
	SetProperty(0x204, VT_BOOL, propVal);
}

CString Free::GetInputMask()
{
	CString result;
	GetProperty(0x205, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetInputMask(LPCTSTR propVal)
{
	SetProperty(0x205, VT_BSTR, propVal);
}

BOOL Free::GetShared()
{
	BOOL result;
	GetProperty(0x206, VT_BOOL, (void*)&result);
	return result;
}

void Free::SetShared(BOOL propVal)
{
	SetProperty(0x206, VT_BOOL, propVal);
}

long Free::GetLength()
{
	long result;
	GetProperty(0x207, VT_I4, (void*)&result);
	return result;
}

void Free::SetLength(long propVal)
{
	SetProperty(0x207, VT_I4, propVal);
}

short Free::GetPadLength()
{
	short result;
	GetProperty(0x208, VT_I2, (void*)&result);
	return result;
}

void Free::SetPadLength(short propVal)
{
	SetProperty(0x208, VT_I2, propVal);
}

BOOL Free::GetFromAFile()
{
	BOOL result;
	GetProperty(0x209, VT_BOOL, (void*)&result);
	return result;
}

void Free::SetFromAFile(BOOL propVal)
{
	SetProperty(0x209, VT_BOOL, propVal);
}

CString Free::GetFileName()
{
	CString result;
	GetProperty(0x20a, VT_BSTR, (void*)&result);
	return result;
}

void Free::SetFileName(LPCTSTR propVal)
{
	SetProperty(0x20a, VT_BSTR, propVal);
}

BOOL Free::GetClearAfterPrint()
{
	BOOL result;
	GetProperty(0x20b, VT_BOOL, (void*)&result);
	return result;
}

void Free::SetClearAfterPrint(BOOL propVal)
{
	SetProperty(0x20b, VT_BOOL, propVal);
}

BOOL Free::GetRestoreAfterPrinting()
{
	BOOL result;
	GetProperty(0x20c, VT_BOOL, (void*)&result);
	return result;
}

void Free::SetRestoreAfterPrinting(BOOL propVal)
{
	SetProperty(0x20c, VT_BOOL, propVal);
}

long Free::GetUpdateAfterPrinting()
{
	long result;
	GetProperty(0x20d, VT_I4, (void*)&result);
	return result;
}

void Free::SetUpdateAfterPrinting(long propVal)
{
	SetProperty(0x20d, VT_I4, propVal);
}

short Free::GetInputMiniLength()
{
	short result;
	GetProperty(0x20e, VT_I2, (void*)&result);
	return result;
}

void Free::SetInputMiniLength(short propVal)
{
	SetProperty(0x20e, VT_I2, propVal);
}

long Free::GetRequired()
{
	long result;
	GetProperty(0x20f, VT_I4, (void*)&result);
	return result;
}

void Free::SetRequired(long propVal)
{
	SetProperty(0x20f, VT_I4, propVal);
}

BOOL Free::GetPickListUse()
{
	BOOL result;
	GetProperty(0x210, VT_BOOL, (void*)&result);
	return result;
}

void Free::SetPickListUse(BOOL propVal)
{
	SetProperty(0x210, VT_BOOL, propVal);
}

BOOL Free::GetValueFromPickList()
{
	BOOL result;
	GetProperty(0x211, VT_BOOL, (void*)&result);
	return result;
}

void Free::SetValueFromPickList(BOOL propVal)
{
	SetProperty(0x211, VT_BOOL, propVal);
}

BOOL Free::GetSkipSpaces()
{
	BOOL result;
	GetProperty(0x212, VT_BOOL, (void*)&result);
	return result;
}

void Free::SetSkipSpaces(BOOL propVal)
{
	SetProperty(0x212, VT_BOOL, propVal);
}

VARIANT Free::GetRestoreValue()
{
	VARIANT result;
	GetProperty(0x213, VT_VARIANT, (void*)&result);
	return result;
}

void Free::SetRestoreValue(const VARIANT& propVal)
{
	SetProperty(0x213, VT_VARIANT, &propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Free operations

long Free::ShowDialog(long hParent)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hParent);
	return result;
}

LPDISPATCH Free::GetPickList()
{
	LPDISPATCH result;
	InvokeHelper(0x230, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// FormVariables properties

short FormVariables::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void FormVariables::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH FormVariables::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void FormVariables::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH FormVariables::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void FormVariables::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN FormVariables::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void FormVariables::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// FormVariables operations

LPDISPATCH FormVariables::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH FormVariables::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH FormVariables::Add(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void FormVariables::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// DatabaseVariables properties

short DatabaseVariables::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void DatabaseVariables::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH DatabaseVariables::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void DatabaseVariables::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH DatabaseVariables::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void DatabaseVariables::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN DatabaseVariables::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void DatabaseVariables::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DatabaseVariables operations

LPDISPATCH DatabaseVariables::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH DatabaseVariables::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH DatabaseVariables::Add(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void DatabaseVariables::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// Counters properties

short Counters::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void Counters::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH Counters::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Counters::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH Counters::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void Counters::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN Counters::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void Counters::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Counters operations

LPDISPATCH Counters::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Counters::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Counters::Add(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void Counters::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// Counter properties

CString Counter::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void Counter::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString Counter::GetValue()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void Counter::SetValue(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString Counter::Get_Value()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void Counter::Set_Value(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long Counter::GetDataSource()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void Counter::SetDataSource(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

LPDISPATCH Counter::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x4, VT_DISPATCH, (void*)&result);
	return result;
}

void Counter::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

LPDISPATCH Counter::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void Counter::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

CString Counter::GetDisplayValue()
{
	CString result;
	GetProperty(0x7, VT_BSTR, (void*)&result);
	return result;
}

void Counter::SetDisplayValue(LPCTSTR propVal)
{
	SetProperty(0x7, VT_BSTR, propVal);
}

VARIANT Counter::GetIncrement()
{
	VARIANT result;
	GetProperty(0x101, VT_VARIANT, (void*)&result);
	return result;
}

void Counter::SetIncrement(const VARIANT& propVal)
{
	SetProperty(0x101, VT_VARIANT, &propVal);
}

long Counter::GetIso()
{
	long result;
	GetProperty(0x102, VT_I4, (void*)&result);
	return result;
}

void Counter::SetIso(long propVal)
{
	SetProperty(0x102, VT_I4, propVal);
}

long Counter::GetBaseType()
{
	long result;
	GetProperty(0x103, VT_I4, (void*)&result);
	return result;
}

void Counter::SetBaseType(long propVal)
{
	SetProperty(0x103, VT_I4, propVal);
}

long Counter::GetTriggerMode()
{
	long result;
	GetProperty(0x104, VT_I4, (void*)&result);
	return result;
}

void Counter::SetTriggerMode(long propVal)
{
	SetProperty(0x104, VT_I4, propVal);
}

VARIANT Counter::GetTriggerParameter()
{
	VARIANT result;
	GetProperty(0x105, VT_VARIANT, (void*)&result);
	return result;
}

void Counter::SetTriggerParameter(const VARIANT& propVal)
{
	SetProperty(0x105, VT_VARIANT, &propVal);
}

CString Counter::GetCustomSet()
{
	CString result;
	GetProperty(0x106, VT_BSTR, (void*)&result);
	return result;
}

void Counter::SetCustomSet(LPCTSTR propVal)
{
	SetProperty(0x106, VT_BSTR, propVal);
}

VARIANT Counter::GetMaxValue()
{
	VARIANT result;
	GetProperty(0x107, VT_VARIANT, (void*)&result);
	return result;
}

void Counter::SetMaxValue(const VARIANT& propVal)
{
	SetProperty(0x107, VT_VARIANT, &propVal);
}

VARIANT Counter::GetResetToValue()
{
	VARIANT result;
	GetProperty(0x108, VT_VARIANT, (void*)&result);
	return result;
}

void Counter::SetResetToValue(const VARIANT& propVal)
{
	SetProperty(0x108, VT_VARIANT, &propVal);
}

CString Counter::GetPrefix()
{
	CString result;
	GetProperty(0x109, VT_BSTR, (void*)&result);
	return result;
}

void Counter::SetPrefix(LPCTSTR propVal)
{
	SetProperty(0x109, VT_BSTR, propVal);
}

CString Counter::GetSuffix()
{
	CString result;
	GetProperty(0x10a, VT_BSTR, (void*)&result);
	return result;
}

void Counter::SetSuffix(LPCTSTR propVal)
{
	SetProperty(0x10a, VT_BSTR, propVal);
}

CString Counter::GetPadCharacter()
{
	CString result;
	GetProperty(0x10b, VT_BSTR, (void*)&result);
	return result;
}

void Counter::SetPadCharacter(LPCTSTR propVal)
{
	SetProperty(0x10b, VT_BSTR, propVal);
}

BOOL Counter::GetDecimalUse()
{
	BOOL result;
	GetProperty(0x10c, VT_BOOL, (void*)&result);
	return result;
}

void Counter::SetDecimalUse(BOOL propVal)
{
	SetProperty(0x10c, VT_BOOL, propVal);
}

short Counter::GetNumberOfDecimals()
{
	short result;
	GetProperty(0x10d, VT_I2, (void*)&result);
	return result;
}

void Counter::SetNumberOfDecimals(short propVal)
{
	SetProperty(0x10d, VT_I2, propVal);
}

CString Counter::GetDecimalSeparator()
{
	CString result;
	GetProperty(0x10e, VT_BSTR, (void*)&result);
	return result;
}

void Counter::SetDecimalSeparator(LPCTSTR propVal)
{
	SetProperty(0x10e, VT_BSTR, propVal);
}

CString Counter::GetThousandSeparator()
{
	CString result;
	GetProperty(0x10f, VT_BSTR, (void*)&result);
	return result;
}

void Counter::SetThousandSeparator(LPCTSTR propVal)
{
	SetProperty(0x10f, VT_BSTR, propVal);
}

long Counter::GetLabelCopies()
{
	long result;
	GetProperty(0x110, VT_I4, (void*)&result);
	return result;
}

void Counter::SetLabelCopies(long propVal)
{
	SetProperty(0x110, VT_I4, propVal);
}

CString Counter::GetOutputMask()
{
	CString result;
	GetProperty(0x111, VT_BSTR, (void*)&result);
	return result;
}

void Counter::SetOutputMask(LPCTSTR propVal)
{
	SetProperty(0x111, VT_BSTR, propVal);
}

CString Counter::GetAccumulatorFile()
{
	CString result;
	GetProperty(0x112, VT_BSTR, (void*)&result);
	return result;
}

void Counter::SetAccumulatorFile(LPCTSTR propVal)
{
	SetProperty(0x112, VT_BSTR, propVal);
}

long Counter::GetIncludeInLogFile()
{
	long result;
	GetProperty(0x113, VT_I4, (void*)&result);
	return result;
}

void Counter::SetIncludeInLogFile(long propVal)
{
	SetProperty(0x113, VT_I4, propVal);
}

long Counter::GetSharedCounter()
{
	long result;
	GetProperty(0x114, VT_I4, (void*)&result);
	return result;
}

void Counter::SetSharedCounter(long propVal)
{
	SetProperty(0x114, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Counter operations

long Counter::ShowDialog(long hParent)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hParent);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Formulas properties

short Formulas::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void Formulas::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH Formulas::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Formulas::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH Formulas::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void Formulas::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN Formulas::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void Formulas::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Formulas operations

LPDISPATCH Formulas::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Formulas::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Formulas::Add(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void Formulas::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// Formula properties

CString Formula::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void Formula::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString Formula::GetValue()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void Formula::SetValue(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString Formula::Get_Value()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void Formula::Set_Value(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long Formula::GetDataSource()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void Formula::SetDataSource(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

LPDISPATCH Formula::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x4, VT_DISPATCH, (void*)&result);
	return result;
}

void Formula::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

LPDISPATCH Formula::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void Formula::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

CString Formula::GetDisplayValue()
{
	CString result;
	GetProperty(0x7, VT_BSTR, (void*)&result);
	return result;
}

void Formula::SetDisplayValue(LPCTSTR propVal)
{
	SetProperty(0x7, VT_BSTR, propVal);
}

VARIANT Formula::GetIncrement()
{
	VARIANT result;
	GetProperty(0x101, VT_VARIANT, (void*)&result);
	return result;
}

void Formula::SetIncrement(const VARIANT& propVal)
{
	SetProperty(0x101, VT_VARIANT, &propVal);
}

long Formula::GetIso()
{
	long result;
	GetProperty(0x102, VT_I4, (void*)&result);
	return result;
}

void Formula::SetIso(long propVal)
{
	SetProperty(0x102, VT_I4, propVal);
}

long Formula::GetBaseType()
{
	long result;
	GetProperty(0x103, VT_I4, (void*)&result);
	return result;
}

void Formula::SetBaseType(long propVal)
{
	SetProperty(0x103, VT_I4, propVal);
}

long Formula::GetTriggerMode()
{
	long result;
	GetProperty(0x104, VT_I4, (void*)&result);
	return result;
}

void Formula::SetTriggerMode(long propVal)
{
	SetProperty(0x104, VT_I4, propVal);
}

VARIANT Formula::GetTriggerParameter()
{
	VARIANT result;
	GetProperty(0x105, VT_VARIANT, (void*)&result);
	return result;
}

void Formula::SetTriggerParameter(const VARIANT& propVal)
{
	SetProperty(0x105, VT_VARIANT, &propVal);
}

CString Formula::GetCustomSet()
{
	CString result;
	GetProperty(0x106, VT_BSTR, (void*)&result);
	return result;
}

void Formula::SetCustomSet(LPCTSTR propVal)
{
	SetProperty(0x106, VT_BSTR, propVal);
}

VARIANT Formula::GetMaxValue()
{
	VARIANT result;
	GetProperty(0x107, VT_VARIANT, (void*)&result);
	return result;
}

void Formula::SetMaxValue(const VARIANT& propVal)
{
	SetProperty(0x107, VT_VARIANT, &propVal);
}

VARIANT Formula::GetResetToValue()
{
	VARIANT result;
	GetProperty(0x108, VT_VARIANT, (void*)&result);
	return result;
}

void Formula::SetResetToValue(const VARIANT& propVal)
{
	SetProperty(0x108, VT_VARIANT, &propVal);
}

CString Formula::GetPrefix()
{
	CString result;
	GetProperty(0x109, VT_BSTR, (void*)&result);
	return result;
}

void Formula::SetPrefix(LPCTSTR propVal)
{
	SetProperty(0x109, VT_BSTR, propVal);
}

CString Formula::GetSuffix()
{
	CString result;
	GetProperty(0x10a, VT_BSTR, (void*)&result);
	return result;
}

void Formula::SetSuffix(LPCTSTR propVal)
{
	SetProperty(0x10a, VT_BSTR, propVal);
}

CString Formula::GetPadCharacter()
{
	CString result;
	GetProperty(0x10b, VT_BSTR, (void*)&result);
	return result;
}

void Formula::SetPadCharacter(LPCTSTR propVal)
{
	SetProperty(0x10b, VT_BSTR, propVal);
}

BOOL Formula::GetDecimalUse()
{
	BOOL result;
	GetProperty(0x10c, VT_BOOL, (void*)&result);
	return result;
}

void Formula::SetDecimalUse(BOOL propVal)
{
	SetProperty(0x10c, VT_BOOL, propVal);
}

short Formula::GetNumberOfDecimals()
{
	short result;
	GetProperty(0x10d, VT_I2, (void*)&result);
	return result;
}

void Formula::SetNumberOfDecimals(short propVal)
{
	SetProperty(0x10d, VT_I2, propVal);
}

CString Formula::GetDecimalSeparator()
{
	CString result;
	GetProperty(0x10e, VT_BSTR, (void*)&result);
	return result;
}

void Formula::SetDecimalSeparator(LPCTSTR propVal)
{
	SetProperty(0x10e, VT_BSTR, propVal);
}

CString Formula::GetThousandSeparator()
{
	CString result;
	GetProperty(0x10f, VT_BSTR, (void*)&result);
	return result;
}

void Formula::SetThousandSeparator(LPCTSTR propVal)
{
	SetProperty(0x10f, VT_BSTR, propVal);
}

long Formula::GetLabelCopies()
{
	long result;
	GetProperty(0x110, VT_I4, (void*)&result);
	return result;
}

void Formula::SetLabelCopies(long propVal)
{
	SetProperty(0x110, VT_I4, propVal);
}

CString Formula::GetOutputMask()
{
	CString result;
	GetProperty(0x111, VT_BSTR, (void*)&result);
	return result;
}

void Formula::SetOutputMask(LPCTSTR propVal)
{
	SetProperty(0x111, VT_BSTR, propVal);
}

CString Formula::GetAccumulatorFile()
{
	CString result;
	GetProperty(0x112, VT_BSTR, (void*)&result);
	return result;
}

void Formula::SetAccumulatorFile(LPCTSTR propVal)
{
	SetProperty(0x112, VT_BSTR, propVal);
}

long Formula::GetIncludeInLogFile()
{
	long result;
	GetProperty(0x113, VT_I4, (void*)&result);
	return result;
}

void Formula::SetIncludeInLogFile(long propVal)
{
	SetProperty(0x113, VT_I4, propVal);
}

long Formula::GetSharedCounter()
{
	long result;
	GetProperty(0x114, VT_I4, (void*)&result);
	return result;
}

void Formula::SetSharedCounter(long propVal)
{
	SetProperty(0x114, VT_I4, propVal);
}

long Formula::GetCounterUse()
{
	long result;
	GetProperty(0x201, VT_I4, (void*)&result);
	return result;
}

void Formula::SetCounterUse(long propVal)
{
	SetProperty(0x201, VT_I4, propVal);
}

CString Formula::GetExpression()
{
	CString result;
	GetProperty(0x202, VT_BSTR, (void*)&result);
	return result;
}

void Formula::SetExpression(LPCTSTR propVal)
{
	SetProperty(0x202, VT_BSTR, propVal);
}

long Formula::GetLength()
{
	long result;
	GetProperty(0x203, VT_I4, (void*)&result);
	return result;
}

void Formula::SetLength(long propVal)
{
	SetProperty(0x203, VT_I4, propVal);
}

short Formula::GetPadLength()
{
	short result;
	GetProperty(0x204, VT_I2, (void*)&result);
	return result;
}

void Formula::SetPadLength(short propVal)
{
	SetProperty(0x204, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Formula operations

long Formula::ShowDialog(long hParent)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hParent);
	return result;
}

BOOL Formula::Test()
{
	BOOL result;
	InvokeHelper(0x205, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// TableLookups properties

short TableLookups::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void TableLookups::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH TableLookups::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void TableLookups::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH TableLookups::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void TableLookups::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN TableLookups::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void TableLookups::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// TableLookups operations

LPDISPATCH TableLookups::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH TableLookups::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH TableLookups::Add(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void TableLookups::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// TableLookup properties

CString TableLookup::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString TableLookup::GetValue()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetValue(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString TableLookup::Get_Value()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::Set_Value(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long TableLookup::GetDataSource()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void TableLookup::SetDataSource(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

LPDISPATCH TableLookup::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x4, VT_DISPATCH, (void*)&result);
	return result;
}

void TableLookup::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

LPDISPATCH TableLookup::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void TableLookup::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

CString TableLookup::GetDisplayValue()
{
	CString result;
	GetProperty(0x7, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetDisplayValue(LPCTSTR propVal)
{
	SetProperty(0x7, VT_BSTR, propVal);
}

VARIANT TableLookup::GetIncrement()
{
	VARIANT result;
	GetProperty(0x101, VT_VARIANT, (void*)&result);
	return result;
}

void TableLookup::SetIncrement(const VARIANT& propVal)
{
	SetProperty(0x101, VT_VARIANT, &propVal);
}

long TableLookup::GetIso()
{
	long result;
	GetProperty(0x102, VT_I4, (void*)&result);
	return result;
}

void TableLookup::SetIso(long propVal)
{
	SetProperty(0x102, VT_I4, propVal);
}

long TableLookup::GetBaseType()
{
	long result;
	GetProperty(0x103, VT_I4, (void*)&result);
	return result;
}

void TableLookup::SetBaseType(long propVal)
{
	SetProperty(0x103, VT_I4, propVal);
}

long TableLookup::GetTriggerMode()
{
	long result;
	GetProperty(0x104, VT_I4, (void*)&result);
	return result;
}

void TableLookup::SetTriggerMode(long propVal)
{
	SetProperty(0x104, VT_I4, propVal);
}

VARIANT TableLookup::GetTriggerParameter()
{
	VARIANT result;
	GetProperty(0x105, VT_VARIANT, (void*)&result);
	return result;
}

void TableLookup::SetTriggerParameter(const VARIANT& propVal)
{
	SetProperty(0x105, VT_VARIANT, &propVal);
}

CString TableLookup::GetCustomSet()
{
	CString result;
	GetProperty(0x106, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetCustomSet(LPCTSTR propVal)
{
	SetProperty(0x106, VT_BSTR, propVal);
}

VARIANT TableLookup::GetMaxValue()
{
	VARIANT result;
	GetProperty(0x107, VT_VARIANT, (void*)&result);
	return result;
}

void TableLookup::SetMaxValue(const VARIANT& propVal)
{
	SetProperty(0x107, VT_VARIANT, &propVal);
}

VARIANT TableLookup::GetResetToValue()
{
	VARIANT result;
	GetProperty(0x108, VT_VARIANT, (void*)&result);
	return result;
}

void TableLookup::SetResetToValue(const VARIANT& propVal)
{
	SetProperty(0x108, VT_VARIANT, &propVal);
}

CString TableLookup::GetPrefix()
{
	CString result;
	GetProperty(0x109, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetPrefix(LPCTSTR propVal)
{
	SetProperty(0x109, VT_BSTR, propVal);
}

CString TableLookup::GetSuffix()
{
	CString result;
	GetProperty(0x10a, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetSuffix(LPCTSTR propVal)
{
	SetProperty(0x10a, VT_BSTR, propVal);
}

CString TableLookup::GetPadCharacter()
{
	CString result;
	GetProperty(0x10b, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetPadCharacter(LPCTSTR propVal)
{
	SetProperty(0x10b, VT_BSTR, propVal);
}

BOOL TableLookup::GetDecimalUse()
{
	BOOL result;
	GetProperty(0x10c, VT_BOOL, (void*)&result);
	return result;
}

void TableLookup::SetDecimalUse(BOOL propVal)
{
	SetProperty(0x10c, VT_BOOL, propVal);
}

short TableLookup::GetNumberOfDecimals()
{
	short result;
	GetProperty(0x10d, VT_I2, (void*)&result);
	return result;
}

void TableLookup::SetNumberOfDecimals(short propVal)
{
	SetProperty(0x10d, VT_I2, propVal);
}

CString TableLookup::GetDecimalSeparator()
{
	CString result;
	GetProperty(0x10e, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetDecimalSeparator(LPCTSTR propVal)
{
	SetProperty(0x10e, VT_BSTR, propVal);
}

CString TableLookup::GetThousandSeparator()
{
	CString result;
	GetProperty(0x10f, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetThousandSeparator(LPCTSTR propVal)
{
	SetProperty(0x10f, VT_BSTR, propVal);
}

long TableLookup::GetLabelCopies()
{
	long result;
	GetProperty(0x110, VT_I4, (void*)&result);
	return result;
}

void TableLookup::SetLabelCopies(long propVal)
{
	SetProperty(0x110, VT_I4, propVal);
}

CString TableLookup::GetOutputMask()
{
	CString result;
	GetProperty(0x111, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetOutputMask(LPCTSTR propVal)
{
	SetProperty(0x111, VT_BSTR, propVal);
}

CString TableLookup::GetAccumulatorFile()
{
	CString result;
	GetProperty(0x112, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetAccumulatorFile(LPCTSTR propVal)
{
	SetProperty(0x112, VT_BSTR, propVal);
}

long TableLookup::GetIncludeInLogFile()
{
	long result;
	GetProperty(0x113, VT_I4, (void*)&result);
	return result;
}

void TableLookup::SetIncludeInLogFile(long propVal)
{
	SetProperty(0x113, VT_I4, propVal);
}

long TableLookup::GetSharedCounter()
{
	long result;
	GetProperty(0x114, VT_I4, (void*)&result);
	return result;
}

void TableLookup::SetSharedCounter(long propVal)
{
	SetProperty(0x114, VT_I4, propVal);
}

long TableLookup::GetCounterUse()
{
	long result;
	GetProperty(0x201, VT_I4, (void*)&result);
	return result;
}

void TableLookup::SetCounterUse(long propVal)
{
	SetProperty(0x201, VT_I4, propVal);
}

CString TableLookup::GetDatabaseSource()
{
	CString result;
	GetProperty(0x202, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetDatabaseSource(LPCTSTR propVal)
{
	SetProperty(0x202, VT_BSTR, propVal);
}

CString TableLookup::GetTableName()
{
	CString result;
	GetProperty(0x203, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetTableName(LPCTSTR propVal)
{
	SetProperty(0x203, VT_BSTR, propVal);
}

CString TableLookup::GetResultField()
{
	CString result;
	GetProperty(0x204, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetResultField(LPCTSTR propVal)
{
	SetProperty(0x204, VT_BSTR, propVal);
}

CString TableLookup::GetKeys()
{
	CString result;
	GetProperty(0x205, VT_BSTR, (void*)&result);
	return result;
}

void TableLookup::SetKeys(LPCTSTR propVal)
{
	SetProperty(0x205, VT_BSTR, propVal);
}

long TableLookup::GetLength()
{
	long result;
	GetProperty(0x206, VT_I4, (void*)&result);
	return result;
}

void TableLookup::SetLength(long propVal)
{
	SetProperty(0x206, VT_I4, propVal);
}

short TableLookup::GetPadLength()
{
	short result;
	GetProperty(0x207, VT_I2, (void*)&result);
	return result;
}

void TableLookup::SetPadLength(short propVal)
{
	SetProperty(0x207, VT_I2, propVal);
}

long TableLookup::GetDatabaseType()
{
	long result;
	GetProperty(0x20a, VT_I4, (void*)&result);
	return result;
}

void TableLookup::SetDatabaseType(long propVal)
{
	SetProperty(0x20a, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// TableLookup operations

long TableLookup::ShowDialog(long hParent)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hParent);
	return result;
}

void TableLookup::AddKey(LPCTSTR strField, LPCTSTR strVariableName)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x208, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strField, strVariableName);
}

void TableLookup::DeleteKey(LPCTSTR strField)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x209, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strField);
}

long TableLookup::Connect()
{
	long result;
	InvokeHelper(0x20b, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

long TableLookup::Disconnect()
{
	long result;
	InvokeHelper(0x20c, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Dates properties

short Dates::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void Dates::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH Dates::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Dates::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH Dates::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void Dates::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN Dates::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void Dates::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Dates operations

LPDISPATCH Dates::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Dates::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Dates::Add(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void Dates::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// DateTime properties

CString DateTime::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void DateTime::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString DateTime::GetValue()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void DateTime::SetValue(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString DateTime::Get_Value()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void DateTime::Set_Value(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long DateTime::GetDataSource()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void DateTime::SetDataSource(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

LPDISPATCH DateTime::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x4, VT_DISPATCH, (void*)&result);
	return result;
}

void DateTime::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

LPDISPATCH DateTime::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void DateTime::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

CString DateTime::GetDisplayValue()
{
	CString result;
	GetProperty(0x7, VT_BSTR, (void*)&result);
	return result;
}

void DateTime::SetDisplayValue(LPCTSTR propVal)
{
	SetProperty(0x7, VT_BSTR, propVal);
}

long DateTime::GetDevice()
{
	long result;
	GetProperty(0x101, VT_I4, (void*)&result);
	return result;
}

void DateTime::SetDevice(long propVal)
{
	SetProperty(0x101, VT_I4, propVal);
}

CString DateTime::GetFormat()
{
	CString result;
	GetProperty(0x102, VT_BSTR, (void*)&result);
	return result;
}

void DateTime::SetFormat(LPCTSTR propVal)
{
	SetProperty(0x102, VT_BSTR, propVal);
}

long DateTime::GetIncludeInLogFile()
{
	long result;
	GetProperty(0x103, VT_I4, (void*)&result);
	return result;
}

void DateTime::SetIncludeInLogFile(long propVal)
{
	SetProperty(0x103, VT_I4, propVal);
}

long DateTime::GetOffsetValue()
{
	long result;
	GetProperty(0x104, VT_I4, (void*)&result);
	return result;
}

void DateTime::SetOffsetValue(long propVal)
{
	SetProperty(0x104, VT_I4, propVal);
}

short DateTime::GetOffsetUnit()
{
	short result;
	GetProperty(0x105, VT_I2, (void*)&result);
	return result;
}

void DateTime::SetOffsetUnit(short propVal)
{
	SetProperty(0x105, VT_I2, propVal);
}

CString DateTime::GetOffsetVariableName()
{
	CString result;
	GetProperty(0x106, VT_BSTR, (void*)&result);
	return result;
}

void DateTime::SetOffsetVariableName(LPCTSTR propVal)
{
	SetProperty(0x106, VT_BSTR, propVal);
}

LPDISPATCH DateTime::GetOffsetVariableObject()
{
	LPDISPATCH result;
	GetProperty(0x107, VT_DISPATCH, (void*)&result);
	return result;
}

void DateTime::SetOffsetVariableObject(LPDISPATCH propVal)
{
	SetProperty(0x107, VT_DISPATCH, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DateTime operations

long DateTime::ShowDialog(long hParent)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hParent);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Variable properties

CString Variable::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void Variable::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString Variable::GetValue()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void Variable::SetValue(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString Variable::Get_Value()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void Variable::Set_Value(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long Variable::GetDataSource()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void Variable::SetDataSource(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

LPDISPATCH Variable::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x4, VT_DISPATCH, (void*)&result);
	return result;
}

void Variable::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

LPDISPATCH Variable::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void Variable::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

CString Variable::GetDisplayValue()
{
	CString result;
	GetProperty(0x7, VT_BSTR, (void*)&result);
	return result;
}

void Variable::SetDisplayValue(LPCTSTR propVal)
{
	SetProperty(0x7, VT_BSTR, propVal);
}

long Variable::GetIncludeInLogFile()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}

void Variable::SetIncludeInLogFile(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}

long Variable::GetLength()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void Variable::SetLength(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Variable operations

long Variable::ShowDialog(long hParent)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hParent);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DocObjects properties

short DocObjects::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void DocObjects::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH DocObjects::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void DocObjects::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH DocObjects::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void DocObjects::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN DocObjects::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void DocObjects::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

LPDISPATCH DocObjects::GetTexts()
{
	LPDISPATCH result;
	GetProperty(0x4, VT_DISPATCH, (void*)&result);
	return result;
}

void DocObjects::SetTexts(LPDISPATCH propVal)
{
	SetProperty(0x4, VT_DISPATCH, propVal);
}

LPDISPATCH DocObjects::GetBarcodes()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void DocObjects::SetBarcodes(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

LPDISPATCH DocObjects::GetImages()
{
	LPDISPATCH result;
	GetProperty(0x6, VT_DISPATCH, (void*)&result);
	return result;
}

void DocObjects::SetImages(LPDISPATCH propVal)
{
	SetProperty(0x6, VT_DISPATCH, propVal);
}

LPDISPATCH DocObjects::GetShapes()
{
	LPDISPATCH result;
	GetProperty(0x7, VT_DISPATCH, (void*)&result);
	return result;
}

void DocObjects::SetShapes(LPDISPATCH propVal)
{
	SetProperty(0x7, VT_DISPATCH, propVal);
}

LPDISPATCH DocObjects::GetOLEObjects()
{
	LPDISPATCH result;
	GetProperty(0x8, VT_DISPATCH, (void*)&result);
	return result;
}

void DocObjects::SetOLEObjects(LPDISPATCH propVal)
{
	SetProperty(0x8, VT_DISPATCH, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DocObjects operations

LPDISPATCH DocObjects::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH DocObjects::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH DocObjects::Add(long Type, const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_VARIANT;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Type, &Key);
	return result;
}

void DocObjects::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// Texts properties

short Texts::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void Texts::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH Texts::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Texts::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH Texts::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void Texts::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN Texts::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void Texts::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Texts operations

LPDISPATCH Texts::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Texts::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Texts::Add(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void Texts::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// Text properties

long Text::GetLeft()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void Text::SetLeft(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long Text::GetTop()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void Text::SetTop(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long Text::GetWidth()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void Text::SetWidth(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long Text::GetHeight()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void Text::SetHeight(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

CString Text::GetName()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void Text::SetName(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

CString Text::Get_Name()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void Text::Set_Name(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long Text::GetType()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void Text::SetType(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

LPDISPATCH Text::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x7, VT_DISPATCH, (void*)&result);
	return result;
}

void Text::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x7, VT_DISPATCH, propVal);
}

LPDISPATCH Text::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x8, VT_DISPATCH, (void*)&result);
	return result;
}

void Text::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x8, VT_DISPATCH, propVal);
}

long Text::GetLocked()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void Text::SetLocked(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

long Text::GetPrintable()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}

void Text::SetPrintable(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}

long Text::GetAnchorPoint()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void Text::SetAnchorPoint(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

long Text::GetRotation()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}

void Text::SetRotation(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}

long Text::GetBackColor()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}

void Text::SetBackColor(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}

long Text::GetForeColor()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void Text::SetForeColor(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

long Text::GetMarginLeft()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}

void Text::SetMarginLeft(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}

long Text::GetMarginRight()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}

void Text::SetMarginRight(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}

long Text::GetMarginTop()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}

void Text::SetMarginTop(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}

long Text::GetMarginBottom()
{
	long result;
	GetProperty(0x15, VT_I4, (void*)&result);
	return result;
}

void Text::SetMarginBottom(long propVal)
{
	SetProperty(0x15, VT_I4, propVal);
}

long Text::GetPropertyCount()
{
	long result;
	GetProperty(0x50, VT_I4, (void*)&result);
	return result;
}

void Text::SetPropertyCount(long propVal)
{
	SetProperty(0x50, VT_I4, propVal);
}

CString Text::GetData()
{
	CString result;
	GetProperty(0x54, VT_BSTR, (void*)&result);
	return result;
}

void Text::SetData(LPCTSTR propVal)
{
	SetProperty(0x54, VT_BSTR, propVal);
}

CString Text::GetDataSourceNames()
{
	CString result;
	GetProperty(0x55, VT_BSTR, (void*)&result);
	return result;
}

void Text::SetDataSourceNames(LPCTSTR propVal)
{
	SetProperty(0x55, VT_BSTR, propVal);
}

LPDISPATCH Text::GetFont()
{
	LPDISPATCH result;
	GetProperty(0x101, VT_DISPATCH, (void*)&result);
	return result;
}

void Text::SetFont(LPDISPATCH propVal)
{
	SetProperty(0x101, VT_DISPATCH, propVal);
}

CString Text::GetValue()
{
	CString result;
	GetProperty(0x102, VT_BSTR, (void*)&result);
	return result;
}

void Text::SetValue(LPCTSTR propVal)
{
	SetProperty(0x102, VT_BSTR, propVal);
}

long Text::GetWordWrap()
{
	long result;
	GetProperty(0x103, VT_I4, (void*)&result);
	return result;
}

void Text::SetWordWrap(long propVal)
{
	SetProperty(0x103, VT_I4, propVal);
}

long Text::GetWordHyphenation()
{
	long result;
	GetProperty(0x104, VT_I4, (void*)&result);
	return result;
}

void Text::SetWordHyphenation(long propVal)
{
	SetProperty(0x104, VT_I4, propVal);
}

long Text::GetFitToFrame()
{
	long result;
	GetProperty(0x105, VT_I4, (void*)&result);
	return result;
}

void Text::SetFitToFrame(long propVal)
{
	SetProperty(0x105, VT_I4, propVal);
}

long Text::GetAlignment()
{
	long result;
	GetProperty(0x106, VT_I4, (void*)&result);
	return result;
}

void Text::SetAlignment(long propVal)
{
	SetProperty(0x106, VT_I4, propVal);
}

LPDISPATCH Text::GetVariableObject()
{
	LPDISPATCH result;
	GetProperty(0x107, VT_DISPATCH, (void*)&result);
	return result;
}

void Text::SetVariableObject(LPDISPATCH propVal)
{
	SetProperty(0x107, VT_DISPATCH, propVal);
}

CString Text::GetVariableName()
{
	CString result;
	GetProperty(0x108, VT_BSTR, (void*)&result);
	return result;
}

void Text::SetVariableName(LPCTSTR propVal)
{
	SetProperty(0x108, VT_BSTR, propVal);
}

LPDISPATCH Text::GetSelText()
{
	LPDISPATCH result;
	GetProperty(0x109, VT_DISPATCH, (void*)&result);
	return result;
}

void Text::SetSelText(LPDISPATCH propVal)
{
	SetProperty(0x109, VT_DISPATCH, propVal);
}

long Text::GetLineSpacing()
{
	long result;
	GetProperty(0x114, VT_I4, (void*)&result);
	return result;
}

void Text::SetLineSpacing(long propVal)
{
	SetProperty(0x114, VT_I4, propVal);
}

LPDISPATCH Text::GetStrings()
{
	LPDISPATCH result;
	GetProperty(0x115, VT_DISPATCH, (void*)&result);
	return result;
}

void Text::SetStrings(LPDISPATCH propVal)
{
	SetProperty(0x115, VT_DISPATCH, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Text operations

void Text::Move(long X, long Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void Text::Bound(long Left, long Top, long Right, long Bottom)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top, Right, Bottom);
}

long Text::ShowDialog(long hParent)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hParent);
	return result;
}

LPDISPATCH Text::InsertString(LPCTSTR strString, const VARIANT& Position, LPDISPATCH Font)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_DISPATCH;
	InvokeHelper(0x10a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		strString, &Position, Font);
	return result;
}

void Text::InsertTextObject(LPDISPATCH TextObject, const VARIANT& Position)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_VARIANT;
	InvokeHelper(0x10b, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 TextObject, &Position);
}

void Text::InsertVariable(LPDISPATCH Var, const VARIANT& Position, LPDISPATCH Font)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_VARIANT VTS_DISPATCH;
	InvokeHelper(0x10c, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Var, &Position, Font);
}

void Text::InsertCRLF(const VARIANT& Position, LPDISPATCH Font)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_DISPATCH;
	InvokeHelper(0x10d, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Position, Font);
}

LPDISPATCH Text::AppendString(LPCTSTR strString, const VARIANT& Position, LPDISPATCH Font)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT VTS_DISPATCH;
	InvokeHelper(0x10e, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		strString, &Position, Font);
	return result;
}

void Text::AppendTextObject(LPDISPATCH TextObject)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x10f, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 TextObject);
}

void Text::AppendVariable(LPDISPATCH Var, LPDISPATCH Font)
{
	static BYTE parms[] =
		VTS_DISPATCH VTS_DISPATCH;
	InvokeHelper(0x110, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Var, Font);
}

void Text::AppendCRLF(LPDISPATCH Font)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x111, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Font);
}

void Text::Copy()
{
	InvokeHelper(0x112, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void Text::Paste(const VARIANT& Position)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x113, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Position);
}

LPDISPATCH Text::GetProperty_(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH Text::GetPropertyID(long ID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ID);
	return result;
}

void Text::Refresh()
{
	InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// IFontDisp2 properties

CString IFontDisp2::GetName()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void IFontDisp2::SetName(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

CY IFontDisp2::GetSize()
{
	CY result;
	GetProperty(0x2, VT_CY, (void*)&result);
	return result;
}

void IFontDisp2::SetSize(const CY& propVal)
{
	SetProperty(0x2, VT_CY, &propVal);
}

BOOL IFontDisp2::GetBold()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void IFontDisp2::SetBold(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

BOOL IFontDisp2::GetItalic()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void IFontDisp2::SetItalic(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

BOOL IFontDisp2::GetUnderline()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void IFontDisp2::SetUnderline(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

BOOL IFontDisp2::GetStrikethrough()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}

void IFontDisp2::SetStrikethrough(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}

short IFontDisp2::GetWeight()
{
	short result;
	GetProperty(0x7, VT_I2, (void*)&result);
	return result;
}

void IFontDisp2::SetWeight(short propVal)
{
	SetProperty(0x7, VT_I2, propVal);
}

short IFontDisp2::GetCharset()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}

void IFontDisp2::SetCharset(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}

short IFontDisp2::GetOutPrecision()
{
	short result;
	GetProperty(0x9, VT_I2, (void*)&result);
	return result;
}

void IFontDisp2::SetOutPrecision(short propVal)
{
	SetProperty(0x9, VT_I2, propVal);
}

short IFontDisp2::GetPitchAndFamily()
{
	short result;
	GetProperty(0xc, VT_I2, (void*)&result);
	return result;
}

void IFontDisp2::SetPitchAndFamily(short propVal)
{
	SetProperty(0xc, VT_I2, propVal);
}

long IFontDisp2::GetWidthRatio()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}

void IFontDisp2::SetWidthRatio(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}

long IFontDisp2::GetHfont()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void IFontDisp2::SetHfont(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

long IFontDisp2::GetHeight()
{
	long result;
	GetProperty(0xf, VT_I4, (void*)&result);
	return result;
}

void IFontDisp2::SetHeight(long propVal)
{
	SetProperty(0xf, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IFontDisp2 operations


/////////////////////////////////////////////////////////////////////////////
// TextSelection properties

LPDISPATCH TextSelection::GetFont()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void TextSelection::SetFont(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

CString TextSelection::GetValue()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void TextSelection::SetValue(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

long TextSelection::GetBackColor()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void TextSelection::SetBackColor(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long TextSelection::GetForeColor()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void TextSelection::SetForeColor(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

BOOL TextSelection::GetIsEmpty()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void TextSelection::SetIsEmpty(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// TextSelection operations

void TextSelection::Select(const VARIANT& FirstPosition, const VARIANT& LastPosition)
{
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &FirstPosition, &LastPosition);
}

void TextSelection::Cut()
{
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void TextSelection::Copy()
{
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void TextSelection::Paste()
{
	InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// TextStrings properties

short TextStrings::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void TextStrings::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH TextStrings::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void TextStrings::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH TextStrings::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void TextStrings::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN TextStrings::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void TextStrings::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// TextStrings operations

LPDISPATCH TextStrings::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH TextStrings::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH TextStrings::Add(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void TextStrings::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// TextString properties

LPDISPATCH TextString::GetFont()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void TextString::SetFont(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

long TextString::GetBackColor()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void TextString::SetBackColor(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long TextString::GetForeColor()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void TextString::SetForeColor(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

CString TextString::GetData()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}

void TextString::SetData(LPCTSTR propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}

CString TextString::GetVariableName()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void TextString::SetVariableName(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// TextString operations


/////////////////////////////////////////////////////////////////////////////
// Barcodes properties

short Barcodes::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void Barcodes::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH Barcodes::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Barcodes::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH Barcodes::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void Barcodes::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN Barcodes::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void Barcodes::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Barcodes operations

LPDISPATCH Barcodes::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Barcodes::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Barcodes::Add(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void Barcodes::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// Barcode properties

long Barcode::GetLeft()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetLeft(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long Barcode::GetTop()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetTop(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long Barcode::GetWidth()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetWidth(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long Barcode::GetHeight()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetHeight(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

CString Barcode::GetName()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void Barcode::SetName(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

CString Barcode::Get_Name()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void Barcode::Set_Name(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long Barcode::GetType()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetType(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

LPDISPATCH Barcode::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x7, VT_DISPATCH, (void*)&result);
	return result;
}

void Barcode::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x7, VT_DISPATCH, propVal);
}

LPDISPATCH Barcode::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x8, VT_DISPATCH, (void*)&result);
	return result;
}

void Barcode::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x8, VT_DISPATCH, propVal);
}

long Barcode::GetLocked()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetLocked(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

long Barcode::GetPrintable()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetPrintable(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}

long Barcode::GetAnchorPoint()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetAnchorPoint(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

long Barcode::GetRotation()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetRotation(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}

long Barcode::GetBackColor()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetBackColor(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}

long Barcode::GetForeColor()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetForeColor(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

long Barcode::GetMarginLeft()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetMarginLeft(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}

long Barcode::GetMarginRight()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetMarginRight(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}

long Barcode::GetMarginTop()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetMarginTop(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}

long Barcode::GetMarginBottom()
{
	long result;
	GetProperty(0x15, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetMarginBottom(long propVal)
{
	SetProperty(0x15, VT_I4, propVal);
}

long Barcode::GetPropertyCount()
{
	long result;
	GetProperty(0x50, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetPropertyCount(long propVal)
{
	SetProperty(0x50, VT_I4, propVal);
}

CString Barcode::GetData()
{
	CString result;
	GetProperty(0x54, VT_BSTR, (void*)&result);
	return result;
}

void Barcode::SetData(LPCTSTR propVal)
{
	SetProperty(0x54, VT_BSTR, propVal);
}

CString Barcode::GetDataSourceNames()
{
	CString result;
	GetProperty(0x55, VT_BSTR, (void*)&result);
	return result;
}

void Barcode::SetDataSourceNames(LPCTSTR propVal)
{
	SetProperty(0x55, VT_BSTR, propVal);
}

long Barcode::GetBarHeight()
{
	long result;
	GetProperty(0x101, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetBarHeight(long propVal)
{
	SetProperty(0x101, VT_I4, propVal);
}

long Barcode::GetNarrowBarWidth()
{
	long result;
	GetProperty(0x102, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetNarrowBarWidth(long propVal)
{
	SetProperty(0x102, VT_I4, propVal);
}

short Barcode::GetRatio()
{
	short result;
	GetProperty(0x103, VT_I2, (void*)&result);
	return result;
}

void Barcode::SetRatio(short propVal)
{
	SetProperty(0x103, VT_I2, propVal);
}

long Barcode::GetCheckMode()
{
	long result;
	GetProperty(0x104, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetCheckMode(long propVal)
{
	SetProperty(0x104, VT_I4, propVal);
}

long Barcode::GetDevice()
{
	long result;
	GetProperty(0x105, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetDevice(long propVal)
{
	SetProperty(0x105, VT_I4, propVal);
}

LPDISPATCH Barcode::GetCode2D()
{
	LPDISPATCH result;
	GetProperty(0x106, VT_DISPATCH, (void*)&result);
	return result;
}

void Barcode::SetCode2D(LPDISPATCH propVal)
{
	SetProperty(0x106, VT_DISPATCH, propVal);
}

CString Barcode::GetValue()
{
	CString result;
	GetProperty(0x107, VT_BSTR, (void*)&result);
	return result;
}

void Barcode::SetValue(LPCTSTR propVal)
{
	SetProperty(0x107, VT_BSTR, propVal);
}

CString Barcode::GetVariableName()
{
	CString result;
	GetProperty(0x108, VT_BSTR, (void*)&result);
	return result;
}

void Barcode::SetVariableName(LPCTSTR propVal)
{
	SetProperty(0x108, VT_BSTR, propVal);
}

long Barcode::GetSymbology()
{
	long result;
	GetProperty(0x109, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetSymbology(long propVal)
{
	SetProperty(0x109, VT_I4, propVal);
}

LPDISPATCH Barcode::GetVariableObject()
{
	LPDISPATCH result;
	GetProperty(0x10a, VT_DISPATCH, (void*)&result);
	return result;
}

void Barcode::SetVariableObject(LPDISPATCH propVal)
{
	SetProperty(0x10a, VT_DISPATCH, propVal);
}

long Barcode::GetHRGap()
{
	long result;
	GetProperty(0x10b, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetHRGap(long propVal)
{
	SetProperty(0x10b, VT_I4, propVal);
}

long Barcode::GetHRAlignment()
{
	long result;
	GetProperty(0x10c, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetHRAlignment(long propVal)
{
	SetProperty(0x10c, VT_I4, propVal);
}

long Barcode::GetHRPosition()
{
	long result;
	GetProperty(0x10d, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetHRPosition(long propVal)
{
	SetProperty(0x10d, VT_I4, propVal);
}

long Barcode::GetHRCheckCharacter()
{
	long result;
	GetProperty(0x10e, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetHRCheckCharacter(long propVal)
{
	SetProperty(0x10e, VT_I4, propVal);
}

LPDISPATCH Barcode::GetHRFont()
{
	LPDISPATCH result;
	GetProperty(0x10f, VT_DISPATCH, (void*)&result);
	return result;
}

void Barcode::SetHRFont(LPDISPATCH propVal)
{
	SetProperty(0x10f, VT_DISPATCH, propVal);
}

LPDISPATCH Barcode::GetHRFreeTextObject()
{
	LPDISPATCH result;
	GetProperty(0x110, VT_DISPATCH, (void*)&result);
	return result;
}

void Barcode::SetHRFreeTextObject(LPDISPATCH propVal)
{
	SetProperty(0x110, VT_DISPATCH, propVal);
}

BOOL Barcode::GetHRDevice()
{
	BOOL result;
	GetProperty(0x111, VT_BOOL, (void*)&result);
	return result;
}

void Barcode::SetHRDevice(BOOL propVal)
{
	SetProperty(0x111, VT_BOOL, propVal);
}

CString Barcode::GetCheckValue()
{
	CString result;
	GetProperty(0x112, VT_BSTR, (void*)&result);
	return result;
}

void Barcode::SetCheckValue(LPCTSTR propVal)
{
	SetProperty(0x112, VT_BSTR, propVal);
}

CString Barcode::GetHROutputMask()
{
	CString result;
	GetProperty(0x114, VT_BSTR, (void*)&result);
	return result;
}

void Barcode::SetHROutputMask(LPCTSTR propVal)
{
	SetProperty(0x114, VT_BSTR, propVal);
}

long Barcode::GetNarrowBarWidthDot()
{
	long result;
	GetProperty(0x115, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetNarrowBarWidthDot(long propVal)
{
	SetProperty(0x115, VT_I4, propVal);
}

long Barcode::GetExtendedDescenders()
{
	long result;
	GetProperty(0x116, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetExtendedDescenders(long propVal)
{
	SetProperty(0x116, VT_I4, propVal);
}

long Barcode::GetShape()
{
	long result;
	GetProperty(0x117, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetShape(long propVal)
{
	SetProperty(0x117, VT_I4, propVal);
}

long Barcode::GetInnerRadius()
{
	long result;
	GetProperty(0x118, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetInnerRadius(long propVal)
{
	SetProperty(0x118, VT_I4, propVal);
}

long Barcode::GetBeginAngle()
{
	long result;
	GetProperty(0x119, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetBeginAngle(long propVal)
{
	SetProperty(0x119, VT_I4, propVal);
}

long Barcode::GetAngle()
{
	long result;
	GetProperty(0x11a, VT_I4, (void*)&result);
	return result;
}

void Barcode::SetAngle(long propVal)
{
	SetProperty(0x11a, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Barcode operations

void Barcode::Move(long X, long Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void Barcode::Bound(long Left, long Top, long Right, long Bottom)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top, Right, Bottom);
}

long Barcode::ShowDialog(long hParent)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hParent);
	return result;
}

long Barcode::Is2D()
{
	long result;
	InvokeHelper(0x113, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH Barcode::GetProperty_(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH Barcode::GetPropertyID(long ID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ID);
	return result;
}

void Barcode::Refresh()
{
	InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Code2D properties

long Code2D::GetModuleX()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void Code2D::SetModuleX(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long Code2D::GetModuleY()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void Code2D::SetModuleY(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long Code2D::GetColumns()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void Code2D::SetColumns(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long Code2D::GetRows()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void Code2D::SetRows(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

CString Code2D::GetEcc()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void Code2D::SetEcc(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

long Code2D::GetModuleXDot()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}

void Code2D::SetModuleXDot(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}

long Code2D::GetModuleYDot()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void Code2D::SetModuleYDot(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

LPDISPATCH Code2D::GetOptionsAvailable()
{
	LPDISPATCH result;
	GetProperty(0xa, VT_DISPATCH, (void*)&result);
	return result;
}

void Code2D::SetOptionsAvailable(LPDISPATCH propVal)
{
	SetProperty(0xa, VT_DISPATCH, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Code2D operations

void Code2D::SetOption(LPCTSTR strOptionName, const VARIANT& varValue)
{
	static BYTE parms[] =
		VTS_BSTR VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strOptionName, &varValue);
}

VARIANT Code2D::GetOption(LPCTSTR strOptionName)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		strOptionName);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Strings properties

short Strings::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void Strings::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH Strings::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Strings::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH Strings::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void Strings::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN Strings::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void Strings::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Strings operations

CString Strings::Item(long Key)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Key);
	return result;
}

CString Strings::_Item(long Key)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Key);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Images properties

short Images::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void Images::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH Images::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Images::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH Images::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void Images::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN Images::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void Images::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Images operations

LPDISPATCH Images::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Images::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Images::Add(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void Images::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// Image properties

long Image::GetLeft()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void Image::SetLeft(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long Image::GetTop()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void Image::SetTop(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long Image::GetWidth()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void Image::SetWidth(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long Image::GetHeight()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void Image::SetHeight(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

CString Image::GetName()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void Image::SetName(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

CString Image::Get_Name()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void Image::Set_Name(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long Image::GetType()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void Image::SetType(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

LPDISPATCH Image::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x7, VT_DISPATCH, (void*)&result);
	return result;
}

void Image::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x7, VT_DISPATCH, propVal);
}

LPDISPATCH Image::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x8, VT_DISPATCH, (void*)&result);
	return result;
}

void Image::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x8, VT_DISPATCH, propVal);
}

long Image::GetLocked()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void Image::SetLocked(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

long Image::GetPrintable()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}

void Image::SetPrintable(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}

long Image::GetAnchorPoint()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void Image::SetAnchorPoint(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

long Image::GetRotation()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}

void Image::SetRotation(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}

long Image::GetBackColor()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}

void Image::SetBackColor(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}

long Image::GetForeColor()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void Image::SetForeColor(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

long Image::GetMarginLeft()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}

void Image::SetMarginLeft(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}

long Image::GetMarginRight()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}

void Image::SetMarginRight(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}

long Image::GetMarginTop()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}

void Image::SetMarginTop(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}

long Image::GetMarginBottom()
{
	long result;
	GetProperty(0x15, VT_I4, (void*)&result);
	return result;
}

void Image::SetMarginBottom(long propVal)
{
	SetProperty(0x15, VT_I4, propVal);
}

long Image::GetPropertyCount()
{
	long result;
	GetProperty(0x50, VT_I4, (void*)&result);
	return result;
}

void Image::SetPropertyCount(long propVal)
{
	SetProperty(0x50, VT_I4, propVal);
}

CString Image::GetData()
{
	CString result;
	GetProperty(0x54, VT_BSTR, (void*)&result);
	return result;
}

void Image::SetData(LPCTSTR propVal)
{
	SetProperty(0x54, VT_BSTR, propVal);
}

CString Image::GetDataSourceNames()
{
	CString result;
	GetProperty(0x55, VT_BSTR, (void*)&result);
	return result;
}

void Image::SetDataSourceNames(LPCTSTR propVal)
{
	SetProperty(0x55, VT_BSTR, propVal);
}

CString Image::GetFileName()
{
	CString result;
	GetProperty(0x101, VT_BSTR, (void*)&result);
	return result;
}

void Image::SetFileName(LPCTSTR propVal)
{
	SetProperty(0x101, VT_BSTR, propVal);
}

long Image::GetVertFlip()
{
	long result;
	GetProperty(0x102, VT_I4, (void*)&result);
	return result;
}

void Image::SetVertFlip(long propVal)
{
	SetProperty(0x102, VT_I4, propVal);
}

long Image::GetHorzFlip()
{
	long result;
	GetProperty(0x103, VT_I4, (void*)&result);
	return result;
}

void Image::SetHorzFlip(long propVal)
{
	SetProperty(0x103, VT_I4, propVal);
}

long Image::GetNegative()
{
	long result;
	GetProperty(0x104, VT_I4, (void*)&result);
	return result;
}

void Image::SetNegative(long propVal)
{
	SetProperty(0x104, VT_I4, propVal);
}

short Image::GetBrightness()
{
	short result;
	GetProperty(0x105, VT_I2, (void*)&result);
	return result;
}

void Image::SetBrightness(short propVal)
{
	SetProperty(0x105, VT_I2, propVal);
}

LPDISPATCH Image::GetVariableObject()
{
	LPDISPATCH result;
	GetProperty(0x106, VT_DISPATCH, (void*)&result);
	return result;
}

void Image::SetVariableObject(LPDISPATCH propVal)
{
	SetProperty(0x106, VT_DISPATCH, propVal);
}

CString Image::GetVariableName()
{
	CString result;
	GetProperty(0x107, VT_BSTR, (void*)&result);
	return result;
}

void Image::SetVariableName(LPCTSTR propVal)
{
	SetProperty(0x107, VT_BSTR, propVal);
}

short Image::GetRatio()
{
	short result;
	GetProperty(0x108, VT_I2, (void*)&result);
	return result;
}

void Image::SetRatio(short propVal)
{
	SetProperty(0x108, VT_I2, propVal);
}

LPDISPATCH Image::GetPicture()
{
	LPDISPATCH result;
	GetProperty(0x109, VT_DISPATCH, (void*)&result);
	return result;
}

void Image::SetPicture(LPDISPATCH propVal)
{
	SetProperty(0x109, VT_DISPATCH, propVal);
}

short Image::GetContrast()
{
	short result;
	GetProperty(0x10a, VT_I2, (void*)&result);
	return result;
}

void Image::SetContrast(short propVal)
{
	SetProperty(0x10a, VT_I2, propVal);
}

long Image::GetDefaultFolder()
{
	long result;
	GetProperty(0x10b, VT_I4, (void*)&result);
	return result;
}

void Image::SetDefaultFolder(long propVal)
{
	SetProperty(0x10b, VT_I4, propVal);
}

CString Image::GetFolderName()
{
	CString result;
	GetProperty(0x10c, VT_BSTR, (void*)&result);
	return result;
}

void Image::SetFolderName(LPCTSTR propVal)
{
	SetProperty(0x10c, VT_BSTR, propVal);
}

long Image::GetNumColors()
{
	long result;
	GetProperty(0x10d, VT_I4, (void*)&result);
	return result;
}

void Image::SetNumColors(long propVal)
{
	SetProperty(0x10d, VT_I4, propVal);
}

long Image::GetMethod()
{
	long result;
	GetProperty(0x10e, VT_I4, (void*)&result);
	return result;
}

void Image::SetMethod(long propVal)
{
	SetProperty(0x10e, VT_I4, propVal);
}

long Image::GetNumMaxColors()
{
	long result;
	GetProperty(0x10f, VT_I4, (void*)&result);
	return result;
}

void Image::SetNumMaxColors(long propVal)
{
	SetProperty(0x10f, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Image operations

void Image::Move(long X, long Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void Image::Bound(long Left, long Top, long Right, long Bottom)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top, Right, Bottom);
}

long Image::ShowDialog(long hParent)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hParent);
	return result;
}

LPDISPATCH Image::GetProperty_(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH Image::GetPropertyID(long ID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ID);
	return result;
}

void Image::Refresh()
{
	InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Shapes properties

short Shapes::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void Shapes::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH Shapes::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Shapes::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH Shapes::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void Shapes::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN Shapes::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void Shapes::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Shapes operations

LPDISPATCH Shapes::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Shapes::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void Shapes::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}

LPDISPATCH Shapes::AddLine(long Left, long Top, long Right, long Bottom)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x101, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Left, Top, Right, Bottom);
	return result;
}

LPDISPATCH Shapes::AddOblique(long Left, long Top, long Right, long Bottom)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x102, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Left, Top, Right, Bottom);
	return result;
}

LPDISPATCH Shapes::AddPolygon(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x103, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH Shapes::AddRectangle(long Left, long Top, long Right, long Bottom)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x104, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Left, Top, Right, Bottom);
	return result;
}

LPDISPATCH Shapes::AddRoundRect(long Left, long Top, long Right, long Bottom, long Radius)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x105, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Left, Top, Right, Bottom, Radius);
	return result;
}

LPDISPATCH Shapes::AddEllipse(long Left, long Top, long Right, long Bottom)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x106, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Left, Top, Right, Bottom);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Shape properties

long Shape::GetLeft()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void Shape::SetLeft(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long Shape::GetTop()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void Shape::SetTop(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long Shape::GetWidth()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void Shape::SetWidth(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long Shape::GetHeight()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void Shape::SetHeight(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

CString Shape::GetName()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void Shape::SetName(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

CString Shape::Get_Name()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void Shape::Set_Name(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long Shape::GetType()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void Shape::SetType(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

LPDISPATCH Shape::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x7, VT_DISPATCH, (void*)&result);
	return result;
}

void Shape::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x7, VT_DISPATCH, propVal);
}

LPDISPATCH Shape::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x8, VT_DISPATCH, (void*)&result);
	return result;
}

void Shape::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x8, VT_DISPATCH, propVal);
}

long Shape::GetLocked()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void Shape::SetLocked(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

long Shape::GetPrintable()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}

void Shape::SetPrintable(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}

long Shape::GetAnchorPoint()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void Shape::SetAnchorPoint(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

long Shape::GetRotation()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}

void Shape::SetRotation(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}

long Shape::GetBackColor()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}

void Shape::SetBackColor(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}

long Shape::GetForeColor()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void Shape::SetForeColor(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

long Shape::GetMarginLeft()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}

void Shape::SetMarginLeft(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}

long Shape::GetMarginRight()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}

void Shape::SetMarginRight(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}

long Shape::GetMarginTop()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}

void Shape::SetMarginTop(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}

long Shape::GetMarginBottom()
{
	long result;
	GetProperty(0x15, VT_I4, (void*)&result);
	return result;
}

void Shape::SetMarginBottom(long propVal)
{
	SetProperty(0x15, VT_I4, propVal);
}

long Shape::GetPropertyCount()
{
	long result;
	GetProperty(0x50, VT_I4, (void*)&result);
	return result;
}

void Shape::SetPropertyCount(long propVal)
{
	SetProperty(0x50, VT_I4, propVal);
}

CString Shape::GetData()
{
	CString result;
	GetProperty(0x54, VT_BSTR, (void*)&result);
	return result;
}

void Shape::SetData(LPCTSTR propVal)
{
	SetProperty(0x54, VT_BSTR, propVal);
}

CString Shape::GetDataSourceNames()
{
	CString result;
	GetProperty(0x55, VT_BSTR, (void*)&result);
	return result;
}

void Shape::SetDataSourceNames(LPCTSTR propVal)
{
	SetProperty(0x55, VT_BSTR, propVal);
}

long Shape::GetLineWidth()
{
	long result;
	GetProperty(0x101, VT_I4, (void*)&result);
	return result;
}

void Shape::SetLineWidth(long propVal)
{
	SetProperty(0x101, VT_I4, propVal);
}

long Shape::GetClosed()
{
	long result;
	GetProperty(0x104, VT_I4, (void*)&result);
	return result;
}

void Shape::SetClosed(long propVal)
{
	SetProperty(0x104, VT_I4, propVal);
}

long Shape::GetBackMode()
{
	long result;
	GetProperty(0x105, VT_I4, (void*)&result);
	return result;
}

void Shape::SetBackMode(long propVal)
{
	SetProperty(0x105, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Shape operations

void Shape::Move(long X, long Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void Shape::Bound(long Left, long Top, long Right, long Bottom)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top, Right, Bottom);
}

long Shape::ShowDialog(long hParent)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hParent);
	return result;
}

VARIANT Shape::GetPoints()
{
	VARIANT result;
	InvokeHelper(0x103, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}

void Shape::SetPoints(const VARIANT& pts)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x102, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &pts);
}

LPDISPATCH Shape::GetProperty_(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH Shape::GetPropertyID(long ID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ID);
	return result;
}

void Shape::Refresh()
{
	InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// OLEObjects properties

short OLEObjects::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void OLEObjects::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH OLEObjects::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void OLEObjects::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH OLEObjects::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void OLEObjects::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN OLEObjects::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void OLEObjects::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// OLEObjects operations

LPDISPATCH OLEObjects::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH OLEObjects::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH OLEObjects::Add(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void OLEObjects::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// OLEObject properties

long OLEObject::GetLeft()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetLeft(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long OLEObject::GetTop()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetTop(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long OLEObject::GetWidth()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetWidth(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long OLEObject::GetHeight()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetHeight(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

CString OLEObject::GetName()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void OLEObject::SetName(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

CString OLEObject::Get_Name()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void OLEObject::Set_Name(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long OLEObject::GetType()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetType(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

LPDISPATCH OLEObject::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x7, VT_DISPATCH, (void*)&result);
	return result;
}

void OLEObject::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x7, VT_DISPATCH, propVal);
}

LPDISPATCH OLEObject::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x8, VT_DISPATCH, (void*)&result);
	return result;
}

void OLEObject::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x8, VT_DISPATCH, propVal);
}

long OLEObject::GetLocked()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetLocked(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

long OLEObject::GetPrintable()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetPrintable(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}

long OLEObject::GetAnchorPoint()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetAnchorPoint(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

long OLEObject::GetRotation()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetRotation(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}

long OLEObject::GetBackColor()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetBackColor(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}

long OLEObject::GetForeColor()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetForeColor(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

long OLEObject::GetMarginLeft()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetMarginLeft(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}

long OLEObject::GetMarginRight()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetMarginRight(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}

long OLEObject::GetMarginTop()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetMarginTop(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}

long OLEObject::GetMarginBottom()
{
	long result;
	GetProperty(0x15, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetMarginBottom(long propVal)
{
	SetProperty(0x15, VT_I4, propVal);
}

long OLEObject::GetPropertyCount()
{
	long result;
	GetProperty(0x50, VT_I4, (void*)&result);
	return result;
}

void OLEObject::SetPropertyCount(long propVal)
{
	SetProperty(0x50, VT_I4, propVal);
}

CString OLEObject::GetData()
{
	CString result;
	GetProperty(0x54, VT_BSTR, (void*)&result);
	return result;
}

void OLEObject::SetData(LPCTSTR propVal)
{
	SetProperty(0x54, VT_BSTR, propVal);
}

CString OLEObject::GetDataSourceNames()
{
	CString result;
	GetProperty(0x55, VT_BSTR, (void*)&result);
	return result;
}

void OLEObject::SetDataSourceNames(LPCTSTR propVal)
{
	SetProperty(0x55, VT_BSTR, propVal);
}

LPDISPATCH OLEObject::GetObject()
{
	LPDISPATCH result;
	GetProperty(0x101, VT_DISPATCH, (void*)&result);
	return result;
}

void OLEObject::SetObject(LPDISPATCH propVal)
{
	SetProperty(0x101, VT_DISPATCH, propVal);
}

CString OLEObject::GetConnectedServer()
{
	CString result;
	GetProperty(0x105, VT_BSTR, (void*)&result);
	return result;
}

void OLEObject::SetConnectedServer(LPCTSTR propVal)
{
	SetProperty(0x105, VT_BSTR, propVal);
}

CString OLEObject::GetLinkedFileName()
{
	CString result;
	GetProperty(0x106, VT_BSTR, (void*)&result);
	return result;
}

void OLEObject::SetLinkedFileName(LPCTSTR propVal)
{
	SetProperty(0x106, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// OLEObject operations

void OLEObject::Move(long X, long Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void OLEObject::Bound(long Left, long Top, long Right, long Bottom)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top, Right, Bottom);
}

BOOL OLEObject::EmbedFile(LPCTSTR strFileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x102, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strFileName);
	return result;
}

BOOL OLEObject::LinkFile(LPCTSTR strLinkFileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x103, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strLinkFileName);
	return result;
}

BOOL OLEObject::ConnectServer(LPCTSTR strCLSIDOrServerName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x104, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strCLSIDOrServerName);
	return result;
}

LPDISPATCH OLEObject::GetProperty_(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH OLEObject::GetPropertyID(long ID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ID);
	return result;
}

void OLEObject::Refresh()
{
	InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// DocObject properties

long DocObject::GetLeft()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetLeft(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}

long DocObject::GetTop()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetTop(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long DocObject::GetWidth()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetWidth(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long DocObject::GetHeight()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetHeight(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

CString DocObject::GetName()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void DocObject::SetName(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

CString DocObject::Get_Name()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void DocObject::Set_Name(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

long DocObject::GetType()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetType(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

LPDISPATCH DocObject::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x7, VT_DISPATCH, (void*)&result);
	return result;
}

void DocObject::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x7, VT_DISPATCH, propVal);
}

LPDISPATCH DocObject::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x8, VT_DISPATCH, (void*)&result);
	return result;
}

void DocObject::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x8, VT_DISPATCH, propVal);
}

long DocObject::GetLocked()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetLocked(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

long DocObject::GetPrintable()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetPrintable(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}

long DocObject::GetAnchorPoint()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetAnchorPoint(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

long DocObject::GetRotation()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetRotation(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}

long DocObject::GetBackColor()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetBackColor(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}

long DocObject::GetForeColor()
{
	long result;
	GetProperty(0xe, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetForeColor(long propVal)
{
	SetProperty(0xe, VT_I4, propVal);
}

long DocObject::GetMarginLeft()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetMarginLeft(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}

long DocObject::GetMarginRight()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetMarginRight(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}

long DocObject::GetMarginTop()
{
	long result;
	GetProperty(0x14, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetMarginTop(long propVal)
{
	SetProperty(0x14, VT_I4, propVal);
}

long DocObject::GetMarginBottom()
{
	long result;
	GetProperty(0x15, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetMarginBottom(long propVal)
{
	SetProperty(0x15, VT_I4, propVal);
}

long DocObject::GetPropertyCount()
{
	long result;
	GetProperty(0x50, VT_I4, (void*)&result);
	return result;
}

void DocObject::SetPropertyCount(long propVal)
{
	SetProperty(0x50, VT_I4, propVal);
}

CString DocObject::GetData()
{
	CString result;
	GetProperty(0x54, VT_BSTR, (void*)&result);
	return result;
}

void DocObject::SetData(LPCTSTR propVal)
{
	SetProperty(0x54, VT_BSTR, propVal);
}

CString DocObject::GetDataSourceNames()
{
	CString result;
	GetProperty(0x55, VT_BSTR, (void*)&result);
	return result;
}

void DocObject::SetDataSourceNames(LPCTSTR propVal)
{
	SetProperty(0x55, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DocObject operations

void DocObject::Move(long X, long Y)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0xf, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 X, Y);
}

void DocObject::Bound(long Left, long Top, long Right, long Bottom)
{
	static BYTE parms[] =
		VTS_I4 VTS_I4 VTS_I4 VTS_I4;
	InvokeHelper(0x10, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Left, Top, Right, Bottom);
}

long DocObject::ShowDialog(long hParent)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hParent);
	return result;
}

LPDISPATCH DocObject::GetProperty_(long Index)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x51, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Index);
	return result;
}

LPDISPATCH DocObject::GetPropertyID(long ID)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x52, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		ID);
	return result;
}

void DocObject::Refresh()
{
	InvokeHelper(0x53, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Printer properties

CString Printer::GetName()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void Printer::SetName(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

long Printer::GetXdpi()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}

void Printer::SetXdpi(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}

long Printer::GetYdpi()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void Printer::SetYdpi(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

CString Printer::GetFullName()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}

void Printer::SetFullName(LPCTSTR propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}

CString Printer::Get_FullName()
{
	CString result;
	GetProperty(0x0, VT_BSTR, (void*)&result);
	return result;
}

void Printer::Set_FullName(LPCTSTR propVal)
{
	SetProperty(0x0, VT_BSTR, propVal);
}

LPDISPATCH Printer::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x5, VT_DISPATCH, (void*)&result);
	return result;
}

void Printer::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x5, VT_DISPATCH, propVal);
}

LPDISPATCH Printer::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x6, VT_DISPATCH, (void*)&result);
	return result;
}

void Printer::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x6, VT_DISPATCH, propVal);
}

LPDISPATCH Printer::GetWindowsFontNames()
{
	LPDISPATCH result;
	GetProperty(0x7, VT_DISPATCH, (void*)&result);
	return result;
}

void Printer::SetWindowsFontNames(LPDISPATCH propVal)
{
	SetProperty(0x7, VT_DISPATCH, propVal);
}

LPDISPATCH Printer::GetDeviceFontNames()
{
	LPDISPATCH result;
	GetProperty(0x8, VT_DISPATCH, (void*)&result);
	return result;
}

void Printer::SetDeviceFontNames(LPDISPATCH propVal)
{
	SetProperty(0x8, VT_DISPATCH, propVal);
}

LPDISPATCH Printer::GetWindowsCodeNames()
{
	LPDISPATCH result;
	GetProperty(0x9, VT_DISPATCH, (void*)&result);
	return result;
}

void Printer::SetWindowsCodeNames(LPDISPATCH propVal)
{
	SetProperty(0x9, VT_DISPATCH, propVal);
}

LPDISPATCH Printer::GetDeviceCodeNames()
{
	LPDISPATCH result;
	GetProperty(0xa, VT_DISPATCH, (void*)&result);
	return result;
}

void Printer::SetDeviceCodeNames(LPDISPATCH propVal)
{
	SetProperty(0xa, VT_DISPATCH, propVal);
}

CString Printer::GetDriverName()
{
	CString result;
	GetProperty(0xf, VT_BSTR, (void*)&result);
	return result;
}

void Printer::SetDriverName(LPCTSTR propVal)
{
	SetProperty(0xf, VT_BSTR, propVal);
}

CString Printer::GetModelName()
{
	CString result;
	GetProperty(0x10, VT_BSTR, (void*)&result);
	return result;
}

void Printer::SetModelName(LPCTSTR propVal)
{
	SetProperty(0x10, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Printer operations

short Printer::ShowSetup()
{
	short result;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL Printer::SetParameter(const VARIANT& Parameter, const VARIANT& Value)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&Parameter, &Value);
	return result;
}

BOOL Printer::SwitchTo(LPCTSTR strPrinterName, LPCTSTR strPortName, BOOL DirectAccess)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strPrinterName, strPortName, DirectAccess);
	return result;
}

BOOL Printer::Send(LPCTSTR strEscapeSequence)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xe, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strEscapeSequence);
	return result;
}

BOOL Printer::IsDevice()
{
	BOOL result;
	InvokeHelper(0x11, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

long Printer::GetDevMode(VARIANT* DevMode)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		DevMode);
	return result;
}

long Printer::SetDevMode(VARIANT* DevMode)
{
	long result;
	static BYTE parms[] =
		VTS_PVARIANT;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		DevMode);
	return result;
}

BOOL Printer::SwitchToEx(LPCTSTR strPrinterName, LPCTSTR strPortName, BOOL DirectAccess, long Flags)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BOOL VTS_I4;
	InvokeHelper(0x14, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strPrinterName, strPortName, DirectAccess, Flags);
	return result;
}

BOOL Printer::GetParameter(const VARIANT& Parameter, VARIANT* Value)
{
	BOOL result;
	static BYTE parms[] =
		VTS_VARIANT VTS_PVARIANT;
	InvokeHelper(0x15, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		&Parameter, Value);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Format properties

LPDISPATCH Format::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void Format::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

LPDISPATCH Format::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Format::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

long Format::GetPageHeight()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void Format::SetPageHeight(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long Format::GetPageWidth()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void Format::SetPageWidth(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

long Format::GetLabelHeight()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}

void Format::SetLabelHeight(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}

long Format::GetLabelWidth()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void Format::SetLabelWidth(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

long Format::GetMarginLeft()
{
	long result;
	GetProperty(0x7, VT_I4, (void*)&result);
	return result;
}

void Format::SetMarginLeft(long propVal)
{
	SetProperty(0x7, VT_I4, propVal);
}

long Format::GetMarginTop()
{
	long result;
	GetProperty(0x8, VT_I4, (void*)&result);
	return result;
}

void Format::SetMarginTop(long propVal)
{
	SetProperty(0x8, VT_I4, propVal);
}

long Format::GetCorner()
{
	long result;
	GetProperty(0x9, VT_I4, (void*)&result);
	return result;
}

void Format::SetCorner(long propVal)
{
	SetProperty(0x9, VT_I4, propVal);
}

long Format::GetHorizontalGap()
{
	long result;
	GetProperty(0xa, VT_I4, (void*)&result);
	return result;
}

void Format::SetHorizontalGap(long propVal)
{
	SetProperty(0xa, VT_I4, propVal);
}

long Format::GetVerticalGap()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}

void Format::SetVerticalGap(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}

long Format::GetRowCount()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}

void Format::SetRowCount(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}

long Format::GetColumnCount()
{
	long result;
	GetProperty(0xd, VT_I4, (void*)&result);
	return result;
}

void Format::SetColumnCount(long propVal)
{
	SetProperty(0xd, VT_I4, propVal);
}

BOOL Format::GetPortrait()
{
	BOOL result;
	GetProperty(0xe, VT_BOOL, (void*)&result);
	return result;
}

void Format::SetPortrait(BOOL propVal)
{
	SetProperty(0xe, VT_BOOL, propVal);
}

BOOL Format::GetAutoSize()
{
	BOOL result;
	GetProperty(0xf, VT_BOOL, (void*)&result);
	return result;
}

void Format::SetAutoSize(BOOL propVal)
{
	SetProperty(0xf, VT_BOOL, propVal);
}

CString Format::GetStockType()
{
	CString result;
	GetProperty(0x10, VT_BSTR, (void*)&result);
	return result;
}

void Format::SetStockType(LPCTSTR propVal)
{
	SetProperty(0x10, VT_BSTR, propVal);
}

CString Format::GetStockName()
{
	CString result;
	GetProperty(0x11, VT_BSTR, (void*)&result);
	return result;
}

void Format::SetStockName(LPCTSTR propVal)
{
	SetProperty(0x11, VT_BSTR, propVal);
}

long Format::GetSupportType()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}

void Format::SetSupportType(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Format operations

void Format::SaveStock()
{
	InvokeHelper(0x12, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// Database properties

LPDISPATCH Database::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void Database::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

LPDISPATCH Database::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Database::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

BOOL Database::GetEof()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void Database::SetEof(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

BOOL Database::GetBof()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void Database::SetBof(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

BOOL Database::GetCreatingVariables()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void Database::SetCreatingVariables(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

BOOL Database::GetIsOpen()
{
	BOOL result;
	GetProperty(0x6, VT_BOOL, (void*)&result);
	return result;
}

void Database::SetIsOpen(BOOL propVal)
{
	SetProperty(0x6, VT_BOOL, propVal);
}

CString Database::GetConnectionString()
{
	CString result;
	GetProperty(0xf, VT_BSTR, (void*)&result);
	return result;
}

void Database::SetConnectionString(LPCTSTR propVal)
{
	SetProperty(0xf, VT_BSTR, propVal);
}

CString Database::GetQueryString()
{
	CString result;
	GetProperty(0x10, VT_BSTR, (void*)&result);
	return result;
}

void Database::SetQueryString(LPCTSTR propVal)
{
	SetProperty(0x10, VT_BSTR, propVal);
}

CString Database::GetQueryFileName()
{
	CString result;
	GetProperty(0x11, VT_BSTR, (void*)&result);
	return result;
}

void Database::SetQueryFileName(LPCTSTR propVal)
{
	SetProperty(0x11, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Database operations

BOOL Database::MoveNext()
{
	BOOL result;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL Database::MovePrevious()
{
	BOOL result;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL Database::MoveFirst()
{
	BOOL result;
	InvokeHelper(0x9, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL Database::MoveLast()
{
	BOOL result;
	InvokeHelper(0xa, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL Database::OpenASCII(LPCTSTR strFileName, LPCTSTR strDescriptorFileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0xb, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strFileName, strDescriptorFileName);
	return result;
}

BOOL Database::OpenQuery(LPCTSTR strQueryFileName)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0xc, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strQueryFileName);
	return result;
}

BOOL Database::OpenODBC(LPCTSTR strDatasourceName, LPCTSTR strSQLQuery)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0xd, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		strDatasourceName, strSQLQuery);
	return result;
}

void Database::Close()
{
	InvokeHelper(0xe, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL Database::Refresh()
{
	BOOL result;
	InvokeHelper(0x12, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

long Database::Find(long lDatabaseFind, LPCTSTR strField, LPCTSTR strValue)
{
	long result;
	static BYTE parms[] =
		VTS_I4 VTS_BSTR VTS_BSTR;
	InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		lDatabaseFind, strField, strValue);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DocumentProperties properties

short DocumentProperties::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void DocumentProperties::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH DocumentProperties::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void DocumentProperties::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH DocumentProperties::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void DocumentProperties::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN DocumentProperties::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void DocumentProperties::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DocumentProperties operations

LPDISPATCH DocumentProperties::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH DocumentProperties::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// DocumentProperty properties

LPDISPATCH DocumentProperty::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void DocumentProperty::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

LPDISPATCH DocumentProperty::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void DocumentProperty::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

VARIANT DocumentProperty::GetValue()
{
	VARIANT result;
	GetProperty(0x3, VT_VARIANT, (void*)&result);
	return result;
}

void DocumentProperty::SetValue(const VARIANT& propVal)
{
	SetProperty(0x3, VT_VARIANT, &propVal);
}

VARIANT DocumentProperty::Get_Value()
{
	VARIANT result;
	GetProperty(0x0, VT_VARIANT, (void*)&result);
	return result;
}

void DocumentProperty::Set_Value(const VARIANT& propVal)
{
	SetProperty(0x0, VT_VARIANT, &propVal);
}

long DocumentProperty::GetType()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void DocumentProperty::SetType(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

CString DocumentProperty::GetName()
{
	CString result;
	GetProperty(0x5, VT_BSTR, (void*)&result);
	return result;
}

void DocumentProperty::SetName(LPCTSTR propVal)
{
	SetProperty(0x5, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// DocumentProperty operations


/////////////////////////////////////////////////////////////////////////////
// SelectedDocObjects properties

short SelectedDocObjects::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void SelectedDocObjects::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH SelectedDocObjects::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void SelectedDocObjects::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH SelectedDocObjects::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void SelectedDocObjects::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN SelectedDocObjects::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void SelectedDocObjects::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// SelectedDocObjects operations

LPDISPATCH SelectedDocObjects::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH SelectedDocObjects::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

void SelectedDocObjects::Add(LPDISPATCH pObject)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pObject);
}

void SelectedDocObjects::Remove(const VARIANT& Key)
{
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 &Key);
}


/////////////////////////////////////////////////////////////////////////////
// Dialogs properties

short Dialogs::GetCount()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}

void Dialogs::SetCount(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}

LPDISPATCH Dialogs::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Dialogs::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

LPDISPATCH Dialogs::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x3, VT_DISPATCH, (void*)&result);
	return result;
}

void Dialogs::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x3, VT_DISPATCH, propVal);
}

LPUNKNOWN Dialogs::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void Dialogs::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Dialogs operations

LPDISPATCH Dialogs::Item(long Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Key);
	return result;
}

LPDISPATCH Dialogs::_Item(long Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Key);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Dialog properties

LPDISPATCH Dialog::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void Dialog::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

LPDISPATCH Dialog::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void Dialog::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

long Dialog::GetType()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void Dialog::SetType(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Dialog operations

short Dialog::Show(const VARIANT& varParent)
{
	short result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_I2, (void*)&result, parms,
		&varParent);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// Options properties

BOOL Options::GetLoadPrinterSetup()
{
	BOOL result;
	GetProperty(0x1, VT_BOOL, (void*)&result);
	return result;
}

void Options::SetLoadPrinterSetup(BOOL propVal)
{
	SetProperty(0x1, VT_BOOL, propVal);
}

BOOL Options::GetLoadPrinter()
{
	BOOL result;
	GetProperty(0x2, VT_BOOL, (void*)&result);
	return result;
}

void Options::SetLoadPrinter(BOOL propVal)
{
	SetProperty(0x2, VT_BOOL, propVal);
}

BOOL Options::GetOpenMergeDatabase()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}

void Options::SetOpenMergeDatabase(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}

BOOL Options::GetCreateBackup()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}

void Options::SetCreateBackup(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}

BOOL Options::GetOpenReadOnly()
{
	BOOL result;
	GetProperty(0x5, VT_BOOL, (void*)&result);
	return result;
}

void Options::SetOpenReadOnly(BOOL propVal)
{
	SetProperty(0x5, VT_BOOL, propVal);
}

long Options::GetMeasureSystem()
{
	long result;
	GetProperty(0x6, VT_I4, (void*)&result);
	return result;
}

void Options::SetMeasureSystem(long propVal)
{
	SetProperty(0x6, VT_I4, propVal);
}

CString Options::GetDefaultImagePath()
{
	CString result;
	GetProperty(0x7, VT_BSTR, (void*)&result);
	return result;
}

void Options::SetDefaultImagePath(LPCTSTR propVal)
{
	SetProperty(0x7, VT_BSTR, propVal);
}

CString Options::GetDefaultQueryPath()
{
	CString result;
	GetProperty(0x8, VT_BSTR, (void*)&result);
	return result;
}

void Options::SetDefaultQueryPath(LPCTSTR propVal)
{
	SetProperty(0x8, VT_BSTR, propVal);
}

CString Options::GetDefaultDescriberPath()
{
	CString result;
	GetProperty(0x9, VT_BSTR, (void*)&result);
	return result;
}

void Options::SetDefaultDescriberPath(LPCTSTR propVal)
{
	SetProperty(0x9, VT_BSTR, propVal);
}

CString Options::GetDefaultPrintOutPath()
{
	CString result;
	GetProperty(0xa, VT_BSTR, (void*)&result);
	return result;
}

void Options::SetDefaultPrintOutPath(LPCTSTR propVal)
{
	SetProperty(0xa, VT_BSTR, propVal);
}

CString Options::GetDefaultSharedPath()
{
	CString result;
	GetProperty(0xb, VT_BSTR, (void*)&result);
	return result;
}

void Options::SetDefaultSharedPath(LPCTSTR propVal)
{
	SetProperty(0xb, VT_BSTR, propVal);
}

LPDISPATCH Options::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0xc, VT_DISPATCH, (void*)&result);
	return result;
}

void Options::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0xc, VT_DISPATCH, propVal);
}

LPDISPATCH Options::GetParent()
{
	LPDISPATCH result;
	GetProperty(0xd, VT_DISPATCH, (void*)&result);
	return result;
}

void Options::SetParent(LPDISPATCH propVal)
{
	SetProperty(0xd, VT_DISPATCH, propVal);
}

BOOL Options::GetTrayNotification()
{
	BOOL result;
	GetProperty(0xe, VT_BOOL, (void*)&result);
	return result;
}

void Options::SetTrayNotification(BOOL propVal)
{
	SetProperty(0xe, VT_BOOL, propVal);
}

long Options::GetLanguage()
{
	long result;
	GetProperty(0xf, VT_I4, (void*)&result);
	return result;
}

void Options::SetLanguage(long propVal)
{
	SetProperty(0xf, VT_I4, propVal);
}

float Options::GetEuroConversionRate()
{
	float result;
	GetProperty(0x10, VT_R4, (void*)&result);
	return result;
}

void Options::SetEuroConversionRate(float propVal)
{
	SetProperty(0x10, VT_R4, propVal);
}

CString Options::GetDefaultUserSettingsPath()
{
	CString result;
	GetProperty(0x11, VT_BSTR, (void*)&result);
	return result;
}

void Options::SetDefaultUserSettingsPath(LPCTSTR propVal)
{
	SetProperty(0x11, VT_BSTR, propVal);
}

long Options::GetSharedFileAccessTimeout()
{
	long result;
	GetProperty(0x12, VT_I4, (void*)&result);
	return result;
}

void Options::SetSharedFileAccessTimeout(long propVal)
{
	SetProperty(0x12, VT_I4, propVal);
}

long Options::GetSettingsMode()
{
	long result;
	GetProperty(0x13, VT_I4, (void*)&result);
	return result;
}

void Options::SetSettingsMode(long propVal)
{
	SetProperty(0x13, VT_I4, propVal);
}

CString Options::GetDefaultDataPath()
{
	CString result;
	GetProperty(0x14, VT_BSTR, (void*)&result);
	return result;
}

void Options::SetDefaultDataPath(LPCTSTR propVal)
{
	SetProperty(0x14, VT_BSTR, propVal);
}

BOOL Options::GetDoNotDisplayWarning()
{
	BOOL result;
	GetProperty(0x15, VT_BOOL, (void*)&result);
	return result;
}

void Options::SetDoNotDisplayWarning(BOOL propVal)
{
	SetProperty(0x15, VT_BOOL, propVal);
}

long Options::GetHfont()
{
	long result;
	GetProperty(0x16, VT_I4, (void*)&result);
	return result;
}

void Options::SetHfont(long propVal)
{
	SetProperty(0x16, VT_I4, propVal);
}

LPDISPATCH Options::GetFont()
{
	LPDISPATCH result;
	GetProperty(0x17, VT_DISPATCH, (void*)&result);
	return result;
}

void Options::SetFont(LPDISPATCH propVal)
{
	SetProperty(0x17, VT_DISPATCH, propVal);
}

long Options::GetAutoSaveDelay()
{
	long result;
	GetProperty(0x18, VT_I4, (void*)&result);
	return result;
}

void Options::SetAutoSaveDelay(long propVal)
{
	SetProperty(0x18, VT_I4, propVal);
}

CString Options::GetDefaultDocumentPath()
{
	CString result;
	GetProperty(0x19, VT_BSTR, (void*)&result);
	return result;
}

void Options::SetDefaultDocumentPath(LPCTSTR propVal)
{
	SetProperty(0x19, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// Options operations


/////////////////////////////////////////////////////////////////////////////
// RecentFiles properties

LPDISPATCH RecentFiles::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void RecentFiles::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

LPDISPATCH RecentFiles::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void RecentFiles::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

long RecentFiles::GetMaximum()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}

void RecentFiles::SetMaximum(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}

long RecentFiles::GetCount()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}

void RecentFiles::SetCount(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}

LPUNKNOWN RecentFiles::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void RecentFiles::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// RecentFiles operations

LPDISPATCH RecentFiles::Add(const VARIANT& Key, BOOL ReadOnly)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key, ReadOnly);
	return result;
}

LPDISPATCH RecentFiles::Item(long Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Key);
	return result;
}

LPDISPATCH RecentFiles::_Item(long Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Key);
	return result;
}

void RecentFiles::Clear()
{
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void RecentFiles::Remove(short Key)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Key);
}


/////////////////////////////////////////////////////////////////////////////
// RecentFile properties

LPDISPATCH RecentFile::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void RecentFile::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

LPDISPATCH RecentFile::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void RecentFile::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

CString RecentFile::GetPath()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void RecentFile::SetPath(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

CString RecentFile::GetName()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}

void RecentFile::SetName(LPCTSTR propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// RecentFile operations

LPDISPATCH RecentFile::Open()
{
	LPDISPATCH result;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// SharedOwner properties

LPDISPATCH SharedOwner::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedOwner::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

LPDISPATCH SharedOwner::GetCounters()
{
	LPDISPATCH result;
	GetProperty(0x201, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedOwner::SetCounters(LPDISPATCH propVal)
{
	SetProperty(0x201, VT_DISPATCH, propVal);
}

LPDISPATCH SharedOwner::GetLists()
{
	LPDISPATCH result;
	GetProperty(0x202, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedOwner::SetLists(LPDISPATCH propVal)
{
	SetProperty(0x202, VT_DISPATCH, propVal);
}

LPDISPATCH SharedOwner::GetFrees()
{
	LPDISPATCH result;
	GetProperty(0x203, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedOwner::SetFrees(LPDISPATCH propVal)
{
	SetProperty(0x203, VT_DISPATCH, propVal);
}

LPDISPATCH SharedOwner::GetForms()
{
	LPDISPATCH result;
	GetProperty(0x204, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedOwner::SetForms(LPDISPATCH propVal)
{
	SetProperty(0x204, VT_DISPATCH, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// SharedOwner operations


/////////////////////////////////////////////////////////////////////////////
// SharedCounters properties

LPDISPATCH SharedCounters::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedCounters::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

short SharedCounters::GetCount()
{
	short result;
	GetProperty(0x101, VT_I2, (void*)&result);
	return result;
}

void SharedCounters::SetCount(short propVal)
{
	SetProperty(0x101, VT_I2, propVal);
}

LPUNKNOWN SharedCounters::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void SharedCounters::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// SharedCounters operations

LPDISPATCH SharedCounters::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x102, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH SharedCounters::Add(LPCTSTR strName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x103, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		strName);
	return result;
}

long SharedCounters::Remove(const VARIANT& Key)
{
	long result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x104, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		&Key);
	return result;
}

long SharedCounters::ShowDialog()
{
	long result;
	InvokeHelper(0x106, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void SharedCounters::Refresh()
{
	InvokeHelper(0x107, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long SharedCounters::Write()
{
	long result;
	InvokeHelper(0x108, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH SharedCounters::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// SharedCounter properties

LPDISPATCH SharedCounter::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedCounter::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

CString SharedCounter::GetName()
{
	CString result;
	GetProperty(0x101, VT_BSTR, (void*)&result);
	return result;
}

void SharedCounter::SetName(LPCTSTR propVal)
{
	SetProperty(0x101, VT_BSTR, propVal);
}

CString SharedCounter::GetValue()
{
	CString result;
	GetProperty(0x102, VT_BSTR, (void*)&result);
	return result;
}

void SharedCounter::SetValue(LPCTSTR propVal)
{
	SetProperty(0x102, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// SharedCounter operations


/////////////////////////////////////////////////////////////////////////////
// SharedListsCollection properties

LPDISPATCH SharedListsCollection::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedListsCollection::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

short SharedListsCollection::GetCount()
{
	short result;
	GetProperty(0x101, VT_I2, (void*)&result);
	return result;
}

void SharedListsCollection::SetCount(short propVal)
{
	SetProperty(0x101, VT_I2, propVal);
}

LPUNKNOWN SharedListsCollection::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void SharedListsCollection::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// SharedListsCollection operations

LPDISPATCH SharedListsCollection::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x102, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH SharedListsCollection::Add(LPCTSTR strName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x103, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		strName);
	return result;
}

long SharedListsCollection::Remove(const VARIANT& Key)
{
	long result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x104, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		&Key);
	return result;
}

void SharedListsCollection::Refresh()
{
	InvokeHelper(0x105, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long SharedListsCollection::Write()
{
	long result;
	InvokeHelper(0x106, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH SharedListsCollection::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// SharedLists properties

LPDISPATCH SharedLists::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedLists::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

short SharedLists::GetCount()
{
	short result;
	GetProperty(0x101, VT_I2, (void*)&result);
	return result;
}

void SharedLists::SetCount(short propVal)
{
	SetProperty(0x101, VT_I2, propVal);
}

CString SharedLists::GetName()
{
	CString result;
	GetProperty(0x105, VT_BSTR, (void*)&result);
	return result;
}

void SharedLists::SetName(LPCTSTR propVal)
{
	SetProperty(0x105, VT_BSTR, propVal);
}

LPUNKNOWN SharedLists::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void SharedLists::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// SharedLists operations

long SharedLists::ShowDialog()
{
	long result;
	InvokeHelper(0x106, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void SharedLists::Refresh()
{
	InvokeHelper(0x107, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long SharedLists::Write()
{
	long result;
	InvokeHelper(0x108, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH SharedLists::AddValue(LPCTSTR sValue)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x201, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		sValue);
	return result;
}

long SharedLists::DeleteValue(LPCTSTR sValue)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x202, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		sValue);
	return result;
}

LPDISPATCH SharedLists::GetAt(long lValue)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x203, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		lValue);
	return result;
}

LPDISPATCH SharedLists::_GetAt(long lValue)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		lValue);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// SharedList properties

LPDISPATCH SharedList::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedList::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

CString SharedList::GetValue()
{
	CString result;
	GetProperty(0x102, VT_BSTR, (void*)&result);
	return result;
}

void SharedList::SetValue(LPCTSTR propVal)
{
	SetProperty(0x102, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// SharedList operations


/////////////////////////////////////////////////////////////////////////////
// SharedFrees properties

LPDISPATCH SharedFrees::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedFrees::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

short SharedFrees::GetCount()
{
	short result;
	GetProperty(0x101, VT_I2, (void*)&result);
	return result;
}

void SharedFrees::SetCount(short propVal)
{
	SetProperty(0x101, VT_I2, propVal);
}

LPUNKNOWN SharedFrees::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void SharedFrees::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// SharedFrees operations

LPDISPATCH SharedFrees::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x102, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH SharedFrees::Add(LPCTSTR strName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x103, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		strName);
	return result;
}

long SharedFrees::Remove(const VARIANT& Key)
{
	long result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x104, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		&Key);
	return result;
}

long SharedFrees::ShowDialog()
{
	long result;
	InvokeHelper(0x106, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void SharedFrees::Refresh()
{
	InvokeHelper(0x107, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long SharedFrees::Write()
{
	long result;
	InvokeHelper(0x108, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH SharedFrees::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// SharedFree properties

LPDISPATCH SharedFree::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedFree::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

CString SharedFree::GetName()
{
	CString result;
	GetProperty(0x101, VT_BSTR, (void*)&result);
	return result;
}

void SharedFree::SetName(LPCTSTR propVal)
{
	SetProperty(0x101, VT_BSTR, propVal);
}

CString SharedFree::GetValue()
{
	CString result;
	GetProperty(0x102, VT_BSTR, (void*)&result);
	return result;
}

void SharedFree::SetValue(LPCTSTR propVal)
{
	SetProperty(0x102, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// SharedFree operations


/////////////////////////////////////////////////////////////////////////////
// SharedForms properties

LPDISPATCH SharedForms::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedForms::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

short SharedForms::GetCount()
{
	short result;
	GetProperty(0x101, VT_I2, (void*)&result);
	return result;
}

void SharedForms::SetCount(short propVal)
{
	SetProperty(0x101, VT_I2, propVal);
}

LPUNKNOWN SharedForms::Get_NewEnum()
{
	LPUNKNOWN result;
	GetProperty(0xfffffffc, VT_UNKNOWN, (void*)&result);
	return result;
}

void SharedForms::Set_NewEnum(LPUNKNOWN propVal)
{
	SetProperty(0xfffffffc, VT_UNKNOWN, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// SharedForms operations

LPDISPATCH SharedForms::Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x102, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}

LPDISPATCH SharedForms::Add(LPCTSTR strName)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x103, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		strName);
	return result;
}

long SharedForms::Remove(const VARIANT& Key)
{
	long result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x104, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		&Key);
	return result;
}

long SharedForms::ShowDialog()
{
	long result;
	InvokeHelper(0x106, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void SharedForms::Refresh()
{
	InvokeHelper(0x107, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

long SharedForms::Write()
{
	long result;
	InvokeHelper(0x108, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

LPDISPATCH SharedForms::_Item(const VARIANT& Key)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		&Key);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// SharedForm properties

LPDISPATCH SharedForm::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void SharedForm::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

CString SharedForm::GetName()
{
	CString result;
	GetProperty(0x101, VT_BSTR, (void*)&result);
	return result;
}

void SharedForm::SetName(LPCTSTR propVal)
{
	SetProperty(0x101, VT_BSTR, propVal);
}

CString SharedForm::GetValue()
{
	CString result;
	GetProperty(0x102, VT_BSTR, (void*)&result);
	return result;
}

void SharedForm::SetValue(LPCTSTR propVal)
{
	SetProperty(0x102, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// SharedForm operations


/////////////////////////////////////////////////////////////////////////////
// PrinterSystem properties

/////////////////////////////////////////////////////////////////////////////
// PrinterSystem operations

LPDISPATCH PrinterSystem::Families()
{
	LPDISPATCH result;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH PrinterSystem::Models(LPCTSTR Family)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		Family);
	return result;
}

LPDISPATCH PrinterSystem::Printers(long KindOfPrinter)
{
	LPDISPATCH result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms,
		KindOfPrinter);
	return result;
}

LPDISPATCH PrinterSystem::Ports()
{
	LPDISPATCH result;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString PrinterSystem::Add(LPCTSTR strPrinterName, LPCTSTR strPortName, BOOL DirectAccess)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		strPrinterName, strPortName, DirectAccess);
	return result;
}

void PrinterSystem::Remove(LPCTSTR strPrinterName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strPrinterName);
}

void PrinterSystem::Rename(LPCTSTR strPrinterName, LPCTSTR strNewPrinterName)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strPrinterName, strNewPrinterName);
}

void PrinterSystem::ChangePort(LPCTSTR strPrinterName, LPCTSTR strPortName, BOOL DirectAccess)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strPrinterName, strPortName, DirectAccess);
}


/////////////////////////////////////////////////////////////////////////////
// INotifyApplicationEvent properties

/////////////////////////////////////////////////////////////////////////////
// INotifyApplicationEvent operations

void INotifyApplicationEvent::Quit()
{
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void INotifyApplicationEvent::Close()
{
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void INotifyApplicationEvent::DocumentClosed(LPCTSTR strDocTitle)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strDocTitle);
}


/////////////////////////////////////////////////////////////////////////////
// INotifyDocumentEvent properties

/////////////////////////////////////////////////////////////////////////////
// INotifyDocumentEvent operations

void INotifyDocumentEvent::Change()
{
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void INotifyDocumentEvent::BeginPrinting(LPCTSTR strDocName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strDocName);
}

void INotifyDocumentEvent::ProgressPrinting(short Percent, short* Cancel)
{
	static BYTE parms[] =
		VTS_I2 VTS_PI2;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Percent, Cancel);
}

void INotifyDocumentEvent::EndPrinting(long Reason)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Reason);
}

void INotifyDocumentEvent::PausedPrinting(long Reason, short* Cancel)
{
	static BYTE parms[] =
		VTS_I4 VTS_PI2;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Reason, Cancel);
}

void INotifyDocumentEvent::Close()
{
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// PickList properties

LPDISPATCH PickList::GetApplication()
{
	LPDISPATCH result;
	GetProperty(0x1, VT_DISPATCH, (void*)&result);
	return result;
}

void PickList::SetApplication(LPDISPATCH propVal)
{
	SetProperty(0x1, VT_DISPATCH, propVal);
}

LPDISPATCH PickList::GetParent()
{
	LPDISPATCH result;
	GetProperty(0x2, VT_DISPATCH, (void*)&result);
	return result;
}

void PickList::SetParent(LPDISPATCH propVal)
{
	SetProperty(0x2, VT_DISPATCH, propVal);
}

CString PickList::GetName()
{
	CString result;
	GetProperty(0x101, VT_BSTR, (void*)&result);
	return result;
}

void PickList::SetName(LPCTSTR propVal)
{
	SetProperty(0x101, VT_BSTR, propVal);
}

long PickList::GetShared()
{
	long result;
	GetProperty(0x102, VT_I4, (void*)&result);
	return result;
}

void PickList::SetShared(long propVal)
{
	SetProperty(0x102, VT_I4, propVal);
}

short PickList::GetDefaultId()
{
	short result;
	GetProperty(0x103, VT_I2, (void*)&result);
	return result;
}

void PickList::SetDefaultId(short propVal)
{
	SetProperty(0x103, VT_I2, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// PickList operations

CString PickList::GetValue(short iId)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x151, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		iId);
	return result;
}

void PickList::SetValue(short iId, LPCTSTR sValue)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x152, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 iId, sValue);
}

long PickList::AddValue(LPCTSTR sValue)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x153, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		sValue);
	return result;
}

long PickList::DeleteValue(const VARIANT& vValue)
{
	long result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x154, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		&vValue);
	return result;
}

short PickList::GetSize()
{
	short result;
	InvokeHelper(0x155, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

long PickList::ShowDialog(long hParent)
{
	long result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x156, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		hParent);
	return result;
}
