= 3次元解析

//abstract{
@<em>{GetFEM}を使用して3次元はりの固有値解析を行います。
初めての人は、先に@<chapref>{02-tutorial}を見てください。
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
pv.start_xvfb() # ローカルのPythonで実行する場合は省略してください。
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
表示から分かるように、モード 1-6 は固有値が微小値、すなわち固有角振動数の2乘が @<m>$0.0$ となっており、
剛体モードが計算されていることが分かります。
//list[][固有値の確認][lang=python]{
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

この節の固有値解析の方法は@<href>{https://watlab-blog.com/2019/06/30/multidof-vibration-eigen/, こちらのページ}を参考にさせていただきました。

== 解のエクスポート

以上で固有値解析が解けました。
次に、解いた解を可視化する必要があります。
固有ベクトルを配列としてVTKに表示します。

7次の固有モードを表示しプロットします。
//list[][固有モードの出力][lang=python]{
U = v[:, sort_index[6]].real
mfu.export_to_vtk("mfu.vtk", "ascii", mfu, U, "U")
//}

同じディレクトリにファイル@<em>{"mfu.vtk"}が表示されます。
出力したファイルを読み込み固有モードをプロットさせます。

//list[][固有モードの表示][lang=python]{
m = pv.read("mfu.vtk")
m.plot()
//}
//image[mfu][モード図][scale=1.0]

== 結果の可視化

@<em>{PyVista}を使い結果を表示します。

//list[][モードによるワープ][lang=python]{
w = m.warp_by_vector("U", factor=1000.0)
w.plot()
//}

//image[mfu2][モード図(変形後)][scale=1.0]


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
求めた固有振動数ははりの固有振動数と有効数字1桁目が一致していることが分かります。
ソリッド要素を使用してはりをモデル化しているため完全には一致しません。


== scipyによる疎行列固有値解析

@<em>{numpy} で固有値解析をしましたが、使用しているソルバーは密行列のものです。
しかし、有限要素法で扱う行列は疎行列であるため、疎行列のソルバーを使用するほうがより効率的です。
この節の固有値解析の方法は @<href>{https://mapdldocs.pyansys.com/examples/01-apdlmath-examples/mapdl_vs_scipy.html, こちらのページ} を参考にさせていただきました。

//list[][モジュールインポート][lang=python]{
from scipy import io 
from scipy.sparse.linalg import eigsh
import matplotlib.pyplot as plt
//}

@<em>{GetFEM} で @<em>{scipy} の疎行列固有値解析をするには剛性行列 @<m>{K} と 質量行列 @<m>{M} をMatrix-Market形式で出力しておきます。
先ほど作成した行列は @<em>{GetFEM} の @<em>{Spmat} オブジェクトで作成されており、Matrix-Market形式で保存するには @<em>{save} メソッドを使用します。
@<href>{https://math.nist.gov/MatrixMarket/formats.html, Matrix-Market形式} は疎行列を表現するためのテキスト形式の1つです。

//list[][Matrix-Market形式への出力][lang=python]{
K.save("mm", "K.mtx")
M.save("mm", "M.mtx")
//}

Matrix-Market形式で出力されたファイルは @<em>{scipy.io.mmread} を使用して読み込むことが可能です。
読み込み後、 @<em>{matplotlib} の @<em>{spy} メソッドを使用して疎行列の非ゼロ要素を可視化します。
剛性行列 @<m>{K} と 質量行列 @<m>{M} がいずれも疎行列であることが確認できます。

//list[][剛性行列 @<m>{K} と 質量行列 @<m>{M} の描画][lang=python]{
pk = io.mmread("K.mtx")
pm = io.mmread("M.mtx")

fig, (ax1, ax2) = plt.subplots(1, 2)
fig.suptitle("K and M Matrix profiles")
ax1.spy(pk, markersize=0.01)
ax1.set_title("K Matrix")
ax2.spy(pm, markersize=0.01)
ax2.set_title("M Matrix")
plt.show(block=True)
//}

//image[matrix][行列の描画][scale=1.0]

@<em>{scipy} の @<em>{eigsh} で疎行列の一般化固有値解析を行うことができます。
@<em>{sigma} を設定するとその付近の固有値を求めます。
@<m>{0.00} 付近の固有値を求めたいため @<m>{-0.0001} を設定します。

//list[][疎行列の固有値と固有モードの計算][lang=python]{
vals, vecs = eigsh(A=pk, M=pm, k=10, sigma=-0.0001, which="LA")
//}

固有値を確認すると @<em>{numpy} で計算した結果と一致します。

//list[][固有値の確認][lang=python]{
print(vals)
//}

//output[][出力結果]{
array([3.88741096e-14, 1.17108119e-13, 1.40025727e-13,
       2.06940002e-13, 3.00736934e-13, 3.12765317e-13,
       8.03242475e-02, 8.03242475e-02, 5.52361001e-01,
       5.52361001e-01])
//}

1次固有振動数を計算します。

//list[][1 次固有振動数の確認][lang=python]{
f1 = np.sqrt(vals[6])/(2.0*np.pi)
print(f1.real, "Hz")
//}

//output[][出力結果]{
0.045106950231340426 Hz
//}
