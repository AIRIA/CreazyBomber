package com.giant.crazy.pay;

import java.util.ArrayList;
import java.util.List;

import net.umipay.android.GameParamInfo;
import net.umipay.android.UmiPaySDKManager;
import net.umipay.android.UmiPaymentInfo;
import net.umipay.android.UmipayOrderInfo;
import net.umipay.android.UmipaySDKStatusCode;
import net.umipay.android.interfaces.InitCallbackListener;
import net.umipay.android.interfaces.OrderReceiverListener;
import net.umipay.android.interfaces.PayProcessListener;
import android.content.Context;
import android.util.Log;
import android.widget.Toast;

public class UmiPayManager {
	
	static Context context;
	static final String TAG = "Crazy Bomber";

	public static void init(Context ctx){
		context = ctx;
	    GameParamInfo gameParamInfo = new GameParamInfo();
	    gameParamInfo.setAppId("a4b8e736cb33d417");
	    gameParamInfo.setAppSecret("b3f07b604b6f126e");
	    //设置成True时，接收SDK回调。 设置成False，不接收SDK回调(接口仍要实现，以便代码能编译通过)
	    gameParamInfo.setSDKCallBack(true);
	    //初始化结果回调接口
	    final InitCallbackListener initCallbackListener  = new InitCallbackListener() {
	        @Override
	        public void onInitCallback(int code, String msg) {
	            if (code == UmipaySDKStatusCode.SUCCESS) {
	                Log.e(TAG, "支付sdk 初始化成功"+msg);
	            } else if (code == UmipaySDKStatusCode.INIT_FAIL) {
	            	Log.e(TAG, "支付sdk 初始化失败 "+msg);
	            	Toast.makeText(context, "初始化失败,请检查网络是否连接正常", Toast.LENGTH_SHORT).show();
	            }
	        }
	    };
	    //订单回调接口
	    final OrderReceiverListener orderReceiverListener = new OrderReceiverListener() {
			@Override
	        public List onReceiveOrders(List list) {
				Log.v(TAG, "order receiver listener");
	            @SuppressWarnings("unchecked")
				List<UmipayOrderInfo> newOrderList = list;
	            List<UmipayOrderInfo> doneOrderList = new ArrayList<UmipayOrderInfo>();
	            for (UmipayOrderInfo newOrder : newOrderList) {
	                try {
	                    if (newOrder.getStatus()==1) {
	                        doneOrderList.add(newOrder);
	                    }
	                } catch (Exception e) {

	                }
	            }
	            return doneOrderList;
	        }
	    };
	    
	    
	    UmiPaySDKManager.setPayProcessListener(new PayProcessListener() {
            @Override
            public void OnPayProcess(int code) {
                switch (code) {
                    case UmipaySDKStatusCode.PAY_PROCESS_SUCCESS :
                        //充值完成，不等于充值成功，实际充值结果以订单回调接口为准
                        Toast.makeText(UmiPayManager.context.getApplicationContext(),"充值完成！请耐心等候充值结果", Toast.LENGTH_SHORT).show();
                        break;
                    case UmipaySDKStatusCode.PAY_PROCESS_FAIL :
                        Toast.makeText(UmiPayManager.context.getApplicationContext(),"取消充值！", Toast.LENGTH_SHORT).show();
                        break;
                }
            }
        });
	    
	    UmiPaySDKManager.initSDK(ctx,gameParamInfo, initCallbackListener, orderReceiverListener);
	}

	
	/**
	 * 显示出来支付界面
	 */
	public static void showPayView(){
        UmiPaymentInfo paymentInfo = new UmiPaymentInfo();
        paymentInfo.setServiceType(UmiPaymentInfo.SERVICE_TYPE_QUOTA);
        paymentInfo.setPayMoney(0);
        paymentInfo.setDesc("1000金币");
        UmiPaySDKManager.showPayView(context, paymentInfo);
	}
	
}
