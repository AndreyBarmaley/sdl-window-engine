SWE.StreamBuf
/**
    @api {streambuf constructor} SWE.StreamBuf(),(buf) SWE.StreamBuf
    @apiGroup SWE.StreamBuf
    @apiName Constructor
            
    @apiParam {SWE.BinaryBuf}			buf		initial buffer
    @apiSuccess (Return) {SWE.StreamBuf}	result		streambuf object
*/
SWE.StreamBuf.GetByte
/**
    @api {streambuf get 8 bit value} SWE.StreamBuf.GetByte(self) SWE.StreamBuf.GetByte
    @apiGroup SWE.StreamBuf
    @apiName GetByte
            
    @apiParam {SWE.StreamBuf}			owner		streambuf object
    @apiSuccess (Return) {number}		result		8 bit value
*/
SWE.StreamBuf.GetBE16
/**
    @api {streambuf get 16 bit value} SWE.StreamBuf.GetBE16(self) SWE.StreamBuf.GetBE16
    @apiGroup SWE.StreamBuf
    @apiName GetBE16

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiSuccess (Return) {number}		result		16 bit value (big endian)
*/
SWE.StreamBuf.GetBE32
/**
    @api {streambuf get 32 bit value} SWE.StreamBuf.GetBE32(self) SWE.StreamBuf.GetBE32
    @apiGroup SWE.StreamBuf
    @apiName GetBE32

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiSuccess (Return) {number}		result		32 bit value (big endian)
*/
SWE.StreamBuf.GetBE64
/**
    @api {streambuf get 64 bit value} SWE.StreamBuf.GetBE64(self) SWE.StreamBuf.GetBE64
    @apiGroup SWE.StreamBuf
    @apiName GetBE64

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiSuccess (Return) {number}		result		64 bit value (big endian)
*/

SWE.StreamBuf.GetLE16
/**
    @api {streambuf get 16 bit value} SWE.StreamBuf.GetLE16(self) SWE.StreamBuf.GetLE16
    @apiGroup SWE.StreamBuf
    @apiName GetLE16

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiSuccess (Return) {number}		result		16 bit value (little endian)
*/
SWE.StreamBuf.GetLE32
/**
    @api {streambuf get 32 bit value} SWE.StreamBuf.GetLE32(self) SWE.StreamBuf.GetLE32
    @apiGroup SWE.StreamBuf
    @apiName GetLE32

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiSuccess (Return) {number}		result		32 bit value (little endian)
*/
SWE.StreamBuf.GetLE64
/**
    @api {streambuf get 64 bit value} SWE.StreamBuf.GetLE64(self) SWE.StreamBuf.GetLE64
    @apiGroup SWE.StreamBuf
    @apiName GetLE64

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiSuccess (Return) {number}		result		64 bit value (little endian)
*/
SWE.StreamBuf.GetBytes
/**
    @api {streambuf get bytes} SWE.StreamBuf.GetBytes(self,length) SWE.StreamBuf.GetBytes
    @apiGroup SWE.StreamBuf
    @apiName GetBytes

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiParam {number}				length		length data
    @apiSuccess (Return) {SWE.BinaryBuf}	result		binarybuf object
*/
SWE.StreamBuf.GetString
/**
    @api {streambuf get string} SWE.StreamBuf.GetString(self,length) SWE.StreamBuf.GetString
    @apiGroup SWE.StreamBuf
    @apiName GetString

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiParam {number}				length		length data
    @apiSuccess (Return) {string}		result		string data
*/
SWE.StreamBuf.PutByte
/**
    @api {streambuf put 8 bit value} SWE.StreamBuf.PutByte(self,value) SWE.StreamBuf.PutByte
    @apiGroup SWE.StreamBuf
    @apiName PutByte

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiParam {number}				value		8 bit value put
*/
SWE.StreamBuf.PutBE16
/**
    @api {streambuf put 16 bit value} SWE.StreamBuf.PutBE16(self,value) SWE.StreamBuf.PutBE16
    @apiGroup SWE.StreamBuf
    @apiName PutBE16

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiParam {number}				value		16 bit value put (big endian)
*/
SWE.StreamBuf.PutBE32
/**
    @api {streambuf put 32 bit value} SWE.StreamBuf.PutBE32(self,value) SWE.StreamBuf.PutBE32
    @apiGroup SWE.StreamBuf
    @apiName PutBE32

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiParam {number}				value		32 bit value put (big endian)
*/
SWE.StreamBuf.PutBE16
/**
    @api {streambuf put 64 bit value} SWE.StreamBuf.PutBE64(self,value) SWE.StreamBuf.PutBE64
    @apiGroup SWE.StreamBuf
    @apiName PutBE64

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiParam {number}				value		64 bit value put (big endian)
*/

SWE.StreamBuf.PutLE16
/**
    @api {streambuf put 16 bit value} SWE.StreamBuf.PutLE16(self,value) SWE.StreamBuf.PutLE16
    @apiGroup SWE.StreamBuf
    @apiName PutLE16

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiParam {number}				value		16 bit value put (little endian)
*/
SWE.StreamBuf.PutLE32
/**
    @api {streambuf put 32 bit value} SWE.StreamBuf.PutLE32(self,value) SWE.StreamBuf.PutLE32
    @apiGroup SWE.StreamBuf
    @apiName PutLE32

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiParam {number}				value		32 bit value put (little endian)
*/
SWE.StreamBuf.PutLE16
/**
    @api {streambuf put 64 bit value} SWE.StreamBuf.PutLE64(self,value) SWE.StreamBuf.PutLE64
    @apiGroup SWE.StreamBuf
    @apiName PutLE64

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiParam {number}				value		64 bit value put (little endian)
*/
SWE.StreamBuf.PutBytes
/**
    @api {streambuf put bytes} SWE.StreamBuf.PutBytes(self,inputbuf) SWE.StreamBuf.PutBytes
    @apiGroup SWE.StreamBuf
    @apiName PutBytes

    @apiParam {SWE.StreamBuf}			self		streambuf object
    @apiParam {SWE.BinaryBuf}			inputbuf	binarybuf object
*/
SWE.StreamBuf.ToJson
/**
    @api {convert streambuf to json string} SWE.StreamBuf.ToJson(self) SWE.StreamBuf.ToJson
    @apiGroup SWE.StreamBuf
    @apiName ToJson
            
    @apiParam {SWE.StreamBuf}                   self            streambuf object
    @apiSuccess (Return) {string}               result          json string
*/
