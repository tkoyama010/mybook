= ローカルでの環境構築方法

//abstract{
@<em>{GetFEM}のソースをコンパイルする方法について解説します。
//}

== Dockerを使った方法

Dockerの使い方が分かる人は、@<href>{https://hub.docker.com/r/getfemdoc/getfem/, getfemdoc/getfem}のDockerイメージを使ってください。
これで@<em>{GetFEM}と@<em>{PyVista}の両方が使えるようになります。

//terminal[][Dockerイメージのダウンロードと動作確認]{
$ @<userinput>{docker pull getfemdoc/getfem}
//}


== @<em>{GetFEM}と@<em>{PyVista}のインストール

本書のインストールには @<em>{Ubuntu 20.04} を使用してください。
@<em>{Ubuntu 20.04} のインストール自体については本書では扱いません。

これらのインストール手順を説明します。

==== @<em>{GetFEM}のインストール

LinuxがDebianおよびUbuntuディストリビューションの場合、公式リポジトリからインストールすることができます。
以下のコマンドでインストールを行います。
//terminal[][apt-getコマンドによるインストール]{
$ @<userinput>{sudo apt-get install python3-getfem++}     @<balloon>{下線が引かれたコマンドだけを入力すること}
//}

==== @<em>{PyVista}のインストール

以下のように、pipコマンドを入力すると@<em>{PyVista}をインストールすることができます。
//terminal[][pipコマンドによるインストール]{
$ @<userinput>{pip install pyvista}     @<balloon>{下線が引かれたコマンドだけを入力すること}
//}

== インポートの確認

@<em>{GetFEM}と@<em>{PyVista}をインストールしたら、次のようにインポートができるか確認しましょう。
//terminal[][pipコマンドによるインストール]{
$ @<userinput>{python}
>>>import getfem as gf
>>>import pyvista as pv
//}
エラーなくインポートができれば完了です。
