#=============================================================
#
#	TV.MAK - Makefile for project C:\BCOS2\tvlib\tv.PRJ
#		Created on 05/19/99 at 12:18
#
#=============================================================

.AUTODEPEND

#=============================================================
#		Translator Definitions
#=============================================================
CC = bcc +TV.CFG
TASM = tasm.exe
TLIB = tlib.exe
TLINK = tlink
RC = brcc.exe
RB = rc.exe
LIBPATH = \BCOS2\LIB
INCLUDEPATH = \BCOS2\INCLUDE;.\;.\tvision;


#=============================================================
#		Implicit Rules
#=============================================================
.c.obj:
  $(CC) -c {$< }

.cpp.obj:
  $(CC) -c {$< }

.asm.obj:
  $(TASM) -Mx $*.asm,$*.obj

.rc.res:
  $(RC) -r $*.rc

#=============================================================
#		List Macros
#=============================================================


LIB_DEPENDENCIES =  \
 twindow.obj \
 tvtext.obj \
 tview.obj \
 tvalidator.obj \
 tstrlistmaker.obj \
 tstringcollection.obj \
 tstatusline.obj \
 tstatictext.obj \
 tsortedcollection.obj \
 tscroller.obj \
 tscrollbar.obj \
 tresourcefile.obj \
 tresourcecollection.obj \
 tradiobuttons.obj \
 tprogram.obj \
 tpoint.obj \
 tparamtext.obj \
 toutline.obj \
 tobjstrm.obj \
 tobject.obj \
 tmulticheckboxes.obj \
 tmenuview.obj \
 tmenupopup.obj \
 tmenubox.obj \
 tmenubar.obj \
 tmemo.obj \
 tlistviewer.obj \
 tlistbox.obj \
 tlabel.obj \
 tinputline.obj \
 tindicator.obj \
 thistorywindow.obj \
 thistoryviewer.obj \
 thistory.obj \
 tgroup.obj \
 tframe.obj \
 tfilelist.obj \
 tfileeditor.obj \
 tfiledialog.obj \
 tfilecollection.obj \
 textview.obj \
 teditwindow.obj \
 teditor.obj \
 tdirlistbox.obj \
 tdircollection.obj \
 tdialog.obj \
 tdesktop.obj \
 tcommandset.obj \
 tcollection.obj \
 tcluster.obj \
 tcheckboxes.obj \
 tchdirdialog.obj \
 tbutton.obj \
 tbackground.obj \
 tapplication.obj \
 system.obj \
 stddlg.obj \
 sall.obj \
 palette.obj \
 new.obj \
 msgbox.obj \
 misc.obj \
 menu.obj \
 histlist.obj \
 helpbase.obj \
 help.obj \
 editstat.obj \
 drivers.obj \
 colorsel.obj \
 asm.obj \
 allnames.obj

#=============================================================
#		Explicit Rules
#=============================================================
tv.lib: tv.cfg $(LIB_DEPENDENCIES)
  - del tv.lib
  $(TLIB) $< /P64 /C  @&&|
-+twindow.obj &
-+tvtext.obj &
-+tview.obj &
-+tvalidator.obj &
-+tstrlistmaker.obj &
-+tstringcollection.obj &
-+tstatusline.obj &
-+tstatictext.obj &
-+tsortedcollection.obj &
-+tscroller.obj &
-+tscrollbar.obj &
-+tresourcefile.obj &
-+tresourcecollection.obj &
-+tradiobuttons.obj &
-+tprogram.obj &
-+tpoint.obj &
-+tparamtext.obj &
-+toutline.obj &
-+tobjstrm.obj &
-+tobject.obj &
-+tmulticheckboxes.obj &
-+tmenuview.obj &
-+tmenupopup.obj &
-+tmenubox.obj &
-+tmenubar.obj &
-+tmemo.obj &
-+tlistviewer.obj &
-+tlistbox.obj &
-+tlabel.obj &
-+tinputline.obj &
-+tindicator.obj &
-+thistorywindow.obj &
-+thistoryviewer.obj &
-+thistory.obj &
-+tgroup.obj &
-+tframe.obj &
-+tfilelist.obj &
-+tfileeditor.obj &
-+tfiledialog.obj &
-+tfilecollection.obj &
-+textview.obj &
-+teditwindow.obj &
-+teditor.obj &
-+tdirlistbox.obj &
-+tdircollection.obj &
-+tdialog.obj &
-+tdesktop.obj &
-+tcommandset.obj &
-+tcollection.obj &
-+tcluster.obj &
-+tcheckboxes.obj &
-+tchdirdialog.obj &
-+tbutton.obj &
-+tbackground.obj &
-+tapplication.obj &
-+system.obj &
-+stddlg.obj &
-+sall.obj &
-+palette.obj &
-+new.obj &
-+msgbox.obj &
-+misc.obj &
-+menu.obj &
-+histlist.obj &
-+helpbase.obj &
-+help.obj &
-+editstat.obj &
-+drivers.obj &
-+colorsel.obj &
-+asm.obj &
-+allnames.obj
|


#=============================================================
#		Individual File Dependencies
#=============================================================
TWindow.obj: tv.cfg src\TWindow.cpp 
	$(CC) -c src\TWindow.cpp

tvtext.obj: tv.cfg src\tvtext.cpp 
	$(CC) -c src\tvtext.cpp

TView.obj: tv.cfg src\TView.cpp 
	$(CC) -c src\TView.cpp

TValidator.obj: tv.cfg src\TValidator.cpp 
	$(CC) -c src\TValidator.cpp

TStrListMaker.obj: tv.cfg src\TStrListMaker.cpp 
	$(CC) -c src\TStrListMaker.cpp

TStringCollection.obj: tv.cfg src\TStringCollection.cpp 
	$(CC) -c src\TStringCollection.cpp

TStatusLine.obj: tv.cfg src\TStatusLine.cpp 
	$(CC) -c src\TStatusLine.cpp

TStaticText.obj: tv.cfg src\TStaticText.cpp 
	$(CC) -c src\TStaticText.cpp

TSortedCollection.obj: tv.cfg src\TSortedCollection.cpp 
	$(CC) -c src\TSortedCollection.cpp

TScroller.obj: tv.cfg src\TScroller.cpp 
	$(CC) -c src\TScroller.cpp

TScrollBar.obj: tv.cfg src\TScrollBar.cpp 
	$(CC) -c src\TScrollBar.cpp

TResourceFile.obj: tv.cfg src\TResourceFile.cpp 
	$(CC) -c src\TResourceFile.cpp

TResourceCollection.obj: tv.cfg src\TResourceCollection.cpp 
	$(CC) -c src\TResourceCollection.cpp

TRadioButtons.obj: tv.cfg src\TRadioButtons.cpp 
	$(CC) -c src\TRadioButtons.cpp

TProgram.obj: tv.cfg src\TProgram.cpp 
	$(CC) -c src\TProgram.cpp

TPoint.obj: tv.cfg src\TPoint.cpp 
	$(CC) -c src\TPoint.cpp

TParamText.obj: tv.cfg src\TParamText.cpp 
	$(CC) -c src\TParamText.cpp

TOutline.obj: tv.cfg src\TOutline.cpp 
	$(CC) -c src\TOutline.cpp

tobjstrm.obj: tv.cfg src\tobjstrm.cpp 
	$(CC) -c src\tobjstrm.cpp

TObject.obj: tv.cfg src\TObject.cpp 
	$(CC) -c src\TObject.cpp

TMultiCheckBoxes.obj: tv.cfg src\TMultiCheckBoxes.cpp 
	$(CC) -c src\TMultiCheckBoxes.cpp

TMenuView.obj: tv.cfg src\TMenuView.cpp 
	$(CC) -c src\TMenuView.cpp

TMenuPopup.obj: tv.cfg src\TMenuPopup.cpp 
	$(CC) -c src\TMenuPopup.cpp

TMenuBox.obj: tv.cfg src\TMenuBox.cpp 
	$(CC) -c src\TMenuBox.cpp

TMenuBar.obj: tv.cfg src\TMenuBar.cpp 
	$(CC) -c src\TMenuBar.cpp

TMemo.obj: tv.cfg src\TMemo.cpp 
	$(CC) -c src\TMemo.cpp

TListViewer.obj: tv.cfg src\TListViewer.cpp 
	$(CC) -c src\TListViewer.cpp

TListBox.obj: tv.cfg src\TListBox.cpp 
	$(CC) -c src\TListBox.cpp

TLabel.obj: tv.cfg src\TLabel.cpp 
	$(CC) -c src\TLabel.cpp

TInputLine.obj: tv.cfg src\TInputLine.cpp 
	$(CC) -c src\TInputLine.cpp

TIndicator.obj: tv.cfg src\TIndicator.cpp 
	$(CC) -c src\TIndicator.cpp

THistoryWindow.obj: tv.cfg src\THistoryWindow.cpp 
	$(CC) -c src\THistoryWindow.cpp

THistoryViewer.obj: tv.cfg src\THistoryViewer.cpp 
	$(CC) -c src\THistoryViewer.cpp

THistory.obj: tv.cfg src\THistory.cpp 
	$(CC) -c src\THistory.cpp

TGroup.obj: tv.cfg src\TGroup.cpp 
	$(CC) -c src\TGroup.cpp

TFrame.obj: tv.cfg src\TFrame.cpp 
	$(CC) -c src\TFrame.cpp

TFileList.obj: tv.cfg src\TFileList.cpp 
	$(CC) -c src\TFileList.cpp

TFileEditor.obj: tv.cfg src\TFileEditor.cpp 
	$(CC) -c src\TFileEditor.cpp

TFileDialog.obj: tv.cfg src\TFileDialog.cpp 
	$(CC) -c src\TFileDialog.cpp

TFileCollection.obj: tv.cfg src\TFileCollection.cpp 
	$(CC) -c src\TFileCollection.cpp

textview.obj: tv.cfg src\textview.cpp 
	$(CC) -c src\textview.cpp

TEditWindow.obj: tv.cfg src\TEditWindow.cpp 
	$(CC) -c src\TEditWindow.cpp

TEditor.obj: tv.cfg src\TEditor.cpp 
	$(CC) -c src\TEditor.cpp

TDirListBox.obj: tv.cfg src\TDirListBox.cpp 
	$(CC) -c src\TDirListBox.cpp

TDirCollection.obj: tv.cfg src\TDirCollection.cpp 
	$(CC) -c src\TDirCollection.cpp

TDialog.obj: tv.cfg src\TDialog.cpp 
	$(CC) -c src\TDialog.cpp

TDeskTop.obj: tv.cfg src\TDeskTop.cpp 
	$(CC) -c src\TDeskTop.cpp

TCommandSet.obj: tv.cfg src\TCommandSet.cpp 
	$(CC) -c src\TCommandSet.cpp

TCollection.obj: tv.cfg src\TCollection.cpp 
	$(CC) -c src\TCollection.cpp

TCluster.obj: tv.cfg src\TCluster.cpp 
	$(CC) -c src\TCluster.cpp

TCheckBoxes.obj: tv.cfg src\TCheckBoxes.cpp 
	$(CC) -c src\TCheckBoxes.cpp

TChDirDialog.obj: tv.cfg src\TChDirDialog.cpp 
	$(CC) -c src\TChDirDialog.cpp

TButton.obj: tv.cfg src\TButton.cpp 
	$(CC) -c src\TButton.cpp

TBackground.obj: tv.cfg src\TBackground.cpp 
	$(CC) -c src\TBackground.cpp

TApplication.obj: tv.cfg src\TApplication.cpp 
	$(CC) -c src\TApplication.cpp

system.obj: tv.cfg src\system.cpp 
	$(CC) -c src\system.cpp

stddlg.obj: tv.cfg src\stddlg.cpp 
	$(CC) -c src\stddlg.cpp

sall.obj: tv.cfg src\sall.cpp 
	$(CC) -c src\sall.cpp

palette.obj: tv.cfg src\palette.cpp 
	$(CC) -c src\palette.cpp

new.obj: tv.cfg src\new.cpp 
	$(CC) -c src\new.cpp

msgbox.obj: tv.cfg src\msgbox.cpp 
	$(CC) -c src\msgbox.cpp

misc.obj: tv.cfg src\misc.cpp 
	$(CC) -c src\misc.cpp

menu.obj: tv.cfg src\menu.cpp 
	$(CC) -c src\menu.cpp

histlist.obj: tv.cfg src\histlist.cpp 
	$(CC) -c src\histlist.cpp

helpbase.obj: tv.cfg src\helpbase.cpp 
	$(CC) -c src\helpbase.cpp

help.obj: tv.cfg src\help.cpp 
	$(CC) -c src\help.cpp

editstat.obj: tv.cfg src\editstat.cpp 
	$(CC) -c src\editstat.cpp

drivers.obj: tv.cfg src\drivers.cpp 
	$(CC) -c src\drivers.cpp

colorsel.obj: tv.cfg src\colorsel.cpp 
	$(CC) -c src\colorsel.cpp

asm.obj: tv.cfg src\asm.cpp 
	$(CC) -c src\asm.cpp

allnames.obj: tv.cfg src\allnames.cpp 
	$(CC) -c src\allnames.cpp

#=============================================================
#		Compiler Configuration File
#=============================================================
tv.cfg: tv.mak
  copy &&|
-R
-DPATH_MAX=260
-L$(LIBPATH)
-I$(INCLUDEPATH)
-vi-
-sm
-y
#-v
-C
-K
-w-hid
| tv.cfg


