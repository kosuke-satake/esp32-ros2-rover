# 参加の手引き

チームのメンバーとしてこのリポジトリで作業する方法．

[English](CONTRIBUTING.md)

## 担当とフォルダ

| フォルダ | 担当 | 内容 |
| --- | --- | --- |
| [`hardware/`](hardware) | ハードウェア担当 | CAD，レーザーカット用データ，部品表 |
| [`software/`](software) | ソフトウェア担当 | ESP32 のファームウェア，後に ROS 2 とツール |
| [`drafts/`](drafts) | 全員 | スケッチや思いつき |
| [`docs/`](docs) | 全員（決めるのはメンテナー） | 決定済みの設計と未確定事項 |

各フォルダには説明書（英語の `README.md` と日本語の `README.ja.md`）があり，何をどう置くかが書いてある．作業を始める前に読む．
フォルダの担当者は [`.github/CODEOWNERS`](.github/CODEOWNERS) に書いてあり，担当フォルダが変更されると GitHub が担当者にレビューを依頼する．

## はじめに

1. リポジトリへの招待を承認する．
2. クローンする．
   ```sh
   git clone https://github.com/kosuke-satake/esp32-ros2-rover.git
   ```
3. [README.ja.md](README.ja.md)，[docs/architecture.md](docs/architecture.md)（英語），作業するフォルダの説明書を読む．

## 変更の手順

1. 最新の `main` から始める．
   ```sh
   git switch main
   git pull
   ```
2. `<種類>/<短い説明>` という名前のブランチを作る．
   ```sh
   git switch -c feat/chassis-base
   ```
   種類：`feat/`（新しいもの），`fix/`（修正），`docs/`（文書だけ），`refactor/`（動きを変えない整理），`chore/`（設定や保守）．
3. 小さく意味のある単位でコミットする．1行目は英語の短い命令形にする（例："Add chassis base plate"）．理由が明らかでないときは本文に書く．
4. ブランチを push し，`main` に向けて PR を出す．何をなぜ変えたかを書く．ハードウェアならスクリーンショットか写真を付ける．
5. CI が通り，フォルダの担当者がレビューする．マージはメンテナーが行う．

`main` は保護されている．直接 push しない．

## コミットしてはいけないもの

- パスワード，API キー，トークン．Wi-Fi の認証情報は `secrets.h` に書く（Git は無視する）．
- ビルドの成果物，キャッシュ，`.DS_Store` などの OS のファイル．よくあるものは `.gitignore` で除外済み．
- 50 MB を超えるファイル（先に相談する）．GitHub は 100 MB を超えるファイルを受け付けない．

## 言語

コード，コメント，コミットメッセージ，PR，文書は英語で書く．フォルダの説明書とこのファイルには日本語版（`*.ja.md`）もある．片方を変えたら，同じ PR でもう片方も直す．

## 設計の判断

[docs/architecture.md](docs/architecture.md) に決定済みの事項がある．変えたいときは，理由を書いた issue か PR を出す．決めるのはメンテナー．まだ決まっていない考えは [`drafts/`](drafts) か [docs/open-questions.md](docs/open-questions.md) に置く．

## AI アシスタント

AI コーディングアシスタントを使ってよい．AI 向けの指示は [AGENTS.md](AGENTS.md) にあり，AI の変更も同じ PR とレビューを通る．
