#ifndef __CMD_BASE_H__
#define __CMD_BASE_H__

template <class T> class CCmdObjectPtr
{
public:
	CCmdObjectPtr <T> () : m_pCmdObject( NULL ){}

	CCmdObjectPtr <T> ( T *pRefCmdObject ) : m_pCmdObject( pRefCmdObject ){}

	CCmdObjectPtr <T> ( const CCmdObjectPtr <T> &pRefCmdObject ) : m_pCmdObject( pRefCmdObject.m_pCmdObject ){}

	~CCmdObjectPtr <T> ()
	{
		if(this->m_pCmdObject)
			delete this->m_pCmdObject;
	}

	const T* operator -> () const
	{
		return this->m_pCmdObject;
	}

	T* operator ->()
	{
		return this->m_pCmdObject;
	}

private:
	T *m_pCmdObject;
};

class ICmdBase
{
public:
	ICmdBase(){}
	virtual ~ICmdBase(){}
	virtual const BYTE *GetCmd() const = 0;
	virtual const BYTE *GetResponse() const = 0;
	virtual const UINT GetCmdSize() const = 0;
	virtual bool HasReturnValue() const = 0;
	virtual UINT GetTimeOut() const = 0;
    virtual void SetResponseBufferSize(UINT size) = 0;
    virtual UINT GetResponseSize() const = 0;
};

typedef CCmdObjectPtr<ICmdBase> CCmdPtr;

#endif
