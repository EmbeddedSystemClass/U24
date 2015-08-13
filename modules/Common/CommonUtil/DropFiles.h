/* *** THIS LINE MUST BE THE FIRST LINE AND DO NOT MODIFY OR REMOVE IT *** */
/***************************************************************************
 *
 *							BenQ QSyncher Project
 *
 *						Copyright (c) 2005 BENQ Corp.,
 *
 ***************************************************************************
 *
 *  $Workfile:   DropFiles.h  $
 *  $Revision:   1.0  $
 *  $Creater:   Keven Yang $
 *  $Date:   Apr 08 2011 11:41:56  $ 
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 ***************************************************************************/
 /*$Log:   //BQT/VM67/Prjbase3/PlatformTool/Code/Modules/Common/CommonUtil/DropFiles.h_V  $
 * 
 *    Rev 1.0   Apr 08 2011 11:41:56   Magan Mei
 * Initial revision.
 * 
 *    Rev 1.1   Apr 01 2009 14:17:08   Kevin Tai
 * no change
 * 
 *    Rev 1.0   Apr 22 2008 20:54:06   Alan Tu
 * Initial revision.
 * 
 *    Rev 1.2   Jan 08 2008 15:22:52   Alan Tu
 * update
 * 
 *    Rev 1.1   Mar 16 2007 19:17:10   Alan Tu
 * update
 * 
 *    Rev 1.0   Mar 16 2007 08:47:22   Alan Tu
 * Initial revision.
 * 
 *    Rev 1.0   Mar 02 2005 18:53:02   Neko Hsieh
 * Initial revision.
  */ 
 /**************************************************************************/
/* *** <<<DO NOT MODIFY OR REMOVE THIS LINE>>> *** */
#ifndef BS_CDropFiles_H
#define BS_CDropFiles_H
#include <shlobj.h>
#include <afxcoll.h>
class CDropFiles
{
	public:
		CDropFiles();
		~CDropFiles();

		void	AddFile(const CString &sFile);
		void	CreateBuffer();
		HGLOBAL	GetBuffer() const;
	private:
		CStringArray m_aFiles;
		HGLOBAL m_hMemData;
};
#endif