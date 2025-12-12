#include "Input.h"
#include<DxLib.h>
#include"StringFunctions.h"

constexpr char config_file_name[] = "keyconf.kcnf";

namespace {
    //シグネチャ"kcnf"
    //バージョン番号
    //データ数
    //先頭のデータサイズ固定部分を「ヘッダ」として書き込むため
    //構造体を定義します
    struct KeyConfHeader {
        char signature[4];//シグネチャ4バイト
        float version;//バージョン番号4バイト
        int dataNum;//データ数4バイト
    };//12バイト
}

void Input::InitializeInputTable()
{
    //イベント名を添え字にして、右辺値に実際の入力種別と入力コードの配列をおく
    inputTable_["ok"] = { {PeripheralType::keyboard , KEY_INPUT_RETURN },
                            {PeripheralType::pad1 , PAD_INPUT_L } };//SELECTキー

    inputTable_["pause"] = { {PeripheralType::keyboard , KEY_INPUT_P },
                            {PeripheralType::pad1 , PAD_INPUT_R } };//STARTキー


    inputTable_["shot"] = { {PeripheralType::keyboard , KEY_INPUT_Z },
                            {PeripheralType::pad1 , PAD_INPUT_C } };//PADのXボタン


    inputTable_["slow"] = { {PeripheralType::keyboard , KEY_INPUT_LSHIFT },
                            {PeripheralType::pad1 , PAD_INPUT_A } };//PADのAボタン

    inputTable_["bomb"] = { {PeripheralType::keyboard , KEY_INPUT_X },
                            {PeripheralType::pad1 , PAD_INPUT_X } };//PADのYボタン


    inputTable_["up"] = { { PeripheralType::keyboard , KEY_INPUT_UP },
                            {PeripheralType::pad1 , PAD_INPUT_UP } };
    inputTable_["down"] = { {PeripheralType::keyboard,KEY_INPUT_DOWN},
                            {PeripheralType::pad1,PAD_INPUT_DOWN} };
    inputTable_["left"] = { {PeripheralType::keyboard,KEY_INPUT_LEFT},
                            {PeripheralType::pad1,PAD_INPUT_LEFT} };
    inputTable_["right"] = { {PeripheralType::keyboard,KEY_INPUT_RIGHT},
                            {PeripheralType::pad1,PAD_INPUT_RIGHT} };

}

Input::Input() :inputData_{}, lastInputData_{}, inputTable_{}
{
    InitializeInputTable();
    Load();

    //エディットしたいイベントだけを表示＆順序をコントロールするため
    editableEventNames_ = {"ok","pause","shot","slow","bomb"};


    //あらかじめ入力データのための枠を開けておく
    //ここで枠を開けておかないと、チェックの際にAt関数でクラッシュする可能性がある
    for (const auto& inputInfo : inputTable_) {
        inputData_[inputInfo.first] = false;
        lastInputData_[inputInfo.first] = false;
    }
}

void Input::Update() {
    //まず現在の入力情報を取得
    char keyState[256];
    GetHitKeyStateAll(keyState);//生のキーボード情報
    int padState = GetJoypadInputState(DX_INPUT_PAD1);//生のPAD1情報
    lastInputData_ = inputData_;//直前のフレームを更新(前のフレーム情報をコピー)
    //すべての入力イベントをチェックします
    //ここでinputData_が更新される
    //inputTable_を回して各イベントの入力をチェックする
    for (const auto& inputInfo : inputTable_) {
        auto& input = inputData_[inputInfo.first];//inputInfo.firstには"ok"等が入ってる
        //inputを書き換えると、inputData_のそのイベントが押されてるかどうかを
        //書き換えることになる
        //InputStateのベクタを回す
        for (const auto& state : inputInfo.second) {
            //このループはInputState配列のループなので
            //まず、入力種別をチェックします
            switch (state.type) {
            case PeripheralType::keyboard://キーボードだったら
                //GetHitKeyStateAllで取ってきた配列の中身を見て
                //入力されているかどうかをチェック
                input = keyState[state.id];
                break;
            case PeripheralType::pad1:
                //GetJoypadStateで取ってきたビット情報を見て
                //登録されてるビット情報と&を取り、そのビットが立っているかどうかをチェック
                input = (padState & state.id);
                break;
            }
            if (input) {//必須！
                break;//ここでbreakしないと、最後のチェックで押されてないとfalseになる
            }
        }
    }

}

bool Input::IsPressed(const char* name) const
{
    //もし「ない」イベントを送られるとクラッシュします
    //クラッシュがいやだったら
    //if (inputData_.contains(name)) {
    //    return false;//で回避できます
    //}
    return inputData_.at(name);//const関数内部なので[]ではなくatを使用してる
}

bool Input::IsTriggered(const char* name) const
{
    return inputData_.at(name) && !lastInputData_.at(name);
}

void Input::Save()
{
    FILE* fp = nullptr;
    auto err = fopen_s(&fp, config_file_name, "wb");
    if (fp == nullptr) {
        return;
    }
    KeyConfHeader header = {};
    header.signature[0] = 'k';
    header.signature[1] = 'c';
    header.signature[2] = 'n';
    header.signature[3] = 'f';
    header.version = 1.0f;
    header.dataNum = inputTable_.size();
    fwrite(&header, sizeof(header), 1, fp);//12バイトまるまる書き込んでいる

    //個別のデータ
    for (const auto& info : inputTable_) {
        const auto& name = info.first;
        byte nameLen = name.size();//イベント名(文字列数)

        //文字列を書き込む際は先頭１バイトを「文字列数」にすることをお勧めします
        fwrite(&nameLen, sizeof(nameLen), 1, fp);//文字サイズの書き込み
        fwrite(name.data(), nameLen, 1, fp);//文字データの書き込み(イベント名)

        const auto& data = info.second;
        byte dataNum = data.size();
        //データのサイズをあらかじめ書き込んでおく
        fwrite(&dataNum, sizeof(dataNum), 1, fp);
        //データをまとめて書き込む(サイズがわかってるから一気に書き込める)
        fwrite(data.data(), data.size() * sizeof(InputState), 1, fp);
    }

    fclose(fp);
}

void Input::Load()
{
    auto fileName = StringFunctions::WStringFromString(config_file_name);
    int handle = FileRead_open(fileName.c_str());
    KeyConfHeader header = {};
    FileRead_read(&header, sizeof(header), handle);
    for (int i = 0; i < header.dataNum; ++i) {
        byte nameSize = 0;//名前の文字列数が分からないため、１バイトロード
        FileRead_read(&nameSize, sizeof(nameSize), handle);

        std::string name;//イベント名受け取り用
        name.resize(nameSize);//受け取るために名前文字列の領域を確保
        //確保した名前領域にセーブされているイベント名をコピーします
        FileRead_read(name.data(), name.size(), handle);
        //その名前がinputTable_にあるかどうかをチェック
        if (inputTable_.contains(name)) {
            //もしあったらそのテーブルデータを取得します
            auto& info = inputTable_.at(name);
            byte dataNum = 0;
            FileRead_read(&dataNum, sizeof(dataNum), handle);//データ数を取得
            //もとのvectorサイズを越えないようにminで小さいほうを選ぶようにする
            dataNum = min(dataNum, info.size());//inputTable_[name]内のvectorをオーバーしないように
            //必要なぶんだけデータをファイルからリードしてコピー
            FileRead_read(info.data(), dataNum * sizeof(InputState), handle);
        }
    }
    FileRead_close(handle);//クローズを忘れないように
}

