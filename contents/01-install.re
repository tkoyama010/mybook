= 基本とインストール

//abstract{
まずは手元のパソコンで実装ができるような環境を整えていきましょう。

この章では、Pythonの有限要素法実装のために必要となる「GetFEM」と「PyVista」のインストール方法を説明します。

本書のインストールには @<em>{Ubuntu 20.04} を使用してください。
@<em>{Ubuntu 20.04} のインストール自体については本書では扱いません。
//}

#@#//makechaptitlepage[toc=on]



== GetFEMとPyVistaのインストール

Pythonで有限要素法の実装をするには、GetFEMとPyVistaのインストールが必要です。

 * GetFEMとは有限要素法コードを実装するためのフレームワークのひとつです。
   有限要素法のモデルを解くのに必要です。
 * PyVistaとは3次元可視化フレームワークのひとつです。
   GetFEMの結果を可視化するのに必要です。

これらのインストール手順を説明します。

==== GetFEMのインストール

LinuxがDebianおよびUbuntuディストリビューションの場合、公式リポジトリからインストールすることができます。
以下のコマンドでインストールを行います。
//terminal[][apt-getコマンドによるインストール]{
$ @<userinput>{sudo apt-get install python3-getfem++}     @<balloon>{下線が引かれたコマンドだけを入力すること}
//}

==== PyVistaのインストール

以下のように、pipコマンドを入力するとPyVistaをインストールすることができます。
//terminal[][pipコマンドによるインストール]{
$ @<userinput>{pip install pyvista}     @<balloon>{下線が引かれたコマンドだけを入力すること}
//}

== サンプルファイルの実行

GetFEMとPyVistaをインストールしたら、次のようにインポートができるか確認しましょう。
//terminal[][pipコマンドによるインストール]{
$ @<userinput>{python}
>>>import getfem as gf
>>>import pyvista as pv
//}
エラーなくインポートができれば完了です。
