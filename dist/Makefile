TARGET		:= libswe
SOURCEDIR       := ../swe

ifdef OLDENGINE
ifndef DISABLE_IMAGE
SOURCEDIR       := $(SOURCEDIR) ../swe/savepng
endif
endif

SEARCH          := $(addsuffix /*.cpp, $(SOURCEDIR))


$(TARGET).a: $(notdir $(patsubst %.cpp, %.o, $(wildcard $(SEARCH))))
	$(AR) crvs $@ $^

$(TARGET).so: $(notdir $(patsubst %.cpp, %.o, $(wildcard $(SEARCH))))
	$(CXX) -shared $^ $(LIBS) -o $@ $(LDFLAGS)

VPATH		:= $(SOURCEDIR)

%.o: %.cpp
	$(CXX) -c -MD $< $(CFLAGS)
#	$(CXX) -c -MD $(addprefix -I, $(SRCDIRLIST)) $< $(CFLAGS)

include $(wildcard *.d)

.PHONY: clean

clean:
	rm -f *.a *.so *.d *.o
