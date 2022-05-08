# anchorGrabber.aip

__anchorGrabber.aip__ は、Adobe Illustrator 2022 (mac) プラグインです。

ドラッグを開始した位置に近いアンカーをまとめて移動することができます。

設定ダイアログに [Dear ImGui](https://github.com/ocornut/imgui) を使用しています。

未解決のバグあり（下記参照）

## 使い方

![image](https://gist.github.com/shspage/5e54612b4b46ee946327a4436ad2f410/raw/b789c544524a2cd49c68a815210f480678fc89d8/anchorgrabber.png)  
動画 - 
https://twitter.com/shspage/status/1502867048992423937


## 設定ウィンドウの項目：

ツールアイコンをダブルクリックすると、設定ダイアログが表示されます。

(__NOTE:__ ダイアログ内のテキストを日本語に変えるには、MyDialogParms.h 冒頭の //#define DIALOG_LANG_JP をアンコメントしてください。)

* __アンカーを捕らえる範囲__  : ポインタの先端を中心に、設定したピクセル数（※）の距離にあるアンカーが移動対象になります。（※：画面上のピクセル数）
* __移動を無視する距離__  : 設定したピクセル数より少ない移動は無視されます。（不用意なクリックによる移動を防ぐため。）いったん設定距離以上に移動してから戻すことで、設定距離以内の移動も可能です。


## 動作環境：

Adobe Illustrator 2022 (macOS (Apple Silicon))

<!-- Windows10 (win版) -->


## 開発環境

Adobe Illustrator 2022 SDK  
Xcode 13.3.1 / macOS 12.3 sdk / MacOS Monterey 12.3.1  
<!-- Xcode 12.5.1 / macOS 11.3 sdk / MacOS Big Sur -->
MacBook Air (M1)

<!-- Visual Studio 2017 / Windows10 -->


## ビルド

### Mac

myImGuiDialog_mac/myImGuiDialog.xcodeproj で libmyImGuiDialog.a をビルドしてから、
anchorGrabber.xcodeproj で anchorGrabber.aip をビルドしてください。


<!-- ### Windows

SHFXRegularPolygon.sln でソリューションのビルドを実行してください。myImGuiDailog_win、SHFXRegularPolygon の順でプロジェクトがビルドされます。 -->

## 補足（ビルド）
<!-- ソースコードは https://github.com/shspage/anchorGrabber_aip にあります。ビルドする際は以下をご一読ください。-->
* anchorGrabber_aip フォルダはIllustrator SDKのsamplecodeフォルダの直下に置いてください。
<!-- * (Windows) 添付のプロジェクトファイルでビルドするには、ソースコード(.cpp, .h, .hpp)の文字コードをUTF-8からMultibyte(cp932)に変換する必要があります。 -->

## 未解決のバグ
* プラグインの設定ウィンドウを開いた後に、Illustratorの環境設定ウィンドウを開くと、Illustratorが落ちます。

## インストール

（ビルド後）
__anchorGrabber.aip__ をAdobe Illustrator のプラグインフォルダ、または追加プラグインフォルダに 置いてください。

次回 Illustrator の実行時にプラグインが読み込まれ、ツールボックスの「すべてのツール」ドロワーの最下部「アドオン」のカテゴリに、本プラグインのアイコンが追加されます。アイコンをツールボックスにドラッグして配置すると使用可能になります。


## ライセンス

MIT License  
Copyright (c) 2022 Hiroyuki Sato  
https://github.com/shspage  
詳細は LICENSE.txt をご覧ください。


以下については、それぞれのライセンスを参照ください。

* Dear ImGui : Copyright (c) 2014-2020 Omar Cornut  
Licensed under the MIT License  
https://github.com/ocornut/imgui

* Adobe Illustrator 2022 SDK  
Copyright (c) 2022 Adobe. All rights reserved.  
https://www.adobe.io/



