#pragma once

#define FI_FILE_DESCRIPTION		_T("FileDescription")
#define FI_COMPANY_NAME			_T("CompanyName")
#define FI_FILE_VERSION			_T("FileVersion")
#define FI_INTERNAL_NAME		_T("InternalName")
#define FI_LEGAL_COPYRIGHT		_T("LegalCopyright")
#define FI_LEGAL_TRADEMARKS		_T("LegalTradeMarks")
#define FI_ORIGINAL_FILENAME	_T("OriginalFilename")
#define FI_PRODUCT_NAME			_T("ProductName")
#define FI_PRODUCT_VERSION		_T("ProductVersion")
#define FI_COMMENTS				_T("Comments")
#define FI_AUTHOR				_T("Author")

tstring GetFileVersionInfo(tstring tsName, tstring tsParamName);