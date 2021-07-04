= 2次元解析

//abstract{
@<em>{GetFEM}を使用して2次元の片持ちはりの剛性方程式を解きます。
初めての人は、先に前の章を見たほうがいいでしょう。
//}

#@#//makechaptitlepage[toc=on]


== 集中荷重を受けるはりのたわみ解析

本節では図に示すような、一端が壁に固定されたはりの先端に荷重@<m>$F=1.0N/mm$を載荷する問題を考えます。

//image[diagram][モデル図][scale=1.0]

=={sec-comment} モデルの作成


=== インポートする

まずは、ライブラリをインポートします。

//list[][モジュールインポート][lang=python]{
import getfem as gf
import numpy as np
import pyvista as pv
pv.start_xvfb() # ローカルのPythonで実行する場合は省略してください。
//}


=== @<em>{Mesh}オブジェクトを作成する

まずは、@<em>{Mesh}オブジェクトを設定します。
今回は四角形の規則的なメッシュを作成するためにコンストラクタで@<em>{"cartesian"}コマンドを使用します。
このコマンドは@<em>{x}座標と@<m>{y}座標の座標値を指定するとその点に節点をもつメッシュを作成します。
今回は@<em>{x}方向に長さ@<m>{10000.0}mmを4分割したメッシュを作成し、@<em>{y}方向に長さ@<m>{1000.0}mmを2分割したメッシュを作成します。
@<em>{numpy}を使うと次のように定義できます。

//list[][メッシュの作成][lang=python]{
b = 1.0 # mm
L = 10000.0 # mm
h = 1000.0 # mm
X = np.linspace(0.0, L, 4 + 1)
Y = np.linspace(0.0, 1000.0, 2 + 1)
mesh = gf.Mesh("cartesian", X, Y)
mesh.export_to_vtk("mesh.vtk", "ascii")
m = pv.read("mesh.vtk")
m.plot(show_edges="True", cpos="xy")
//}

//image[mesh][メッシュの作成][scale=1.0]

//note[より複雑な@<em>{Mesh}オブジェクトの作成]{
@<em>{GetFEM}内のメッシャーは実験的なもののみであるため。
//}

今回も左端と右端に領域を設定しておきます。
前節と同様に@<em>{outer_faces_with_direction}メソッドを使用して領域を設定します。
//list[][領域の設定][lang=python]{
fb1 = mesh.outer_faces_with_direction([-1.0, 0.0], 0.01)
fb2 = mesh.outer_faces_with_direction([1.0, 0.0], 0.01)
LEFT = 1
RIGHT = 2
mesh.set_region(LEFT, fb1)
mesh.set_region(RIGHT, fb2)
//}

=== @<em>{MeshFem}オブジェクトを作成する

@<em>{MeshFem}オブジェクトを作成します。
2次元有限要素法を定義する場合は@<em>{FEM_PRODUCT}を使用して1次元の有限要素法から2次元の有限要素法を作成します。

//list[][@<em>{MeshFem}オブジェクトの作成][lang=python]{
mfu = gf.MeshFem(mesh, 2)
elements_degree = 2
mfu.set_classical_fem(elements_degree)
//}

=== @<em>{MeshIm}オブジェクトを作成する

2次要素の場合、ガウス積分点は3を使用することが多いです。
@<em>{IM_GAUSS1D(K)}は、@<m>$K/2+1$点の積分点を表します。
2次元の積分法を定義するには@<em>{IM_PRODUCT}を使用して1次元の積分法から2次元の積分法を作成します。

//list[][@<em>{MeshIm}オブジェクトの作成][lang=python]{
im = gf.Integ("IM_PRODUCT(IM_GAUSS1D(4), IM_GAUSS1D(4))")
mim = gf.MeshIm(mesh, im)
//}

=== @<em>{Model}オブジェクトを作成する

@<em>{Model}オブジェクトを作成します。
今回は
@<href>{https://getfem.readthedocs.io/ja/latest/userdoc/model_plasticity_small_strain.html#plane-strain-approximation, 平面ひずみ近似}
で定式化をするため、@<em>{add_isotropic_linearized_elasticity_brick_pstrain}メソッドを使用します。
パラメータであるヤング率(@<m>$E=205000.0N/mm^2$)とポアソン比(@<m>$\nu=0.0$)は@<em>{add_initialized_data}メソッドを使用してデータとして定義します。
//list[][@<em>{Model}オブジェクトの作成][lang=python]{
E = 205000.0 # N/mm2
nu = 0.0
md = gf.Model("real")
md.add_fem_variable("u", mfu)
md.add_initialized_data("E", E)
md.add_initialized_data("nu", nu)
md.add_isotropic_linearized_elasticity_brick_pstrain(
    mim, "u", "E", "nu"
)
//}

//output[][出力結果]{
0
//}

次に、Dirichlet条件(固定条件)を追加します。
2次元のため行列とベクトルの次元が2であることに注意してください。

//list[][Dirichlet条件(固定条件)の追加][lang=python]{
md.add_initialized_data("H", [[1.0, 0.0], [0.0, 1.0]])
md.add_initialized_data("r", [0.0, 0.0])
md.add_generalized_Dirichlet_condition_with_multipliers(
    mim, "u", mfu, LEFT, "r", "H"
)
//}

//output[][出力結果]{
1
//}

最後にソース項(荷重条件)を追加します。
2次元のためベクトルの次元が2であることに注意してください。

//list[][ソース項(荷重条件)の追加][lang=python]{
F = -1.0 # N/mm2
md.add_initialized_data("F", [0, F])
md.add_source_term_brick(mim, "u", "F", RIGHT)
//}

//output[][出力結果]{
Trace 2 in getfem_models.cc, line 4387: Mass term assembly for Dirichlet condition
Trace 2 in getfem_models.cc, line 4424: Source term assembly for Dirichlet condition
2
//}


=== 求解

以上で、モデルが定義できましたので@<em>{solve}メソッドで解を求め、結果を保存します。

//list[][求解][lang=python]{
md.solve()
//}

//output[][出力結果]{
Trace 2 in getfem_models.cc, line 3464: Linearized isotropic elasticity: generic matrix assembly
Trace 2 in getfem_models.cc, line 4387: Mass term assembly for Dirichlet condition
Trace 2 in getfem_models.cc, line 4424: Source term assembly for Dirichlet condition
Trace 2 in getfem_models.cc, line 3300: Generic source term assembly
Trace 2 in getfem_models.cc, line 3307: Source term: generic source term assembly
(0, 1)
//}

//list[][VTKへの出力][lang=python]{
U = md.variable("u")
mfu.export_to_vtk("mfu.vtk", "ascii", mfu, U, "U")
//}

== 結果の可視化

@<em>{PyVista}を使い結果を表示します。

//list[][結果の可視化][lang=python]{
m = pv.read("mfu.vtk")
m.plot(cpos="xy")
//}

//image[mfu][変位コンター図][scale=1.0]

コンター図から分かるように固定端である左端は0.0であり荷重を加えた右端の変位が最大になっていることが分かります。
コンターのみでは変位の様子が分からないため
@<href>{https://tkoyama010.github.io/pyvista-docs-dev-ja/core/filters.html#pyvista.DataSetFilters.warp_by_vector, @<em>{warp_by_vector}}
メソッドで変位ベクトルでメッシュをワープさせてみます。
//list[][変位によるワープ][lang=python]{
w = m.warp_by_vector("U", factor=100.0)
w.plot(cpos="xy")
//}

//image[mfu2][変形図(変位は100倍)][scale=1.0]

片持はりの自由端に荷重が与えられた際の変形の様子を確認することができます。

== 検証

先端集中荷重を受けるはりのたわみの公式は @<eq>{beam} で与えられます。

//texequation[beam][先端集中荷重を受けるはりのたわみの公式]{
u = \dfrac{Px^3}{3EI}
//}

ここで、 @<m>$P$ は片持梁先端に与えられている荷重、 @<m>$x$ は自由端位置を @<m>$0$ とした場合のたわみの計算位置を表します。

解析結果は@<em>{PyVista}のサンプリング機能を使うことによりデータを取得します。
はり要素の上部の変位をプロットしてみます。

//list[][変位のサンプリング][lang=python]{
import matplotlib.pyplot as plt
fig = plt.figure()
ax = fig.add_subplot(111)
sampled = m.sample_over_line([0, h, 0], [L, h, 0], 4)
U = sampled["U"][:, 1]
ax.plot(X, U, label="GetFEM")
//}

@<em>{matplotlib}を使用して公式と比較したものを@<img>{U}に示します。
有限要素法の解と公式の解が一致していることが分かります。

//list[][公式の描画][lang=python]{
I = b*h**3/12.0
P = F*(b*h)
u = -(P/(E*I))*(X**3/6.0-L*X**2/2.0)
ax.plot(X, u, label="Theory", linestyle="dashed")
ax.legend()
plt.show()
//}

//image[U][変位の比較][scale=1.0]
