// Adobe Illustrator プラグイン用の設定ダイアログ。
// Dear ImGui のサンプルコード (OSX + Metal) をベースにしています。
// "#" を付したコメントは私のもので、それ以外の英語のはサンプルコードのものです。

// # Dear ImGui : Copyright (c) 2014-2020 Omar Cornut
// # Licensed under the MIT License
// # https://github.com/ocornut/imgui
#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>

#include "../imgui/imgui.h"
#include "../imgui/examples/imgui_impl_metal.h"
#include "../imgui/examples/imgui_impl_osx.h"
#include <stdio.h>
#import "MyWindowController.h"
#include "../Source/myDialogConfig.h"

@interface MyImGuiView : NSViewController
{
    ImGuiWindowFlags _flag;
}
@property (nonatomic, assign) MyDialogParms* parms;
@property (nonatomic, assign) int result;
@property (nonatomic, assign) std::function<void(void)> callbackFunc;
-(void)setFlags;
@end

@interface MyImGuiView () <MTKViewDelegate>
@property (nonatomic, readonly) MTKView *mtkView;
@property (nonatomic, strong) id <MTLDevice> device;
@property (nonatomic, strong) id <MTLCommandQueue> commandQueue;
@end

@implementation MyImGuiView

-(void)setFlags
{
    // # 通常のダイアログっぽく見せるためのフラグ。
    // # 基本的に ImGui はウィンドウの中にサブウィンドウ的にダイアログを描画する。
    // # 以下のフラグはこのサブウィンドウに適用される。
    // # ref(Japanese): https://qiita.com/mizuma/items/73218dab2f6b022b0227
    _flag = 0;
    _flag |= ImGuiWindowFlags_NoTitleBar; // タイトルバーを非表示にします。
    _flag |= ImGuiWindowFlags_NoResize; // ウィンドウをリサイズ不可にします。
    _flag |= ImGuiWindowFlags_NoMove; // ウィンドウを移動不可にします。
    _flag |= ImGuiWindowFlags_NoScrollbar; // スクロールバーを無効にします。
    _flag |= ImGuiWindowFlags_NoScrollWithMouse; // マウスホイールでのスクロール操作を無効にします。
    _flag |= ImGuiWindowFlags_NoCollapse; // タイトルバーをダブルクリックして閉じる挙動を無効にします。
    _flag |= ImGuiWindowFlags_NoBackground; // ウィンドウ内の背景を非表示にします。
    _flag |= ImGuiWindowFlags_NoBringToFrontOnFocus; // ウィンドウをクリックしてフォーカスした際に他のウィンドウよりも前面に表示する挙動を無効にします。
    _flag |= ImGuiWindowFlags_NoNav; // ゲームパッドやキーボードでのUIの操作を無効にします。
    _flag |= ImGuiWindowFlags_NoSavedSettings; // imgui.iniでウィンドウの位置などを自動保存/ロードさせないようにします。
    _flag |= ImGuiWindowFlags_AlwaysAutoResize; // 自動でウィンドウ内のコンテンツに合わせてリサイズします。
    _flag |= ImGuiWindowFlags_NoFocusOnAppearing; // 表示/非表示の際のトランジションアニメーションを無効にします。
}

// --------
-(instancetype)initWithNibName:(nullable NSString *)nibNameOrNil bundle:(nullable NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];

    _device = MTLCreateSystemDefaultDevice();
    _commandQueue = [_device newCommandQueue];

    if (!self.device)
    {
        NSLog(@"Metal is not supported");
        abort();
    }

    // Setup Dear ImGui context
    // FIXME: This example doesn't have proper cleanup...
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    io.Fonts->AddFontDefault();
#ifdef DIALOG_LANG_JP
    ImFontConfig config;
    config.MergeMode = true;
    ImFont* font = io.Fonts->AddFontFromFileTTF(FONT_NAME_JP, FONT_SIZE_JP, &config, io.Fonts->GetGlyphRangesJapanese());
    IM_ASSERT(font != NULL);
#endif

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    ImGuiStyle* style = &ImGui::GetStyle();
    // background color of input fields
    style->Colors[ImGuiCol_FrameBg] = ImVec4(0.5f, 0.5f, 0.5f, 0.5f);

    // Setup Renderer backend
    ImGui_ImplMetal_Init(_device);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.txt' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    return self;
}

-(MTKView *)mtkView
{
    return (MTKView *)self.view;
}

-(void)loadView
{
    self.view = [[MTKView alloc] initWithFrame:CGRectMake(0, 0, kMyDialogWidth, kMyDialogHeight)];
}

-(void)viewDidLoad
{
    [super viewDidLoad];

    self.mtkView.device = self.device;
    self.mtkView.delegate = self;

    // Add a tracking area in order to receive mouse events whenever the mouse is within the bounds of our view
    NSTrackingArea *trackingArea = [[NSTrackingArea alloc] initWithRect:NSZeroRect
                                                           options:NSTrackingMouseMoved | NSTrackingInVisibleRect | NSTrackingActiveAlways
                                                           owner:self
                                                           userInfo:nil];
    [self.view addTrackingArea:trackingArea];

    ImGui_ImplOSX_Init(self.view);
}

-(void)drawInMTKView:(MTKView*)view
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = view.bounds.size.width;
    io.DisplaySize.y = view.bounds.size.height;

    CGFloat framebufferScale = view.window.screen.backingScaleFactor ?: NSScreen.mainScreen.backingScaleFactor;
    io.DisplayFramebufferScale = ImVec2(framebufferScale, framebufferScale);

    io.DeltaTime = 1 / float(view.preferredFramesPerSecond ?: 60);

    id<MTLCommandBuffer> commandBuffer = [self.commandQueue commandBuffer];

    MTLRenderPassDescriptor* renderPassDescriptor = view.currentRenderPassDescriptor;
    if (renderPassDescriptor == nil)
    {
        [commandBuffer commit];
        return;
    }

    // Start the Dear ImGui frame
    ImGui_ImplMetal_NewFrame(renderPassDescriptor);
    ImGui_ImplOSX_NewFrame(view);
    ImGui::NewFrame();
    
    static ImVec4 clear_color = ImVec4(0.25f,0.25f,0.25f,1.0f);
    
    // # ダイアログの部品。流用する際の可変要素。コードはwindowsと共通。
    {
        ImGui::SetNextWindowPos(ImVec2(0,0), 0, ImVec2(0,0));
        
        static bool is_open = true;
        ImGui::Begin("settings", &is_open, _flag);
        
        //ImGui::Text("* pixel distance settings");
        ImGui::Text("%s", LABEL_TITLE);
        ImGui::Spacing();
        
        ImGui::PushItemWidth(100);
        
        char buf_drag[64];
        //sprintf(buf_drag, "drag/track (%d-%d)", (int)MIN_HIT_TOLE, (int)MAX_HIT_TOLE);
        sprintf(buf_drag, LABEL_TOLERANCE, (int)MIN_HIT_TOLE,
            (int)MAX_HIT_TOLE);
        if(ImGui::InputInt(buf_drag, &(_parms->tolerance_drag))){
            _parms->tolerance_drag = fmax(MIN_HIT_TOLE, fmin(MAX_HIT_TOLE, _parms->tolerance_drag));
            //if(show_preview) callbackFunc();
        }
        char buf_safe[64];
        //sprintf(buf_safe, "safe click (%d-%d)", (int)MIN_SAFE_CLICK, (int)MAX_SAFE_CLICK);
        sprintf(buf_safe, LABEL_SAFECLICK, (int)MIN_SAFE_CLICK, (int)MAX_SAFE_CLICK);
        if(ImGui::InputInt(buf_safe, &(_parms->tolerance_safe_click))){
            _parms->tolerance_safe_click = fmax(MIN_SAFE_CLICK, fmin(MAX_SAFE_CLICK, _parms->tolerance_safe_click));
            //if(show_preview) callbackFunc();
        }
        
        ImGui::Spacing();
        //if (ImGui::Button("Cancel"))
        if (ImGui::Button(LABEL_CANCEL))
        {
            _result = 1;
        }
        ImGui::SameLine();
        if (ImGui::Button(LABEL_OK))
        {
            _result = 2;
        }

        ImGui::End();
    }
    // # 可変要素ここまで
    
    // Rendering
    ImGui::Render();
    ImDrawData* draw_data = ImGui::GetDrawData();

    renderPassDescriptor.colorAttachments[0].clearColor = MTLClearColorMake(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    id <MTLRenderCommandEncoder> renderEncoder = [commandBuffer renderCommandEncoderWithDescriptor:renderPassDescriptor];
    [renderEncoder pushDebugGroup:@"Dear ImGui rendering"];
    ImGui_ImplMetal_RenderDrawData(draw_data, commandBuffer, renderEncoder);
    [renderEncoder popDebugGroup];
    [renderEncoder endEncoding];

    // Present
    [commandBuffer presentDrawable:view.currentDrawable];
    [commandBuffer commit];
}

- (void)mtkView:(nonnull MTKView *)view drawableSizeWillChange:(CGSize)size {
}

- (BOOL)commitEditingAndReturnError:(NSError *__autoreleasing  _Nullable * _Nullable)error {
    return (YES);
}

- (void)encodeWithCoder:(nonnull NSCoder *)coder {
}

@end

//-----------------------------------------------------------------------------------
 @implementation MyWindowController

- (instancetype)initWithWindow:(NSWindow*)window
{
#ifdef IS_STANDALONE
    // # プラグインのダイアログの場合は以下は不要
    // Make the application a foreground application (else it won't receive keyboard events)
    ProcessSerialNumber psn = {0, kCurrentProcess};
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
#endif

    [window setTitle:[NSString stringWithUTF8String:kMyDialogTitle]];
    [window setAcceptsMouseMovedEvents:YES];
    [window setOpaque:YES];

    MyImGuiView* imguiview = [[MyImGuiView alloc] initWithNibName:nil bundle:nil];
    imguiview.parms = nil;
    [imguiview setFlags];
    window.contentViewController = imguiview;
    
    return [super initWithWindow:window];
}

- (int) runModal:(MyDialogParms*)parms completion:(std::function<void(void)>)callbackFunc
{
    int result = 0;
    MyImGuiView* vcon = (MyImGuiView*)self.contentViewController;
    vcon.result = 0;
    vcon.parms = parms;
    vcon.callbackFunc = callbackFunc;
    
    NSModalSession session
    = [[NSApplication sharedApplication] beginModalSessionForWindow:self.window];
    while([self.window isVisible]){
        if([NSApp runModalSession: session] != NSModalResponseContinue) break;
        result = vcon.result;
        if(result != 0) break;
    }
    callbackFunc();
    [NSApp endModalSession:session];
    if(result != 0) [self.window performClose:nil];
    return result;
}

- (void) releaseDialog
{
    ImGui_ImplOSX_Shutdown();
    ImGui_ImplMetal_Shutdown();
    ImGui::DestroyContext();
    [self.window close];
}
@end

