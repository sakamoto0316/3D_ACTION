# 3D_ACTION

・タイトル							：Square Hero2
・ジャンル							：3Dアクションゲーム
・開発時期、期間					：2024年1月10日～2024年3月1日(約2ヶ月間)
・開発人数							：1人
・開発環境、ツール、言語、ライブラリ：VisualStudio2019、GitHub、DirectX9、C++言語

・担当箇所：
全て

・制作エピソード(アピールポイント、苦労した点、制作背景など)：
アピールポイントはボスとの駆け引きや行動の制御、ゲーム中のイベントの管理です。
プレイヤーの挙動が移動、攻撃、ジャンプ、回避と非常にシンプルにまとめたため、
プレイヤーのアクションを100%引き出せるようなボスの攻撃を作成しました。
ボスに第二形態を準備し、派手な演出や2Dや見下ろし視点に変化する攻撃などの実装。
メッシュ構造を利用した球体の背景や波紋の形をした攻撃など、今のところ9種類のバリエーションの攻撃を作成しました。
C++言語での個人制作の中では3Dゲームに挑戦するのは初めてだったため、カメラの制御にとても苦労しました。
周りの人に実際にゲームをプレイしてもらい、感想や意見を元にカメラや攻撃の調整を行っています。
カメラはまだロックオン時に動きが激しかったり、見下ろしになる際にプレイヤーが落下してしまう事があるので随時調整中です。
この作品は一年生の時に制作した2Dアクションゲームのリメイク版として制作しました。全体の世界観を球や四角形で統一しました。
私は元々プレイヤーに動きを生み出すギミックや敵などの作成が好きなのでプレイヤー自体の動きはシンプルにし、
敵の動きをド派手に分かりやすく表現するようにこだわっています。

開発の流れ～開発メモ
・プロトタイプ版＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
プレイヤーのモーション完成
左クリックで攻撃、右クリックで回避
メッシュでキューブをつくり、自由な大きさで当たるようにした。
他に残像を追加し、モーションの間を保管するようにブレンドした。
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

・α版＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
ボスの処理に力を入れた。
ある程度の攻撃をループするようにした。
攻撃の種類
弾攻撃
突進攻撃
ブロックの上を乗り継いでいく攻撃
ブロックの柱がぐるぐると回る処理
ボスがブロックの雨を降らせてくる攻撃

そしてボスの体力が無くなった時第二形態へ変化して攻撃が強化されるようにした。
今は第二形態への蘇生処理と一部の攻撃の強化まで出来ているが演出面がまだ弱い
第二形態にはなるがそれ以降ボスの体力がなくなっても死亡処理などがない。

βではカメラに影響を与える攻撃や第二形態の攻撃、開始、形態変化、終了の演出面を作る
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

・β版＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
第二形態の変化演出、開始時の演出などの追加に加えて
第二形態のボスの攻撃変化を加えた。
紫のブロックを追加し、第二形態のボスが放つように調整。
ボスの第二形態のブロックがあまりに邪魔なので上に位置調整した
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

・プレマスター版＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
開始時演出や終了時演出の追加、見下ろしや2Dの攻撃を追加しました
他にも雑魚敵を追加、最低限のリザルトでギリゲームループ出来るようになりました。
リアクション系統を強化し、背景の動きなども追加。
攻撃の理不尽な要素を少し緩和するなど細かい調整を行っています。

マスター版までに
ボス戦前に軽いステージを作って操作に慣れる時間を設ける
ランキングひょうじなどのリザルト画面のさくせいを本格化
カメラの気持ち悪さはもっと修正せねばならぬ。
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

・マスター版＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
ボス前にチュートリアルを実装説明は画面左のログによってわかる
チュートリアル用のマップを作成するためにマップエディタを作成

思ったよりも進捗がよくない。
リザルトの作りこみの甘さやチュートリアル中やほかの箇所も音がなく違和感がある箇所がぽつぽつ
ボスの攻撃やカメラの調整に時間を割くことがあまりできず、
展示会までにコードの整理、ボスとカメラの調整。
リザルトの作りこみ(経過時間と残り体力を使用したランキングの実装)など
まだ出来る事は沢山ある。
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

・(マスター・オブ・ジエンド版）＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
プレイヤーの編集箇所のデータぶっ飛んで一部作り直しなどのハプニングは起きたりなどもあり以前進捗はよくない
ボス出現の演出時にUIをけしたり、リアクション系統をはっきりさせるなどの細かい調整。
ゲームオーバーの概念を消してクリアのみに、代わりにライフがなくなると復活するように。
復活の処理実装をしたが追加のＵＩがあってもいいなと今になって思うので作る
リザルトはほぼ完成、タイムとくらったダメージによってS,A+,A,A-,B+,B,B-,Cと8段階評価をつけてリプレイ動機にしている
家での作業効率があまり良くないので見直さないとなと反省
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝

GitHubのURL
https://github.com/sakamoto0316/3D_ACTION.git