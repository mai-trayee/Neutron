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
cscin =	CfInScint	\

ctrig = CfTrigger	\
	VisTrigger

csk = visSK

cother = VMEread	\
	 Unify
	#ReadRoot	\
	ExtractRoot	

#header folders
hscin =	InScint/InScintMsgOutput	\
	InScint/InScintMsgVolume	\
	InScint/InScintVolume		\
	InScint/InScintRun		\
	InScint/InScintGenerator	\
	InScint/InScintEvent		\
	InScint/InScintStep		

htrig = Trigger/TriggerMsgOutput	\
	Trigger/TriggerMsgVolume	\
	Trigger/TriggerVolume		\
	Trigger/TriggerRun		\
	Trigger/TriggerGenerator	\
	Trigger/TriggerEvent		\
	Trigger/TriggerStep		\

hsk = SK/WCSimDarkRateMessenger.hh	\
      SK/WCSimEnumerations.hh		\
      SK/WCSimRootOptions.hh       	\
      SK/WCSimWCDigi.hh	\
      SK/WCSimDetectorConstruction.hh  	\
      SK/WCSimTuningMessenger.hh   	\
      SK/WCSimWCHit.hh	\
      SK/WCSimDetectorMessenger.hh     	\
      SK/WCSimTuningParameters.hh  	\
      SK/WCSimWCPMT.hh	\
      SK/WCSimPmtInfo.hh               	\
      SK/WCSimVisManager.hh	\
      SK/WCSimPMTObject.hh             	\
      SK/WCSimWCAddDarkNoise.hh	\

hother = LoadTrack			\
	 PMT2root			\
	 Unify

#main targets
Tscin := $(cscin:%=$(APPDIR)/%)
Ttrig := $(ctrig:%=$(APPDIR)/%)
Tsk   := $(csk:%=$(APPDIR)/%)
Tother := $(cother:%=$(APPDIR)/%)

#dependencies of targers
#INCDEP :=	$(HPP:%=$(INCDIR)/%/*.cpp)
Iscin := $(hscin:%=$(INCDIR)/%.cpp)
Itrig := $(htrig:%=$(INCDIR)/%.cpp)
Isk   := $(hsk:%=$(INCDIR)/%.cpp)
Iother := $(hother:%=$(INCDIR)/%.cpp)

Dscin := $(patsubst %.cpp,%.o,$(wildcard $(Iscin)))
Dtrig := $(patsubst %.cpp,%.o,$(wildcard $(Itrig)))
Dsk   := $(patsubst %.cpp,%.o,$(wildcard $(Isk)))
Dother := $(patsubst %.cpp,%.o,$(wildcard $(Iother)))

inscint: $(Tscin) all
	-@mv $(Tscin) $(BINDIR)

trigger: $(Ttrig) all
	-@mv $(Ttrig) $(BINDIR)

sk : $(Tsk) all
	-@mv $(Tsk) $(BINDIR)

other: $(Tother) all
	-@mv $(Tother) $(BINDIR)

all:
	@mkdir -p $(BINDIR)
	@mkdir -p $(LIBDIR)
	@echo "Moving stuff..."

$(Tscin): $(Dscin)
$(Ttrig): $(Dtrig)
$(Tother): $(Dother)

clean:
	find $(INCDIR) -mindepth 1 -name "*.o" -delete
	find $(INCDIR) -mindepth 1 -name "*~" -delete
	find $(APPDIR) -mindepth 1 -name "*~" -delete
	find $(BINDIR) -mindepth 1 -name "*" -delete
