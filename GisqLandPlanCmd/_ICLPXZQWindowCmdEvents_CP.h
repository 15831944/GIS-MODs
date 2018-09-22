#pragma once

template<class T>
class CProxy_ICLPXZQWindowCmdEvents :
	public IConnectionPointImpl<T, &__uuidof(_ICLPXZQWindowCmdEvents)>
{
public:
	HRESULT Fire_OnSelectXZQItem( BSTR * bstrDM,  BSTR * bstrMC)
	{
		HRESULT hr = S_OK;
		T * pThis = static_cast<T *>(this);
		int cConnections = m_vec.GetSize();

		for (int iConnection = 0; iConnection < cConnections; iConnection++)
		{
			pThis->Lock();
			CComPtr<IUnknown> punkConnection = m_vec.GetAt(iConnection);
			pThis->Unlock();

			IDispatch * pConnection = static_cast<IDispatch *>(punkConnection.p);

			if (pConnection)
			{
				CComVariant avarParams[2];
				avarParams[1].byref = bstrDM;
				avarParams[1].vt = VT_BSTR|VT_BYREF;
				avarParams[0].byref = bstrMC;
				avarParams[0].vt = VT_BSTR|VT_BYREF;
				CComVariant varResult;

				DISPPARAMS params = { avarParams, NULL, 2, 0 };
				hr = pConnection->Invoke(1, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, &varResult, NULL, NULL);
			}
		}
		return hr;
	}
};

