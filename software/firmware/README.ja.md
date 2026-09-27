# ファームウェア

ESP32（身体）のファームウェア．Arduino で書き，PlatformIO でビルドする．

[English](README.md)

## 準備

1. [VS Code](https://code.visualstudio.com/) と，その拡張機能 PlatformIO IDE を入れる．
2. Mac では，PC 上のテストに C++ コンパイラも必要．`xcode-select --install` で Xcode Command Line Tools を入れる．
3. このフォルダ（`software/firmware`）を VS Code で開くか，`cd` で移動する．

## テストの実行

```sh
pio test -e native
```

単体テストを自分の PC で実行する．ESP32 は不要．CI もすべての PR で同じテストを実行する．

ESP32 へのビルドと書き込みの手順は，ボードが決まったらここに追加する．

## 構成

| パス | 内容 |
| --- | --- |
| `platformio.ini` | ビルド環境の設定．今は `native`（PC 上のテスト）だけ． |
| `lib/` | ハードウェアに依存しない処理（運動学，PID，指令タイムアウト）．Arduino のコードは入れない． |
| `test/` | `lib/` の単体テスト．モジュールごとに1フォルダ． |

モジュールを追加するときは，`lib/<名前>/<名前>.h` と `lib/<名前>/<名前>.cpp` を作り，テストを `test/test_<名前>/test_main.cpp` に書く．

## ルール

[docs/architecture.md](../../docs/architecture.md) より．

- `delay()` を使わない．一定周期の処理は `millis()` で行う．
- シリアルや UDP は届いた分だけ読み，ループを止めない．
- 指令が途切れたらモーターを止める．
- エンコーダは割り込みかハードウェアのパルスカウンタで読む．
- ハードウェアに依存しない処理は Arduino のコードを入れずに `lib/` に置き，テストを `test/` に書く．
- Wi-Fi のパスワードなどの秘密情報は `secrets.h` に書く（Git は無視する）．コミットするのは `secrets.example.h` だけ．
- 安全の仕組みを弱めない（例：指令タイムアウトを延ばす，無効にする）．変える場合はメンテナーが決める．
