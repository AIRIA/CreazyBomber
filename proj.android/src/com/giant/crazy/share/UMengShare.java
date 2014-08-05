package com.giant.crazy.share;

import android.app.Activity;
import android.graphics.BitmapFactory;
import android.util.Log;

import com.umeng.socialize.bean.SHARE_MEDIA;
import com.umeng.socialize.bean.SocializeEntity;
import com.umeng.socialize.controller.UMServiceFactory;
import com.umeng.socialize.controller.UMSocialService;
import com.umeng.socialize.controller.listener.SocializeListeners.SnsPostListener;
import com.umeng.socialize.media.QQShareContent;
import com.umeng.socialize.media.QZoneShareContent;
import com.umeng.socialize.media.UMImage;
import com.umeng.socialize.sso.QZoneSsoHandler;
import com.umeng.socialize.sso.UMQQSsoHandler;
import com.umeng.socialize.weixin.controller.UMWXHandler;
import com.umeng.socialize.weixin.media.CircleShareContent;
import com.umeng.socialize.weixin.media.WeiXinShareContent;

public class UMengShare {

	private static UMengShare _instance;
	final static UMSocialService mController = UMServiceFactory
			.getUMSocialService("com.umeng.share");
	Activity activity;
	
	static String TAG = "Crazy Bomber";

	/* 分享的内容和分享的图片路径 */
	String content = "当和谐的世界被打破，怪物横行。来自各界的四位代表正义的勇士将带你突破重重困难，战胜怪物，还世界以和平！精美的画面、简单的操作、趣味无穷的关卡、节奏感非常强的音乐，所有这一切尽在疯狂炸弹人 Crazy Bomber中。";
	String path;
	String title = "疯狂炸弹人";
	String appPage = "https://play.google.com/store/apps/details?id=com.giant.creazybomber";

	UMImage shareImage;
	
	public static UMengShare getInstance() {
		if (_instance == null) {
			_instance = new UMengShare();
		}
		return _instance;
	}

	public void share(Activity activity, String path) {
		this.activity = activity;
		this.path = path;
		mController.getConfig().setPlatforms(SHARE_MEDIA.WEIXIN,
				SHARE_MEDIA.WEIXIN_CIRCLE, SHARE_MEDIA.QZONE, SHARE_MEDIA.QQ,
				SHARE_MEDIA.SINA);
		
		shareImage = new UMImage(activity, BitmapFactory.decodeFile(path));
		
		mController.setShareContent(content);
		mController.setAppWebSite(appPage);
		
		addQQPlatform();	
		addWXPlatform();
		
		mController.openShare(activity, new SnsPostListener() {
			
			@Override
			public void onStart() {
				Log.v(TAG, "share start");
				
			}
			
			@Override
			public void onComplete(SHARE_MEDIA arg0, int arg1, SocializeEntity arg2) {
				Log.v(TAG, "share complete");
				
			}
		});
	}

	public void addWXPlatform() {
		String appId = "wxcf4ab54c33ca1c12";

		// 添加微信平台
		UMWXHandler wxHandler = new UMWXHandler(activity, appId);
		wxHandler.addToSocialSDK();
		WeiXinShareContent weixinContent = new WeiXinShareContent();
		weixinContent.setShareImage(shareImage);
		weixinContent.setShareContent(content);
		weixinContent.setTitle(title);
		weixinContent.setTargetUrl(appPage);
		mController.setShareMedia(weixinContent);

		// 支持微信朋友圈
		UMWXHandler wxCircleHandler = new UMWXHandler(activity, appId);
		wxCircleHandler.setToCircle(true);
		wxCircleHandler.addToSocialSDK();
		CircleShareContent circleMedia = new CircleShareContent();
		circleMedia.setShareImage(shareImage);
		circleMedia.setShareContent(content);
		circleMedia.setTitle(title);
		circleMedia.setTargetUrl(appPage);
		mController.setShareMedia(circleMedia);
	}

	public void addQQPlatform() {
		String appId = "1101950201";
		String appKey = "mrekKffu3njXCT0h";
		// 添加QQ支持, 并且设置QQ分享内容的target url
		UMQQSsoHandler qqSsoHandler = new UMQQSsoHandler(activity, appId,
				appKey);
		qqSsoHandler.addToSocialSDK();  
		QQShareContent qqShareContent = new QQShareContent();
		qqShareContent.setShareContent(content);
		qqShareContent.setTitle(title);
		qqShareContent.setShareImage(shareImage);
		qqShareContent.setTargetUrl(appPage);
		mController.setShareMedia(qqShareContent);
		

		// 参数1为当前Activity， 参数2为开发者在QQ互联申请的APP ID，
		// 参数3为开发者在QQ互联申请的APP kEY.
		QZoneSsoHandler qZoneSsoHandler = new QZoneSsoHandler(activity,
				appId, appKey);
		qZoneSsoHandler.addToSocialSDK();
		QZoneShareContent qzone = new QZoneShareContent();
		qzone.setShareContent(content);
		qzone.setTargetUrl(appPage);
		qzone.setShareImage(shareImage);
		qzone.setTitle(title);
		mController.setShareMedia(qzone);

	}

}
