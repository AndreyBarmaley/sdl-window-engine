SWE.StreamFile
/**
    @api {streamfile constructor} SWE.StreamFile(),(name,mode) SWE.StreamFile
    @apiGroup SWE.StreamFile
    @apiName Constructor
            
    @apiParam {string}				name		filename
    @apiParam {string}				mode		open mode

    @apiSuccess (Return) {SWE.StreamFile}	result		streamfile object

    @apiExample usage
    local file = SWE.StreamFile("/tmp/test.dump","rb")
    if file then
        local id1 = file:ReadBE16()
        local id2 = file:ReadBE16()
        local id3 = file:ReadBE16()
	file:Seek(12, SWE.RW.SeekSet)
        local id4 = file:ReadBE32()
        print(id1,id2,id3,id4)
    end

    @apiExample ext table fields (read only)
    | filename | string  |
    | openmode | string  |
    | size     | number  |
    | valid    | boolean |

    @apiExample open modes
    r  Open a file for reading. The file must exist.
    w  Create an empty file for writing.
       If a file with the same name already exists its content is erased and the file is treated as a new empty file.
    a  Append to a file.
       Writing operations append data at the end of the file.
       The file is created if it does not exist.
    r+ Open a file for update both reading and writing.
       The file must exist.
    w+ Create an empty file for both reading and writing.
       If a file with the same name already exists its content is erased and the file is treated as a new empty file.
    a+ Open a file for reading and appending.
       All writing operations are performed at the end of the file, protecting the previous content to be overwritten.
       You can reposition (fseek, rewind) the internal pointer to anywhere in the file for reading, but writing operations will move it back to the end of file.
       The file is created if it does not exist.

    In order to open a file as a binary file, a "b" character has to be included in the mode string.
    This additional "b" character can either be appended at the end of the string
    (thus making the following compound modes: "rb", "wb", "ab", "r+b", "w+b", "a+b") or be inserted between the letter and
    the "+" sign for the mixed modes ("rb+", "wb+", "ab+").
    Additional characters may follow the sequence, although they should have no effect.
    For example, "t" is sometimes appended to make explicit the file is a text file. 
*/
SWE.StreamFile.ReadByte
/**
    @api {streamfile read 8 bit value} SWE.StreamFile.ReadByte(self) SWE.StreamFile.ReadByte
    @apiGroup SWE.StreamFile
    @apiName ReadByte
            
    @apiParam {SWE.StreamFile}			owner		streamfile object
    @apiSuccess (Return) {number}		result		8 bit value
*/
SWE.StreamFile.ReadBE16
/**
    @api {streamfile read 16 bit value} SWE.StreamFile.ReadBE16(self) SWE.StreamFile.ReadBE16
    @apiGroup SWE.StreamFile
    @apiName ReadBE16

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiSuccess (Return) {number}		result		16 bit value (big endian)
*/
SWE.StreamFile.ReadBE32
/**
    @api {streamfile read 32 bit value} SWE.StreamFile.ReadBE32(self) SWE.StreamFile.ReadBE32
    @apiGroup SWE.StreamFile
    @apiName ReadBE32

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiSuccess (Return) {number}		result		32 bit value (big endian)
*/
SWE.StreamFile.ReadBE64
/**
    @api {streamfile read 64 bit value} SWE.StreamFile.ReadBE64(self) SWE.StreamFile.ReadBE64
    @apiGroup SWE.StreamFile
    @apiName ReadBE64

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiSuccess (Return) {number}		result		64 bit value (big endian)
*/

SWE.StreamFile.ReadLE16
/**
    @api {streamfile read 16 bit value} SWE.StreamFile.ReadLE16(self) SWE.StreamFile.ReadLE16
    @apiGroup SWE.StreamFile
    @apiName ReadLE16

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiSuccess (Return) {number}		result		16 bit value (little endian)
*/
SWE.StreamFile.ReadLE32
/**
    @api {streamfile read 32 bit value} SWE.StreamFile.ReadLE32(self) SWE.StreamFile.ReadLE32
    @apiGroup SWE.StreamFile
    @apiName ReadLE32

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiSuccess (Return) {number}		result		32 bit value (little endian)
*/
SWE.StreamFile.ReadLE64
/**
    @api {streamfile read 64 bit value} SWE.StreamFile.ReadLE64(self) SWE.StreamFile.ReadLE64
    @apiGroup SWE.StreamFile
    @apiName ReadLE64

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiSuccess (Return) {number}		result		64 bit value (little endian)
*/
SWE.StreamFile.ReadBytes
/**
    @api {streamfile read bytes} SWE.StreamFile.ReadBytes(self,length) SWE.StreamFile.ReadBytes
    @apiGroup SWE.StreamFile
    @apiName ReadBytes

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiParam {number}				length		length data
    @apiSuccess (Return) {SWE.BinaryBuf}	result		binarybuf object
*/
SWE.StreamFile.WriteByte
/**
    @api {streamfile write 8 bit value} SWE.StreamFile.WriteByte(self,value) SWE.StreamFile.WriteByte
    @apiGroup SWE.StreamFile
    @apiName WriteByte

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiParam {number}				value		8 bit value write
*/
SWE.StreamFile.WriteBE16
/**
    @api {streamfile write 16 bit value} SWE.StreamFile.WriteBE16(self,value) SWE.StreamFile.WriteBE16
    @apiGroup SWE.StreamFile
    @apiName WriteBE16

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiParam {number}				value		16 bit value write (big endian)
*/
SWE.StreamFile.WriteBE32
/**
    @api {streamfile write 32 bit value} SWE.StreamFile.WriteBE32(self,value) SWE.StreamFile.WriteBE32
    @apiGroup SWE.StreamFile
    @apiName WriteBE32

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiParam {number}				value		32 bit value write (big endian)
*/
SWE.StreamFile.WriteBE16
/**
    @api {streamfile write 64 bit value} SWE.StreamFile.WriteBE64(self,value) SWE.StreamFile.WriteBE64
    @apiGroup SWE.StreamFile
    @apiName WriteBE64

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiParam {number}				value		64 bit value write (big endian)
*/

SWE.StreamFile.WriteLE16
/**
    @api {streamfile write 16 bit value} SWE.StreamFile.WriteLE16(self,value) SWE.StreamFile.WriteLE16
    @apiGroup SWE.StreamFile
    @apiName WriteLE16

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiParam {number}				value		16 bit value write (little endian)
*/
SWE.StreamFile.WriteLE32
/**
    @api {streamfile write 32 bit value} SWE.StreamFile.WriteLE32(self,value) SWE.StreamFile.WriteLE32
    @apiGroup SWE.StreamFile
    @apiName WriteLE32

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiParam {number}				value		32 bit value write (little endian)
*/
SWE.StreamFile.WriteLE16
/**
    @api {streamfile write 64 bit value} SWE.StreamFile.WriteLE64(self,value) SWE.StreamFile.WriteLE64
    @apiGroup SWE.StreamFile
    @apiName WriteLE64

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiParam {number}				value		64 bit value write (little endian)
*/
SWE.StreamFile.WriteBytes
/**
    @api {streamfile write bytes} SWE.StreamFile.WriteBytes(self,inputbuf) SWE.StreamFile.WriteBytes
    @apiGroup SWE.StreamFile
    @apiName WriteBytes

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiParam {SWE.BinaryBuf}			inputbuf	binarybuf object
*/
SWE.StreamFile.Close
/**
    @api {streamfile close stream} SWE.StreamFile.Close(self) SWE.StreamFile.Close
    @apiGroup SWE.StreamFile
    @apiName Close

    @apiParam {SWE.StreamFile}			self		streamfile object
*/
SWE.StreamFile.Open
/**
    @api {streamfile open stream} SWE.StreamFile.Open(self,name,mode) SWE.StreamFile.Open
    @apiGroup SWE.StreamFile
    @apiName Open

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiParam {string}				name		filename
    @apiParam {string}				mode		open mode
    @apiSuccess (Return) {boolean}              result          true if success

    @apiExample open modes
    r  Open a file for reading. The file must exist.
    w  Create an empty file for writing.
       If a file with the same name already exists its content is erased and the file is treated as a new empty file.
    a  Append to a file.
       Writing operations append data at the end of the file.
       The file is created if it does not exist.
    r+ Open a file for update both reading and writing.
       The file must exist.
    w+ Create an empty file for both reading and writing.
       If a file with the same name already exists its content is erased and the file is treated as a new empty file.
    a+ Open a file for reading and appending.
       All writing operations are performed at the end of the file, protecting the previous content to be overwritten.
       You can reposition (fseek, rewind) the internal pointer to anywhere in the file for reading, but writing operations will move it back to the end of file.
       The file is created if it does not exist.

    In order to open a file as a binary file, a "b" character has to be included in the mode string.
    This additional "b" character can either be appended at the end of the string
    (thus making the following compound modes: "rb", "wb", "ab", "r+b", "w+b", "a+b") or be inserted between the letter and
    the "+" sign for the mixed modes ("rb+", "wb+", "ab+").
    Additional characters may follow the sequence, although they should have no effect.
    For example, "t" is sometimes appended to make explicit the file is a text file. 
*/
SWE.StreamFile.Seek
/**
    @api {streamfile seek pos} SWE.StreamFile.Seek(self,offset,whence) SWE.StreamFile.Seek
    @apiGroup SWE.StreamFile
    @apiName Seek

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiParam {number}				offset		seek position
    @apiParam {number}				whence		SWE.RW {Seek.Set, SeekCur, SeekEnd}
    @apiSuccess (Return) {boolean}              result          true if success
*/
SWE.StreamFile.Tell
/**
    @api {streamfile tell pos} SWE.StreamFile.Tell(self) SWE.StreamFile.Tell
    @apiGroup SWE.StreamFile
    @apiName Tell

    @apiParam {SWE.StreamFile}			self		streamfile object
    @apiSuccess (Return) {number}               result          tell position
*/
SWE.StreamFile.ToJson
/**
    @api {convert streamfile to json string} SWE.StreamFile.ToJson(self) SWE.StreamFile.ToJson
    @apiGroup SWE.StreamFile
    @apiName ToJson
            
    @apiParam {SWE.StreamFile}                   self            streamfile object
    @apiSuccess (Return) {string}               result           json string
*/
