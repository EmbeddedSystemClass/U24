#include "stdafx.h"
#include "IErrorHandling.h"


RF_IMPLEMENT_DYNAMIC(IErrorHandling)

bool IErrorHandling::GetTestResult(int nIndex, st_UIResult* psResult)
{
	if (psResult == NULL)
		return false;

	sprintf_s(psResult->sz_result, MAX_BUFFER_SIZE - 1, "%s", m_strResult.c_str());
	//sprintf_s(psResult->sz_itemCode, MAX_BUFFER_SIZE - 1, "%s", m_strItemCode.c_str());
	sprintf_s(psResult->sz_errCode, MAX_BUFFER_SIZE - 1, "%s", m_strErrorCode.c_str());
	//sprintf_s(psResult->sz_band, MAX_BUFFER_SIZE - 1, "%s", m_strBand.c_str());
	//sprintf_s(psResult->sz_channel, MAX_BUFFER_SIZE - 1, "%s", m_strChannel.c_str());
	//sprintf_s(psResult->sz_upper, MAX_BUFFER_SIZE - 1, "%s", m_strUpper.c_str());
	//sprintf_s(psResult->sz_lower, MAX_BUFFER_SIZE - 1, "%s", m_strLower.c_str());
	//sprintf_s(psResult->sz_measured, MAX_BUFFER_SIZE - 1, "%s", m_strMeasured.c_str());
	//sprintf_s(psResult->sz_unit, MAX_BUFFER_SIZE - 1, "%s", m_strUnit.c_str());
	sprintf_s(psResult->sz_msg, MAX_BUFFER_SIZE - 1, "%s", m_strMessage.c_str());

	return true;
}