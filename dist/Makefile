TARGET		:= libswe
SOURCEDIR       := ../swe

ifdef SWE_SDL12
ifndef SWE_DISABLE_IMAGE
SOURCEDIR       := $(SOURCEDIR) ../swe/savepng
endif
else
SOURCEDIR       := $(SOURCEDIR) ../swe/rotozoom
endif
ifdef SWE_WITH_JSON
SOURCEDIR       := $(SOURCEDIR) ../swe/jsmn
endif
ifdef SWE_WITH_XML
SOURCEDIR       := $(SOURCEDIR) ../swe/tinyxml2
endif

SEARCH          := $(addsuffix /*.cpp, $(SOURCEDIR))


$(TARGET).a: $(notdir $(patsubst %.cpp, %.o, $(wildcard $(SEARCH))))
	$(AR) crvs $@ $^

$(TARGET).so: $(notdir $(patsubst %.cpp, %.o, $(wildcard $(SEARCH))))
	$(CXX) -shared $^ $(LIBS) -o $@ $(LDFLAGS)

VPATH		:= $(SOURCEDIR)

%.o: %.cpp
	$(CXX) -c -MD $< $(CFLAGS)

include $(wildcard *.d)

.PHONY: clean

clean:
	rm -f *.a *.so *.d *.o
