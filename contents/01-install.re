= 環境構築

//abstract{
まずは実装ができるような環境を整えていきましょう。
本書では@<em>{Binder}という無料サービスを使用して有限要素法の実装を説明していきます。
//}

#@#//makechaptitlepage[toc=on]

=== @<em>{Binder}の起動
@<em>{Binder}は@<em>{JupyterLab}を無料で試せるサービスです。
本書では@<href>{https://mybinder.org/v2/gh/getfem-doc/getfem-examples/master?urlpath=lab, こちら}の環境を使用します。
URLにアクセスすると以下の画面に遷移します。
//image[mybinder][@<em>{Binder}の起動][scale=1.0]
しばらく待つと@<em>{JupyterLab}が起動します。
@<em>{Python3}を起動すると@<em>{Notebook}が起動します。
本書で紹介する@<em>{Python}コマンドをこの@<em>{Notebook}のセルに入力して実行してください。

//caution[一定時間操作がない場合]{
@<em>{Binder}には制限があるため一定時間操作がないとセッションが終了します。
制限のない環境を構築するためにはローカルに環境を構築する必要があります。
詳しくは@<chapref>{92-filelist}を参照してください。
//}

本書ではPythonで有限要素法の実装をするため、@<em>{GetFEM}と@<em>{PyVista}というライブラリを使用します。

 * @<em>{GetFEM}とは有限要素法コードを実装するためのフレームワークのひとつです。
   有限要素法のモデルを解くのに必要です。
 * @<em>{PyVista}とは3次元可視化フレームワークのひとつです。
   @<em>{GetFEM}の結果を可視化するのに必要です。
