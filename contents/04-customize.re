= 3次元解析

//abstract{
@<em>{GetFEM}を使用して3次元はりの固有値解析を行います。
初めての人は、先に@<secref>{02-tutorial|sec-basicsyntax}を見たほうがいいでしょう。
//}

#@#//makechaptitlepage[toc=on]


== はりの固有値解析

固有値解析の対象のモデルはモデル図に示すような3次元両端自由端のはりを対象とします。
//image[diagram][モデル図][scale=1.0]

== モデルの作成


=== インポートする

まずは、ライブラリをインポートします。

//list[][モジュールインポート][lang=python]{
import getfem as gf
import numpy as np
import pyvista as pv
pv.start_xvfb()
//}

=== @<em>{Mesh}オブジェクトを作成する

まずは、@<em>{Mesh}オブジェクトを設定します。
3次元の六面体の規則的なメッシュは2次元の場合と同様にコンストラクタで@<em>{"cartesian"}コマンドを使用します。
このコマンドは@<em>{x}座標と@<m>{y}座標および@<m>{z}座標の座標値を指定するとその点に節点をもつメッシュを作成します。
今回は@<em>{x}方向に長さ@<m>{100.0}mmを4分割したメッシュを作成し、@<em>{y}方向に長さ@<m>{100.0}mmを4分割、@<em>{z}方向に長さ@<m>{1000.0}mmを36分割したメッシュを作成します。
@<em>{numpy}を使うと次のように定義できます。

//list[][メッシュの作成][lang=python]{
X = np.linspace(0.0, 100.0, 2 + 1)
Y = np.linspace(0.0, 100.0, 2 + 1)
Z = np.linspace(0.0, 1000.0, 20 + 1)
mesh = gf.Mesh("cartesian", X, Y, Z)
mesh.export_to_vtk("mesh.vtk", "ascii")
m = pv.read("mesh.vtk")
m.plot(show_edges="True")
//}

//image[mesh][メッシュの作成][scale=1.0]

今回は領域は使用しないため設定は省略します。

=== @<em>{MeshFem}オブジェクトを作成する

@<em>{MeshFem}オブジェクトを作成します。

//list[][@<em>{MeshFem}オブジェクトの作成][lang=python]{
elements_degree = 2
mfu = gf.MeshFem(mesh, 3)
mfu.set_classical_fem(elements_degree)
mfd = gf.MeshFem(mesh, 1)
mfd.set_classical_fem(elements_degree)
//}

=== @<em>{MeshIm}オブジェクトを作成する

2次要素の場合、ガウス積分点は3を使用することが多いです。
@<em>{IM_GAUSS1D(K)}は、@<m>$K/2+1$点の積分点を表します。
2次元の積分法を定義するには@<em>{IM_PRODUCT}を使用して1次元の積分法から2次元の積分法を作成します。

//list[][@<em>{MeshIm}オブジェクトの作成][lang=python]{
mim = gf.MeshIm(mesh, elements_degree*2)
//}

=== 質量行列と剛性行列の作成

質量行列と剛性行列を直接計算するには@<em>{asm_mass_matrix}と@<em>{asm_linear_elasticity}を使用します。

//list[][質量行列の作成][lang=python]{
M = gf.asm_mass_matrix(mim, mfu)
//}

//list[][剛性行列の作成][lang=python]{
E = 205000.0 # N/mm2
Nu = 0.0
Lambda = E*Nu/((1+Nu)*(1-2*Nu))
Mu =E/(2*(1+Nu))
K = gf.asm_linear_elasticity(
    mim, mfu, mfd, np.repeat([Lambda], mfd.nbdof()), np.repeat(Mu, mfd.nbdof())
)
//}

=== 固有値と固有モードの計算

固有値と固有モードの計算には@<em>{numpy}を使用します。
固有値解析の計算には時間がかかりますので根気強く待ってください。


//list[][固有値と固有モードの計算][lang=python]{
omega2, v = np.linalg.eig(np.linalg.inv(M.full()) @ K.full())
//}

計算した固有値を@<em>{numpy.sort}でソートします。
//list[][固有値と固有ベクトルのソート][lang=python]{
omega2_sort = np.sort(omega2)
sort_index = np.argsort(omega2)
//}
求めた固有値を表示します。
表示から分かるように、モード 1-6 は固有値がほぼ @<m>$0.0$ 、すなわち固有角振動数の2乘が @<m>$0.0$ となっており、
剛体モードが計算されていることが分かります。
//list[][固有値の確認][lang=python]{
omega2_sort = np.sort(omega2)
print(omega2_sort[:10])
//}

//output[][出力結果]{
[-2.96792735e-12+0.j  0.00000000e+00+0.j  1.67761410e-12+0.j
  3.30174611e-12+0.j  7.96734349e-12+0.j  1.05152296e-11+0.j
  8.03242476e-02+0.j  8.03242476e-02+0.j  5.52361001e-01+0.j
  5.52361001e-01+0.j]
//}

1次固有振動数は以下の通りになります。
//list[][1次固有振動数の確認][lang=python]{
f1 = np.sqrt(omega2_sort[6])/(2.0*np.pi)
print(f1.real, "Hz")
//}

//output[][出力結果]{
0.04510695023261866 Hz
//}

(TODO) @<em>{numpy}と@<em>{scipy}の固有値と固有モードの計算について

== 解のエクスポート

以上で固有値解析が解けました。
次に、解いた解を可視化する必要があります。
固有ベクトルを配列としてVTKに表示します。

7次の固有モードを表示しプロットします。
//list[][固有ベクトルの表示][lang=python]{
U = v[:, sort_index[6]].real
mfu.export_to_vtk("mfu.vtk", "ascii", mfu, U, "U")
m = pv.read("mfu.vtk")
m.plot()
//}
//image[mfu][モード図][scale=1.0]

表示後、同じディレクトリにファイル@<em>{"mfu.vtk"}が表示されます。

== 結果の可視化

@<em>{PyVista}を使い結果を表示します。

//list[][モードによるワープ][lang=python]{
w = m.warp_by_vector("U", factor=1000.0)
w.plot()
//}

//image[mfu2][モード図][scale=1.0]


== 検証

両端自由のはりの固有振動数@<m>$f$は次の式で計算できます。
固有振動数を求める式は下記の通りです。
//texequation[dirichlet][両端自由のはりの固有振動数]{
f=\dfrac{1}{2\pi}\dfrac{\lambda^{2}}{l^{2}}\sqrt{\dfrac{EI}{\rho A}}
//}
となります。
ただし、1次の場合は @<m>$\lambda = 4.730$ です。
この式を使用して今回の振動数との比較をしてみましょう。
//list[][固有ベクトルの表示][lang=python]{
L = 1000.0
clambda = 4.730
rho = 1.0
A = 100.0*100.0
I = 100.0*100.0**3/12.0
f = 1.0/(2.0*np.pi)*clambda**2/L**2*np.sqrt((E*I)/(rho*A))
print("f = ", f, "Hz")
//}

//output[][出力結果]{
f =  0.046540250939535864 Hz
//}
求めた固有振動数ははりの固有振動数とほぼ一致していることが分かります。
ソリッド要素を使用してはりをモデル化しているため完全には一致しません。
