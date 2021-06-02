# speaker_selecter

伝達関数測定装置のスピーカーセレクター。
[TFMeasure](https://github.com/kudolab/TFMeasure)と連携して使います。

## 内容物

- conf_files
  - Proxy関連とdeamonの設定ファイル
- raspi_backup
  - 下川さんによる旧システムのバックアップ
- speaker
  - HTTPサーバー型のスピーカーセレクター制御プログラム

## 仕組み

speaker-selecterはflask製のWebサーバーでリクエストを受け取り、GPIOを制御しています。
また、systemdで動かすので電源を入れればすぐに動かすことができます。
サーバーで受け付けているエンドポイントは以下の通りです。

- GET http://172.24.176.169/health
- GET http://172.24.176.169/speaker
- PUT http://172.24.176.169/health

以下はコマンドで動作を確認する例です。

```shell
$ curl http://172.24.176.169/health
ok⏎

$ curl -X PUT -H "Content-Type: application/json" -d '{"speaker_num":5}' 172.24.176.169/speaker
{"message":"speaker_num changed to 5"}

$ curl http://172.24.176.169/speaker
{"speaker_num":5}
```

## セットアップ方法

### 1. ファイルの配置

conf_filesの中のproxyのパスワードを書き込み、それぞれ以下の位置に配置する。

- .bashrc -> /home/pi/.bashrc
- apt.conf -> /etc/apt/apt.conf
- dhcpcd.conf -> /etc/dhcpcd.conf
- speaker-selecter.service -> /etc/systemd/system/speaker-selecter.service

### 2. 実行環境構築

```shell
sudo apt install -y python3-dev python3-pip
sudo pip3 install -r speaker_selecter/speaker/requirements.txt
sudo systemctl daemon-reload
sudo systemctl enable speaker-selecter.service
sudo reboot
```

### 3.　動作確認

上のコマンド例と同じ動作をすればOKです。
