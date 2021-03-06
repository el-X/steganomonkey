/* 
 * File:   SMAboutDialog.cpp
 * Author: Alexander Keller, Robert Heimsoth, Thomas Matern
 *
 * HS BREMEN, SS2014, TI6.2
 */

#include "SMAboutDialog.h"

SMAboutDialog::SMAboutDialog(wxWindow* parent, wxWindowID id,
        const wxString& title, const wxPoint& pos, const wxSize& size)
: wxDialog(parent, id, title, pos, size) {
    this->SetTitle(TEXT_TITLE);
    this->create();
    this->doLayout();
}

/**
 * Erstellt alle Widgets des About-Dialogs.
 */
void SMAboutDialog::create() {
    mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

    appImage = new wxStaticBitmap(mainPanel, wxID_ANY, wxBITMAP_PNG(monkey_about), wxDefaultPosition, wxDefaultSize, 0);
    appImage->SetFocus();
    separator = new wxStaticLine(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
    developerHeadingLabel = new wxStaticText(mainPanel, wxID_ANY, TEXT_DEVELOPER_HEADING, wxDefaultPosition, wxDefaultSize, 0);
    developerHeadingLabel->SetFont(FONT_BOLD);
    developerLabel1 = new wxStaticText(mainPanel, wxID_ANY, TEXT_DEVELOPER1, wxDefaultPosition, wxDefaultSize, 0);
    developerLabel2 = new wxStaticText(mainPanel, wxID_ANY, TEXT_DEVELOPER2, wxDefaultPosition, wxDefaultSize, 0);
    copyleftImage = new wxStaticBitmap(mainPanel, wxID_ANY, wxBITMAP_PNG(copyleft15x15), wxDefaultPosition, wxDefaultSize, 0);
    copyleftImage->SetToolTip(TOOLTIP_COPYLEFT);
    copyleftLabel = new wxStaticText(mainPanel, wxID_ANY, TEXT_COPYLEFT, wxDefaultPosition, wxDefaultSize, 0);
    clipartLabel = new wxStaticText(mainPanel, wxID_ANY, TEXT_CLIPART_BY, wxDefaultPosition, wxDefaultSize, 0);
    clipartLink = new wxHyperlinkCtrl(mainPanel, wxID_ANY, TEXT_CLIPART, LINK_CLIPART);
    clipartLink->SetToolTip(LINK_CLIPART);
    iconLabel = new wxStaticText(mainPanel, wxID_ANY, TEXT_ICON_BY, wxDefaultPosition, wxDefaultSize, 0);
    iconLink = new wxHyperlinkCtrl(mainPanel, wxID_ANY, TEXT_ICON, LINK_ICON);
    iconLink->SetToolTip(LINK_ICON);
}

/**
 * Sorgt für das Layout des About-Dialogs.
 */
void SMAboutDialog::doLayout() {
    wxBoxSizer* mainPanelSizer;
    wxBoxSizer* mainSizer;
    wxBoxSizer* copyleftSizer;
    wxBoxSizer* clipartSizer;
    wxBoxSizer* iconSizer;

    // Oberer Bereich
    mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(appImage, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(separator, 0, wxEXPAND | wxALL, 5);
    mainSizer->Add(developerHeadingLabel, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
    mainSizer->Add(developerLabel1, 0, wxALIGN_CENTER_HORIZONTAL, 5);
    mainSizer->Add(developerLabel2, 0, wxALIGN_CENTER_HORIZONTAL, 5);

    // Copyleft
    copyleftSizer = new wxBoxSizer(wxHORIZONTAL);
    copyleftSizer->Add(copyleftImage, 0, wxALIGN_CENTER_VERTICAL, 5);
    copyleftSizer->Add(copyleftLabel, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxTOP | wxBOTTOM, 5);
    mainSizer->Add(copyleftSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxTOP | wxLEFT | wxRIGHT, 5);

    // Clipart
    clipartSizer = new wxBoxSizer(wxHORIZONTAL);
    clipartSizer->Add(clipartLabel, 0, wxALIGN_CENTER_VERTICAL, 5);
    clipartSizer->Add(clipartLink, 0, wxALIGN_CENTER_VERTICAL, 5);
    mainSizer->Add(clipartSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5);

    // Icon
    iconSizer = new wxBoxSizer(wxHORIZONTAL);
    iconSizer->Add(iconLabel, 0, wxALIGN_CENTER_VERTICAL, 5);
    iconSizer->Add(iconLink, 0, wxALIGN_CENTER_VERTICAL, 5);
    mainSizer->Add(iconSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5);

    mainPanel->SetSizer(mainSizer);
    mainPanel->Layout();
    mainSizer->Fit(mainPanel);

    mainPanelSizer = new wxBoxSizer(wxVERTICAL);
    mainPanelSizer->Add(mainPanel, 1, wxEXPAND | wxALL, 5);
    this->SetSizer(mainPanelSizer);
    this->Layout();
    mainPanelSizer->Fit(this);
    this->Centre(wxBOTH);
}

