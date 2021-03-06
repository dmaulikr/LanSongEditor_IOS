

#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>

#import <UIKit/UIKit.h>

@protocol Mp3ToAacDelegate;

@interface Mp3ToAAC : NSObject


/**
 MP3格式转AAC格式的构造函数.

 @param delegate 完成监听, 进度监听,错误监听的回调
 @param srcPath MP3源文件
 @param dstPath 转换后的AAC/M4A 文件的保存路径, 绝对路径,后缀是m4a的文件
 @return 创建好的对象
 */
- (id)initWithDelegate:(id<Mp3ToAacDelegate>)delegate source:(NSString*)srcPath destination:(NSString*)dstPath;
/**
 开始运行, 内部会开启一个线程来运行.
 */
- (void)start;

/**
  暂停
 */
- (void)pause;

/**
 恢复
 */
- (void)resume;
/**
停止
 */
- (void)stop;
@end

//---------------以下是监听回调.

@protocol Mp3ToAacDelegate <NSObject>

/**
 转换结束的回调

 @param convert
 */
- (void)Mp3ToAacDelegateCompleted:(Mp3ToAAC*)convert;

@optional
/**
 转换错误的回调[可选, 当前未使用]
 
 @param converter 对象
 @param error 错误描述
 */
- (void)Mp3ToAacDelegateError:(Mp3ToAAC*)converter error:(NSError*)error;
/**
 进度监听[可选]
 (一般很快完成, 也不建议使用.)
 @param converter 当前对象
 @param progress 进度值,  从0.0---1.0
 */
- (void)Mp3ToAacDelegateProgress:(Mp3ToAAC*)converter progress:(CGFloat)progress;

/**
 以下是举例代码:
 
 */
@end
