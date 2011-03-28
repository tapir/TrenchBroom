//
//  TextureManager.m
//  TrenchBroom
//
//  Created by Kristian Duske on 21.01.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "TextureManager.h"
#import <OpenGL/gl.h>
#import "Texture.h"
#import "Wad.h"
#import "WadPaletteEntry.h"
#import "WadTextureEntry.h"

NSString* const TexturesAdded = @"TexturesAdded";
NSString* const TexturesRemoved = @"TexturesRemoved";

NSString* const UserInfoTextures = @"Textures";

NSString* const UnknownTextureNameException = @"UnknownTextureNameException";
NSString* const MissingPaletteException = @"MissingPaletteException";

@implementation TextureManager

- (id)init {
    if (self = [super init]) {
        textures = [[NSMutableDictionary alloc] init];
    }
    
    return self;
}

- (id)initWithPalette:(NSData *)thePalette {
    if (self = [self init]) {
        palette = [thePalette retain];
    }
    
    return self;
}

- (NSData *)convertTexture:(WadTextureEntry *)textureEntry {
    uint8_t pixelBuffer[32];
    uint8_t colorBuffer[3];
    
    int width = [textureEntry width];
    int height = [textureEntry height];
    
    NSData* textureData = [textureEntry mip0];
    
    int size = width * height;
    NSMutableData* texture = [[NSMutableData alloc] initWithCapacity:size * 3];
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixelIndex = y * width + x;
            int pixelBufferIndex = pixelIndex % 32;
            if (pixelBufferIndex == 0) {
                int length = fmin(32, size - (pixelBufferIndex * 32));
                [textureData getBytes:pixelBuffer range:NSMakeRange(pixelIndex, length)];
            }
            
            int paletteIndex = pixelBuffer[pixelBufferIndex];
            [palette getBytes:colorBuffer range:NSMakeRange(paletteIndex * 3, 3)];
            [texture appendBytes:colorBuffer length:3];
        }
    }
    
    return [texture autorelease];
}

- (void)loadTexturesFrom:(Wad *)wad {
    glEnable(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    NSArray* textureEntries = [wad textureEntries];
    NSMutableArray* addedTextures = [[NSMutableArray alloc] init];
    
    GLsizei numTextures = [textureEntries count];
    GLuint textureIds[numTextures];
    glGenTextures(numTextures, textureIds);
    
    
    NSLog(@"loading %i textures from %@", [textureEntries count], [wad name]);
    for (int i = 0; i < numTextures; i++) {
        WadTextureEntry* textureEntry = [textureEntries objectAtIndex:i];
        int texId = textureIds[i];

        NSData* textureData = [self convertTexture:textureEntry];
        
        int width = [textureEntry width];
        int height = [textureEntry height];
        const void* buffer = [textureData bytes];
        
        glBindTexture(GL_TEXTURE_2D, texId);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
        
        Texture* texture = [[Texture alloc] initWithName:[textureEntry name] width:(int)width height:(int)height textureId:texId];
        [textures setObject:texture forKey:[textureEntry name]];
        [addedTextures addObject:texture];
        [texture release];
    }
    
    NSNotificationCenter* center = [NSNotificationCenter defaultCenter];
    [center postNotificationName:TexturesAdded object:self userInfo:[NSDictionary dictionaryWithObject:addedTextures forKey:UserInfoTextures]];
    [addedTextures release];
}

- (void)deleteTextures {
    GLuint textureIds[[textures count]];
    
    NSEnumerator* texEn = [textures objectEnumerator];
    Texture* texture;
    
    int i = 0;
    while ((texture = [texEn nextObject]))
        textureIds[i++] = [texture textureId];
    
    glDeleteTextures([textures count], textureIds);
}

- (void)removeAllTextures {
    NSArray* removedTextures = [NSArray arrayWithArray:[textures allValues]];
    [self deleteTextures];
    
    [textures removeAllObjects];

    NSNotificationCenter* center = [NSNotificationCenter defaultCenter];
    [center postNotificationName:TexturesRemoved object:self userInfo:[NSDictionary dictionaryWithObject:removedTextures forKey:UserInfoTextures]];
}

- (Texture *)textureForName:(NSString *)name {
    return [textures objectForKey:name];
}

- (NSArray *)texturesForNames:(NSArray *)names {
    NSMutableArray* result = [[NSMutableArray alloc] init];
    
    NSEnumerator* nameEn = [names objectEnumerator];
    NSString* name;
    while ((name = [nameEn nextObject]))
        [result addObject:[textures objectForKey:name]];
    
    return [result autorelease];
}

- (NSArray *)textures:(ESortCriterion)sortCriterion {
    NSMutableArray* result = [NSMutableArray arrayWithArray:[textures allValues]];
    if (sortCriterion == SC_USAGE)
        [result sortUsingSelector:@selector(compareByUsageCount:)];
    else
        [result sortUsingSelector:@selector(compareByName:)];
    
    return result;
}

- (void)activateTexture:(NSString *)name {
    Texture* texture = [textures objectForKey:name];
    if (texture == nil)
        [NSException raise:UnknownTextureNameException format:@"unknown texture name: %@", name];

    [texture activate];
}

- (void)deactivateTexture {
    glBindTexture(GL_TEXTURE_2D, 0);
}

- (void)incUsageCount:(NSString *)name {
    Texture* texture = [textures objectForKey:name];
    if (texture != nil)
        [texture incUsageCount];
}

- (void)decUsageCount:(NSString *)name {
    Texture* texture = [textures objectForKey:name];
    if (texture != nil)
        [texture decUsageCount];
}

- (void)dealloc {
    [self deleteTextures];
    [palette release];
    [textures release];
    [super dealloc];
}

@end
