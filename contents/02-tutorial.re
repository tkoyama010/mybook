= @<em>{GetFEM}と@<em>{PyVista}の基礎

//abstract{
前の章では、@<em>{GetFEM}と@<em>{PyVista}のインストールを説明し、サンプルのスクリプトを実行しました。
この章では、自分で書いたスクリプトファイルで解析を実行する方法を説明します。
なおこの章は、@<em>{GetFEM}と@<em>{PyVista}のインストールが済んでいること、またサンプルのスクリプトが実行できたことを前提にしています。
まだの人は前の章を見てください。
//}

#@#//makechaptitlepage[toc=on]

== 用語の説明

このあとの説明で使用する用語を紹介します。

: @<em>{Model}オブジェクト
    @<em>{GetFEM}の中心となるオブジェクトです。全体剛性方程式を管理し解析を実行します。

: @<em>{Mesh}オブジェクト
    メッシュを管理するオブジェクトです。@<em>{GetFEM}は有限要素法の要素の種類と幾何情報が完全に分離されています。

: 凸(convex)
    メッシュを構成する要素です、要素と呼ばれることもありますが、本書では凸(convex)で統一します。

: 点
    凸(convex)を定義するするために定義される点です。点を複数結ぶことにより凸(convex)となります。
    節点と呼ばれることもありますが、点で統一します。

: @<em>{MeshFem}オブジェクト
    メッシュの有限要素法を管理するオブジェクトです。メッシュの凸(convex)にどの有限要素法を設定するかを管理します。
    全ての凸(convex)に個別に有限要素法を定義することも可能ですが本書では説明しません。

: @<em>{MeshIm}オブジェクト
    メッシュの積分法を管理するオブジェクトです。メッシュの凸(convex)にどの積分法を設定するかを管理します。
    全ての凸(convex)に個別に積分法を定義することも可能ですが本書では説明しません。

: 弱形式言語(weak form language)
    微分方程式(PDE)を有限要素法で解析する際は強形式の微分方程式を弱形式に変形をして離散化を行います。
    @<em>{GetFEM}では弱形式を自動的に離散化する機能を実装しており、その際に使用される言語を弱形式言語(weak form language)と呼称します。
    本書では弱形式言語を使用する際に弱形式言語の記法について説明します。

: ブリック(brick)
    ブリック(brick)はModelオブジェクトの全体剛性方程式に追加する項を指します。
    ブリック(brick)の多くは弱形式言語(weak form language)で記述されており典型的な項は@<em>{Model}オブジェクトのメソッドとして定式化されています。

== @<em>{GetFEM}の特徴

ここでは@<em>{GetFEM}の大きな特徴を2つ紹介します。

1つ目はメッシュと有限要素法と積分法のオブジェクトが完全に分離されていることです。
図に示すように@<em>{Mesh}(メッシュ)オブジェクトと@<em>{Fem}(有限要素法)オブジェクトと@<em>{Integ}(積分法)オブジェクトは完全に分離されいます。
これには利点があります。
例えば、3種類のメッシュ形状と3種類の有限要素法と3種類の積分法がそれぞれ存在した場合、
通常の有限要素法プログラムでは全ての組み合わせに対応した@<m>{3 \times 3 \times 3 = 27}種類の要素を個別に開発する必要があります。
しかし、独立していればそれらのオブジェクトを組み合わせることで実装の手間を省くことができます。
新しい有限要素法の手法や積分法を追加したいとき、@<em>{Fem}(有限要素法)オブジェクトや@<em>{Integ}(積分法)オブジェクトに追加するだけでよいのです。

2つ目は弱形式言語(weak form language)による自動モデリング機能が実装されていることです。
有限要素法では微分方程式を弱定式化して離散化します。
通常の有限要素法プログラムでは弱定式した式を離散化して定式化します。
@<em>{GetFEM}では弱定式化した式を弱形式言語(weak form language)で入力することにより自動的に離散化が行われます。

== GetFEMの実行手順

GetFEMを実行する際の手順について説明します。

=== インポートする

まずは@<em>{GetFEM}を使用できるようにモジュールをインポートします。
@<em>{GetFEM}では@<em>{numpy}もよく使われるのでインポートしておきます。
慣例として@<em>{GetFEM}は@<em>{gf}として、@<em>{Numpy}は@<em>{np}としてインポートされるので覚えておきましょう。

//list[][モジュールインポート][lang=python]{
>>> import getfem as gf
>>> import numpy as np
>>> import pyvista as pv
//}


=== @<em>{GeoTrans}オブジェクトを作成する

//list[][GeoTransオブジェクトの作成][lang=python]{
>>> gt = gf.GeoTrans("GT_PK(1, 1)")
>>> print(gt)
GT_PK(1,1)
//}


=== @<em>{Mesh}オブジェクトを作成する

@<em>{Mesh}オブジェクトを作成します。
今回は長さ10.0の1次元のメッシュを作成します。

//list[][Meshオブジェクトの作成][lang=python]{
>>> mesh = gf.Mesh("empty", 1)
>>> pts = np.array([[0.0, 10.0]])
>>> mesh.add_convex(gt, pts)
array([0], dtype=int32)
>>> print(mesh)

BEGIN POINTS LIST

  POINT COUNT 2
  POINT  0  0
  POINT  1  10

END POINTS LIST



BEGIN MESH STRUCTURE DESCRIPTION

  CONVEX COUNT 1
  CONVEX 0    'GT_PK(1,1)'      0  1

END MESH STRUCTURE DESCRIPTION

//}

出力結果からインデックス0の点が0.0にあり、インデックス1の点が10.0の位置にあることが分かります。
また、点0と点1に接続された凸(convex)が定義されていることが分かります。


=== @<em>{MeshFem}オブジェクトを作成する

@<em>{MeshFem}オブジェクトを作成します。
まずは@<href>{https://getfem.readthedocs.io/ja/latest/python/cmdref_MeshFem.html, 公式サイト}に載っている@<em>{MeshFem}オブジェクトの定義から見ていきましょう。
公式サイトには「このオブジェクトは，メッシュ全体で定義された有限要素法を表します．」と書かれています。
@<em>{GetFEM}の特徴で説明した通りこのライブラリは@<em>{Mesh}オブジェクトと@<em>{Fem}(有限要素法)オブジェクトが完全に分離されています。
そのため、2つのオブジェクトの対応関係を管理するオブジェクトが必要になります。
それがこのオブジェクトです。

@<em>{Mesh}オブジェクトを元に@<em>{MeshFem}オブジェクトを作っていきます。
作成の際には、@<em>{Mesh}オブジェクトと求めたい物理量の次元を与えます。
スカラーの場合は1、ベクトルの場合はそのベクトルの次元(1, 2, 3...)を与えます。

//list[][@<em>{MeshFem}オブジェクトの作成][lang=python]{
>>> mfu = gf.MeshFem(mesh, 2)
>>> print(mfu)
BEGIN MESH_FEM

QDIM 2
 BEGIN DOF_ENUMERATION
 END DOF_ENUMERATION
END MESH_FEM
//}

出力から分かるように、この時点では凸(convex)に@<em>{Fem}(有限要素法)オブジェクトは割り当てられていません。
そのため、@<em>{Fem}(有限要素法)オブジェクトを作成して設定します。
ここでは、一般的に使われている単体の古典的Lagrange要素(@<em>{"FEM_PK(1,1)"})を設定します。
凸(convex)のIDを指定して設定することもできますが、今回は1要素ですので指定はしません。
凸(convex)に@<em>{"FEM_PK(1,1)"}が設定されていることが分かります。

//list[][@<em>{MeshFem}オブジェクトへの有限要素法の設定][lang=python]{
>>> f = gf.Fem("FEM_PK(1,1)")
>>> mfu.set_fem(f)
>>> print(mfu)
BEGIN MESH_FEM

QDIM 2
 CONVEX 0 'FEM_PK(1,1)'
 BEGIN DOF_ENUMERATION
  0:  0 2
 END DOF_ENUMERATION
END MESH_FEM
//}

//note[有限要素法の種類について]{

通常の解析では単体の古典的Lagrange要素を使用します。

//list[][@<em>{MeshFem}オブジェクトへの単体の古典的Lagrange要素の設定][lang=python]{
>>> mfu.set_fem(f)
//}

また、その他の有限要素法には次のものがあります。

//table[tbl-31][サンプル表]{
Name	Val1	Val2
--------------------
AA	12	34
BB	56	78
//}

//}

ここまでが、@<em>{Mesh}オブジェクトと@<em>{Fem}オブジェクトから@<em>{MeshFem}オブジェクトを作成する方法でした。
次に、積分法を定義するための@<em>{MeshIm}オブジェクトを作成する方法を説明します。


=== @<em>{MeshIm}オブジェクトを作成する

@<em>{MeshIm}オブジェクトを作成します。
まずは@<href>{https://getfem.readthedocs.io/ja/latest/python/cmdref_MeshIm.html, 公式サイト}に載っている@<em>{MeshIm}オブジェクトの定義から見ていきましょう。
公式サイトには「このオブジェクトは，メッシュ全体で定義された積分法を表します」と書かれています。
@<em>{GetFEM}の特徴で説明した通りこのライブラリは@<em>{Mesh}オブジェクトと@<em>{Integ}(積分法)オブジェクトが完全に分離されています。
そのため、2つのオブジェクトの対応関係を管理するオブジェクトが必要になります。
それがこのオブジェクトです。
ここでは、積分点が1点のGauss積分法を定義して@<em>{MeshIm}オブジェクトを作成します。

//list[][MeshImオブジェクトの作成][lang=python]{
>>> im = gf.Integ("IM_GAUSS1D(1)")
>>> mim = gf.MeshIm(mesh, im)
//}

//note[積分法の種類について]{

また、その他の積分法には次のものがあります。

//}

次に剛性方程式を管理作成するための@<em>{Model}オブジェクトを作成します。

=== @<em>{Model}オブジェクトを作成する

新しい@<em>{Model}オブジェクトを作成するには、次のようにします。

 - (1) @<em>{Model}オブジェクトを定義する。
 - (2) 変数を定義する。
 - (3) ブリック(brick)を定義する。

順番に説明しましょう。
各メソッドの詳しい説明は@<href>{https://getfem.readthedocs.io/ja/latest/python/cmdref_Model.html, 公式サイト}を参照してください。

===== (1) @<em>{Model}オブジェクトを定義する。

@<em>{Model}オブジェクトは次のように定義します。
実数が未知数のオブジェクトを作成するため、@<em>{"real"}を引数とします。

//list[][@<em>{Model}オブジェクトの作成][lang=python]{
>>> md = gf.Model("real")
//}

===== (2) 変数を定義する。

変数を定義します。
問題が解かれた際にはこの変数に解が出力されます。
変数は@<em>{MeshFem}オブジェクトにリンクして定義します。
メソッドは@<em>{add_fem_variable}を使用します。

//list[][変数の定義][lang=python]{
>>> md.add_fem_variable("u", mf_u)
//}

===== (3) ブリック(brick)を定義する。

ブリック(brick)を定義します。
今回は図XXXXに示すモデルを作成します。

このモデルを作成するために必要なブリックは以下の通りです。

 * 等方性線形弾性項
 * Dirichlet条件(固定条件)
 * ソース項(荷重条件)

まずは、等方性線形弾性項を追加します。

//list[][等方性線形弾性項の追加][lang=python]{
>>> md.add_initialized_scalar_data("clambda", clambda)
>>> md.add_initialized_scalar_data("cmu", cmu)
>>> md.add_isotropic_linearized_elasticity_brick(mim, "u", "clambda", "cmu")
//}


==={subsec-compileerror} コンパイルエラーになったら

PDFオブジェクトを生成するときにエラーになったら、以下の点を確認してください。

 * インライン命令がきちんと閉じているか
 * ブロック命令の引数が足りているか、多すぎないか
 * 「@<code>|//}|」が足りてないか、または多すぎないか
 * 「@<code>|@@<nop>{}<fn>{}|」や「@<code>|@@<nop>{}<img>{}|」や「@<code>|@@<nop>{}<table>{}|」のラベルが合っているか
 * 「@<code>|@@<nop>{}<chapref>{}|」で指定した章IDが合っているか
 * 「@<code>|@@<nop>{}<secref>{}|」で指定した節や項が存在するか
 * 脚注の中で「@<code>{]}」を「@<code>{\]}」とエスケープしているか
 * 「@<code>|//image|」で指定した画像オブジェクトが存在するか
 * 原稿オブジェクト名を間違っていないか
 * 原稿オブジェクトの文字コードがUTF-8になっているか

詳しくは@<secref>{05-faq|sec-compileerror}を見てください。



=={sec-basicsyntax} 基本的な記法

原稿オブジェクトは、ある決まった書き方（記法）に従って記述します。
たとえば、次のような記法があります。

 * 章(Chapter)は「@<code>{= }」で始め、節(Section)は「@<code>{== }」で始める。
 * 箇条書きは「@<code>{ * }」で始める。
 * プログラムコードは「@<code>|//list{...//}|」で囲う。
 * 強調は「@<code>|@@<nop>{}<strong>{...}|」または「@<code>|@@<nop>{}<B>{...}|」で囲う。

ここでは記法のうち基本的なものを説明します。
詳しいことは@<chapref>{03-syntax}で説明します。


=== コメント

行コメントは「@<code>{#@#}」で、また範囲コメントは「@<code>{#@+++}」と「@<code>{#@---}」で表します。
どちらも行の先頭から始まってないと、コメントとして認識されません。

//list[][サンプル]{
本文1
@<b>|#@#|本文2
本文3

@<b>|#@+++|
本文4
@<b>|#@---|
本文5
//}

//sampleoutputbegin[表示結果]

本文1
#@#本文2
本文3

#@+++
本文4
#@---
本文5

//sampleoutputend



詳しくは@<secref>{03-syntax|sec-comment}を参照してください。


=== 段落と改行

空行があると、新しい段落になります。
改行は無視されるか、または半角空白扱いになります。
通常は1文ごとに改行して書きます。

//list[][サンプル]{
言葉を慎みたまえ！君はラピュタ王の前にいるのだ！

これから王国の復活を祝って、諸君にラピュタの力を見せてやろうと思ってね。見せてあげよう、ラピュタの雷を！

旧約聖書にあるソドムとゴモラを滅ぼした天の火だよ。
ラーマヤーナではインドラの矢とも伝えているがね。
//}

//sampleoutputbegin[表示結果]

言葉を慎みたまえ！君はラピュタ王の前にいるのだ！

これから王国の復活を祝って、諸君にラピュタの力を見せてやろうと思ってね。見せてあげよう、ラピュタの雷を！

旧約聖書にあるソドムとゴモラを滅ぼした天の火だよ。
ラーマヤーナではインドラの矢とも伝えているがね。

//sampleoutputend



詳しくは@<secref>{03-syntax|sec-paragraph}を参照してください。


=== 見出し

章(Chapter)や節(Section)といった見出しは、「@<code>{= }」や「@<code>{== }」で始めます。
また章には概要を書くといいでしょう。

//list[][サンプル]{
@<b>|=| 章(Chapter)見出し

@<b>|//abstract{|
章の概要。
@<b>|//}|

@<b>|==| 節(Section)見出し

@<b>|===| 項(Subsection)見出し

@<b>|====| 目(Subsubsection)見出し

@<b>|=====| 段(Paragraph)見出し

@<b>|======| 小段(Subparagraph)見出し
//}



Starterでは、章(Chapter)は1オブジェクトにつき1つだけにしてください。
1つのオブジェクトに複数の章(Chapter)を入れないでください。

見出しについての詳細は@<secref>{03-syntax|sec-heading}を参照してください。


=== 箇条書き

番号なし箇条書きは「@<code>{ * }」で始めます（先頭に半角空白が必要）。

//list[][サンプル]{
@<b>| * |箇条書き1
@<b>| * |箇条書き2
@<b>| ** |入れ子の箇条書き
@<b>| *** |入れ子の入れ子
//}

//sampleoutputbegin[表示結果]

 * 箇条書き1
 * 箇条書き2
 ** 入れ子の箇条書き
 *** 入れ子の入れ子

//sampleoutputend



番号つき箇条書きは「@<code>{ 1. }」のように始める方法と、「@<code>{ - A. }」のように始める方法があります（どちらも先頭に半角空白が必要）。
後者では番号の自動採番はされず、指定された文字列がそのまま出力されます。

//list[][サンプル]{
 @<b>|1.| この記法では数字しか指定できず、
 @<b>|2.| また入れ子にもできない。

 @<b>|- (A)| この記法だと、英数字だけでなく
 @<b>|- (B)| 任意の文字列が使える
 @<b>|-- (B-1)| 入れ子もできるし、
 @<b>|***| 番号なし箇条書きも含められる
//}

//sampleoutputbegin[表示結果]

 1. この記法では数字しか指定できず、
 2. また入れ子にもできない。

 - (A) この記法だと、英数字だけでなく
 - (B) 任意の文字列が使える
 -- (B-1) 入れ子もできるし、
 *** 番号なし箇条書きも含められる

//sampleoutputend



箇条書きの詳細は@<secref>{03-syntax|sec-list}を参照してください。


=== 用語リスト

用語リスト（HTMLでいうところの「@<code>{<dl><dt><dd>}」）は、「@<code>{ : }」で始めて@<fn>{axm3t}、次の行からインデントします@<fn>{diwmv}。
//footnote[axm3t][先頭の半角空白は入れるようにしましょう。過去との互換性のため、先頭の半角空白がなくても動作しますが、箇条書きとの整合性のために半角空白を入れることを勧めます。]
//footnote[diwmv][説明文のインデントは、半角空白でもタブ文字でもいいです。]

//list[][サンプル]{
@<b>| : |用語1
    説明文。
    説明文。
@<b>| : |用語2
    説明文。
    説明文。
//}

//sampleoutputbegin[表示結果]

 : 用語1
    説明文。
    説明文。
 : 用語2
    説明文。
    説明文。

//sampleoutputend



詳しくは@<secref>{03-syntax|sec-termlist}を参照してください。


=== 太字と強調

太字は「@<code>|@@<nop>{}<b>{...}|」で囲み、強調は「@<code>|@@<nop>{}<B>{...}|」または「@<code>|@@<nop>{}<strong>{...}|」で囲みます。
強調は、太字になるだけでなくフォントがゴシック体になります。

//list[][サンプル]{
テキスト@<b>|@@<nop>$$<b>{|テキスト@<b>|}|テキスト

テキスト@<b>|@@<nop>$$<B>{|テキスト@<b>|}|テキスト
//}

//sampleoutputbegin[表示結果]

テキスト@<b>{テキスト}テキスト

テキスト@<B>{テキスト}テキスト

//sampleoutputend



日本語の文章では、強調するときは太字のゴシック体にするのがよいとされています。
なので強調には「@<code>|@@<nop>{}<B>{...}|」または「@<code>|@@<nop>{}<strong>{...}|」を使い、「@<code>|@@<nop>{}<b>{...}|」は使わないでください。

強調や太字などテキストの装飾についての詳細は@<secref>{03-syntax|sec-decoration}を参照してください。

//note[インライン命令]{
「@<code>|@@<nop>{}<B>{...}|」のような記法は@<em>{インライン命令}と呼ばれます。
インライン命令は入れ子にできますが（Starterによる拡張）、複数行を含めることはできません。
詳細は@<secref>{03-syntax|subsec-inlinecmd}を見てください。

#@#インライン命令は入れ子にできます（Starterによる拡張）。
#@#たとえば「@<code>|@@<nop>{}<code>{fn(@@<nop>{}<b>{@@<nop>{}<i>{arg}})}|」と書くと「@<code>{fn(@<b>{@<i>{arg}})}」のように表示されます。
#@#
#@#またインライン命令は必ず1行に記述します。
#@#複数行を含めることはできません。
#@#
#@#//list[][このような書き方はできない]{
#@#@<B>|@@<nop>{}<B>{|テキスト
#@#テキスト
#@#テキスト@<B>|}|
#@#//}

//}


=== プログラムリスト

プログラムリストは「@<code>|//list[ラベル][説明文]{ ... //}|」で囲みます。

  * ラベルは、他と重複しない文字列にします。
  * 「@<code>|@@<nop>{}<list>{ラベル名}|」のようにプログラムリストを参照できます。
  * 説明文に「@<code>{]}」を含める場合は、「@<code>{\]}」のようにエスケープします。

//list[][サンプル]{
サンプルコードを@<b>|@@<nop>$$<list>{fib1}|に示します。

@<b>|//list[fib1][フィボナッチ数列]|{
def fib(n):
    return n if n <= 1 else fib(n-1) + fib(n-2)
@<b>|//}|
//}

//sampleoutputbegin[表示結果]

サンプルコードを@<list>{fib1}に示します。

//list[fib1][フィボナッチ数列]{
def fib(n):
    return n if n <= 1 else fib(n-1) + fib(n-2)
//}

//sampleoutputend



第1引数も第2引数も、省略できます。
たとえば第1引数だけを省略するには「@<code>|//list[][説明]{|」のようにします。
両方とも省略するには「@<code>|//list[][]{|」または「@<code>|//list{|」のようにします。

またプログラムリストの中で、インライン命令が使えます。
たとえば次の例では、追加した箇所を「@<code>|@@<nop>{}<b>{...}|」で@<fn>{0hz8w}、削除した箇所を「@<code>|@@<nop>{}<del>{...}|」で表しています。
//footnote[0hz8w][プログラムリストの中では、「@<code>|@@<nop>{}<B>{}|」（強調）ではなく「@<code>|@@<nop>{}<b>{}|」（太字）を使ってください。なぜなら、「@<code>|@@<nop>{}<B>{}|」を使うとフォントが等幅フォントからゴシック体に変更されてしまい、表示がずれてしまうからです。]

//list[][サンプル]{
/@<nop>$$/list{
function fib(n) {
  @<b>{@@<nop>$$<del>|}if (n <= 1) { return n; }@<b>{|}
  @<b>{@@<nop>$$<del>|}else        { return fib(n-1) + fib(n-2); }@<b>{|}
  @<b>{@@<nop>$$<b>|}return n <= 1 ? n : fib(n-1) + fib(n-2);@<b>{|}
}
/@<nop>$$/}
//}

//sampleoutputbegin[表示結果]

//list{
function fib(n) {
  @<del>|if (n <= 1) { return n; }|
  @<del>|else        { return fib(n-1) + fib(n-2); }|
  @<b>|return n <= 1 ? n : fib(n-1) + fib(n-2);|
}
//}

//sampleoutputend



プログラムコードについての詳細は@<secref>{03-syntax|sec-program}を参照してください。

//note[ブロック命令]{
「@<code>|//list{ ... //}|」のような形式の命令を、@<B>{ブロック命令}といいます。
ブロック命令は入れ子にできますが（Starterによる拡張）、できないものもあります。
詳細は@<secref>{03-syntax|subsec-blockcmd}を見てください。
//}


=== ターミナル画面

ターミナル（端末）の画面は、「@<code>|//terminal{ ... //}|」を使います。
次の例では、引数にラベル名と説明文字列を指定します。
また「@<code>|@@<nop>{}<userinput>{...}|」はユーザ入力を表します。

//list[][サンプル]{
@<b>|//terminal[term-1][PDFを生成]{|
$ @@<nop>$$<userinput>{rake pdf}          @@<nop>$$<balloon>{Dockerを使わない場合}
$ @@<nop>$$<userinput>{rake docker:pdf}   @@<nop>$$<balloon>{Dockerを使う場合}
@<b>|//}|
//}

//sampleoutputbegin[表示結果]

//terminal[term-1][PDFを生成]{
$ @<userinput>{rake pdf}          @<balloon>{Dockerを使わない場合}
$ @<userinput>{rake docker:pdf}   @<balloon>{Dockerを使う場合}
//}

//sampleoutputend



ラベル名を指定していれば、プログラムリストと同じように「@<code>|@@<nop>{}<list>{ラベル名}|」で参照できます。
またラベル名と説明文字列はどちらも省略可能です。
どちらも省略すると「@<code>|//terminal{ ... //}|」のように書けます。

詳しくは@<secref>{03-syntax|sec-terminal}を参照してください。


=== 脚注

脚注は、脚注をつけたい箇所に「@<code>|@@<nop>{}<fn>{ラベル}|」をつけ、段落の終わりに「@<code>|//footnote[ラベル][脚注文]|」を書きます。

//list[][サンプル]{
本文テキスト@<b>|@@<nop>$$<fn>{fn-12}|。

@<b>|//footnote[fn-12][このように脚注文を書きます。]|
//}

//sampleoutputbegin[表示結果]

本文テキスト@<fn>{fn-12}。

//footnote[fn-12][このように脚注文を書きます。]

//sampleoutputend




=== 図

図を入れるには、「@<code>|//image[画像オブジェクト名][説明文][オプション]|」を使います。

 * 画像オブジェクトは「@<file>{images/}」フォルダに置きます。
 * 画像オブジェクトの拡張子は指定しません。
 * ラベルを使って「@<code>|@@<nop>{}<img>{ラベル}|」のように参照できます。

//list[][サンプル]{
@<b>|//image[tw-icon][サンプル画像][scale=0.3]|
//}

//sampleoutputbegin[表示結果]

//image[tw-icon][サンプル画像][scale=0.3]

//sampleoutputend



Starterでは、画像を入れる位置を指定したり、画像の周りに枠線をつけたりできます。
詳しくは@<secref>{03-syntax|sec-image}を参照してください。


=== ノート

Starterでは、本文を補足する文章を「ノート」という囲み枠で表します。

//list[][サンプル]{
/@<nop>$$/note[ムスカの苦手なもの]{
実は、ムスカには「虫が苦手」という公式な設定があります。
有名な『読める、読めるぞ！』のシーンでムスカが顔の周りに群がるハエを追い払うのは、邪魔だったからだけでなく、虫が苦手だったからです。
/@<nop>$$/}
//}

//sampleoutputbegin[表示結果]

//note[ムスカの苦手なもの]{
実は、ムスカには「虫が苦手」という公式な設定があります。
有名な『読める、読めるぞ！』のシーンでムスカが顔の周りに群がるハエを追い払うのは、邪魔だったからだけでなく、虫が苦手だったからです。
//}

//sampleoutputend



ノート本文には箇条書きやプログラムコードを埋め込めます。
詳しくは@<secref>{03-syntax|sec-note}を参照してください。


=== 表

表は、次のように書きます。

 * 「@<code>|//table[ラベル][説明文]{ ... //}|」で囲みます。
 * セルは1つ以上のタブで区切ります。
 * ヘッダの区切りは「@<code>{-}」または「@<code>{=}」を12個以上並べます。
 * ラベルを使って「@<code>|@@<nop>{}<table>{ラベル}|」のように参照できます。

//list[][サンプル]{
@<b>|//table[tbl-31][サンプル表]{|
Name	Val1	Val2
@<b>|--------------------|
AA	12	34
BB	56	78
@<b>|//}|
//}

//sampleoutputbegin[表示結果]

//table[tbl-31][サンプル表]{
Name	Val1	Val2
--------------------
AA	12	34
BB	56	78
//}

//sampleoutputend



PDFではセルの右寄せ・左寄せ・中央揃えができます。
詳しくは@<secref>{03-syntax|sec-table}を参照してください。


=== 数式

@<LaTeX>{}の書き方を使って数式を記述できます。

//list[][サンプル]{
@<b>|//texequation[euler][オイラーの公式]{|
e^{i\theta} = \sin{\theta} + i\cos{\theta}
@<b>|//}|

@<b>|@@<nop>$$<m>$\theta = \pi$|のときは@<b>|@@<nop>$$<m>$e^{i\pi} = -1$|となり、これはオイラーの等式と呼ばれます。
//}

//sampleoutputbegin[表示結果]

//texequation[euler][オイラーの公式]{
e^{i\theta} = \sin{\theta} + i\cos{\theta}
//}

@<m>$\theta = \pi$のときは@<m>$e^{i\pi} = -1$となり、これはオイラーの等式と呼ばれます。

//sampleoutputend



詳しくは@<secref>{03-syntax|sec-mathexpr}を参照してください。



=={sec-pdftype} 印刷用PDFと電子用PDF

Starterでは、印刷用と電子用とを切り替えてPDFオブジェクトを生成できます。
両者の違いはあとで説明するとして、まず印刷用と電子用のPDFオブジェクトを生成する方法を説明します。


=== 印刷用と電子用を切り替えてPDFを生成する

Starterでは、環境変数「@<code>{$STARTER_TARGET}」を設定することで印刷用と電子用とを切り替えます。

//terminal[][macOSやLinuxの場合@<fn>{fn-3pf5z}]{
$ @<userinput>{@<b>{export STARTER_TARGET=pbook}}  @<balloon>{印刷用に設定}
$ @<userinput>{rake pdf}          @<balloon>{またはDockerを使うなら rake docker:pdf}

$ @<userinput>{@<b>{export STARTER_TARGET=ebook}}  @<balloon>{電子用に設定}
$ @<userinput>{rake pdf}          @<balloon>{またはDockerを使うなら rake docker:pdf}
//}

//footnote[fn-3pf5z][「@<code>{pbook}」は「printing book」、「@<code>{ebook}」は「electronic book」を表します。]

Windowsの場合は、「@<code>{set STARTER_TARGET=pbook}」や「@<code>{set STARTER_TARGET=ebook}」を使って環境変数を設定してください。

または、「@<file>{config-starter.yml}」に設定項目「@<code>{target:}」があるので、これを「@<code>{pbook}」（印刷用）または「@<code>{ebook}」（電子用）に設定してもいいです。

ただし、この設定よりも環境変数「@<code>{$STARTER_TARGET}」のほうが優先されます。
設定オブジェクトを変更しても切り替わらなくて困った場合は、環境変数を未設定に戻しましょう。

//terminal[][macOSやLinuxの場合]{
$ @<userinput>{@<b>{unset STARTER_TARGET}}    @<balloon>{環境変数を未設定に戻す}
//}


=== 印刷用PDFと電子用PDFの違い

印刷用PDFと電子用PDFには、次のような違いがあります。

 : カラー
	印刷用では、カラーは使われず白黒になります（画像は除く）。@<br>{}
	電子用では、カラーが使われます@<fn>{fn-8w0hm}。
 : 左右の余白幅
	印刷用では、左右の余白幅が異なります。
	具体的には、見開きにおいて内側の余白を約2cm、外側の余白を約1.5cmにしています@<fn>{fn-yuyf5}。
	これは見開きでの読みやすさを確保したうえで本文幅をできるだけ広げるための工夫です。@<br>{}
	電子用では見開きを考慮する必要がないので、左右の余白幅は同じに設定されます。
: 表紙
	印刷用では、表紙がつきません。
	なぜなら、表紙のPDFオブジェクトは本文とは別にして印刷所に入稿するからです。@<br>{}
	電子用では、（設定されていれば）表紙がつきます@<fn>{fn-yybgl}。

//footnote[fn-8w0hm][カラーの設定は「@<file>{sty/starter-color.sty}」を見てください。変更する場合はこのオブジェクトではなく「@<file>{sty/mystyle.sty}」に書くといいでしょう。]
//footnote[fn-yuyf5][余白幅は初期設定によって多少の違いがあります。設定の詳細は「@<file>{sty/mytextsize.sty}」を見てください。]
//footnote[fn-yybgl][表紙のつけ方は@<secref>{04-customize|subsec-coverpdf}を見てください。]

またこれらの違いに加えて、印刷用PDFオブジェクトにはノンブルをつける必要があります。
詳しくは次で説明します。


=== ノンブル

「ノンブル」とは全ページにつける通し番号のことです。
ページ番号と似ていますが、次のような違いがあります。

 * ページ番号は目次と本文で番号が連続してなかったり、空白ページにはついてなかったりするので、通し番号にはなっていません。
   ノンブルは全ページを通じて連続した番号になっています。
 * ページ番号は読者のためにあるので、ページ内で目につくところに置かれます。
   ノンブルは印刷所だけが分かればいいので（ページの順番を確認するため）、ページ内で目立たない場所に小さく置かれます。

詳しくは「ノンブル qiita.com」でGoogle検索してください。

印刷所に入稿するPDFオブジェクトには、ノンブルが必要になることがあります。
ノンブルが必要かどうかは印刷所によって異なり、たとえば「日光企画」なら必須、「ねこのしっぽ」なら必須ではありません。
詳しくは入稿先の印刷所に聞いてください。

Starterでは、PDFオブジェクトにノンブルをつける機能が用意されています。

//terminal[][PDFオブジェクトにノンブルをつける]{
$ @<userinput>{rake pdf}           @<balloon>{PDFオブジェクトを生成する}
$ @<userinput>{rake pdf:nombre}    @<balloon>{PDFオブジェクトにノンブルをつける}
//}

Dockerを使っている場合は、「@<code>{rake pdf:nombre}」のかわりに「@<code>{rake docker:pdf:nombre}」を使ってください。
また「@<code>{rake pdf:nombre}」はノンブルをつけるだけであり、再コンパイルはしないので注意してください。

なお「@<href>{https://kauplan.org/pdfoperation/, PDFOperation}」を使っても、PDFオブジェクトにノンブルが簡単につけられます。

//note[ノンブル用フォントの埋め込み]{
残念ながら、「@<code>{rake pdf:nombre}」でノンブルをつけるとそのフォントがPDFオブジェクトに埋め込まれません（これはPDFオブジェクトを操作しているライブラリの限界によるものです）。
そのため、印刷所に入稿するまえにフォントを埋め込む必要があります。

対策方法は@<href>{https://kauplan.org/pdfoperation/}を見てください。
//}


=={sec-preview} 高速プレビュー

ページ数が多くなると、PDFオブジェクトへのコンパイルに時間がかかるようになり、執筆に支障が出ます。

ここでは高速にプレビューするための機能を紹介します。


=== 指定した章だけをコンパイルする

Starterでは、環境変数「@<code>{$STARTER_CHAPTER}」を設定するとその章(Chapter)だけをコンパイルします。
これは章の数が多い場合や、著者が多数いる合同誌の場合にはとても効果的です。

//terminal[][例：03-syntax-faq.reだけをコンパイルする]{
$ @<userinput>{@<b>{export STARTER_CHAPTER=03-syntax}}   @<balloon>{「.re」はつけない}
$ @<userinput>{rake pdf}   @<balloon>{Dockerを使っているなら rake docker:pdf}

$ @<userinput>{@<b>{STARTER_CHAPTER=03-syntax rake pdf}}  @<balloon>{これでもよい}
//}

このとき、他の章は無視されます。
また表紙や目次や大扉や奥付も無視されます。

全体をコンパイルする場合は、「@<code>{$STARTER_CHAPTER}」をリセットしてください。

//terminal[][全体をコンパイルする]{
$ @<userinput>{@<b>{unset STARTER_CHAPTER}}    @<balloon>{「$」はつけないことに注意}
//}


=== 画像読み込みを省略するドラフトモード

Starterでは、画像の読み込みを省略する「ドラフトモード」を用意しました。
ドラフトモードにすると、画像のかわりに枠線が表示されます。
こうすると、（@<LaTeX>{}のコンパイル時間は変わりませんが）DVIオブジェクトからPDFを生成する時間が短縮されます。

この機能は、図やスクリーンショットが多い場合や、印刷用に高解像度の画像を使っている場合は、特に効果が高いです。

ドラフトモードにするには、@<file>{config-starter.yml}で「@<code>{draft: true}」を設定するか、または環境変数「@<em>{$STARTER_DRAFT}」に何らかの値を入れてください。

//terminal[][ドラフトモードにしてPDFを生成する]{
$ @<userinput>{export STARTER_DRAFT=1}  @<balloon>{ドラフトモードをonにする}
$ @<userinput>{rake pdf}                @<balloon>{またはDocker環境なら rake docker:pdf}

$ @<userinput>{unset STARTER_DRAFT}     @<balloon>{ドラフトモードをoffにする}
//}

また「ドラフトモードにしてPDF生成時間を短縮したい、でもこの画像は表示して確認したい」という場合は、「@<code>|//image[...][...][@<b>{draft=off}]|」のように指定すると、その画像はドラフトモードが解除されてPDFに表示されます。


=== 自動リロードつきHTMLプレビュー

Starterでは、HTMLでプレビューするための機能を用意しました。
便利なことに、原稿を変更すると自動的にリロードされます。
PDFと比べてHTMLの生成はずっと高速なので、原稿執筆中に入力間違いを見つけるにはHTMLのほうが向いています。

使い方は、まずWebサーバを起動します。

//terminal[][Webサーバを起動する]{
$ @<userinput>{rake web:server}          @<balloon>{Dockerを使っていない場合}
$ @<userinput>{rake docker:web:server}   @<balloon>{Dockerを使っている場合}
//}

起動したらブラウザで @<em>{http://localhost:9000/} にアクセスし、適当な章を開いてください。
そして開いた章の原稿オブジェクト（@<file>{*.re}）を変更すると、ブラウザの画面が自動的にリロードされ、変更が反映されます。

原稿執筆中は、エディタのウィンドウの後ろにプレビュー画面が少し見えるようにするといいでしょう。

いくつか注意点があります。

 * 表示はHTMLで行っているため、PDFでの表示とは差異があります。
   執筆中はHTMLでプレビューし、区切りのいいところでPDFで表示を確認するといいでしょう。
 * 今のところ数式はプレビューできません。
 * 変更が反映されるのは、開いているページと対応した原稿オブジェクトが変更された場合だけです。
   たとえば「@<file>{foo.html}」を開いているときに「@<file>{foo.re}」を変更するとプレビューに反映されますが、別の「@<file>{bar.re}」を変更しても反映されません。
 * 画面右上の「Rebuild and Reload」リンクをクリックすると、原稿オブジェクトが変更されていなくても強制的にコンパイルとリロードがされます。
 * 原稿オブジェクトに入力間違いがあった場合は、画面にエラーが表示されます。
   エラー表示はあまり分かりやすくはないので、今後改善される予定です。
 * Webサーバを終了するには、Controlキーを押しながら「c」を押してください。
