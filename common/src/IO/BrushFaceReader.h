/*
 Copyright (C) 2010-2017 Kristian Duske
 
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

#ifndef TrenchBroom_BrushFaceReader
#define TrenchBroom_BrushFaceReader

#include "IO/MapReader.h"
#include "Model/ModelTypes.h"

namespace TrenchBroom {
    namespace Model {
        class ModelFactory;
    }
    
    namespace IO {
        class ParserStatus;
        
        class BrushFaceReader : public MapReader {
        private:
            Model::ModelFactory& m_factory;
            Model::BrushFaceList m_brushFaces;
        public:
            BrushFaceReader(const String& str, Model::ModelFactory& factory);
            
            const Model::BrushFaceList& read(const vm::bbox3& worldBounds, ParserStatus& status);
        private: // implement MapReader interface
            Model::ModelFactory& initialize(Model::MapFormat format, const vm::bbox3& worldBounds) override;
            Model::Node* onWorldspawn(const Model::EntityAttribute::List& attributes, const ExtraAttributes& extraAttributes, ParserStatus& status) override;
            void onWorldspawnFilePosition(size_t lineNumber, size_t lineCount, ParserStatus& status) override;
            void onLayer(Model::Layer* layer, ParserStatus& status) override;
            void onNode(Model::Node* parent, Model::Node* node, ParserStatus& status) override;
            void onUnresolvedNode(const ParentInfo& parentInfo, Model::Node* node, ParserStatus& status) override;
            void onBrush(Model::Node* parent, Model::Brush* brush, ParserStatus& status) override;
            void onBrushFace(Model::BrushFace* face, ParserStatus& status) override;
        };
    }
}

#endif /* defined(TrenchBroom_BrushFaceReader) */
