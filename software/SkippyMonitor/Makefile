###
### Skippy Monitor Makefile
###

## Directories
BIN_DIR=bin
DIST_DIR=dist
BUILD_DIR=build
UI_DIR=ui

## Python packages
PY_TOP_PACKAGE	= skippymonitor
PY_UI_PACKAGE	= $(PY_TOP_PACKAGE)/ui

## Applications names
GUI_NAME			= SkippyMonitor
CLI_NAME			= skipmon

## UI files
UI_SRC_FILES		= $(wildcard $(UI_DIR)/*.ui)
UI_PY_FILES			= $(patsubst $(UI_DIR)/%.ui, $(PY_UI_PACKAGE)/%.py, $(UI_SRC_FILES))

## Resource files
RC_SRC_FILES		= $(wildcard $(UI_DIR)/*.qrc)
RC_PY_FILES			= $(patsubst $(UI_DIR)/%.qrc, $(PY_UI_PACKAGE)/%_rc.py, $(RC_SRC_FILES))

## Additional imports to insert in the final bundle
HIDDEN_IMPORTS		= $(PY_TOP_PACKAGE)

## Executables
PYINSTALLER			?= pyinstaller
PYUIC				?= pyuic5
PYRCC				?= pyrcc5

## Icon files for Windows and Mac OS
WIN32_ICON=$(UI_DIR)/skippy.ico
OSX_ICON=$(UI_DIR)/skippy.icns

## Mac OS application' bundle name
OSX_BUNDLE_NAME=fr.strnk.skippy.monitor

###
### Auto-generated variables
###

## PyInstaller flags
PYINSTALLERFLAGS	= $(addprefix --hidden-import=,$(HIDDEN_IMPORTS)) --onefile -y --paths=. --distpath=$(DIST_DIR) --workpath=$(BUILD_DIR)

## OS-specific bundle options
ifeq ($(OS),Windows_NT)
	PYINSTALLERFLAGS += --icon=$(WIN32_ICON)
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        PYINSTALLERFLAGS += --icon=$(OSX_ICON) --osx-bundle-identifier=$(OSX_BUNDLE_NAME)
    endif
endif

.PHONY: all ui resources dist dist-gui dist-cli clean distclean
all: ui resources

ui: $(UI_PY_FILES)
$(PY_UI_PACKAGE)/%.py:$(UI_DIR)/%.ui
	$(PYUIC) --from-imports -o $@ $<

resources: $(RC_PY_FILES)
$(PY_UI_PACKAGE)/%_rc.py:$(UI_DIR)/%.qrc
	$(PYRCC) -o $@ $<

dist: dist-gui dist-cli

dist-gui:
	$(PYINSTALLER) $(PYINSTALLERFLAGS) --windowed $(BIN_DIR)/$(GUI_NAME)

dist-cli:
	$(PYINSTALLER) $(PYINSTALLERFLAGS) --console $(BIN_DIR)/$(CLI_NAME)

clean:
	find . -name __pycache__ -print0 | xargs -0 rm -rf
	rm -rf $(BUILD_DIR)
	rm -f $(GUI_NAME).spec $(CLI_NAME).spec

distclean: clean
	rm -f $(RC_PY_FILES) $(UI_PY_FILES)
	rm -rf $(DIST_DIR)/$(GUI_NAME){,.app,.exe} $(DIST_DIR)/$(CLI_NAME){,.exe}