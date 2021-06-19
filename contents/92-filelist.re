#@# -*- coding: utf-8 -*-
= @<em>{GetFEM}のコンパイル方法

//abstract{
@<em>{GetFEM}のソースをコンパイルする方法について解説します。
//}

=== Dockerの使い方を知っている場合

Dockerとは、簡単に言えば「WindowsやMacでLinuxコマンドを実行するためのソフトウェア」です@<fn>{guoes}。
//footnote[guoes][Dockerについてのこの説明は、技術的にはちっとも正確ではありません。しかしITエンジニア以外に説明するには、このような説明で充分です。]

Dockerの使い方が分かる人は、@<href>{https://hub.docker.com/r/getfemdoc/getfem/, getfemdoc/getfem}のDockerイメージを使ってください。
これでGetFEMとPyVistaの両方が使えるようになります。

//terminal[][Dockerイメージのダウンロードと動作確認]{
$ @<userinput>{docker pull getfemdoc/getfem}
//}


== GetFEMとPyVistaのインストール

本書のインストールには @<em>{Ubuntu 20.04} を使用してください。
@<em>{Ubuntu 20.04} のインストール自体については本書では扱いません。

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
(TODO)
