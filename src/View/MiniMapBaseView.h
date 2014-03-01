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

#ifndef __TrenchBroom__MiniMapBaseView__
#define __TrenchBroom__MiniMapBaseView__

#include "TrenchBroom.h"
#include "VecMath.h"

#include "Model/ModelTypes.h"
#include "Renderer/Vbo.h"
#include "Renderer/Camera.h"
#include "View/ViewTypes.h"

#include <wx/event.h>
#include <wx/glcanvas.h>

namespace TrenchBroom {
    namespace Renderer {
        class Camera;
        class MiniMapRenderer;
        class RenderContext;
        class RenderResources;
    }
    
    namespace Model {
        class SelectionResult;
    }

    namespace View {
        class MiniMapBaseView : public wxGLCanvas {
        private:
            View::MapDocumentWPtr m_document;
            Renderer::RenderResources& m_renderResources;
            Renderer::Camera& m_camera3D;
            
            wxGLContext* m_glContext;
            Renderer::MiniMapRenderer& m_renderer;
            Renderer::Vbo m_auxVbo;
            
            wxPoint m_lastPos;
        public:
            virtual ~MiniMapBaseView();
            
            void OnMouseButton(wxMouseEvent& event);
            void OnMouseDoubleClick(wxMouseEvent& event);
            void OnMouseMotion(wxMouseEvent& event);
        private:
            void drag3DCamera(const wxPoint& lastPos, const wxPoint& currentPos);
            void panView(const wxPoint& lastPos, const wxPoint& currentPos);
        public:
            void OnMouseWheel(wxMouseEvent& event);
            void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
            
            void OnPaint(wxPaintEvent& event);
            void OnSize(wxSizeEvent& event);
        protected:
            MiniMapBaseView(wxWindow* parent, View::MapDocumentWPtr document, Renderer::RenderResources& renderResources, Renderer::MiniMapRenderer& renderer, Renderer::Camera& camera3D);
            
            View::MapDocumentSPtr document() const;
        private:
            const Renderer::Camera& viewCamera() const;
            void updateViewport(const Renderer::Camera::Viewport& viewport);
            void panView(const Vec3f& delta);
            void zoomView(const Vec3f& factors);
        private:
            void bindObservers();
            void unbindObservers();
            
            void documentWasCleared();
            void documentWasNewedOrLoaded();

            void objectWasAdded(Model::Object* object);
            void objectWillBeRemoved(Model::Object* object);
            void objectDidChange(Model::Object* object);
            void selectionDidChange(const Model::SelectionResult& result);
            
            void cameraDidChange(const Renderer::Camera* camera);

            void bindEvents();
            void setupGL(Renderer::RenderContext& context);
            void clearBackground(Renderer::RenderContext& context);
            void renderMap(Renderer::RenderContext& context);
            
            void fireChangeEvent();
            
            float pick3DCamera(const Ray3f& pickRay) const;
            void render3DCamera(Renderer::RenderContext& context);

            virtual const Renderer::Camera& doGetViewCamera() const = 0;
            virtual void doComputeBounds(BBox3f& bounds) = 0;
            virtual void doUpdateViewport(const Renderer::Camera::Viewport& viewport) = 0;
            virtual void doPanView(const Vec3f& delta) = 0;
            virtual void doZoomView(const Vec3f& factors) = 0;

            virtual void doShowDrag3DCameraCursor() = 0;
            virtual void doDrag3DCamera(const Vec3f& delta, Renderer::Camera& camera) = 0;
            virtual float doPick3DCamera(const Ray3f& pickRay, const Renderer::Camera& camera) const = 0;
            virtual void doRender3DCamera(Renderer::RenderContext& renderContext, Renderer::Vbo& vbo, const Renderer::Camera& camera) = 0;
        };
    }
}

#define WXDLLIMPEXP_CUSTOM_EVENT

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_CUSTOM_EVENT, EVT_MINIMAP_VIEW_CHANGED_EVENT, 1)
END_DECLARE_EVENT_TYPES()

typedef void (wxEvtHandler::*miniMapViewChangedEventFunction)(wxCommandEvent&);

#define EVT_MINIMAP_VIEW_CHANGED_HANDLER(func) \
    (wxObjectEventFunction) \
    (miniMapViewChangedEventFunction) & func

#define EVT_MINIMAP_VIEW_CHANGED(id,func) \
    DECLARE_EVENT_TABLE_ENTRY(EVT_MINIMAP_VIEW_CHANGED_EVENT, \
        id, \
        wxID_ANY, \
        (wxObjectEventFunction) \
        (miniMapViewChangedEventFunction) & func, \
        (wxObject*) NULL ),


#endif /* defined(__TrenchBroom__MiniMapBaseView__) */
