# goblib
Goblin library(C++) STLを使用したライブラリ

## 概要
C++11以降を対象としたハードウェア非依存なライブラリです。  
C++11以降をコンパイルできる環境下での使用を想定しています。

## インストール
各環境の適切な位置に git clone するか、ZIP をダウンロード後展開して下さい。  
コンパイラに当該位置を示すオプション追加が必要かどうかは開発環境によります。

## ドキュメントの作成方法
[Doxygen](http://www.doxygen.jp/) によりライブラリドキュメントを生成できます。 [Doxygen設定ファイル](doc/Doxyfile)  
出力する際に [doxy.sh](doc/doxy.sh) を使用すると、library.properties からVersion、repository から rev を取得し、Doxygen 出力できます。

