= まえがき

//centering{
＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊ @<strong>{注意} ＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊＊
//}

このドキュメントは、Re:VIEWのサンプル原稿を兼ねています。
自分の原稿を書くときは、サンプルの原稿ファイルと画像ファイルを消してください。


//cmd[サンプルファイル（原稿と画像）の消し方（コマンドラインを知らない人はごめん！）]{
$ @<userinput>{ls contents/}            @<balloon>{原稿ファイルの一覧}
00-preface.re		03-syntax.re		06-bestpractice.re
01-install.re		04-customize.re		92-filelist.re
02-tutorial.re		05-faq.re		99-postface.re
$ @<userinput>{rm contents/*.re}        @<balloon>{原稿のファイルを消す}
$ @<userinput>{rm -r images/*}          @<balloon>{画像ファイル（が入ったディレクトリ）も消す}
$ @<userinput>{vi catalog.yml}          @<balloon>{各章のファイル名を変更する}
//}

本文での、1行あたりの文字数の確認：

//noindent
一二三四五六七八九十一二三四五六七八九十一二三四五六七八九十一二三四五六七八九十一二三四五六七八九十一二三四五六七八九十

//list[][プログラムリストでの、1行あたりの文字数の確認（font: beramono）]{
123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
一二三四五六七八九十一二三四五六七八九十一二三四五六七八九十一二三四五六七八九十一二三四五六七八九十
//}

//terminal[][ターミナルでの、1行あたりの文字数の確認（font: inconsolata）]{
123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_
一二三四五六七八九十一二三四五六七八九十一二三四五六七八九十一二三四五六七八九十一二三四五六七八九十
//}


//centering{
＊＊＊＊＊＊＊＊＊＊＊ 以下、まえがきのサンプル ＊＊＊＊＊＊＊＊＊＊＊@<br>{}
//}


本書を手に取っていただき、ありがとうございます。

本書は、XXXについてわかりやすく解説した本です。
この本を読めば、XXXの基本が身につきます。


====[notoc] 本書の目的

本書の目的は、XXXの基礎的な使い方を身につけることです。
具体的には、XXXを使ってXXXやXXXやXXXができるようになることです。

ただし、XXXやXXXといった内容は、本書では扱いません。


====[notoc] 本書の対象読者

本書では次のような人を対象としています。

 * XXXについて興味がある人
 * XXXの入門書を読んだ人（まったくの初心者は対象外です）


====[notoc] 前提とする知識

本書を読むにあたり、次のような知識が必要となります。

 * Linuxについての基礎知識
 * 何らかのプログラミング言語の基礎知識


====[notoc] 問い合わせ先

本書に関する質問やお問い合わせは、次のページまでお願いします。
正誤表とサンプルコードもここにあります。

 * URL: @<href>{https://www.example.com/mybook/}


====[notoc] 謝辞

本書はXXXX氏とXXXX氏にレビューしていただきました。
この場を借りて感謝します。ありがとうございました。