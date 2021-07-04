= ローカルでの環境構築方法

//abstract{
@<em>{GetFEM}と@<em>{PyVista}をローカルにインストールする方法について説明します。
//}


== @<em>{GetFEM}と@<em>{PyVista}のインストール

本書のインストールには @<em>{Ubuntu 20.04} を使用してください。
@<em>{Ubuntu 20.04} のインストール自体については本書では扱いません。

==== @<em>{GetFEM}のインストール

@<em>{GetFEM} は @<em>{Ubuntu 20.04} の公式リポジトリからインストールすることができます。
以下のコマンドでインストールを行います。
//terminal[][apt-getコマンドによるインストール]{
$ @<userinput>{sudo apt-get install python3-getfem++}     @<balloon>{下線が引かれたコマンドだけを入力すること}
//}

==== @<em>{PyVista}のインストール

以下のように、pip3コマンドを入力すると@<em>{PyVista}をインストールすることができます。
//terminal[][pip3コマンドによるインストール]{
$ @<userinput>{sudo apt-get install python3-pip}
$ @<userinput>{pip3 install pyvista}
//}

== インポートの確認

@<em>{GetFEM} と @<em>{PyVista} をインストールしたら、次のようにインポートができるか確認しましょう。

//terminal[][@<em>{GetFEM} と @<em>{PyVista} のインポートの確認]{
$ @<userinput>{python3}
>>> import getfem as gf
>>> import pyvista as pv
//}

エラーなくインポートができれば完了です。
また、 @<em>{python3-getfem++} パッケージをインストールすると依存関係から @<em>{python3-numpy} と @<em>{python3-scipy} もインストールされます。
こちらもインポートできるか確認しておきましょう。

//terminal[][@<em>{numpy} と @<em>{scipy} のインポートの確認]{
$ @<userinput>{python3}
>>> import numpy as np
>>> import scipy as sp
//}
