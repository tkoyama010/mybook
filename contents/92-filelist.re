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


(TODO)
