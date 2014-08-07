package com.giant.crazy.jni;

import net.youmi.android.offers.OffersManager;
import net.youmi.android.offers.PointsManager;
import net.youmi.android.spot.SpotManager;

import org.json.JSONException;
import org.json.JSONObject;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.util.Log;
import android.widget.Toast;

import com.android.vending.billing.util.IabHelper;
import com.android.vending.billing.util.IabResult;
import com.android.vending.billing.util.Inventory;
import com.android.vending.billing.util.Purchase;
import com.giant.crazy.share.UMengShare;
import com.giant.creazybomber.R;
import com.umeng.analytics.game.UMGameAgent;

public class JniBrige {

	public native void payHandler();
	
	private static JniBrige _instance;
	static String TAG = "Crazy Bomber";
	String base64EncodedPublicKey;
	public IabHelper mHelper;
	static final String SKU_PREMIUM = "premium";
	static final int RC_REQUEST = 10001;
	boolean mIsPremium = false;
	Activity context;

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
		base64EncodedPublicKey = ctx.getResources().getString(R.string.pub_key);
		mHelper = new IabHelper(ctx, base64EncodedPublicKey);
		mHelper.enableDebugLogging(false);
		Log.d(TAG, "Starting setup.");

		/* 监听是否可以连接 google play 服务 */
		mHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
			@Override
			public void onIabSetupFinished(IabResult result) {
				Log.d(TAG, "Setup finished.");
				if (!result.isSuccess()) {
					complain("Problem setting up in-app billing: " + result);
					return;
				}
				if (mHelper == null)
					return;

				Log.d(TAG, "Setup successful. Querying inventory.");
				/* 连接成功之后 获取物品信息 检查商品是否可以使用 */
				mHelper.queryInventoryAsync(mGotInventoryListener);
			}
		});

	}

	IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {

		@Override
		public void onQueryInventoryFinished(IabResult result,
				Inventory inventory) {
			Log.d(TAG, "Query inventory finished.");

			if (mHelper == null)
				return;

			if (result.isFailure()) {
				// complain("Failed to query inventory: " + result);
				return;
			}

			Log.d(TAG, "Query inventory was successful.");

			Purchase coinPurchase = inventory.getPurchase(context
					.getResources().getString(R.string.SKU_GOLD_COIN));
			String hasItem = coinPurchase == null ? "no item" : "find item";
			Log.d(TAG, hasItem);
		}
	};

	// Callback for when a purchase is finished
	IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
		public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
			if (mHelper == null)
				return;
			if (result.isFailure()) {
				complain("Error purchasing: " + result);
				return;
			}
			Log.d(TAG, "Purchase successful.");

			if (purchase.getSku().equals(
					context.getResources().getString(R.string.SKU_GOLD_COIN))) {
				mHelper.consumeAsync(purchase, mConsumeFinishedListener);
			}
		}
	};

	// Called when consumption is complete
	IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
		public void onConsumeFinished(Purchase purchase, IabResult result) {
			if (mHelper == null)
				return;

			if (result.isSuccess()) {
				payHandler();
			} else {
				complain("Error while consuming: " + result);
			}
			Log.d(TAG, "End consumption flow.");
		}
	};

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
		Log.v(TAG, "invoke show ads method");
		context.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				boolean type = true;
				if(type){
//					UmiPayManager.showPayView();
					doSdkShowOffersWall(params);
				}else{
					String payLoad = "";
					mHelper.launchPurchaseFlow(context, context.getResources()
							.getString(R.string.SKU_GOLD_COIN), RC_REQUEST,
							mPurchaseFinishedListener, payLoad);
				}
			}
		});
	}

	public void doSdkToast(final String params) {
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(context, params, Toast.LENGTH_LONG).show();
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
		Log.v(TAG, "invoke sdk share");
		context.runOnUiThread(new Runnable() {
			@Override
			public void run() {
				UMengShare.getInstance().share(context, params);
			}
		});
	}
	
	public void dispose() {
		if (mHelper != null)
			mHelper.dispose();
		mHelper = null;
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
