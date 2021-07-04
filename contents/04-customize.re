= 3次元解析

//abstract{
@<em>{GetFEM}を使用して3次元はりの固有値解析を行います。
初めての人は先に@<chapref>{02-tutorial}を見てください。
//}

#@#//makechaptitlepage[toc=on]


== はりの固有値解析

下図に示すような3次元両端自由端のはりを対象とします。
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
このコマンドは@<m>{x}座標と@<m>{y}座標および@<m>{z}座標の座標値を指定するとその点に節点をもつメッシュを作成します。
今回は@<m>{x}方向に長さ@<m>{100.0}mmを4分割したメッシュを作成し、@<m>{y}方向に長さ@<m>{100.0}mmを4分割、@<m>{z}方向に長さ@<m>{1000.0}mmを36分割したメッシュを作成します。
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

@<img>{mesh}に示すメッシュが作成されます。
今回は例えば角柱の上面や下面などの領域は使用しないため設定は省略します。

//image[mesh][メッシュの作成][scale=1.0]

=== @<em>{MeshFem}オブジェクトを作成する

@<em>{MeshFem}オブジェクトを作成します。
3次の自由度を定義し要素次数は2次とします。

//list[][@<em>{MeshFem}オブジェクトの作成][lang=python]{
elements_degree = 2
mfu = gf.MeshFem(mesh, 3)
mfu.set_classical_fem(elements_degree)
mfd = gf.MeshFem(mesh, 1)
mfd.set_classical_fem(elements_degree)
//}

=== @<em>{MeshIm}オブジェクトを作成する

@<em>{MeshIm}オブジェクトはコンストラクタを使用して作成します。
出力結果から3次元の各方向に @<em>{IM_GAUSS1D(4)} の積分法が定義されていることが分かります。

//list[][@<em>{MeshIm}オブジェクトの作成][lang=python]{
mim = gf.MeshIm(mesh, elements_degree*2)
print(mim)
//}

//output[][出力結果]{
BEGIN MESH_IM

 CONVEX 0 'IM_PRODUCT(IM_PRODUCT(IM_GAUSS1D(4),IM_GAUSS1D(4)),IM_GAUSS1D(4))'
 CONVEX 1 'IM_PRODUCT(IM_PRODUCT(IM_GAUSS1D(4),IM_GAUSS1D(4)),IM_GAUSS1D(4))'
                              中略
 CONVEX 78 'IM_PRODUCT(IM_PRODUCT(IM_GAUSS1D(4),IM_GAUSS1D(4)),IM_GAUSS1D(4))'
 CONVEX 79 'IM_PRODUCT(IM_PRODUCT(IM_GAUSS1D(4),IM_GAUSS1D(4)),IM_GAUSS1D(4))'

END MESH_IM
//}

=== 質量行列と剛性行列の作成

有限要素法モデルの運動方程式は @<eq>{vibration} で与えられます。

//texequation[vibration][運動方程式]{
\boldsymbol{M}\dfrac{d^{2}\boldsymbol{u}}{dt^{2}}+\boldsymbol{C}\dfrac{d\boldsymbol{u}}{dt}+\boldsymbol{K}\boldsymbol{u}=\boldsymbol{f}\left(t\right)
//}

ここで、 @<m>$\boldsymbol{M}$ は質量行列、 @<m>$\boldsymbol{C}$ は減衰行列、 @<m>$\boldsymbol{K}$ は剛性行列を表します。
また、 @<m>$t$ は時刻、 @<m>$\boldsymbol{u}$ は変位ベクトル、 @<m>$\boldsymbol{f}\left(t\right)$ は外力ベクトルを表します。
減衰および外力のない自由振動の場合、運動方程式は @<eq>{free} で与えられます。

//texequation[free][自由振動の場合の運動方程式]{
\boldsymbol{M}\dfrac{d^{2}\boldsymbol{u}}{dt^{2}}+\boldsymbol{K}\boldsymbol{u}=\boldsymbol{0}
//}

ここで、変位の解を @<m>$\boldsymbol{u}=\boldsymbol{\Phi}\exp\left(i\omega t\right)$ と仮定します。
ただし、 @<m>$\boldsymbol{\Phi}$ は固有モード、 @<m>$\omega$ は固有角振動数を表します。
@<eq>{free} に代入して @<m>$\exp\left(i\omega t\right)$ を約分すると @<eq>{mode} が導かれます。

//texequation[mode][固有モードと固有角振動数の一般化固有値問題]{
\boldsymbol{K}\boldsymbol{\Phi}=\omega^{2}\boldsymbol{M}\boldsymbol{\Phi}
//}

ここで、両辺に @<m>$\boldsymbol{M}$ の逆行列をかけると @<eq>{eigen} のように固有値問題と同型になります。

//texequation[eigen][固有モードと固有角振動数の固有値問題]{
\left(\boldsymbol{M}^{-1}\boldsymbol{K}\right)\boldsymbol{\Phi}=\omega^{2}\boldsymbol{\Phi}
//}

ここでは、 @<em>{GetFEM} で質量行列と剛性行列を計算し @<em>{numpy} で固有値と固有ベクトルを計算します。
質量行列と剛性行列を直接作成するには @<href>{https://getfem.readthedocs.io/ja/latest/python/cmdref_Module%20asm.html, @<em>{getfem.asm_mass_matrix}} と  @<href>{https://getfem.readthedocs.io/ja/latest/python/cmdref_Module%20asm.html, @<em>{getfem.asm_linear_elasticity}} を使用します。

//list[][質量行列の作成][lang=python]{
M = gf.asm_mass_matrix(mim, mfu)
//}

//list[][剛性行列の作成][lang=python]{
E = 205000.0 # N/mm2
Nu = 0.0
Lambda = E*Nu/((1+Nu)*(1-2*Nu))
Mu =E/(2*(1+Nu))
Lambdas = np.repeat([Lambda], mfd.nbdof())
Mus = np.repeat(Mu, mfd.nbdof())
K = gf.asm_linear_elasticity(mim, mfu, mfd, Lambdas, Mus)
//}

=== 固有値と固有モードの計算

固有値と固有モードの計算には@<href>{https://numpy.org/doc/stable/reference/generated/numpy.linalg.eigh.html#numpy.linalg.eigh, @<em>{numpy.linalg.eig}}を使用します。
この節の固有値解析の方法は@<href>{https://watlab-blog.com/2019/06/30/multidof-vibration-eigen/, こちらのページ}を参考にさせていただきました。
逆行列と固有値解析の計算には時間がかかりますので根気強く待ってください。

//list[][固有値と固有モードの計算][lang=python]{
A = np.linalg.inv(M.full()) @ K.full()
omega2, vecs = np.linalg.eig(A)
//}

求めた固有値は値の大きさで並べられていないためソートをする必要があります。
計算した固有値を@<href>{https://numpy.org/doc/stable/reference/generated/numpy.sort.html, @<em>{numpy.sort}}でソートします。
//list[][固有値と固有ベクトルのソート][lang=python]{
omega2_sort = np.sort(omega2)
sort_index = np.argsort(omega2)
//}
求めた固有値を出力します。
出力結果から分かるように、モード 1-6 は固有値が微小値、すなわち固有角振動数の2乘が @<m>$0.0$ となっており、
剛体モードとなっていることが分かります。
//list[][固有値の確認][lang=python]{
print(omega2_sort[:10])
//}

//output[][出力結果]{
[-2.96792735e-12+0.j  0.00000000e+00+0.j  1.67761410e-12+0.j
  3.30174611e-12+0.j  7.96734349e-12+0.j  1.05152296e-11+0.j
  8.03242476e-02+0.j  8.03242476e-02+0.j  5.52361001e-01+0.j
  5.52361001e-01+0.j]
//}

求めた固有値 @<m>{\lambda} は固有角振動数の2乘のため、固有振動数との間には以下の式が成り立ちます。

//texequation[mode][固有振動数]{
f_{1}=\dfrac{\omega_{1}}{2\pi}=\dfrac{\sqrt{\lambda_{1}}}{2\pi}
//}

ゆえに、剛体モードを無視した場合の1次固有振動数は以下の通りになります。

//list[][1次固有振動数の確認][lang=python]{
f1 = np.sqrt(omega2_sort[6])/(2.0*np.pi)
print(f1.real, "Hz")
//}

//output[][出力結果]{
0.04510695023261866 Hz
//}

== 解のエクスポート

以上で固有値解析が解けました。
次に、固有ベクトルを可視化します。
固有ベクトルは2次元配列として計算されているため、該当する列をVTKに出力します。

//list[][固有モードの出力][lang=python]{
U = vecs[:, sort_index[6]].real
mfu.export_to_vtk("mfu.vtk", "ascii", mfu, U, "U")
//}

== 結果の可視化

@<em>{PyVista}を使い結果を表示します。
出力したファイルを読み込み固有モードをプロットさせます(@<img>{mfu}, @<img>{mfu2})。

//list[mode_plot][固有モードの表示][lang=python]{
m = pv.read("mfu.vtk")
m.plot()
//}
//image[mfu][モード図][scale=1.0]

//list[mode_warp_plot][モードによるワープ][lang=python]{
w = m.warp_by_vector("U", factor=1000.0)
w.plot()
//}

//image[mfu2][モード図(変形後)][scale=1.0]


== 検証

両端自由のはりの固有振動数 @<m>$f$ は @<eq>{beam} で計算できます。
(「日本機械学会編：機械工学便覧，基礎編 a2 機械力学，丸善， 2007」より)
//texequation[beam][両端自由のはりの固有振動数]{
f=\dfrac{1}{2\pi}\dfrac{\lambda^{2}}{l^{2}}\sqrt{\dfrac{EI}{\rho A}}
//}
となります。
ただし、1次の場合は @<m>$\lambda = 4.730$ です。
また、 @<m>$l$ ははりの長さ、 @<m>$I$ は断面二次モーメント、 @<m>$A$ ははりの断面積を表します。
この式を使用して今回の振動数との比較をしてみましょう。
//list[][1次固有振動数の計算][lang=python]{
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
しかし、有限要素法で扱う行列は疎行列であるため、 @<em>{scipy} の疎行列のソルバーを使用するほうがより効率的です。
この節の固有値解析の方法は @<href>{https://mapdldocs.pyansys.com/examples/01-apdlmath-examples/mapdl_vs_scipy.html, こちらのページ} を参考にさせていただきました。

//list[][モジュールインポート][lang=python]{
from scipy import io 
from scipy.sparse.linalg import eigsh
import matplotlib.pyplot as plt
//}

@<em>{GetFEM} で @<em>{scipy} の疎行列固有値解析をするには剛性行列 @<m>$\boldsymbol{K}$ と 質量行列 @<m>$\boldsymbol{M}$ をMatrix-Market形式で出力します。
@<href>{https://math.nist.gov/MatrixMarket/formats.html, Matrix-Market形式} は疎行列を表現するためのテキスト形式の1つです。
先ほど作成した行列は @<em>{GetFEM} の @<em>{Spmat} オブジェクトで作成されており、Matrix-Market形式で保存するには @<em>{save} メソッドを使用します。

//list[][Matrix-Market形式への出力][lang=python]{
K.save("mm", "K.mtx")
M.save("mm", "M.mtx")
//}

Matrix-Market形式で出力されたファイルは @<em>{scipy.io.mmread} を使用して読み込むことが可能です。
読み込み後、 @<em>{matplotlib} の @<em>{spy} メソッドを使用して疎行列の非ゼロ要素を可視化します。
剛性行列 @<m>$\boldsymbol{K}$ と @<m>$\boldsymbol{M}$ がいずれも疎行列であることが確認できます(@<img>{matrix})。

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
(もちろん他の @<m>{0.00} 付近の値でも機能します。
それぞれの値で固有値がどのように変化するか確認してみましょう。)

//list[][疎行列の固有値と固有モードの計算][lang=python]{
omega2, vecs = eigsh(A=pk, M=pm, k=10, sigma=-0.0001, which="LA")
//}

固有値を確認すると @<em>{numpy} で計算した結果と一致します。
@<em>{scipy} の @<em>{eigsh} で求めた固有値は昇順に並ぶため並び替えの必要はありません。


//list[][固有値の確認][lang=python]{
print(omega2)
//}

//output[][出力結果]{
array([3.88741096e-14, 1.17108119e-13, 1.40025727e-13,
       2.06940002e-13, 3.00736934e-13, 3.12765317e-13,
       8.03242475e-02, 8.03242475e-02, 5.52361001e-01,
       5.52361001e-01])
//}

1次固有振動数を計算します。

//list[][1 次固有振動数の確認][lang=python]{
f1 = np.sqrt(omega2[6])/(2.0*np.pi)
print(f1.real, "Hz")
//}

//output[][出力結果]{
0.045106950231340426 Hz
//}

以上で @<em>{GetFEM}、@<em>{numpy}、@<em>{scipy} を用いた固有値解析ができました。
今回は1次固有振動数と固有モードのみを確認しましたが、他の次数の固有振動数と固有モードも同様に確認することが可能です。
