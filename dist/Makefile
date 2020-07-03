TARGET		:= libswe
SOURCEDIR       := ../swe

ifdef OLDENGINE
ifndef DISABLE_IMAGE
SOURCEDIR       := $(SOURCEDIR) ../swe/savepng
endif
else
SOURCEDIR       := $(SOURCEDIR) ../swe/rotozoom
endif
ifdef WITH_JSON
SOURCEDIR       := $(SOURCEDIR) ../swe/jsmn
endif
ifdef WITH_XML
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
