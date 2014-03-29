/*
 Copyright (C) 2010-2014 Kristian Duske
 
 This file is part of TrenchBroom.
 
 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __TrenchBroom__FaceAttribsEditor__
#define __TrenchBroom__FaceAttribsEditor__

#include "Model/ModelTypes.h"
#include "View/ViewTypes.h"

#include <wx/panel.h>

class wxBitmap;
class wxButton;
class wxGridBagSizer;
class wxSpinCtrl;
class wxSpinEvent;
class wxStaticText;
class SpinControl;
class SpinControlEvent;

namespace TrenchBroom {
    namespace Model {
        class SelectionResult;
    }
    
    namespace Renderer {
        class RenderResources;
    }
    
    namespace View {
        class ControllerFacade;
        class FlagChangedCommand;
        class FlagsPopupEditor;
        class TexturingView;
        
        class FaceAttribsEditor : public wxPanel {
        private:
            MapDocumentWPtr m_document;
            ControllerWPtr m_controller;
            Model::BrushFaceList m_faces;

            TexturingView* m_texturingView;
            wxSpinCtrl* m_xSubDivisionEditor;
            wxSpinCtrl* m_ySubDivisionEditor;
            
            SpinControl* m_xOffsetEditor;
            SpinControl* m_yOffsetEditor;
            SpinControl* m_xScaleEditor;
            wxButton* m_flipXButton;
            SpinControl* m_yScaleEditor;
            wxButton* m_flipYButton;
            SpinControl* m_rotationEditor;
            wxStaticText* m_surfaceValueLabel;
            SpinControl* m_surfaceValueEditor;
            wxGridBagSizer* m_faceAttribsSizer;
            
            wxStaticText* m_surfaceFlagsLabel;
            FlagsPopupEditor* m_surfaceFlagsEditor;
            wxStaticText* m_contentFlagsLabel;
            FlagsPopupEditor* m_contentFlagsEditor;
        public:
            FaceAttribsEditor(wxWindow* parent, Renderer::RenderResources& resources, MapDocumentWPtr document, ControllerWPtr controller);
            ~FaceAttribsEditor();
            
            void OnSubDivisionChanged(wxSpinEvent& event);
            void OnXOffsetChanged(SpinControlEvent& event);
            void OnYOffsetChanged(SpinControlEvent& event);
            void OnRotationChanged(SpinControlEvent& event);
            void OnXScaleChanged(SpinControlEvent& event);
            void OnFlipXScaleClicked(wxCommandEvent& event);
            void OnYScaleChanged(SpinControlEvent& event);
            void OnFlipYScaleClicked(wxCommandEvent& event);
            void OnSurfaceFlagChanged(FlagChangedCommand& command);
            void OnContentFlagChanged(FlagChangedCommand& command);
            void OnSurfaceValueChanged(SpinControlEvent& event);
            void OnIdle(wxIdleEvent& event);
        private:
            void createGui(Renderer::RenderResources& resources);
            void bindEvents();
            
            void bindObservers();
            void unbindObservers();
            
            void documentWasNewed();
            void documentWasLoaded();
            void faceDidChange(Model::BrushFace* face);
            void selectionDidChange(const Model::SelectionResult& result);
            void textureCollectionsDidChange();
            
            void updateControls();

            bool hasSurfaceAttribs() const;
            void showSurfaceAttribEditors();
            void hideSurfaceAttribEditors();

            void getSurfaceFlags(wxArrayString& names, wxArrayString& descriptions) const;
            void getContentFlags(wxArrayString& names, wxArrayString& descriptions) const;
        };
    }
}

#endif /* defined(__TrenchBroom__FaceAttribsEditor__) */
