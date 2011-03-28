//
//  ClipTool.h
//  TrenchBroom
//
//  Created by Kristian Duske on 28.03.11.
//  Copyright 2011 TU Berlin. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class Vector3i;
@class Picker;
@class Ray3D;
@class Renderer;
@class Grid;
@class ClipPointFeedbackFigure;
@class ClipLineFeedbackFigure;
@class ClipPlaneFeedbackFigure;

@interface ClipTool : NSObject { 
    Picker* picker;
    Grid* grid;
    Renderer* renderer;
    NSSet* brushes;
    Vector3i* point1;
    Vector3i* point2;
    Vector3i* point3;
    ClipPointFeedbackFigure* point1Figure;
    ClipPointFeedbackFigure* point2Figure;
    ClipPointFeedbackFigure* point3Figure;
    ClipLineFeedbackFigure* line1Figure;
    ClipLineFeedbackFigure* line2Figure;
    ClipLineFeedbackFigure* line3Figure;
    ClipPlaneFeedbackFigure* planeFigure;
    Vector3i* currentPoint;
    Vector3i* draggedPoint;
}

- (id)initWithBrushes:(NSSet *)theBrushes picker:(Picker *)thePicker grid:(Grid *)theGrid renderer:(Renderer *)theRenderer;

- (void)handleLeftMouseDragged:(Ray3D *)ray;
- (void)handleLeftMouseDown:(Ray3D *)ray;
- (void)handleLeftMouseUp:(Ray3D *)ray;
- (void)handleMouseMoved:(Ray3D *)ray;

@end
