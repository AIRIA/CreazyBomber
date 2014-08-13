package com.giant.crazy.jni;

import org.json.JSONException;
import org.json.JSONObject;

import a.b.c.os.OffersManager;
import a.b.c.os.PointsManager;
import a.b.c.st.SpotManager;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.util.Log;
import android.widget.Toast;

import com.giant.crazy.net.NetManager;
import com.umeng.analytics.game.UMGameAgent;

public class JniBrige {

	public native void payHandler();
	
	private static JniBrige _instance;
	static String TAG = "Crazy Bomber";
	String base64EncodedPublicKey;
	static final String SKU_PREMIUM = "premium";
	static final int RC_REQUEST = 10001;
	boolean mIsPremium = false;
	Activity context;
	boolean enableAds = false;

	public static Object instance() {
		return getInstance();
	}

	public static JniBrige getInstance() {
		if (_instance == null) {
			_instance = new JniBrige();
			Log.v(TAG, "instance init success");
		}
		return _instance;
	}

	/**
	 * 初始化服务 判断是否可以连接google play
	 * 
	 * @param ctx
	 */
	public void init(Activity ctx) {
		context = ctx;
		Log.d(TAG, "Starting setup.");
		
		context.runOnUiThread(new Runnable() {
			public void run() {
				String res = NetManager.sendHttpRequest("https://gist.githubusercontent.com/AIRIA/7c37b4d444636848de4d/raw/flag.json");
				if(res!=null){
					/* 检查是否开启了广告 */
					try {
						JSONObject json = new JSONObject(res);
//						enableAds = json.getBoolean("meizu");
						enableAds = json.getBoolean("xiaomi_v5");
						Log.v(TAG, res);
					} catch (JSONException e) {
						e.printStackTrace();
					}
						
				}
			}
		});

	}


	void complain(String message) {
		Log.e(TAG, "**** CrazyBomber Error: " + message);
//		alert("Error: " + message);
	}

	void alert(String message) {
		AlertDialog.Builder bld = new AlertDialog.Builder(context);
		bld.setMessage(message);
		bld.setNeutralButton("OK", null);
		Log.d(TAG, "Showing alert dialog: " + message);
		bld.create().show();
	}

	public void doSdkPay(final String params) {
		Log.v(TAG, "invoke pay method");
		context.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				doSdkShowOffersWall(params);
			}
		});
	}

	public void doSdkToast(final String params) {
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(context, params, Toast.LENGTH_SHORT).show();
			}
		});
	}
	/**
	 * 获取玩家的积分信息
	 * @return
	 */
	public int doSdkGetPoint(){
		int point = PointsManager.getInstance(context).queryPoints();
		return point;
	}
	
	AlertDialog exitDialog;
	public void doSdkPayConfirm(final String params){
		if(enableAds==false)
			return;
		
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				exitDialog = new AlertDialog.Builder(context).setTitle("疯狂炸弹人")
						.setMessage("积分不足,是否要使用积分兑换金币?\n"
								+ "100积分 = 1000 金币")
						.setPositiveButton("兑换", new OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								buyCoin();
							}
						}).setNegativeButton("取消", new OnClickListener() {

							@Override
							public void onClick(DialogInterface dialog, int which) {
								exitDialog.dismiss();
								
							}
						}).show();
			}
		});
	}
	AlertDialog confirmDialog;
	public void buyCoin(){
		int point = doSdkGetPoint();
		
		if(point>=100){
			PointsManager.getInstance(context).spendPoints(100);
			payHandler();
		}else{
			context.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					confirmDialog = new AlertDialog.Builder(context).setTitle("疯狂炸弹人")
							.setMessage("积分不足,是否要赚取积分 获得积分后重新进入商城界面就会使用获得的积分兑换金币?\n"
									+ "100积分 = 1000 金币")
							.setPositiveButton("赚取", new OnClickListener() {
								@Override
								public void onClick(DialogInterface dialog, int which) {
									doSdkPay("");
								}
							}).setNegativeButton("取消", new OnClickListener() {

								@Override
								public void onClick(DialogInterface dialog, int which) {
									exitDialog.dismiss();
								}
							}).show();
					
				}
			});
		}
	}

	
	public void doSdkAnalyze(String params){
		Log.v(TAG, params);
		try {
			JSONObject param = new JSONObject(params);
			int key = param.getInt("key");
			String value = param.getString("value");
			switch (key) {
			case 1: //start level
				UMGameAgent.startLevel(value);
				break;
			case 2: //faild level
				UMGameAgent.failLevel(value);
				break;
			case 3: //finish level
				UMGameAgent.finishLevel(value);
				break;
			default:
				break;
			}
		} catch (JSONException e) {
			Log.v(TAG, "json format error");
			e.printStackTrace();
		}
	}
	
	public void doSdkShare(final String params)
	{
		/* 没有相应的推广地址 暂不分享 */
//		doSdkToast("与更多的好友分享游戏吧~");
//		Log.v(TAG, "invoke sdk share");
//		context.runOnUiThread(new Runnable() {
//			@Override
//			public void run() {
//				UMengShare.getInstance().share(context, params);
//			}
//		});
	}
	
	public void dispose() {
		
	}

//--------------------------------------------------------------------------------------
	/**
	 * 显示迷你广告条
	 * @param params
	 */
	public void doSdkShowMiniAds(String params){
		
	}
	
	/**
	 * 显示插屏广告
	 * @param params
	 */
	public void doSdkShowSpotAds(String params){
		if(enableAds==false)
			return;
		Log.v(TAG, "invoke sdk show spot ads");
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				SpotManager.getInstance(context).showSpotAds(context);
			}
		});
	}
	
	/**
	 * 显示积分墙
	 * @param params
	 */
	public void doSdkShowOffersWall(String params){
		Log.v(TAG, "invoke show offerswall method");
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				OffersManager.getInstance(context).showOffersWall();
				// 积分墙配置检查（没有使用“通过 SDK 获取积分订单”功能）：
				boolean isSuccess = OffersManager.getInstance(context).checkOffersAdConfig();
				if(isSuccess){
					Log.v(TAG, "config ok");
				}
				// 积分墙配置检查（使用“通过 SDK 获取积分订单”功能）：
				//boolean isSuccess = OffersManager.getInstance(context).checkOffersAdConfig(true);
			}
		});
	}

}
