# CAD

ロボットのすべての部品と組立の設計データ．

[English](README.md)

使う CAD ソフトはまだ決まっていない（[docs/open-questions.md](../../docs/open-questions.md) を参照）．以下のルールはどのソフトでも使える．

## ここに置くもの

部品または組立ごとに，その名前のフォルダを1つ作る．

```
cad/
  chassis-base/
    chassis-base.step     STEP（必ず置く）
    chassis-base.sldprt   元データ（ファイルで保存する CAD の場合）
    chassis-base.png      確認用の画像
    README.md             説明が必要なときだけ
```

- **STEP は必ず置く．** すべての設計に最新の STEP を用意し，どの CAD でも開けるようにする．
- **元データ．** SolidWorks（`.sldprt`，`.sldasm`，`.slddrw`）や Inventor（`.ipt`，`.iam`，`.idw`）のようにファイルで保存するソフトでは，元データも STEP と並べてコミットする．
- **Onshape．** 設計はファイルではなく Onshape 上にある．フォルダの `README.md` に Onshape のドキュメントへのリンクを書き，STEP は他のソフトと同じようにコミットする．
- **確認用の画像．** PNG のスクリーンショット．CAD を開かなくても GitHub 上で形が分かるようにする．

## 設計を更新するとき

1. CAD で設計を変える．
2. STEP を書き出し直し，元データと確認用の画像も更新する．
3. これらを一緒にコミットし，常に内容をそろえる．
4. 加工データに影響する変更なら，同じ PR で [`../laser-cut/`](../laser-cut) も更新する．

## 注意

- 単位はミリメートル．
- 組立はファイル名で部品を探す．部品のファイル名を変えるときは，それを使う組立も直す．
- CAD が作るロックファイルやバックアップ（`~$*`，`*.lck`，`OldVersions/`）は Git が無視する．
