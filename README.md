# README

### Heyawake Solver/Generator ###
* 2013年に個人依頼のバイトで作成したへやわけと呼ばれるパズルの自動生成プログラムと，パズルを自動で解くプログラムです．
* バイトの依頼者からは公開許可を頂いています．
* http://kyu.pw/demos/heyawake_play/ で生成したパズルを見ることができます．

### パズルのルール説明 ###
* ニコリのサイトをご覧頂くのが良いかと思われます． http://www.nikoli.com/ja/puzzles/heyawake/rule.html

### 仕様 ###
* 指定の形式に沿って標準入力にパズルを与えると，解を出力する(形式はformat.txtに記載)
* 40×40のパズルが解ける．
* 10×10の解が一意なパズルが生成できる．

### 開発環境 ###
* C++のみ．外部ライブラリは特にありません
* g++で動きます．コンパイルすればすぐに動くと思います．

### 工夫した点 ###
* パズルを解くアルゴリズムは完全に独自で設計しました．
* 解く/生成するアルゴリズムの詳細はどちらも　<b>generator.cpp</b> のソースコード内に載っています．

### 構成 ###
* solver.cpp: 問題を指定の形式に沿って標準入力に与えると標準出力に答えを出力するプログラム．10×10程度なら 30ms程度で回答可能．第一引数に制限時間(秒)を指定することができる．
* generator.cpp: 10×10のプログラムを自動生成する．生成されるごとに標準出力に生成盤面を出力する．手元の環境で 1 時間に1つ程度しか生成されない．
* format.txt: 入出力形式のサンプル
* example_puzzle.txt: 実際のパズルのサンプル
* heyawake_play/: webで動作する遊べる簡易GUIのディレクトリ