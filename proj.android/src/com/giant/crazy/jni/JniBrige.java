package com.giant.crazy.jni;

import android.app.Activity;
import android.app.AlertDialog;
import android.util.Log;
import android.widget.Toast;

import com.android.vending.billing.util.IabHelper;
import com.android.vending.billing.util.IabResult;
import com.android.vending.billing.util.Inventory;
import com.android.vending.billing.util.Purchase;
import com.giant.creazybomber.R;

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
		mHelper.enableDebugLogging(true);
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
		alert("Error: " + message);
	}

	void alert(String message) {
		AlertDialog.Builder bld = new AlertDialog.Builder(context);
		bld.setMessage(message);
		bld.setNeutralButton("OK", null);
		Log.d(TAG, "Showing alert dialog: " + message);
		bld.create().show();
	}

	public void doSdkPay(String params) {
		Log.v(TAG, "invoke show ads method");
		context.runOnUiThread(new Runnable() {

			@Override
			public void run() {
				String payLoad = "";
				mHelper.launchPurchaseFlow(context, context.getResources()
						.getString(R.string.SKU_GOLD_COIN), RC_REQUEST,
						mPurchaseFinishedListener, payLoad);
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

	public void dispose() {
		if (mHelper != null)
			mHelper.dispose();
		mHelper = null;
	}

}
