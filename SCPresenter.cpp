/* 
 * File:   SCPresenter.cpp
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/intl.h>
#include <wx/string.h>
#include <wx/regex.h>
#include "SCPresenter.h"
#include <iostream>
#include <string>

IMPLEMENT_APP(SCPresenter)

// Mapping für die Events vornehmen
wxBEGIN_EVENT_TABLE(SCPresenter, wxApp)
EVT_MENU(ID_LOAD_UNMOD_IMG, SCPresenter::onLoad)
EVT_BUTTON(ID_LOAD_UNMOD_IMG, SCPresenter::onLoad)
EVT_MENU(ID_LOAD_MOD_IMG, SCPresenter::onLoad)
EVT_BUTTON(ID_LOAD_MOD_IMG, SCPresenter::onLoad)
EVT_MENU(ID_SAVE_MOD_IMG, SCPresenter::onSave)
EVT_BUTTON(ID_SAVE_MOD_IMG, SCPresenter::onSave)
EVT_MENU(ID_ENCODE, SCPresenter::onEncode)
EVT_BUTTON(ID_ENCODE, SCPresenter::onEncode)
EVT_MENU(ID_DECODE, SCPresenter::onDecode)
EVT_BUTTON(ID_DECODE, SCPresenter::onDecode)
EVT_TEXT(ID_SECRET_MSG, SCPresenter::onSecretMessageChange)
EVT_MENU(wxID_EXIT, SCPresenter::onExit)
EVT_MENU(wxID_ABOUT, SCPresenter::onAbout)
wxEND_EVENT_TABLE()

/**
 * Die main-Funktion des Programms.
 */
bool SCPresenter::OnInit() {
    wxInitAllImageHandlers();
    view = new SCView();
    view->SetMinSize(wxSize(800, 600));
    view->create();
    view->doLayout();
    view->Centre();
    view->Show(true);

    model = new SCModel();
    this->init();
    view->getStatusBar()->SetStatusText(_("Welcome to SteganoCoder!"));
    return true;
}

/**
 * Definiert den Startzustand des Programms.
 */
void SCPresenter::init() {
    view->getSaveModImgBtn()->Disable();
    view->getEncodeBtn()->Disable();
    view->getDecodeBtn()->Disable();
    *(view->getTxtLengthOutput()) << 0;
}

/**
 * Öffnet ein FileDialog zum Laden des Bildes.
 * Anhand der event id wird unterschieden welche Aktion 
 * getätigt wurde (welche Komponente wurde getätigt).
 * 
 * @param event Kommando Event mit der ID
 */
void SCPresenter::onLoad(wxCommandEvent& event) {
    wxFileDialog openDialog(view, _T("Load Image"), wxEmptyString, wxEmptyString,
            _T("Image (*.bmp;*.jpg;*.jpeg;*.png;*.gif)|*.bmp;*.jpg;*.jpeg;*.png;*.gif"));
    openDialog.SetDirectory(wxGetHomeDir()); // OS independency
    openDialog.CentreOnParent();

    if (openDialog.ShowModal() == wxID_OK) {
        wxImage image = openDialog.GetPath();
        wxBitmap bitmap(image);
        if (event.GetId() == ID_LOAD_MOD_IMG) {
            // Bild mit versteckter Nachricht wurde geladen
            view->getModStaticBitmap()->SetBitmap(bitmap);
            model->setModCarrierBytes(image.GetData());
            wxString bitPattern = _(model->getModBitPattern());
            view->getBitpatternOutput()->SetValue(bitPattern);
            view->getDecodeBtn()->Enable(true);
        } else {
            // Bild ohne versteckter Nachricht wurde geladen
            view->getUnmodStaticBitmap()->SetBitmap(image);
            model->setUnmodCarrierBytes(image.GetData());
            view->getEncodeBtn()->Enable(true);
            // Zeige maximale Länge der Nachricht
            view->getMaxTxtLengthOutput()->Clear();
            *(view->getMaxTxtLengthOutput()) << getMaxTextLength();
        }
    }
}

/**
 * Öffnet ein FileDialog zur Speicherung des Bildes mit versteckter Nachricht.
 * 
 * @param event created on the gui.
 */
void SCPresenter::onSave(wxCommandEvent& event) {
    wxFileDialog dialog(view, _T("Save Image"), wxEmptyString, _T("top_secret.bmp"),
            _T("Bitmap (*.bmp)|*.bmp"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    dialog.SetFilterIndex(1);
    if (dialog.ShowModal() == wxID_OK) {
        wxBitmap modBitmap = view->getModStaticBitmap()->GetBitmap();
        modBitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_BMP);
        view->GetStatusBar()->SetStatusText(
                "Image with secret massage saved - " + dialog.GetPath(), 1);
    }
}

/**
 * Versteckt die gegebene Nachricht in dem gegebenem Bild.
 * 
 * @param event created on the gui.
 */
void SCPresenter::onEncode(wxCommandEvent& event) {
    wxString message = view->getSecretMsgInput()->GetValue();
    
    if (message.IsEmpty()) {
        wxMessageDialog notationDialog(NULL,
                wxT("You are about to encrypt an empty message.\nAre you sure you want to continue?"),
                wxT("Notation"),
                wxYES_NO | wxICON_EXCLAMATION);
        notationDialog.CentreOnParent();

        switch (notationDialog.ShowModal()) {
            case wxID_YES:
                model->encode(message.ToStdString());
                //                wxImage image();
                //                view->getModStaticBitmap()->SetBitmap(image.SetData(model->getModCarrierBytes()));
                break;
            case wxID_NO:
                break;
        }
    } else if (wxAtoi(message) > getMaxTextLength()) {
        wxMessageDialog notationDialog(NULL,
                wxT("Sorry but your message is too long.\nSelect either a bigger image or type in a shorter message."),
                wxT("Notation"), wxOK | wxICON_EXCLAMATION);
        notationDialog.CentreOnParent();
        notationDialog.ShowModal();
    } else {
        model->encode(message.ToStdString());
    }
}

/**
 * Holt die Nachricht aus dem geladenen Bild.
 * 
 * @param event created on the gui.
 */
void SCPresenter::onDecode(wxCommandEvent& event) {
    //model->decode();
    //setMessage...
}

/**
 * Überprüft das Eingabefeld auf nicht zulässige Zeichen und 
 * ersetzt diese durch gültige Zeichen. Zusätzlich wird die 
 * angezeigte Länge der einegegebener Nachricht aktualisiert.
 * 
 * @param event created on the gui.
 */
void SCPresenter::onSecretMessageChange(wxCommandEvent& event) {
    // Ersetze alle nicht ASCII Zeichen
    wxString rawInput = view->getSecretMsgInput()->GetValue();
    wxString filtered = view->getSecretMsgInput()->GetValue();
    wxRegEx reg;
    if (reg.Compile(wxT("[ÄÖÜäöüß*]"))) {
        reg.Replace(&filtered, wxT("?")); // gemäß der Maske ersetzen
    }
    if (rawInput.compare(filtered) != 0) {
        view->getSecretMsgInput()->SetValue(filtered);
        view->getSecretMsgInput()->SetInsertionPointEnd();
    }
    // Aktualisiere die angezeigte Länge der Nachricht
    view->getTxtLengthOutput()->Clear();
    int size = view->getSecretMsgInput()->GetValue().size();
    *(view->getTxtLengthOutput()) << size;
}

void SCPresenter::onExit(wxCommandEvent& event) {
    view->Close(true);
}

void SCPresenter::onAbout(wxCommandEvent& event) {
    wxMessageBox("SteganoCoder", "About SteganoCoder", wxOK | wxICON_INFORMATION);
}

int SCPresenter::getMaxTextLength() const {
    unsigned int maxTxtLength = 0;
    if (!view->getUnmodStaticBitmap()->GetBitmap().IsNull()) {
        unsigned int height = view->getUnmodStaticBitmap()->GetBitmap().GetHeight();
        unsigned int width = view->getUnmodStaticBitmap()->GetBitmap().GetWidth();
        maxTxtLength = (height * width * 3) / 8 - model->getHeaderSize();
    }
    return maxTxtLength;
}


std::string SCPresenter::getWXMOTIF() {
#ifdef __WXMOTIF__
    return "Bitmap (*.bmp)|*.bmp";
#else
    return "Bitmap (*.bmp;*.dib)|*.bmp;*.dib";
#endif
}