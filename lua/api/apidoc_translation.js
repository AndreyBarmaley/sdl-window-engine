SWE.Translation.BindDomain
/**
    @api {translation: bind domain} SWE.Translation.BindDomain(domain,filename)(domain,binarybuf) SWE.Translation.BindDomain
    @apiGroup SWE.Translation
    @apiName BindDomain

    @apiParam {string}			domain		translation domain
    @apiParam {string}			filename	filename, mo format
    @apiParam {SWE.BinaryBuf}		binarybuf	binarybuf, mo format
    @apiSuccess (Return) {boolean}	result		true if bind domain

    @apiExample usage
    local res = SWE.Translation.BindDomain("test1", "ru.mo")
    if res then
	SWE.Translation.SetDomain("test1")
	SWE.Translation.SetStripContext('|')
	print(SWE.Translation.GetText("Color|Red"))
	print(SWE.Translation.GetText("Color|Green"))
	print(SWE.Translation.GetText("Color|Blue"))
    end
*/

SWE.Translation.SetDomain
/**
    @api {translation: set domain} SWE.Translation.SetDomain(domain) SWE.Translation.SetDomain
    @apiGroup SWE.Translation
    @apiName SetDomain
            
    @apiParam {string}			domain		translation domain
    @apiSuccess (Return) {boolean}	result		true if set domain

    @apiExample usage
    local res = SWE.Translation.BindDomain("test1", "ru.mo")
    if res then
	SWE.Translation.SetDomain("test1")
	SWE.Translation.SetStripContext('|')
	print(SWE.Translation.GetText("Color|Red"))
	print(SWE.Translation.GetText("Color|Green"))
	print(SWE.Translation.GetText("Color|Blue"))
    end
*/

SWE.Translation.SetStripContext
/**
    @api {translation: set strip context} SWE.Translation.SetStripContext(context) SWE.Translation.SetStripContext
    @apiGroup SWE.Translation
    @apiName SetStripContext
            
    @apiParam {integer}			context		translation strip context symbol

    @apiExample usage
    local res = SWE.Translation.BindDomain("test1", "ru.mo")
    if res then
	SWE.Translation.SetDomain("test1")
	SWE.Translation.SetStripContext('|')
	print(SWE.Translation.GetText("Color|Red"))
	print(SWE.Translation.GetText("Color|Green"))
	print(SWE.Translation.GetText("Color|Blue"))
    end
*/

SWE.Translation.GetText
/**
    @api {translation: same as gettext} SWE.Translation.GetText(text) SWE.Translation.GetText
    @apiGroup SWE.Translation
    @apiName GetText
            
    @apiParam {string}			text		translation string
    @apiSuccess (Return) {string}	result		result string

    @apiExample usage
    local res = SWE.Translation.BindDomain("test1", "ru.mo")
    if res then
	SWE.Translation.SetDomain("test1")
	SWE.Translation.SetStripContext('|')
	print(SWE.Translation.GetText("Color|Red"))
	print(SWE.Translation.GetText("Color|Green"))
	print(SWE.Translation.GetText("Color|Blue"))
    end
*/

SWE.Translation.DGetText
/**
    @api {translation: same as dgettext} SWE.Translation.DGetText(domain,text) SWE.Translation.DGetText
    @apiGroup SWE.Translation
    @apiName DGetText
            
    @apiParam {string}			domain		translation domain
    @apiParam {string}			text		translation string
    @apiSuccess (Return) {string}	result		result string
*/

SWE.Translation.NGetText
/**
    @api {translation: same as ngettext} SWE.Translation.NGetText(text,plural,number) SWE.Translation.NGetText
    @apiGroup SWE.Translation
    @apiName NGetText
            
    @apiParam {string}			text		translation string
    @apiParam {string}			plural		translation plural form
    @apiParam {integer}			number		translation plural number
    @apiSuccess (Return) {string}	result		result string
*/

SWE.Translation.DNGetText
/**
    @api {translation: same as dngettext} SWE.Translation.DNGetText(domain,text,plural,number) SWE.Translation.DNGetText
    @apiGroup SWE.Translation
    @apiName DNGetText
            
    @apiParam {string}			domain		translation domain
    @apiParam {string}			text		translation string
    @apiParam {string}			plural		translation plural form
    @apiParam {integer}			number		translation plural number
    @apiSuccess (Return) {string}	result		result string
*/
