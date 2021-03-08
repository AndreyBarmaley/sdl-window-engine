SWE.StreamNet
/**
    @api {streamnet constructor} SWE.StreamNet(),(server,port) SWE.StreamNet
    @apiGroup SWE.StreamNet
    @apiName Constructor
            
    @apiParam {string}				server		network connect
    @apiParam {number}				port		network port
    @apiSuccess (Return) {SWE.StreamNet}	result		streamnet object

    @apiExample usage
    local net = SWE.StreamNet()
    local res = net:Connect("127.0.0.1", 22)
    if res then
        local info = net:RecvString(0x0A)
        net:SendString("quit\r\n")
        print(info)
    else
        print("connect false")
    end

    .... may be output
    .... SSH-2.0-OpenSSH_5.3

    @apiExample ext table fields (read only)
    | address | string  |
    | port    | number  |
*/
SWE.StreamNet.Connect
/**
    @api {streamnet connect} SWE.StreamNet.Connect(self,server,port) SWE.StreamNet.Connect
    @apiGroup SWE.StreamNet
    @apiName Connect

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {string}				server		network connect
    @apiParam {number}				port		network port
    @apiSuccess (Return) {boolean}		result		true if success
*/
SWE.StreamNet.Listen
/**
    @api {streamnet listen port} SWE.StreamNet.Listen(self,port) SWE.StreamNet.Listen
    @apiGroup SWE.StreamNet
    @apiName Listen

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {number}				port		network port
    @apiSuccess (Return) {boolean}		result		true if success
*/
SWE.StreamNet.WaitAccept
/**
    @api {streamnet wait accept} SWE.StreamNet.WaitAccept(self) SWE.StreamNet.WaitAccept
    @apiGroup SWE.StreamNet
    @apiName WaitAccept

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiSuccess (Return) {string}		result		streamnet object

    @apiExample usage
    local net = SWE.StreamNet()
    local res = net:Listen(22222)
    if res then
        local client = net:WaitAccept()
        if client ~= nil then
            client:SendString("hello world!\r\n")
        end
    else
        print("listen false")
    end
*/
SWE.StreamNet.RecvByte
/**
    @api {streamnet recv 8 bit value} SWE.StreamNet.RecvByte(self) SWE.StreamNet.RecvByte
    @apiGroup SWE.StreamNet
    @apiName RecvByte
            
    @apiParam {SWE.StreamNet}			owner		streamnet object
    @apiSuccess (Return) {number}		result		8 bit value
*/
SWE.StreamNet.RecvBE16
/**
    @api {streamnet recv 16 bit value} SWE.StreamNet.RecvBE16(self) SWE.StreamNet.RecvBE16
    @apiGroup SWE.StreamNet
    @apiName RecvBE16

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiSuccess (Return) {number}		result		16 bit value (big endian)
*/
SWE.StreamNet.RecvBE32
/**
    @api {streamnet recv 32 bit value} SWE.StreamNet.RecvBE32(self) SWE.StreamNet.RecvBE32
    @apiGroup SWE.StreamNet
    @apiName RecvBE32

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiSuccess (Return) {number}		result		32 bit value (big endian)
*/
SWE.StreamNet.RecvBE64
/**
    @api {streamnet recv 64 bit value} SWE.StreamNet.RecvBE64(self) SWE.StreamNet.RecvBE64
    @apiGroup SWE.StreamNet
    @apiName RecvBE64

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiSuccess (Return) {number}		result		64 bit value (big endian)
*/

SWE.StreamNet.RecvLE16
/**
    @api {streamnet recv 16 bit value} SWE.StreamNet.RecvLE16(self) SWE.StreamNet.RecvLE16
    @apiGroup SWE.StreamNet
    @apiName RecvLE16

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiSuccess (Return) {number}		result		16 bit value (little endian)
*/
SWE.StreamNet.RecvLE32
/**
    @api {streamnet recv 32 bit value} SWE.StreamNet.RecvLE32(self) SWE.StreamNet.RecvLE32
    @apiGroup SWE.StreamNet
    @apiName RecvLE32

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiSuccess (Return) {number}		result		32 bit value (little endian)
*/
SWE.StreamNet.RecvLE64
/**
    @api {streamnet recv 64 bit value} SWE.StreamNet.RecvLE64(self) SWE.StreamNet.RecvLE64
    @apiGroup SWE.StreamNet
    @apiName RecvLE64

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiSuccess (Return) {number}		result		64 bit value (little endian)
*/
SWE.StreamNet.RecvBytes
/**
    @api {streamnet recv bytes} SWE.StreamNet.RecvBytes(self,length) SWE.StreamNet.RecvBytes
    @apiGroup SWE.StreamNet
    @apiName RecvBytes

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {number}				length		length data
    @apiSuccess (Return) {SWE.BinaryBuf}	result		binarybuf object
*/
SWE.StreamNet.RecvString
/**
    @api {streamnet recv string} SWE.StreamNet.RecvString(self,endl,waitms,forcebr) SWE.StreamNet.RecvString
    @apiGroup SWE.StreamNet
    @apiName RecvString

    @apiParam {SWE.StreamNet}			owner		streamnet object
    @apiParam {number}				endl		eol byte (default: 0)
    @apiParam {number}				waitms		wait ms (default: 300)
    @apiParam {boolean}				forcebr		force break without recv eol bite (default: false)
    @apiSuccess (Return) {string}		result		string result

    @apiExample usage
    -- echo service
    local net = SWE.StreamNet()
    local res = net:Listen(2121)
    if res then
	local client = net:WaitAccept()
	if client ~= nil then
    	    client:SendString("echo service\r\n")
    	    local echo = client:RecvString(0x0A)
    	    client:SendString(echo)
	end
    end
*/
SWE.StreamNet.SendByte
/**
    @api {streamnet send 8 bit value} SWE.StreamNet.SendByte(self,value) SWE.StreamNet.SendByte
    @apiGroup SWE.StreamNet
    @apiName SendByte

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {number}				value		8 bit value send
*/
SWE.StreamNet.SendBE16
/**
    @api {streamnet send 16 bit value} SWE.StreamNet.SendBE16(self,value) SWE.StreamNet.SendBE16
    @apiGroup SWE.StreamNet
    @apiName SendBE16

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {number}				value		16 bit value send (big endian)
*/
SWE.StreamNet.SendBE32
/**
    @api {streamnet send 32 bit value} SWE.StreamNet.SendBE32(self,value) SWE.StreamNet.SendBE32
    @apiGroup SWE.StreamNet
    @apiName SendBE32

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {number}				value		32 bit value send (big endian)
*/
SWE.StreamNet.SendBE16
/**
    @api {streamnet send 64 bit value} SWE.StreamNet.SendBE64(self,value) SWE.StreamNet.SendBE64
    @apiGroup SWE.StreamNet
    @apiName SendBE64

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {number}				value		64 bit value send (big endian)
*/

SWE.StreamNet.SendLE16
/**
    @api {streamnet send 16 bit value} SWE.StreamNet.SendLE16(self,value) SWE.StreamNet.SendLE16
    @apiGroup SWE.StreamNet
    @apiName SendLE16

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {number}				value		16 bit value send (little endian)
*/
SWE.StreamNet.SendLE32
/**
    @api {streamnet send 32 bit value} SWE.StreamNet.SendLE32(self,value) SWE.StreamNet.SendLE32
    @apiGroup SWE.StreamNet
    @apiName SendLE32

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {number}				value		32 bit value send (little endian)
*/
SWE.StreamNet.SendLE16
/**
    @api {streamnet send 64 bit value} SWE.StreamNet.SendLE64(self,value) SWE.StreamNet.SendLE64
    @apiGroup SWE.StreamNet
    @apiName SendLE64

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {number}				value		64 bit value send (little endian)
*/
SWE.StreamNet.SendBytes
/**
    @api {streamnet send bytes} SWE.StreamNet.SendBytes(self,inputbuf) SWE.StreamNet.SendBytes
    @apiGroup SWE.StreamNet
    @apiName SendBytes

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {SWE.BinaryBuf}			inputbuf	binarybuf object
*/
SWE.StreamNet.SendString
/**
    @api {streamnet send string} SWE.StreamNet.SendString(self,string) SWE.StreamNet.SendString
    @apiGroup SWE.StreamNet
    @apiName SendString

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {string}				string		string data
*/
SWE.StreamNet.Close
/**
    @api {streamnet close connect} SWE.StreamNet.Close(self) SWE.StreamNet.Close
    @apiGroup SWE.StreamNet
    @apiName Close

    @apiParam {SWE.StreamNet}			self		streamnet object
*/
SWE.StreamNet.DataReady
/**
    @api {streamnet data ready} SWE.StreamNet.DataReady(self) SWE.StreamNet.DataReady
    @apiGroup SWE.StreamNet
    @apiName DataReady

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiSuccess (Return) {boolean}		result		true if data ready
*/
SWE.StreamNet.WaitString
/**
    @api {streamnet wait string marker} SWE.StreamNet.WaitString(self,string) SWE.StreamNet.WaitString
    @apiGroup SWE.StreamNet
    @apiName WaitString

    @apiParam {SWE.StreamNet}			self		streamnet object
    @apiParam {string}				string		wait this marker
    @apiSuccess (Return) {boolean}		result		true if success, or false timeout

    @apiExample usage
    local net = SWE.StreamNet()
    -- connect to www.com
    local res = net:Connect("69.172.201.208", 80)
    if res then
	local request = "GET / HTTP/1.1\r\nAccept: *" .. "/" .. "*\r\n\r\n"
	net:SendString(request)
	if net:WaitString("<html xmlns=\"http://www.w3.org/1999/xhtml\">\r\n") then
    	    local html = net:RecvString()
    	    print(html)
	end
    else
	print("error: res false")
    end
*/
SWE.StreamNet.LocalAddresses
/**
    @api {return local addresses} SWE.StreamNet.LocalAddresses() SWE.StreamNet.LocalAddresses
    @apiGroup SWE.StreamNet
    @apiName LocalAddresses
            
    @apiSuccess (Return) {table}               result          local addresses list

    @apiExample usage
    local addrs = SWE.StreamNet.LocalAddresses()
    for i=1,#addrs do
        print("ip address" .. addrs[i])
    end
*/
SWE.StreamNet.ToJson
/**
    @api {convert streamnet to json string} SWE.StreamNet.ToJson(self) SWE.StreamNet.ToJson
    @apiGroup SWE.StreamNet
    @apiName ToJson
            
    @apiParam {SWE.StreamNet}                   self            streamnet object
    @apiSuccess (Return) {string}               result          json string
*/
