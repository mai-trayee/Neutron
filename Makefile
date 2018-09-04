.PHONY: clean

#CXX = $(CXX11)

INCDIR =	include
APPDIR =	app
BINDIR = 	bin
LIBDIR =	lib

ROOTLIB =	-L$(ROOTSYS)/lib -lGui -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic
ROOTCXX = 	-pthread -m64 -I$(ROOTSYS)/include
G4LIB =		$(shell geant4-config --libs)
G4CXX =		$(shell geant4-config --cflags)
#G4CXX = -DG4UI_USE_TCSH -W -Wall -pedantic -Wno-non-virtual-dtor -Wno-long-long -Wwrite-strings -Wpointer-arith -Woverloaded-virtual -Wno-variadic-macros -Wshadow -pipe -DG4USE_STD11 -I/data/tboschi/geant4.10.04.p01/bin/../include/Geant4

LDFLAGS  :=	-Wl,--no-as-needed $(LDFLAGS) $(ROOTLIB) $(G4LIB) -L$(LIBDIR)
CXXFLAGS :=	$(CXXFLAGS) -O3 -mavx $(ROOTCXX) $(G4CXX) -I$(INCDIR)

#exectuables
CPP =	CfInScint \
	ReadRoot
	#First	\

#header folders
HPP =	CfInScint/MsgOutput	\
	CfInScint/MsgVolume	\
	CfInScint/Volume	\
	CfInScint/Run		\
	CfInScint/Generator	\
	CfInScint/Event		\
	CfInScint/Step		\
	#First	\

#main targets
TGT :=	$(CPP:%=$(APPDIR)/%)

#dependencies of targers
#INCDEP :=	$(HPP:%=$(INCDIR)/%/*.cpp)
INCDEP := 	$(HPP:%=$(INCDIR)/%.cpp)
DEP:=		$(patsubst %.cpp,%.o,$(wildcard $(INCDEP)))

all:	$(TGT)
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)
	@echo "Moving stuff..."
	@mv $(TGT) $(BINDIR)

$(TGT): $(DEP)

clean:
	find $(INCDIR) -mindepth 1 -name "*.o" -delete
	find $(INCDIR) -mindepth 1 -name "*~" -delete
	find $(APPDIR) -mindepth 1 -name "*~" -delete
	find $(BINDIR) -mindepth 1 -name "*" -delete
