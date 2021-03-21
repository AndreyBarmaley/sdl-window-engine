SWE.BinaryBuf
/**
    @api {binarybuf constructor} SWE.BinaryBuf(),(inputstr),(length),(inputptr,length) SWE.BinaryBuf
    @apiGroup SWE.BinaryBuf
    @apiName Constructor
            
    @apiParam {string}				inputstr	string data
    @apiParam {userdata}			inputptr	pointer data
    @apiParam {number}				length		length data
    @apiSuccess (Return) {SWE.BinaryBuf}	result		binarybuf data

    @apiExample usage
    local buf = SWE.BinaryBuf("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
    local zip = buf:ZlibCompress()
    local base64 = zip:Base64Encode()
    print(base64)

    local size1 = buf.size
    local size2 = #buf
    if size1 == size2 then
	print("equivalent")
    end

    -- C style offset
    local byte1 = buf:GetByte(5)
    -- Lua style offset
    local byte2 = buf[4]
    if byte1 == byte2 then
	print("equivalent")
    end

    @apiExample ext table fields (read only)
    | size    | number  | binarybuf length
*/
SWE.BinaryBuf.ZlibCompress
/**
    @api {compress binarybuf} SWE.BinaryBuf.ZlibCompress(inputbuf) SWE.BinaryBuf.ZlibCompress
    @apiGroup SWE.BinaryBuf
    @apiName ZlibCompress
            
    @apiParam {SWE.BinaryBuf}			inputbuf	binarybuf raw data
    @apiSuccess (Return) {SWE.BinaryBuf}	result		binarybuf zip data
*/
SWE.BinaryBuf.ZlibDecompress
/**
    @api {decompress binarybuf} SWE.BinaryBuf.ZlibDecompress(inputbuf) SWE.BinaryBuf.ZlibDecompress
    @apiGroup SWE.BinaryBuf
    @apiName ZlibDecompress
    
    @apiParam {SWE.BinaryBuf}               	inputbuf	binarybuf zip data
    @apiSuccess (Return) {SWE.BinaryBuf}    	result		binarybuf raw data
*/
SWE.BinaryBuf.Base64Decode
/**
    @api {base64 decode} SWE.BinaryBuf.Base64Decode(inputbuf),(inputstr) SWE.BinaryBuf.Base64Decode
    @apiGroup SWE.BinaryBuf
    @apiName Base64Decode
    
    @apiParam {SWE.BinaryBuf}     		inputbuf   	binarybuf base64 data
    @apiParam {string}              		inputstr        base64 data
    @apiSuccess (Return) {SWE.BinaryBuf}    	result   	binarybuf raw data
*/
SWE.BinaryBuf.Base64Encode
/**
    @api {base64 encode} SWE.BinaryBuf.Base64Encode(inputbuf) SWE.BinaryBuf.Base64Encode
    @apiGroup SWE.BinaryBuf
    @apiName Base64Encode
    
    @apiParam {SWE.BinaryBuf}     		inputbuf   	binarybuf raw data
    @apiSuccess (Return) {string}    		result   	base64 data
*/
SWE.BinaryBuf.ReadFromFile
/**
    @api {read from file} SWE.BinaryBuf.ReadFromFile(self,filename,offset,length)(filename,offset,length) SWE.BinaryBuf.ReadFromFile
    @apiGroup SWE.BinaryBuf
    @apiName ReadFromFile
    
    @apiParam {SWE.BinaryBuf}     		self   		binarybuf raw data
    @apiParam {string}     			filename   	path to filename
    @apiParam {number}     			offset   	offset read (default 0)
    @apiParam {number}     			length   	block size read (default file size)
    @apiSuccess (Return) {boolean}    		result   	read success

    @apiExample usage variant1
    local buf = SWE.BinaryBuf()
    buf:ReadFromFile("/var/tmp/test.bin")
    if buf ~= nil then
        print(buf:ToJson())
    end

    @apiExample usage variant2
    local file = "/var/tmp/2gis.log"
    local buf = SWE.BinaryBuf.ReadFromFile(file)
    if buf ~= nil then
        print(buf:ToJson())
    end
*/
SWE.BinaryBuf.SaveToFile
/**
    @api {save to file} SWE.BinaryBuf.SaveToFile(self,filename,offset) SWE.BinaryBuf.SaveToFile
    @apiGroup SWE.BinaryBuf
    @apiName SaveTiFile
    
    @apiParam {SWE.BinaryBuf}     		self  		binarybuf raw data
    @apiParam {string}     			filename   	filename
    @apiParam {number}     			offset   	offset read (default 0, endpos: -1)
    @apiSuccess (Return) {boolean}    		result   	write success
*/
SWE.BinaryBuf.GetCRC32b
/**
    @api {calculate crc32b sum} SWE.BinaryBuf.GetCRC32b(inputbuf)SWE.BinaryBuf.GetCRC32b(inputbuf,magic) SWE.BinaryBuf.GetCRC32b
    @apiGroup SWE.BinaryBuf
    @apiName GetCRC32b
    
    @apiParam {SWE.BinaryBuf}     		inputbuf   	binarybuf raw data
    @apiParam {number}    			magic		default value (0xEDB88320)
    @apiSuccess (Return) {number}    		result   	crc32b code
*/
SWE.BinaryBuf.ToString
/**
    @api {convert binarybuf to string} SWE.BinaryBuf.ToString(inputbuf) SWE.BinaryBuf.ToString
    @apiGroup SWE.BinaryBuf
    @apiName ToString
    
    @apiParam {SWE.BinaryBuf}     		inputbuf   	binarybuf raw data
    @apiSuccess (Return) {string}    		result   	string data
*/
SWE.BinaryBuf.ToJson
/**
    @api {convert binarybuf to json string} SWE.BinaryBuf.ToJson(inputbuf) SWE.BinaryBuf.ToJson
    @apiGroup SWE.BinaryBuf
    @apiName ToJson
    
    @apiParam {SWE.BinaryBuf}     		inputbuf   	binarybuf raw data
    @apiSuccess (Return) {string}    		result   	json array string

    @apiExample usage
    local buf = SWE.BinaryBuf("012345678")
    print(buf:ToJson())
    ....
    '[0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38]'
*/
SWE.BinaryBuf.PushBack
/**
    @api {push back byte value} SWE.BinaryBuf.PushBack(self,offset,value...value) SWE.BinaryBuf.PushBack
    @apiGroup SWE.BinaryBuf
    @apiName PushBack
    
    @apiParam {SWE.BinaryBuf}     		self   		binarybuf raw data
    @apiParam {number}     			offset   	offset data
    @apiParam {number}     			value   	byte value (list)
    @apiSuccess (Return) {boolean}    		result   	success
*/
SWE.BinaryBuf.SetByte
/**
    @api {set byte value} SWE.BinaryBuf.SetByte(self,offset,value...value) SWE.BinaryBuf.SetByte
    @apiGroup SWE.BinaryBuf
    @apiName SetByte
    
    @apiParam {SWE.BinaryBuf}     		self   		binarybuf raw data
    @apiParam {number}     			offset   	offset data
    @apiParam {number}     			value   	byte value (list)
    @apiSuccess (Return) {boolean}    		result   	success

    @apiExample usage
    local buf = SWE.BinaryBuf("0123456789")
    -- C style offset
    buf:SetByte(0, 0x31, 0x32, 0x33)
*/
SWE.BinaryBuf.SetBytes
/**
    @api {set byte values} SWE.BinaryBuf.SetBytes(self,offset,binarybuf,offset,length) SWE.BinaryBuf.SetBytes
    @apiGroup SWE.BinaryBuf
    @apiName SetBytes
    
    @apiParam {SWE.BinaryBuf}     		self   		binarybuf raw data
    @apiParam {number}     			offset   	offset data
    @apiParam {SWE.BinaryBuf}     		inputbuf   	binarybuf input data
    @apiParam {number}     			offset   	offset data
    @apiParam {number}     			length   	length data
    @apiSuccess (Return) {boolean}    		result   	success
*/
SWE.BinaryBuf.Insert
/**
    @api {insert byte values} SWE.BinaryBuf.Insert(self,offset,inputbuf,offset,length)(offset,count,value) SWE.BinaryBuf.Insert
    @apiGroup SWE.BinaryBuf
    @apiName Insert

    @apiParam {SWE.BinaryBuf}     		self   		binarybuf raw data
    @apiParam {number}     			offset   	offset data
    @apiParam {SWE.BinaryBuf}     		inputbuf   	binarybuf input data
    @apiParam {number}     			offset   	offset data
    @apiParam {number}     			length   	length data
    @apiParam {number}     			count  		value counts
    @apiParam {number}     			value		byte value
    @apiSuccess (Return) {boolean}    		result   	success
*/
SWE.BinaryBuf.GetByte
/**
    @api {get byte value} SWE.BinaryBuf.GetByte(self,offset1,offset2) SWE.BinaryBuf.GetByte
    @apiGroup SWE.BinaryBuf
    @apiName GetByte
    
    @apiParam {SWE.BinaryBuf}     		self   		binarybuf raw data
    @apiParam {number}     			offset1   	first data offset
    @apiParam {number}     			offset2   	last data offset (default: nil)
    @apiSuccess (Return) {list}    		result   	bytes list values

    @apiExample usage
    local buf = SWE.BinaryBuf("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ")
    -- C style offset
    local byte1 = buf:GetByte(5)
    -- Lua style offset
    local byte2 = buf[4]
    if byte1 == byte2 then
	print("equals")
    end
*/
SWE.BinaryBuf.GetBytes
/**
    @api {get byte values} SWE.BinaryBuf.GetBytes(self,offset,length) SWE.BinaryBuf.GetBytes
    @apiGroup SWE.BinaryBuf
    @apiName GetBytes

    @apiParam {SWE.BinaryBuf}     		self   		binarybuf raw data
    @apiParam {number}     			offset   	offset data
    @apiParam {number}     			length   	length data
    @apiSuccess (Return) {SWE.BinaryBuf}  	result   	bynarybuf raw data
*/
SWE.BinaryBuf.Resize
/**
    @api {resize buf} SWE.BinaryBuf.Resize(self,size,value) SWE.BinaryBuf.Resize
    @apiGroup SWE.BinaryBuf
    @apiName Resize

    @apiParam {SWE.BinaryBuf}     		self   		binarybuf raw data
    @apiParam {number}     			size  	 	new size
    @apiParam {number}     			value   	default value
    @apiSuccess (Return) {SWE.BinaryBuf}  	result   	bynarybuf raw data
*/
SWE.BinaryBuf.Assign
/**
    @api {assign buf} SWE.BinaryBuf.Assign(self,size,value) SWE.BinaryBuf.Assign
    @apiGroup SWE.BinaryBuf
    @apiName Assign

    @apiParam {SWE.BinaryBuf}     		self   		binarybuf raw data
    @apiParam {number}     			size  	 	new size
    @apiParam {number}     			value   	assign content
    @apiSuccess (Return) {SWE.BinaryBuf}  	result   	bynarybuf raw data
*/
SWE.BinaryBuf.Erase
/**
    @api {erase element} SWE.BinaryBuf.Erase(self,pos,count) SWE.BinaryBuf.Erase
    @apiGroup SWE.BinaryBuf
    @apiName Erase

    @apiParam {SWE.BinaryBuf}     		self   		binarybuf raw data
    @apiParam {number}     			pos 	 	position
    @apiParam {number}     			count   	count erase
    @apiSuccess (Return) {SWE.BinaryBuf}  	result   	bynarybuf raw data
*/
SWE.BinaryBuf.Clear
/**
    @api {binarybuf clear} SWE.BinaryBuf.Clear(self) SWE.BinaryBuf.Clear
    @apiGroup SWE.BinaryBuf
    @apiName Clear

    @apiParam {SWE.BinaryBuf}                   self           binarybuf raw data
*/
SWE.BinaryBuf.CompareBytes
/**
    @api {compare bytes} SWE.BinaryBuf.CompareBytes(self,offset,value...value) SWE.BinaryBuf.CompareBytes
    @apiGroup SWE.BinaryBuf
    @apiName CompareBytes
    
    @apiParam {SWE.BinaryBuf}     		self   		binarybuf raw data
    @apiParam {number}     			offset   	offset data
    @apiParam {number}     			value   	byte value (list)
    @apiSuccess (Return) {boolean}    		result   	success
*/
