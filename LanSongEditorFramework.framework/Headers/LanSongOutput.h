#import "LanSongContext.h"
#import "LanSongFramebuffer.h"

#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#import <UIKit/UIKit.h>
#else
// For now, just redefine this on the Mac
typedef NS_ENUM(NSInteger, UIImageOrientation) {
    UIImageOrientationUp,            // default orientation
    UIImageOrientationDown,          // 180 deg rotation
    UIImageOrientationLeft,          // 90 deg CCW
    UIImageOrientationRight,         // 90 deg CW
    UIImageOrientationUpMirrored,    // as above but image mirrored along other axis. horizontal flip
    UIImageOrientationDownMirrored,  // horizontal flip
    UIImageOrientationLeftMirrored,  // vertical flip
    UIImageOrientationRightMirrored, // vertical flip
};
#endif

dispatch_queue_attr_t LanSongDefaultQueueAttribute(void);

//原来的是:runOnMainQueueWithoutDeadlocking
void runOnMainQueueWithoutDeadlock(void (^block)(void));

//原来的是:runSynchronouslyOnVideoProcessingQueue
void runSynchronouslyOnVideoProcessQueue(void (^block)(void));

//原来的是:runAsynchronouslyOnVideoProcessingQueue
void runAsynchronouslyOnVideoProcessQueue(void (^block)(void));



//原来的是:runSynchronouslyOnContextQueue
void runSyncOnContextQueue(LanSongContext *context, void (^block)(void));

//原来的是:runAsynchronouslyOnContextQueue
void runAsyncOnContextQueue(LanSongContext *context, void (^block)(void));


void reportAvailableMemoryForLanSong(NSString *tag);

@class LanSongMovieWriter;

/** LanSong's base source object
 基础代码类, 用LanSongOutput来表示???
 Images or frames of video are uploaded from source objects, which are subclasses of LanSongOutput. These include:
 
 - LanSongVideoCamera (for live video from an iOS camera) 
 - LanSongStillCamera (for taking photos with the camera)
 - LanSongPicture (for still images)
 - LanSongMovie (for movies)
 
 Source objects upload still image frames to OpenGL ES as textures, then hand those textures off to the next objects in the processing chain.
 */
@interface LanSongOutput : NSObject
{
    //作为输出的的纹理传递到下一个TargetXXXXX
    LanSongFramebuffer *outputFramebuffer;
    /**
     *       目标列表
     */
    NSMutableArray *targets;
    /**
     * targetTextureIndices: 在addTarget的时候, Target会分配一个id号给这个输入, 这里保存起来, 当要传递纹理的时候, 把id号一并传递下去.

     */
    NSMutableArray *targetTextureIndices;
    
    CGSize inputTextureSize, cachedMaximumOutputSize, forcedMaximumSize;
    
    BOOL overrideInputSize;
    
    BOOL allTargetsWantMonochromeData;
    BOOL usingNextFrameForImageCapture;
}

@property(readwrite, nonatomic) BOOL shouldSmoothlyScaleOutput;
@property(readwrite, nonatomic) BOOL shouldIgnoreUpdatesToThisTarget;
@property(readwrite, nonatomic, retain) LanSongMovieWriter *audioEncodingTarget;
@property(readwrite, nonatomic, unsafe_unretained) id<LanSongInput> targetToIgnoreForUpdates;
@property(nonatomic, copy) void(^frameProcessingCompletionBlock)(LanSongOutput*, CMTime);
@property(nonatomic) BOOL enabled;
@property(readwrite, nonatomic) GPUTextureOptions outputTextureOptions;

/// @name Managing targets
- (void)setInputFramebufferForTarget:(id<LanSongInput>)target atIndex:(NSInteger)inputTextureIndex;
- (LanSongFramebuffer *)framebufferForOutput;
- (void)removeOutputFramebuffer;
- (void)notifyTargetsAboutNewOutputTexture;

/** Returns an array of the current targets.
 */
- (NSArray*)targets;

/** Adds a target to receive notifications when new frames are available.
 
 The target will be asked for its next available texture.
 
 See [LanSongInput newFrameReadyAtTime:]
 
 @param newTarget Target to be added
 */
- (void)addTarget:(id<LanSongInput>)newTarget;

/** Adds a target to receive notifications when new frames are available.
 
 See [LanSongInput newFrameReadyAtTime:]
 
 @param newTarget Target to be added
 */
- (void)addTarget:(id<LanSongInput>)newTarget atTextureLocation:(NSInteger)textureLocation;

/** Removes a target. The target will no longer receive notifications when new frames are available.
 
 @param targetToRemove Target to be removed
 */
- (void)removeTarget:(id<LanSongInput>)targetToRemove;

/** Removes all targets.
 */
- (void)removeAllTargets;

/// @name Manage the output texture

- (void)forceProcessingAtSize:(CGSize)frameSize;
- (void)forceProcessingAtSizeRespectingAspectRatio:(CGSize)frameSize;

/// @name Still image processing

- (void)useNextFrameForImageCapture;
- (CGImageRef)newCGImageFromCurrentlyProcessedOutput;
- (CGImageRef)newCGImageByFilteringCGImage:(CGImageRef)imageToFilter;

// Platform-specific image output methods
// If you're trying to use these methods, remember that you need to set -useNextFrameForImageCapture before running -processImage or running video and calling any of these methods, or you will get a nil image
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
- (UIImage *)imageFromCurrentFramebuffer;
- (UIImage *)imageFromCurrentFramebufferWithOrientation:(UIImageOrientation)imageOrientation;
- (UIImage *)imageByFilteringImage:(UIImage *)imageToFilter;
- (CGImageRef)newCGImageByFilteringImage:(UIImage *)imageToFilter;
#else
- (NSImage *)imageFromCurrentFramebuffer;
- (NSImage *)imageFromCurrentFramebufferWithOrientation:(UIImageOrientation)imageOrientation;
- (NSImage *)imageByFilteringImage:(NSImage *)imageToFilter;
- (CGImageRef)newCGImageByFilteringImage:(NSImage *)imageToFilter;
#endif

- (BOOL)providesMonochromeOutput;

@end
