package com.giant.crazy.share;

import android.app.Activity;
import android.graphics.BitmapFactory;

import com.umeng.socialize.bean.SHARE_MEDIA;
import com.umeng.socialize.controller.UMServiceFactory;
import com.umeng.socialize.controller.UMSocialService;
import com.umeng.socialize.media.UMImage;
import com.umeng.socialize.sso.SinaSsoHandler;
import com.umeng.socialize.sso.TencentWBSsoHandler;
import com.umeng.socialize.sso.UMQQSsoHandler;
import com.umeng.socialize.weixin.controller.UMWXHandler;

public class UMengShare {

	private static UMengShare _instance;
	final static UMSocialService mController = UMServiceFactory
			.getUMSocialService("com.umeng.share");
	Activity activity;

	/* 分享的内容和分享的图片路径 */
	String content;
	String path;
	String title = "疯狂炸弹人";
	String appPage = "";

	public static UMengShare getInstance() {
		if (_instance == null) {
			_instance = new UMengShare();
			// 配置SSO
			mController.getConfig().setSsoHandler(new SinaSsoHandler());
			mController.getConfig().setSsoHandler(new TencentWBSsoHandler());
			mController.getConfig().removeSsoHandler(SHARE_MEDIA.SINA);
			mController.setShareContent(_instance.content);
		}
		return _instance;
	}

	public void share(Activity activity, String path) {
		this.activity = activity;
		this.path = path;
		mController.getConfig().setPlatforms(SHARE_MEDIA.WEIXIN,
				SHARE_MEDIA.WEIXIN_CIRCLE, SHARE_MEDIA.QZONE, SHARE_MEDIA.QQ,
				SHARE_MEDIA.SINA);
		addQQPlatform();
		addWXPlatform();
		mController.openShare(activity, false);
	}

	public void addWXPlatform() {
		String appId = "wxd0b1a7d9f67f864b";
		mController.setShareImage(new UMImage(activity, BitmapFactory
				.decodeFile(path)));

		// 添加微信平台
		UMWXHandler wxHandler = new UMWXHandler(activity, appId);
		wxHandler.addToSocialSDK();
		wxHandler.setTitle(title);

		// 朋友圈
		UMWXHandler wxCircleHandler = new UMWXHandler(activity, appId);
		wxCircleHandler.setToCircle(true);
		wxCircleHandler.addToSocialSDK();
		wxCircleHandler.setTitle(title);
	}

	public void addQQPlatform() {
		String appId = "1101950201";
		String appKey = "mrekKffu3njXCT0h";
		// 添加QQ支持, 并且设置QQ分享内容的target url
		UMQQSsoHandler qqSsoHandler = new UMQQSsoHandler(activity,
				appId, appKey);
		qqSsoHandler.setTargetUrl(appPage);
		qqSsoHandler.addToSocialSDK();
	}

}
