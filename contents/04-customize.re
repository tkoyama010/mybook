= 3次元はりの固有値解析

//abstract{
@<em>{GetFEM}を使用して3次元の片持ちはりの固有値解析を行います。
初めての人は、先に@<secref>{02-tutorial|sec-basicsyntax}を見たほうがいいでしょう。
//}

#@#//makechaptitlepage[toc=on]



== モデルの作成


=== インポートする

準備ができたら、ライブラリをインポートします。

//list[][モジュールインポート][lang=python]{
>>> import getfem as gf
>>> import numpy as np
>>> import pyvista as pv
//}

=== @<em>{Mesh}オブジェクトを作成する

まずは、@<em>{Mesh}オブジェクトを設定します。
3次元の六面体の規則的なメッシュは2次元の場合と同様にコンストラクタで@<em>{"cartesian"}コマンドを使用します。
このコマンドは@<em>{x}座標と@<m>{y}座標および@<m>{z}座標の座標値を指定するとその点に節点をもつメッシュを作成します。
今回は@<em>{x}方向に長さ@<m>{100.0}mmを4分割したメッシュを作成し、@<em>{y}方向に長さ@<m>{100.0}mmを4分割、@<em>{z}方向に長さ@<m>{1000.0}mmを36分割したメッシュを作成します。
@<em>{numpy}を使うと次のように定義できます。

//list[][メッシュの作成][lang=python]{
>>> X = np.linspace(0.0, 100.0, 4 + 1)
>>> Y = np.linspace(0.0, 100.0, 4 + 1)
>>> Z = np.linspace(0.0, 1000.0, 36 + 1)
>>> mesh = gf.Mesh("cartesian", X, Y, Z)
>>> mesh.export_to_vtk("mesh.vtk", "ascii")
>>> m = pv.read("mesh.vtk")
>>> m.plot(show_edges="True")
//}

//image[mesh][メッシュの作成][scale=1.0]

今回は領域は使用しないため設定は省略します。

=== @<em>{MeshFem}オブジェクトを作成する

@<em>{MeshFem}オブジェクトを作成します。

//list[][@<em>{MeshFem}オブジェクトの作成][lang=python]{
>>> elements_degree = 2
>>> mfu = gf.MeshFem(mesh, 3)
>>> mfu.set_classical_fem(elements_degree)
>>> mfd = gf.MeshFem(mesh, 1)
>>> mfu.set_classical_fem(elements_degree)
//}

=== @<em>{MeshIm}オブジェクトを作成する

2次要素の場合、ガウス積分点は3を使用することが多いです。
@<em>{IM_GAUSS1D(K)}は、@<m>$K/2+1$点の積分点を表します。
2次元の積分法を定義するには@<em>{IM_PRODUCT}を使用して1次元の積分法から2次元の積分法を作成します。

//list[][@<em>{MeshIm}オブジェクトの作成][lang=python]{
>>> mim = gf.MeshIm(mesh, "IM_HEXAHEDRON(5)")
//}

=== 質量行列と剛性行列の作成

質量行列と剛性行列を直接計算するには@<em>{asm_mass_matrix}と@<em>{asm_linear_elasticity}を使用します。

//list[][質量行列の作成][lang=python]{
//}

//list[][剛性行列の作成][lang=python]{
//}

=== 固有値と固有モードの計算

固有値と固有モードの計算には@<em>{numpy}を使用します。

//list[][@<em>{numpy}][lang=python]{
//}

//note[@<em>{numpy}と@<em>{scipy}の固有値と固有モードの計算について]{
//}

== 解のエクスポート

以上で固有値解析が解けました。
次に、解いた解を可視化する必要があります。
固有ベクトルを配列としてVTKに出力します。

//list[][解のエクスポート][lang=python]{
//}

出力後、同じディレクトリにファイル@<em>{"mfu.vtk"}が出力されます。

== 結果の可視化

@<em>{PyVista}を使い結果を表示します。

//list[][結果の可視化][lang=python]{
//}

固有値から分かるように6次の固有値までは剛体モードを表します。

== 検証

固有振動数を求める公式は下記の通りです。
剛性を計算すると。
となります。
求めた固有振動数ははりの固有振動数と一致していることが分かります。
