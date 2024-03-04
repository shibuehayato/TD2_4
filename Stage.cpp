#include "Stage.h"
#include<cassert>

void Stage::Initialize(Model* model)
{
	//問題ないか変数をチェック
	assert(model);
	
	
	//モデルの代入と座標の初期化
	for (int i = 0; i < 281; i++)
	{   
		model_[i] = model;

		worldTransform_[i].Initialize();
	}

	//------------------------------------//

	//右側のz座標0～-50までの座標指定
	worldTransform_[0].translation_ = {10.0f,0.0f,0.0f};
	worldTransform_[1].translation_ = { 10.0f,0.0f,-1.0f };
	worldTransform_[2].translation_ = { 10.0f,0.0f,-2.0f };
	worldTransform_[3].translation_ = { 10.0f,0.0f,-3.0f };
	worldTransform_[4].translation_ = { 10.0f,0.0f,-4.0f };
	worldTransform_[5].translation_ = { 10.0f,0.0f,-5.0f };
	worldTransform_[6].translation_ = { 10.0f,0.0f,-6.0f };
	worldTransform_[7].translation_ = { 10.0f,0.0f,-7.0f };
	worldTransform_[8].translation_ = { 10.0f,0.0f,-8.0f };
	worldTransform_[9].translation_ = { 10.0f,0.0f,-9.0f };
	worldTransform_[10].translation_ = { 10.0f,0.0f,-10.0f };
	worldTransform_[11].translation_ = { 10.0f,0.0f,-11.0f };
	worldTransform_[12].translation_ = { 10.0f,0.0f,-12.0f };
	worldTransform_[13].translation_ = { 10.0f,0.0f,-13.0f };
	worldTransform_[14].translation_ = { 10.0f,0.0f,-14.0f };
	worldTransform_[15].translation_ = { 10.0f,0.0f,-15.0f };
	worldTransform_[16].translation_ = { 10.0f,0.0f,-16.0f };
	worldTransform_[17].translation_ = { 10.0f,0.0f,-17.0f };
	worldTransform_[18].translation_ = { 10.0f,0.0f,-18.0f };
	worldTransform_[19].translation_ = { 10.0f,0.0f,-19.0f };
	worldTransform_[20].translation_ = { 10.0f,0.0f,-20.0f };
	worldTransform_[21].translation_ = { 10.0f,0.0f,-21.0f };
	worldTransform_[22].translation_ = { 10.0f,0.0f,-22.0f };
	worldTransform_[23].translation_ = { 10.0f,0.0f,-23.0f };
	worldTransform_[24].translation_ = { 10.0f,0.0f,-24.0f };
	worldTransform_[25].translation_ = { 10.0f,0.0f,-25.0f };
	worldTransform_[26].translation_ = { 10.0f,0.0f,-26.0f };
	worldTransform_[27].translation_ = { 10.0f,0.0f,-27.0f };
	worldTransform_[28].translation_ = { 10.0f,0.0f,-28.0f };
	worldTransform_[29].translation_ = { 10.0f,0.0f,-29.0f };
	worldTransform_[30].translation_ = { 10.0f,0.0f,-30.0f };
	worldTransform_[31].translation_ = { 10.0f,0.0f,-31.0f };
	worldTransform_[32].translation_ = { 10.0f,0.0f,-32.0f };
	worldTransform_[33].translation_ = { 10.0f,0.0f,-33.0f };
	worldTransform_[34].translation_ = { 10.0f,0.0f,-34.0f };
	worldTransform_[35].translation_ = { 10.0f,0.0f,-35.0f };
	worldTransform_[36].translation_ = { 10.0f,0.0f,-36.0f };
	worldTransform_[37].translation_ = { 10.0f,0.0f,-37.0f };
	worldTransform_[38].translation_ = { 10.0f,0.0f,-38.0f };
	worldTransform_[39].translation_ = { 10.0f,0.0f,-39.0f };
	worldTransform_[40].translation_ = { 10.0f,0.0f,-40.0f };
	worldTransform_[41].translation_ = { 10.0f,0.0f,-41.0f };
	worldTransform_[42].translation_ = { 10.0f,0.0f,-42.0f };
	worldTransform_[43].translation_ = { 10.0f,0.0f,-43.0f };
	worldTransform_[44].translation_ = { 10.0f,0.0f,-44.0f };
	worldTransform_[45].translation_ = { 10.0f,0.0f,-45.0f };
	worldTransform_[46].translation_ = { 10.0f,0.0f,-46.0f };
	worldTransform_[47].translation_ = { 10.0f,0.0f,-47.0f };
	worldTransform_[48].translation_ = { 10.0f,0.0f,-48.0f };
	worldTransform_[49].translation_ = { 10.0f,0.0f,-49.0f };
	worldTransform_[50].translation_ = { 10.0f,0.0f,-50.0f };
	//-----------------------------------------------------//

	//右側のz座標1～50までの座標指定
	worldTransform_[51].translation_ = { 10.0f,0.0f,1.0f };
	worldTransform_[52].translation_ = { 10.0f,0.0f,2.0f };
	worldTransform_[53].translation_ = { 10.0f,0.0f,3.0f };
	worldTransform_[54].translation_ = { 10.0f,0.0f,4.0f };
	worldTransform_[55].translation_ = { 10.0f,0.0f,5.0f };
	worldTransform_[56].translation_ = { 10.0f,0.0f,6.0f };
	worldTransform_[57].translation_ = { 10.0f,0.0f,7.0f };
	worldTransform_[58].translation_ = { 10.0f,0.0f,8.0f };
	worldTransform_[59].translation_ = { 10.0f,0.0f,9.0f };
	worldTransform_[60].translation_ = { 10.0f,0.0f,10.0f };
	worldTransform_[61].translation_ = { 10.0f,0.0f,11.0f };
	worldTransform_[62].translation_ = { 10.0f,0.0f,12.0f };
	worldTransform_[63].translation_ = { 10.0f,0.0f,13.0f };
	worldTransform_[64].translation_ = { 10.0f,0.0f,14.0f };
	worldTransform_[65].translation_ = { 10.0f,0.0f,15.0f };
	worldTransform_[66].translation_ = { 10.0f,0.0f,16.0f };
	worldTransform_[67].translation_ = { 10.0f,0.0f,17.0f };
	worldTransform_[68].translation_ = { 10.0f,0.0f,18.0f };
	worldTransform_[69].translation_ = { 10.0f,0.0f,19.0f };
	worldTransform_[70].translation_ = { 10.0f,0.0f,20.0f };
	worldTransform_[71].translation_ = { 10.0f,0.0f,21.0f };
	worldTransform_[72].translation_ = { 10.0f,0.0f,22.0f };
	worldTransform_[73].translation_ = { 10.0f,0.0f,23.0f };
	worldTransform_[74].translation_ = { 10.0f,0.0f,24.0f };
	worldTransform_[75].translation_ = { 10.0f,0.0f,25.0f };
	worldTransform_[76].translation_ = { 10.0f,0.0f,26.0f };
	worldTransform_[77].translation_ = { 10.0f,0.0f,27.0f };
	worldTransform_[78].translation_ = { 10.0f,0.0f,28.0f };
	worldTransform_[79].translation_ = { 10.0f,0.0f,29.0f };
	worldTransform_[80].translation_ = { 10.0f,0.0f,30.0f };
	worldTransform_[81].translation_ = { 10.0f,0.0f,31.0f };
	worldTransform_[82].translation_ = { 10.0f,0.0f,32.0f };
	worldTransform_[83].translation_ = { 10.0f,0.0f,33.0f };
	worldTransform_[84].translation_ = { 10.0f,0.0f,34.0f };
	worldTransform_[85].translation_ = { 10.0f,0.0f,35.0f };
	worldTransform_[86].translation_ = { 10.0f,0.0f,36.0f };
	worldTransform_[87].translation_ = { 10.0f,0.0f,37.0f };
	worldTransform_[88].translation_ = { 10.0f,0.0f,38.0f };
	worldTransform_[89].translation_ = { 10.0f,0.0f,39.0f };
	worldTransform_[90].translation_ = { 10.0f,0.0f,40.0f };
	worldTransform_[91].translation_ = { 10.0f,0.0f,41.0f };
	worldTransform_[92].translation_ = { 10.0f,0.0f,42.0f };
	worldTransform_[93].translation_ = { 10.0f,0.0f,43.0f };
	worldTransform_[94].translation_ = { 10.0f,0.0f,44.0f };
	worldTransform_[95].translation_ = { 10.0f,0.0f,45.0f };
	worldTransform_[96].translation_ = { 10.0f,0.0f,46.0f };
	worldTransform_[97].translation_ = { 10.0f,0.0f,47.0f };
	worldTransform_[98].translation_ = { 10.0f,0.0f,48.0f };
	worldTransform_[99].translation_ = { 10.0f,0.0f,49.0f };
	worldTransform_[100].translation_ = { 10.0f,0.0f,50.0f };

	//---------------------------------------------------------//

	//左側のz座標-1～-50までの座標指定

	worldTransform_[101].translation_ = { -10.0f,0.0f,-1.0f };
	worldTransform_[102].translation_ = { -10.0f,0.0f,-2.0f };
	worldTransform_[103].translation_ = { -10.0f,0.0f,-3.0f };
	worldTransform_[104].translation_ = { -10.0f,0.0f,-4.0f };
	worldTransform_[105].translation_ = { -10.0f,0.0f,-5.0f };
	worldTransform_[106].translation_ = { -10.0f,0.0f,-6.0f };
	worldTransform_[107].translation_ = { -10.0f,0.0f,-7.0f };
	worldTransform_[108].translation_ = { -10.0f,0.0f,-8.0f };
	worldTransform_[109].translation_ = { -10.0f,0.0f,-9.0f };
	worldTransform_[110].translation_ = { -10.0f,0.0f,-10.0f };
	worldTransform_[111].translation_ = { -10.0f,0.0f,-11.0f };
	worldTransform_[112].translation_ = { -10.0f,0.0f,-12.0f };
	worldTransform_[113].translation_ = { -10.0f,0.0f,-13.0f };
	worldTransform_[114].translation_ = { -10.0f,0.0f,-14.0f };
	worldTransform_[115].translation_ = { -10.0f,0.0f,-15.0f };
	worldTransform_[116].translation_ = { -10.0f,0.0f,-16.0f };
	worldTransform_[117].translation_ = { -10.0f,0.0f,-17.0f };
	worldTransform_[118].translation_ = { -10.0f,0.0f,-18.0f };
	worldTransform_[119].translation_ = { -10.0f,0.0f,-19.0f };
	worldTransform_[120].translation_ = { -10.0f,0.0f,-20.0f };
	worldTransform_[121].translation_ = { -10.0f,0.0f,-21.0f };
	worldTransform_[122].translation_ = { -10.0f,0.0f,-22.0f };
	worldTransform_[123].translation_ = { -10.0f,0.0f,-23.0f };
	worldTransform_[124].translation_ = { -10.0f,0.0f,-24.0f };
	worldTransform_[125].translation_ = { -10.0f,0.0f,-25.0f };
	worldTransform_[126].translation_ = { -10.0f,0.0f,-26.0f };
	worldTransform_[127].translation_ = { -10.0f,0.0f,-27.0f };
	worldTransform_[128].translation_ = { -10.0f,0.0f,-28.0f };
	worldTransform_[129].translation_ = { -10.0f,0.0f,-29.0f };
	worldTransform_[130].translation_ = { -10.0f,0.0f,-30.0f };
	worldTransform_[131].translation_ = { -10.0f,0.0f,-31.0f };
	worldTransform_[132].translation_ = { -10.0f,0.0f,-32.0f };
	worldTransform_[133].translation_ = { -10.0f,0.0f,-33.0f };
	worldTransform_[134].translation_ = { -10.0f,0.0f,-34.0f };
	worldTransform_[135].translation_ = { -10.0f,0.0f,-35.0f };
	worldTransform_[136].translation_ = { -10.0f,0.0f,-36.0f };
	worldTransform_[137].translation_ = { -10.0f,0.0f,-37.0f };
	worldTransform_[138].translation_ = { -10.0f,0.0f,-38.0f };
	worldTransform_[139].translation_ = { -10.0f,0.0f,-39.0f };
	worldTransform_[140].translation_ = { -10.0f,0.0f,-40.0f };
	worldTransform_[141].translation_ = { -10.0f,0.0f,-41.0f };
	worldTransform_[142].translation_ = { -10.0f,0.0f,-42.0f };
	worldTransform_[143].translation_ = { -10.0f,0.0f,-43.0f };
	worldTransform_[144].translation_ = { -10.0f,0.0f,-44.0f };
	worldTransform_[145].translation_ = { -10.0f,0.0f,-45.0f };
	worldTransform_[146].translation_ = { -10.0f,0.0f,-46.0f };
	worldTransform_[147].translation_ = { -10.0f,0.0f,-47.0f };
	worldTransform_[148].translation_ = { -10.0f,0.0f,-48.0f };
	worldTransform_[149].translation_ = { -10.0f,0.0f,-49.0f };
	worldTransform_[150].translation_ = { -10.0f,0.0f,-50.0f };
	//-----------------------------------------------------//

	//左側のz座標1～50までの座標指定
	worldTransform_[151].translation_ = { -10.0f,0.0f,1.0f };
	worldTransform_[152].translation_ = { -10.0f,0.0f,2.0f };
	worldTransform_[153].translation_ = { -10.0f,0.0f,3.0f };
	worldTransform_[154].translation_ = { -10.0f,0.0f,4.0f };
	worldTransform_[155].translation_ = { -10.0f,0.0f,5.0f };
	worldTransform_[156].translation_ = { -10.0f,0.0f,6.0f };
	worldTransform_[157].translation_ = { -10.0f,0.0f,7.0f };
	worldTransform_[158].translation_ = { -10.0f,0.0f,8.0f };
	worldTransform_[159].translation_ = { -10.0f,0.0f,9.0f };
	worldTransform_[160].translation_ = { -10.0f,0.0f,10.0f };
	worldTransform_[161].translation_ = { -10.0f,0.0f,11.0f };
	worldTransform_[162].translation_ = { -10.0f,0.0f,12.0f };
	worldTransform_[163].translation_ = { -10.0f,0.0f,13.0f };
	worldTransform_[164].translation_ = { -10.0f,0.0f,14.0f };
	worldTransform_[165].translation_ = { -10.0f,0.0f,15.0f };
	worldTransform_[166].translation_ = { -10.0f,0.0f,16.0f };
	worldTransform_[167].translation_ = { -10.0f,0.0f,17.0f };
	worldTransform_[168].translation_ = { -10.0f,0.0f,18.0f };
	worldTransform_[169].translation_ = { -10.0f,0.0f,19.0f };
	worldTransform_[170].translation_ = { -10.0f,0.0f,20.0f };
	worldTransform_[171].translation_ = { -10.0f,0.0f,21.0f };
	worldTransform_[172].translation_ = { -10.0f,0.0f,22.0f };
	worldTransform_[173].translation_ = { -10.0f,0.0f,23.0f };
	worldTransform_[174].translation_ = { -10.0f,0.0f,24.0f };
	worldTransform_[175].translation_ = { -10.0f,0.0f,25.0f };
	worldTransform_[176].translation_ = { -10.0f,0.0f,26.0f };
	worldTransform_[177].translation_ = { -10.0f,0.0f,27.0f };
	worldTransform_[178].translation_ = { -10.0f,0.0f,28.0f };
	worldTransform_[179].translation_ = { -10.0f,0.0f,29.0f };
	worldTransform_[180].translation_ = { -10.0f,0.0f,30.0f };
	worldTransform_[181].translation_ = { -10.0f,0.0f,31.0f };
	worldTransform_[182].translation_ = { -10.0f,0.0f,32.0f };
	worldTransform_[183].translation_ = { -10.0f,0.0f,33.0f };
	worldTransform_[184].translation_ = { -10.0f,0.0f,34.0f };
	worldTransform_[185].translation_ = { -10.0f,0.0f,35.0f };
	worldTransform_[186].translation_ = { -10.0f,0.0f,36.0f };
	worldTransform_[187].translation_ = { -10.0f,0.0f,37.0f };
	worldTransform_[188].translation_ = { -10.0f,0.0f,38.0f };
	worldTransform_[189].translation_ = { -10.0f,0.0f,39.0f };
	worldTransform_[190].translation_ = { -10.0f,0.0f,40.0f };
	worldTransform_[191].translation_ = { -10.0f,0.0f,41.0f };
	worldTransform_[192].translation_ = { -10.0f,0.0f,42.0f };
	worldTransform_[193].translation_ = { -10.0f,0.0f,43.0f };
	worldTransform_[194].translation_ = { -10.0f,0.0f,44.0f };
	worldTransform_[195].translation_ = { -10.0f,0.0f,45.0f };
	worldTransform_[196].translation_ = { -10.0f,0.0f,46.0f };
	worldTransform_[197].translation_ = { -10.0f,0.0f,47.0f };
	worldTransform_[198].translation_ = { -10.0f,0.0f,48.0f };
	worldTransform_[199].translation_ = { -10.0f,0.0f,49.0f };
	worldTransform_[200].translation_ = { -10.0f,0.0f,50.0f };

	//--------------------------------------------------------//

	//特定の場所の壁の座標指定
	worldTransform_[201].translation_ = { 10.0f,0.0f,-20.0f };
	worldTransform_[202].translation_ = { 9.0f,0.0f,-20.0f };
	worldTransform_[203].translation_ = { 8.0f,0.0f,-20.0f };
	worldTransform_[204].translation_ = { 7.0f,0.0f,-20.0f };
	worldTransform_[205].translation_ = { 6.0f,0.0f,-20.0f };
	worldTransform_[206].translation_ = { -10.0f,0.0f,-20.0f };
	worldTransform_[207].translation_ = { -9.0f,0.0f,-20.0f };
	worldTransform_[208].translation_ = { -8.0f,0.0f,-20.0f };
	worldTransform_[209].translation_ = { -7.0f,0.0f,-20.0f };
	worldTransform_[210].translation_ = { -6.0f,0.0f,-20.0f };
	worldTransform_[211].translation_ = { -5.0f,0.0f,-20.0f };
	worldTransform_[212].translation_ = { -4.0f,0.0f,-20.0f };
	worldTransform_[213].translation_ = { -3.0f,0.0f,-20.0f };
	worldTransform_[214].translation_ = { -2.0f,0.0f,-20.0f };
	worldTransform_[215].translation_ = { -1.0f,0.0f,-20.0f };
	worldTransform_[216].translation_ = { 10.0f,0.0f,-21.0f };
	worldTransform_[217].translation_ = { 9.0f,0.0f,-21.0f };
	worldTransform_[218].translation_ = { 8.0f,0.0f,-21.0f };
	worldTransform_[219].translation_ = { 7.0f,0.0f,-21.0f };
	worldTransform_[220].translation_ = { 6.0f,0.0f,-21.0f };
	worldTransform_[221].translation_ = { -10.0f,0.0f,-21.0f };
	worldTransform_[222].translation_ = { -9.0f,0.0f,-21.0f };
	worldTransform_[223].translation_ = { -8.0f,0.0f,-21.0f };
	worldTransform_[224].translation_ = { -7.0f,0.0f,-21.0f };
	worldTransform_[225].translation_ = { -6.0f,0.0f,-21.0f };
	worldTransform_[226].translation_ = { -5.0f,0.0f,-21.0f };
	worldTransform_[227].translation_ = { -4.0f,0.0f,-21.0f };
	worldTransform_[228].translation_ = { -3.0f,0.0f,-21.0f };
	worldTransform_[229].translation_ = { -2.0f,0.0f,-21.0f };
	worldTransform_[230].translation_ = { -1.0f,0.0f,-21.0f };
	worldTransform_[231].translation_ = { 10.0f,0.0f,-19.0f };
	worldTransform_[232].translation_ = { 9.0f,0.0f,-19.0f };
	worldTransform_[233].translation_ = { 8.0f,0.0f,-19.0f };
	worldTransform_[234].translation_ = { 7.0f,0.0f,-19.0f };
	worldTransform_[235].translation_ = { 6.0f,0.0f,-19.0f };
	worldTransform_[236].translation_ = { -10.0f,0.0f,-19.0f };
	worldTransform_[237].translation_ = { -9.0f,0.0f,-19.0f };
	worldTransform_[238].translation_ = { -8.0f,0.0f,-19.0f };
	worldTransform_[239].translation_ = { -7.0f,0.0f,-19.0f };
	worldTransform_[240].translation_ = { -6.0f,0.0f,-19.0f };
	worldTransform_[241].translation_ = { -5.0f,0.0f,-19.0f };
	worldTransform_[242].translation_ = { -4.0f,0.0f,-19.0f };
	worldTransform_[243].translation_ = { -3.0f,0.0f,-19.0f };
	worldTransform_[244].translation_ = { -2.0f,0.0f,-19.0f };
	worldTransform_[245].translation_ = { -1.0f,0.0f,-19.0f };


	//----------------------------------------------------------//

	//-------------------------//

	input_=Input::GetInstance();

}

void Stage::Update()
{
	//行列を更新
	for (int i = 0; i < 281; i++)
	{
		worldTransform_[i].UpdateMatrix();
	}
	if (input_->PushKey(DIK_A))
	{
		worldTransform_[5].translation_.x -= 0.1f;
	}
}

void Stage::Draw(ViewProjection& viewProjection)
{
	//モデルの描画
	for (int i = 0; i < 281; i++)
	{
		model_[i]->Draw(worldTransform_[i], viewProjection);
	}
	
}
