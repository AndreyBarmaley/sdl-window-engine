SWE.UnicodeString
/**
    @api {unicodestring constructor} SWE.UnicodeString(),(inputstr),(length,value) SWE.UnicodeString
    @apiGroup SWE.UnicodeString
    @apiName Constructor
            
    @apiParam {string}				inputstr	string data
    @apiParam {number}				length		length data
    @apiParam {number}				value		default value
    @apiSuccess (Return) {SWE.UnicodeString}	result		unicodestring data

    @apiExample ext table fields (read only)
    | size    | number  | unicodestring length
*/
SWE.UnicodeString.ToUtf8String
/**
    @api {convert unicodestring to utf8 string} SWE.UnicodeString.ToUtf8String(inputbuf) SWE.UnicodeString.ToUtf8String
    @apiGroup SWE.UnicodeString
    @apiName ToUtf8String

    @apiParam {SWE.UnicodeString}     		inputbuf   	unicodestring raw data
    @apiSuccess (Return) {string}    		result   	utf8 cstring data
*/
SWE.UnicodeString.PushBack
/**
    @api {push back byte value} SWE.UnicodeString.PushBack(self,offset,value...value) SWE.UnicodeString.PushBack
    @apiGroup SWE.UnicodeString
    @apiName PushBack
    
    @apiParam {SWE.UnicodeString}     		self   		unicodestring raw data
    @apiParam {number}     			offset   	offset data
    @apiParam {number}     			value   	byte value (list)
    @apiSuccess (Return) {boolean}    		result   	success
*/
SWE.UnicodeString.Insert
/**
    @api {insert byte values} SWE.UnicodeString.Insert(self,offset,inputbuf,offset,length)(offset,count,value) SWE.UnicodeString.Insert
    @apiGroup SWE.UnicodeString
    @apiName Insert

    @apiParam {SWE.UnicodeString}     		self   		unicodestring raw data
    @apiParam {number}     			offset   	offset data
    @apiParam {SWE.UnicodeString}     		inputbuf   	unicodestring input data
    @apiParam {number}     			offset   	offset data
    @apiParam {number}     			length   	length data
    @apiParam {number}     			count  		value counts
    @apiParam {number}     			value		byte value
    @apiSuccess (Return) {boolean}    		result   	success
*/
SWE.UnicodeString.Resize
/**
    @api {resize buf} SWE.UnicodeString.Resize(self,size,value) SWE.UnicodeString.Resize
    @apiGroup SWE.UnicodeString
    @apiName Resize

    @apiParam {SWE.UnicodeString}     		self   		unicodestring raw data
    @apiParam {number}     			size  	 	new size
    @apiParam {number}     			value   	default value
    @apiSuccess (Return) {SWE.UnicodeString}  	result   	bynarybuf raw data
*/
SWE.UnicodeString.Erase
/**
    @api {erase element} SWE.UnicodeString.Erase(self,pos,count) SWE.UnicodeString.Erase
    @apiGroup SWE.UnicodeString
    @apiName Erase

    @apiParam {SWE.UnicodeString}     		self   		unicodestring raw data
    @apiParam {number}     			pos 	 	position
    @apiParam {number}     			count   	count erase
    @apiSuccess (Return) {SWE.UnicodeString}  	result   	bynarybuf raw data
*/
SWE.UnicodeString.Clear
/**
    @api {unicodestring clear} SWE.UnicodeString.Clear(self) SWE.UnicodeString.Clear
    @apiGroup SWE.UnicodeString
    @apiName Clear

    @apiParam {SWE.UnicodeString}               self           unicodestring raw data
*/
SWE.UnicodeString.SetChar
/**
    @api {set char value} SWE.UnicodeString.SetChar(self,offset,value...value) SWE.UnicodeString.SetChar
    @apiGroup SWE.UnicodeString
    @apiName SetChar
    
    @apiParam {SWE.UnicodeString}               self            unicodestring raw data
    @apiParam {number}                          offset          offset data
    @apiParam {number}                          value           byte value (list)
    @apiSuccess (Return) {boolean}              result          success
*/
SWE.UnicodeString.GetChar
/**
    @api {get char value} SWE.UnicodeString.GetChar(self,offset) SWE.UnicodeString.GetChar
    @apiGroup SWE.UnicodeString
    @apiName GetChar
    
    @apiParam {SWE.UnicodeString}               self            unicodestring raw data
    @apiParam {number}                          offset          offset data
    @apiSuccess (Return) {number}               result          byte value
*/
SWE.UnicodeString.ToJson
/**
    @api {convert UnicodeString to json string} SWE.UnicodeString.ToJson(inputbuf) SWE.UnicodeString.ToJson
    @apiGroup SWE.UnicodeString
    @apiName ToJson
    
    @apiParam {SWE.UnicodeString}               self		UnicodeString raw data
    @apiSuccess (Return) {string}               result          json array string

    @apiExample usage
    local ucs = SWE.UnicodeString("Привет Бармалей!")
    print(ucs:ToJson())
    ....
    '[0x041F,0x0440,0x0438,0x0432,0x0435,0x0442,0x0020,0x0411,0x0430,0x0440,0x043C,0x0430,0x043B,0x0435,0x0439,0x0021]'
*/
