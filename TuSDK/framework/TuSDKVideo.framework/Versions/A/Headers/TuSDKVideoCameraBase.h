//
//  TuSDKVideoOutputWriter.h
//  TuSDK
//
//  Created by Yanlin on 2/3/16.
//  Copyright © 2016 tusdk.com. All rights reserved.
//

#import <GPUImage/GPUImage.h>
#import "TuSDKVideoImport.h"
#import "TuSDKVideoResult.h"

/**
 *  贴纸可以同时使用的的最大数量
 */
#define LSQ_SMART_STICKER_MAX_NUM 5

/**
 *  输出帧格式类型
 */
typedef NS_ENUM(NSInteger, lsqFrameFormatType)
{
    /**
     *  输出 BGRA 格式 (kCVPixelFormatType_32BGRA)
     */
    lsqFormatTypeBGRA,
    /**
     *  输出 YUV 格式 (kCVPixelFormatType_420YpCbCr8BiPlanarFullRange，即 NV21， Two Plane)
     */
    lsqFormatTypeYUV420F,
    /**
     *  输出基于 BGRA 格式的原始数据
     */
    lsqFormatTypeRawData,
};

#pragma mark - TuSDKVideoCameraBase

/**
 *  视频相机基类
 */
@interface TuSDKVideoCameraBase : GPUImageVideoCamera<TuSDKVideoCameraInterface>
{
    @protected
    // 输出尺寸
    CGSize _outputSize;
    // 视频视图
    TuSDKICFilterVideoViewWrap *_cameraView;
    // 相机聚焦触摸视图
    UIView<TuSDKVideoCameraExtendViewInterface> *_focusTouchView;
    // 相机辅助视图
    TuSDKICGuideRegionView *_guideView;
}

/**
 *  相机帧采样缓冲委托
 */
@property (nonatomic, weak) id<TuSDKVideoCameraSampleBufferDelegate> sampleBufferDelegate;

/**
 *  相机状态
 */
@property (nonatomic, readonly) lsqCameraState state;

/**
 *  采集尺寸
 */
@property (nonatomic, readonly) CGSize captureSize;

/**
 *  选区范围算法
 */
@property (nonatomic, retain) id<TuSDKCPRegionHandler> regionHandler;

/**
 *  是否正在切换滤镜
 */
@property (nonatomic, readonly) BOOL isFilterChanging;

/**
 *  开启滤镜配置选项
 */
@property (nonatomic) BOOL enableFilterConfig;

/**
 *  禁止聚焦功能 (默认: NO)
 */
@property (nonatomic) BOOL disableTapFocus;

/**
 *  是否开启长按拍摄 (默认: NO)
 *  禁用对焦功能
 */
@property (nonatomic) BOOL enableLongTouchCapture;

/**
 *  禁用持续自动对焦 (默认: NO)
 */
@property (nonatomic) BOOL disableContinueFoucs;

/**
 *  自动聚焦延时 (默认: 5秒)
 */
@property (nonatomic) NSTimeInterval autoFoucsDelay;

/**
 *  长按延时 (默认: 1.2秒)
 */
@property (nonatomic) NSTimeInterval longTouchDelay;

/**
 *  视频视图显示比例 (默认：0， 0 <= mRegionRatio, 当设置为0时全屏显示)
 *
 *  1:1 正方形 | 2:3 | 3:4 | 9:16
 */
@property (nonatomic) CGFloat cameraViewRatio;

/**
 *  视频覆盖区域颜色 (默认：[UIColor blackColor])
 */
@property (nonatomic, retain) UIColor *regionViewColor;

/**
 *  默认是否显示辅助线 (默认: false)
 */
@property (nonatomic) BOOL displayGuideLine;

/**
 *  输出画面分辨率，默认原始采样尺寸输出。
 *  如果设置了输出尺寸，则对画面进行等比例缩放，必要时进行裁剪，保证输出尺寸和预设尺寸一致。
 */
@property (nonatomic) CGSize outputSize;

/**
 *  禁用前置摄像头水平镜像 (默认: NO，前置摄像头输出画面进行水平镜像)
 */
@property (nonatomic) BOOL disableMirrorFrontFacing;

/**
 *  是否开启脸部追踪 (默认: YES)
 */
@property (nonatomic) BOOL enableFaceDetection;

/**
 *  是否开启智能美颜和智能贴纸 (默认: 直播相机 NO)
 */
@property (nonatomic) BOOL enableFaceAutoBeauty;

/**
 *  初始化
 *
 *  @param sessionPreset  相机分辨率类型
 *  @param cameraPosition 相机设备标识 （前置或后置）
 *  @param cameraView     相机显示容器视图
 *
 *  @return 相机对象
 */
- (instancetype)initWithSessionPreset:(NSString *)sessionPreset cameraPosition:(AVCaptureDevicePosition)cameraPosition cameraView:(UIView *)view;

/**
 *  初始化相机
 */
-(void)initCamera;

/**
 *  更新相机视图布局
 */
- (void)updateCameraLayout;

/**
 *  获取聚焦视图
 *
 *  @return
 */
- (UIView<TuSDKVideoCameraExtendViewInterface> *)getFocusTouchView;

/**
 *  设置聚焦模式
 *
 *  @param focusMode 聚焦模式
 *
 *  @return 是否支持对焦
 */
- (BOOL)focusWithMode:(AVCaptureFocusMode)focusMode;

/**
 *  设置聚焦模式
 *
 *  @param focusMode 聚焦模式
 *  @param point     聚焦坐标
 *
 *  @return 是否支持对焦
 */
- (BOOL)focusWithMode:(AVCaptureFocusMode)focusMode point:(CGPoint)point;

/**
 *  设置曝光模式
 *
 *  @param exposureMode 曝光模式
 *
 *  @return 是否支持曝光模式
 */
- (BOOL)exposureWithMode:(AVCaptureExposureMode)exposureMode;

/**
 *  设置曝光模式
 *
 *  @param focusMode 曝光模式
 *  @param point     曝光坐标
 *
 *  @return 是否支持曝光模式
 */
- (BOOL)exposureWithMode:(AVCaptureExposureMode)exposureMode point:(CGPoint)point;

/**
 *  当前聚焦状态
 *
 *  @param isFocusing 是否正在聚焦
 */
- (void)onAdjustingFocus:(BOOL)isFocusing;

/**
 *  相机状态发生改变
 *
 *  @param newState 新的状态
 */
- (void)onCameraStateChanged:(lsqCameraState)newState;

/**
 *  更新滤镜输出配置
 */
- (void)updateOutputFilter;

#pragma mark - smart sticker

/**
 *  显示一组动态贴纸。当显示一组贴纸时，会清除画布上的其它贴纸
 *
 *  @param groupSticker 动态贴纸对象
 */
- (void)showGroupSticker:(TuSDKPFStickerGroup *)groupSticker;

/**
 *  动态贴纸组是否已在使用
 *
 *  @param groupSticker 动态贴纸组对象
 *  @return 　是否使用
 */
- (BOOL)isGroupStickerUsed:(TuSDKPFStickerGroup *)groupSticker;

/**
 *  显示动态贴纸
 *
 *  @param liveSticker 动态贴纸对象
 */
- (void)addLiveSticker:(TuSDKPFSticker *)liveSticker;

/**
 *  动态贴纸是否已在使用
 *
 *  @param liveSticker 动态贴纸组对象
 *  @return 　是否使用
 */
- (BOOL)isLiveStickerUsed:(TuSDKPFSticker *)liveSticker;

/**
 *  从相机移除动态贴纸
 *
 *  @param liveSticker 动态贴纸对象
 */
- (void)removeLiveSticker:(TuSDKPFSticker *)liveSticker;

/**
 *  清除动态贴纸
 */
- (void)removeAllLiveSticker;

#pragma mark - destory

/**
 *  销毁
 */
- (void)destory;

@end
