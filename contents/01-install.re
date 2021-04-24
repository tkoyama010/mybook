= 基本とインストール

//abstract{
まずは手元のパソコンで実装ができるような環境を整えていきましょう。

この章では、Pythonの有限要素法実装のために必要となる「GetFEM」と「PyVista」のインストール方法を説明します。
//}

#@#//makechaptitlepage[toc=on]



== GetFEMとPyVistaのインストール

Pythonで有限要素法の実装をするには、GetFEMとPyVistaのインストールが必要です。

 * GetFEMとは有限要素法コードを実装するためのフレームワークのひとつです。
   有限要素法のモデルを解くのに必要です。
 * PyVistaとは3次元可視化フレームワークのひとつです。
   GetFEMの結果を可視化するのに必要です。

これらのインストール手順を説明します。


=== Dockerの使い方を知っている場合

Dockerとは、簡単に言えば「WindowsやMacでLinuxコマンドを実行するためのソフトウェア」です@<fn>{guoes}。
//footnote[guoes][Dockerについてのこの説明は、技術的にはちっとも正確ではありません。しかしITエンジニア以外に説明するには、このような説明で充分です。]

Dockerの使い方が分かる人は、@<href>{https://hub.docker.com/r/getfemdoc/getfem/, getfemdoc/getfem}のDockerイメージを使ってください。
これでGetFEMとPyVistaの両方が使えるようになります。

//terminal[][Dockerイメージのダウンロードと動作確認]{
$ @<userinput>{docker pull getfemdoc/getfem}
//}

=== macOSを使っている場合

macOSにおいて、GetFEMとPyVistaをインストールする方法を説明します。

==== GetFEMのインストール

==== PyVistaのインストール

=== Windowsを使っている場合

Windowsにおいて、GetFEMとPyVistaをインストールする方法を説明します。

==== GetFEMのインストール

==== PyVistaのインストール

=== Linuxを使っている場合

Linuxにおいて、GetFEMとPyVistaをインストールする方法を説明します。

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

GetFEMとPyVistaをインストールしたら、次に本を作るための「プロジェクト」を作成しましょう。

以下のWebサイトにアクセスしてください。

 * @<href>{https://kauplan.org/reviewstarter/}

アクセスしたら、画面の指示に従って操作をしてください（詳細は省略します）。
するとプロジェクトが作成されて、zipファイルがダウンロードできます。

以降では、プロジェクトのzipファイル名が「@<file>{mybook.zip}」だったとして説明します。


=={sec-generatesample} サンプルのPDFファイルを生成

プロジェクトのzipファイルをダウンロードしたら、解凍してサンプルのPDFファイルを生成してみましょう。

==={subsec-indocker} Dockerの場合

Dockerを使う場合は、次のような手順でサンプルのPDFファイルを生成してみましょう。

//terminal[][Dockerを使ってPDFファイルを生成する]{
$ @<userinput>{unzip mybook.zip}           @<balloon>{zipファイルを解凍}
$ @<userinput>{cd mybook/}                 @<balloon>{ディレクトリを移動}
$ @<userinput>{docker run --rm -v $PWD:/work -w /work kauplan/review2.5 rake pdf}
$ @<userinput>{ls *.pdf}                   @<balloon>{PDFファイルが生成できたことを確認}
mybook.pdf
//}

これでPDFファイルが生成されるはずです。生成できなかった場合は、Twitterで「@<em>{#GetFEM}」タグをつけて質問してください（相手先不要）。

Dockerを使ってPDFファイルが作成できることを確認したら、このあとはdockerコマンドを使わずとも「@<code>{rake docker:pdf}」だけでPDFファイルが生成できます。

//terminal[][より簡単にPDFファイルを生成する]{
$ @<del>{docker run --rm -v $PWD:/work -w /work kauplan/review2.5 rake pdf}
$ @<userinput>{@<b>{rake docker:pdf}}            @<balloon>{これだけでPDFファイルが生成される}
//}

//note[もっと簡単にPDFファイルを生成するためのTips]{

環境変数「@<code>{$RAKE_DEFAULT}」を設定すると、引数なしの「@<code>{rake}」コマンドだけでPDFファイルが生成できます。

//terminal[][環境変数「@<code>{$RAKE_DEFAULT}」を設定する]{
$ @<userinput>{export RAKE_DEFAULT="docker:pdf"} @<balloon>{環境変数を設定}
$ @<del>{rake docker:pdf}
$ @<userinput>{rake}                  @<balloon>{「rake docker:pdf」が実行される}
//}

またシェルのエイリアス機能を使うと、コマンドを短縮名で呼び出せます。

//terminal[][シェルのエイリアス機能を使う]{
$ @<userinput>{alias pdf="rake docker:pdf"} @<balloon>{エイリアスを設定}
$ @<userinput>{pdf}                   @<balloon>{「rake docker:pdf」が実行される}
//}

//}


=== macOSの場合

macOSを使っている場合は、Terminal.app@<fn>{qqyo5}を開いて以下のコマンドを実行してください。
ここで、各行の先頭にある「@<code>{$ }」は入力せず、それ以降のコマンドを入力してください。
//footnote[qqyo5][繰り返しになりますが、Terminal.appはファインダで「アプリケーション」フォルダ > 「ユーティリティ」フォルダ > 「ターミナル」をダブルクリックすると起動できます。]

//terminal[][PDFファイルを生成する]{
$ @<userinput>{unzip mybook.zip}       @<balloon>{zipファイルを解凍し、}
$ @<userinput>{cd mybook/}             @<balloon>{ディレクトリを移動}
$ @<userinput>{rake pdf}               @<balloon>{PDFファイルを生成}
$ @<userinput>{ls *.pdf}               @<balloon>{PDFファイルが生成されたことを確認}
mybook.pdf
$ @<userinput>{open mybook.pdf}        @<balloon>{PDFファイルを開く}
//}

これでPDFファイルが生成されるはずです。生成できなかった場合は、Twitterで「@<em>{#reviewstarter}」タグをつけて質問してください（相手先不要）。

//note[もっと簡単にPDFファイルを生成するためのTips]{

環境変数「@<code>{$RAKE_DEFAULT}」を設定すると、引数なしの「@<code>{rake}」コマンドだけでPDFファイルが生成できます。

//terminal[][環境変数「@<code>{$RAKE_DEFAULT}」を設定する]{
$ @<userinput>{export RAKE_DEFAULT="pdf"} @<balloon>{環境変数を設定}
$ @<del>{rake pdf}
$ @<userinput>{rake}                      @<balloon>{「rake pdf」が実行される}
//}

またシェルのエイリアス機能を使うと、コマンドを短縮名で呼び出せます。

//terminal[][シェルのエイリアス機能を使う]{
$ @<userinput>{alias pdf="rake pdf"}      @<balloon>{エイリアスを設定}
$ @<userinput>{pdf}                       @<balloon>{「rake pdf」が実行される}
//}

//}


=== Windowsの場合

Windowsを使っている場合は、まずプロジェクトのzipファイル（@<file>{mybook.zip}）をダブルクリックして解凍してください。

そしてコマンドプロンプトで以下のコマンドを実行してください。

//terminal[][PDFファイルを生成する]{
C:\Users\yourname> @<userinput>{cd mybook}        @<balloon>{解凍してできたフォルダに移動}
C:\Users\yourname\mybook> @<userinput>{rake pdf}  @<balloon>{PDFファイルを生成}
C:\Users\yourname\mybook> @<userinput>{dir *.pdf} @<balloon>{PDFファイルが生成されたことを確認}
//}

これでPDFファイルが生成されるはずです。生成できなかった場合は、Twitterで「@<em>{#reviewstarter}」タグをつけて質問してください（相手先不要）。


=== Dockerの使い方を知っている場合

Dockerとは、簡単に言えば「WindowsやMacでLinuxコマンドを実行するためのソフトウェア」です@<fn>{guoes}。
//footnote[guoes][Dockerについてのこの説明は、技術的にはちっとも正確ではありません。しかしITエンジニア以外に説明するには、このような説明で充分です。]

Dockerの使い方が分かる人は、@<href>{https://hub.docker.com/r/kauplan/review2.5/, kauplan/review2.5}のDockerイメージを使ってください。
これでRubyとTeXLiveの両方が使えるようになります。

//terminal[][Dockerイメージのダウンロードと動作確認]{
$ @<userinput>{docker pull kauplan/review2.5}   @<balloon>{3GB 以上ダウンロードするので注意}
$ @<userinput>{docker run --rm kauplan/review2.5 ruby --version}
ruby 2.5.1p57 (2018-03-29 revision 63029) [x86_64-linux-gnu]
$ @<userinput>{docker run --rm kauplan/review2.5 uplatex --version}
e-upTeX 3.14159265-p3.7.1-u1.22-161114-2.6 (utf8.uptex) (TeX Live 2017/Debian)
...(省略)...
//}


== 注意点

注意点をいくつか説明します。

 * エラーが発生したときの対処法は@<secref>{02-tutorial|subsec-compileerror}で簡単に説明しています。
   必ず読んでください。
 * ePubファイルを生成するときは「@<code>{rake epub}」または「@<code>{rake docker:epub}」、HTMLファイルを生成するときは「@<code>{rake web}」または「@<code>{rake docker:epub}」を実行してください。
 * Starterでは「@<code>{review-pdfmaker}」コマンドや「@<code>{review-epubmaker}」コマンドが使えません（実行はできますが望ましい結果にはなりません）。
   必ず「@<code>{rake pdf}」や「@<code>{rake epub}」を使ってください。
