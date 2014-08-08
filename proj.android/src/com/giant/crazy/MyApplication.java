package com.giant.crazy;

import java.util.Map.Entry;

import android.app.Application;
import android.content.Context;
import android.content.SharedPreferences;
import android.util.Log;
import android.widget.Toast;

import com.umeng.message.PushAgent;
import com.umeng.message.UmengNotificationClickHandler;
import com.umeng.message.entity.UMessage;

public class MyApplication extends Application{

	@Override
	public void onCreate() {
		// TODO Auto-generated method stub
		super.onCreate();
		PushAgent mPushAgent = PushAgent.getInstance(getApplicationContext());
		mPushAgent.setNotificationClickHandler(new UmengNotificationClickHandler(){

			@Override
			public void launchApp(Context arg0, UMessage arg1) {
				super.launchApp(arg0, arg1);
				Log.v("Crazy Bomber", "application oncreate");
			}

			@Override
			public void dealWithCustomAction(Context arg0, UMessage arg1) {
				super.dealWithCustomAction(arg0, arg1);
				super.launchApp(arg0, arg1);
				for (Entry<String, String> entry : arg1.extra.entrySet())
				{
				    String key = entry.getKey();
				    String value = entry.getValue();
				    if(key.equals("coin")){
				    	SharedPreferences datas = getSharedPreferences("Cocos2dxPrefsFile", 0);
				    	SharedPreferences.Editor editor = datas.edit();
				    	int coin = datas.getInt("key_coin_num", 0);
				    	coin += Integer.valueOf(value);
				    	editor.putInt("key_coin_num", coin);
				    	editor.commit();
				    	Toast.makeText(getApplicationContext(), "金币领取成功", Toast.LENGTH_LONG).show();
				    }
				}
			}
			
			
			
		});
	}

}
