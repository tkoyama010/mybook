= 1次元ばね要素解析

//abstract{
前の章では、実行環境の起動方法を説明しました。
この章では、自分で書いたスクリプトファイルで解析を実行する方法を説明します。
なおこの章は、前の章の起動ができたことを前提としています。
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
本書では弱定式言語については扱わず、既に実装されている定型的なブリック(brick)のみを対象とします。

== @<em>{GetFEM}の実行手順

@<em>{GetFEM}を実行する際の手順について説明します。

=== インポートする

まずは@<em>{GetFEM}を使用できるようにモジュールをインポートします。
@<em>{GetFEM}では@<em>{numpy}もよく使われるのでインポートしておきます。
慣例として@<em>{GetFEM}は@<em>{gf}として、@<em>{Numpy}は@<em>{np}としてインポートされるので覚えておきましょう。

//list[][モジュールインポート][lang=python]{
import getfem as gf
import numpy as np
//}

=== @<em>{Mesh}オブジェクトを作成する

まずは、@<em>{Mesh}オブジェクトを設定します。
今回は長さ10.0の1次元のメッシュを作成します。
規則的なメッシュはコンストラクタで@<em>{"cartesian"}コマンドを使用することで作成できます。

//list[][Meshオブジェクトの作成][lang=python]{
X = np.array([0.0, 10.0])
mesh = gf.Mesh("cartesian", X)
print(mesh)

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

//output[][「@<code>|//output|」での表示結果（崩れない）]{
 id  |   name   | height | gender
-----+----------+--------+--------
 101 | エレン   |    170 | M
 102 | ミカサ   |    170 | F
 103 | アルミン |    163 | M
(3 rows)
//}

出力結果からインデックス0の点が0.0にあり、インデックス1の点が10.0の位置にあることが分かります。
また、点0と点1に接続された凸(convex)が定義されていることが分かります。

=== @<em>{Mesh}オブジェクトの面を取得する

@<em>{Mesh}には@<em>{Model}オブジェクトのブリック(brick)を定義するために領域を定義する必要があります。
今回は左端と右端に領域を定義しておきます。
領域の定義には面番号が必要となるので取得します。
面を取得するためには@<em>{outer_faces_with_direction}メソッドを使用します。
このメソッドはベクトルとベクトルからの角度(rad)を指定してその範囲のベクトルを法線とする面を取得します。

//list[][左端の面の取得][lang=python]{
fb1 = mesh.outer_faces_with_direction([-1.0], 0.01)
print(fb1)

[[0]
 [1]]

//}

出力を見ると面番号は2行の配列で構成されています。
1行目は凸(convex)のIDを、2行目は凸(convex)内の面番号を表しています。
つまり、上の出力はIDが0の凸(convex)の面番号1を指定していることが分かります。
左端と同様に右端の面番号も取得します。

//list[][右端の面の取得][lang=python]{
fb2 = mesh.outer_faces_with_direction([1.0], 0.01)
print(fb2)

[[0]
 [0]]

//}

領域を指定して領域を定義することも可能です。
@<em>{outer_faces_in_box}は2つの点で定義されるBOX内にある外面を取得します。
@<m>$x = -1.0$から@<m>$x = 11.0$の範囲にある点を出力してみます。

//list[][BOX内の面の取得][lang=python]{
print(mesh.outer_faces_in_box([-1.0], [11.0]))

[[0 0]
 [0 1]]


//}

この範囲にはメッシュの全ての面(2個)が含まれています。
この面は使用しないため変数には保存しません。

=== @<em>{Mesh}オブジェクトの領域を設定する

取得した面をもとに領域を設定します。
@<em>{set_region}メソッドを使用して領域を設定します。
領域IDを設定する必要があるため変数@<em>{LEFT}と@<em>{RIGHT}に番号を定義しておきます。
//list[][領域の設定][lang=python]{
LEFT = 1
RIGHT = 2
mesh.set_region(LEFT, fb1)
mesh.set_region(RIGHT, fb2)
print(mesh)

BEGIN POINTS LIST

  POINT COUNT 2
  POINT  0  0
  POINT  1  10

END POINTS LIST



BEGIN MESH STRUCTURE DESCRIPTION

CONVEX COUNT 1
CONVEX 0    'GT_PK(1,1)'      0  1

END MESH STRUCTURE DESCRIPTION
BEGIN REGION 1
0/1
END REGION 1
BEGIN REGION 2
0/0
END REGION 2

//}

@<em>{Mesh}オブジェクトに領域(@<em>{REGION})が追加されていることが分かります。
「凸(convex)/面番号」の表記となっていることに注意してください。


=== @<em>{MeshFem}オブジェクトを作成する

@<em>{MeshFem}オブジェクトを作成します。
まずは@<href>{https://getfem.readthedocs.io/ja/latest/python/cmdref_MeshFem.html, 公式サイト}に載っている@<em>{MeshFem}オブジェクトの定義から見ていきましょう。
公式サイトには「このオブジェクトは，メッシュ全体で定義された有限要素法を表します．」と書かれています。
@<em>{GetFEM}の特徴で説明した通りこのライブラリは@<em>{Mesh}オブジェクトと@<em>{Fem}(有限要素法)オブジェクトが完全に分離されています。
そのため、2つのオブジェクトの対応関係を管理するオブジェクトが必要になります。
それがこのオブジェクトです。

@<em>{Mesh}オブジェクトを元に@<em>{MeshFem}オブジェクトを作っていきます。
作成の際には、@<em>{Mesh}オブジェクトと求めたい物理量の次元を与えます。
スカラーの場合は1、ベクトルの場合はそのベクトルの自由度(1, 2, 3...)を与えます。
今回は1自由度のベクトルを定義します。

//list[][@<em>{MeshFem}オブジェクトの作成][lang=python]{
mfu = gf.MeshFem(mesh, 1)
print(mfu)
BEGIN MESH_FEM

QDIM 1
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
f = gf.Fem("FEM_PK(1,1)")
mfu.set_fem(f)
print(mfu)

BEGIN MESH_FEM

QDIM 1
CONVEX 0 'FEM_PK(1,1)'
BEGIN DOF_ENUMERATION
 0:  0 1
 END DOF_ENUMERATION
END MESH_FEM

//}

//note[有限要素法の種類について]{

通常の解析では単体の古典的Lagrange要素を使用します。

//list[][@<em>{MeshFem}オブジェクトへの単体の古典的Lagrange要素の設定][lang=python]{
mfu.set_fem(f)
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
im = gf.Integ("IM_GAUSS1D(1)")
mim = gf.MeshIm(mesh, im)
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
md = gf.Model("real")
//}

===== (2) 変数を定義する。

変数を定義します。
問題が解かれた際にはこの変数に解が出力されます。
変数は@<em>{MeshFem}オブジェクトにリンクして定義します。
メソッドは@<em>{add_fem_variable}を使用します。

//list[][変数の定義][lang=python]{
md.add_fem_variable("u", mfu)
//}

===== (3) ブリック(brick)を定義する。

ブリック(brick)を定義します。
今回は図XXXXに示すモデルを作成します。

このモデルを作成するために必要なブリックは以下の通りです。

 * 等方性線形弾性項
 * Dirichlet条件(固定条件)
 * ソース項(荷重条件)

まずは、楕円項を追加します。

//list[][楕円項の追加][lang=python]{
k = 2.0
md.add_initialized_data("k", [k])
md.add_generic_elliptic_brick(mim, "u", "k")
0
//}

出力された0はブリック(brick)のインデックスを表しています。

次に、Dirichlet条件(固定条件)を追加します。
Dirichlet条件は以下の式で表されます。
//texequation[dirichlet][Dirichlet条件]{
Hu = r
//}
ここで、@<m>$H$は(自由度)を次元とした行列、@<m>$u$と@<m>$r$はそれぞれ(自由度)を次元としたベクトルです。
@<m>$u$は先程定義した変数を表しています。
今回は左端が固定条件となっているため@<m>$u = {0}$を設定します。
ゆえに、@<m>$H = [[0]]$および@<m>$r = \{0\}$となります。
@<m>$H$と@<m>$r$は@<em>{add_initialized_data}メソッドを使用して固定サイズデータとして定義します。
それぞれ@<em>{"H"}と@<em>{"r"}と名前をつけておき、@<em>{add_generalized_Dirichlet_condition_with_multipliers}メソッドでDirichlet条件を定義します。
引数には@<em>{MeshIm}オブジェクトと@<em>{MeshFem}オブジェクトも必要です。

//list[][Dirichlet条件(固定条件)の追加][lang=python]{
md.add_initialized_data("H", [[1.0]])
md.add_initialized_data("r", [0.0])
md.add_generalized_Dirichlet_condition_with_multipliers(
...     mim, "u", mfu, LEFT, "r", "H"
... )
1
//}

出力された1はブリック(brick)のインデックスを表しています。

最後にソース項(荷重条件)を追加します。
使用するメソッドは@<em>{add_source_term_brick}メソッドを使用して荷重条件を追加します。
荷重条件をベクトル@<m>$F = \{1.0\}$で設定します。

//list[][ソース項(荷重条件)の追加][lang=python]{
F = mfu.eval("1.0")
md.add_initialized_fem_data("F", mfu, F)
md.add_source_term_brick(mim, "u", "F", RIGHT)
Trace 2 in getfem_models.cc, line 4387: Mass term assembly for Dirichlet condition
Trace 2 in getfem_models.cc, line 4424: Source term assembly for Dirichlet condition
2
//}

ソース項はアセンブリングが必要となるためその情報が出力されます。
2はブリック(brick)のインデックスを表しています。


=== 求解

モデルを定義したら、@<em>{solve}メソッドを使用して解くことができます。
//list[][求解][lang=python]{
md.solve()
Trace 2 in getfem_models.cc, line 3464: Generic elliptic: generic matrix assembly
Trace 2 in getfem_models.cc, line 4387: Mass term assembly for Dirichlet condition
Trace 2 in getfem_models.cc, line 4424: Source term assembly for Dirichlet condition
Trace 2 in getfem_models.cc, line 3300: Generic source term assembly
Trace 2 in getfem_models.cc, line 3307: Source term: generic source term assembly
(0, 1)
//}
各項がアセンブリングされている旨のメッセージが出力されます。@<em>{(0, 1)}はXXXXX。

==={subsec-compileerror} @<em>{solve}がエラーになったら

XXXXエラーになったら、以下の点を確認してください。

 * AAAAA
 * BBBBB
 * CCCCC

詳しくはXXXXXを見てください。

== 解のエクスポート

以上で有限要素法が解けました。
次に、解いた解を可視化する必要があります。
@<em>{GetFEM}には可視化機能はないため、外部の可視化ライブラリを使用する必要があります。
今回は@<em>{PyVista}を使用するため@<em>{VTK}ファイルをエクスポートします。
変数@<em>{u}の値は@<em>{variable}メソッドを使用して出力します。

//list[][解のエクスポート][lang=python]{
U = md.variable("u")
print(U)
[0. 5.]
mfu.export_to_vtk("mfu.vtk", "ascii", mfu, U, "U")
//}

出力後、同じディレクトリにファイル@<em>{"mfu.vtk"}が出力されます。
//list[][VTKファイルの内容][]{
# vtk DataFile Version 2.0
Exported by GetFEM
ASCII
DATASET UNSTRUCTURED_GRID
POINTS 2 float
 0 0 0
 10 0 0

CELLS 1 3
 2 0 1

CELL_TYPES 1
 3

POINT_DATA 2


SCALARS U float 1
LOOKUP_TABLE default
 0 5
//}
VTKの詳しいフォーマットについてはXXXXを参照してください。
@<m>$x = 10.0$で@<m>$U=5.0$の値となっていることが確認できます。

=={sec-basicsyntax} @<em>{PyVista}の特徴

@<em>{PyVista}は@<em>{numpy}や@<em>{matplotlib}のようなインターフェースを持った3次元データ可視化ライブラリです。

=== インポートする

まずは@<em>{PyVista}を使用できるようにモジュールをインポートします。
慣例として@<em>{PyVista}は@<em>{pv}としてインポートされるので覚えておきましょう。
//list[][モジュールインポート][lang=python]{
import pyvista as pv
pv.start_xvfb()
//}

=== ファイルを読み込む

ファイルを読み込むには@<em>{pyvista.read}を使用します。
//list[][@<em>{PyVista}によるファイルの読み込み][lang=python]{
m = pv.read("mfu.vtk")
print(m)
UnstructuredGrid (0x7fafd288f400)
  N Cells:      1
  N Points:     2
  X Bounds:     0.000e+00, 1.000e+01
  Y Bounds:     0.000e+00, 0.000e+00
  Z Bounds:     0.000e+00, 0.000e+00
  N Arrays:     1
//}
@<em>{m}に2ポイントを持つセルが1個の非構造格子が設定されたことが分かります。
また、前節で保存したデータ配列"U"が含まれることも確認できます。
@<em>{0x7fafd288f400}の部分は実行するたびに異なります。

=== 結果の表示

@<em>{plot}メソッドを使用することにより結果を可視化することができます。

//list[][可視化][lang=python]{
m.plot()
//}

先程計算した1次元のメッシュの結果が可視化されていることが分かります。
@<m>$x = 10.0$で@<m>$U=5.0$の値となっていることを確認してください。

//image[mfu][解析結果の表示][scale=1.0]

1次元モデルを使用して@<em>{GetFEM}と@<em>{PyVista}の基礎的な使い方を習得できました。
次の節では2次元モデルの解析をしてみましょう。
