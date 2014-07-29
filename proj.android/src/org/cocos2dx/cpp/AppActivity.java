/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
 ****************************************************************************/
package org.cocos2dx.cpp;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.KeyEvent;

import com.giant.crazy.jni.JniBrige;

public class AppActivity extends Cocos2dxActivity {
	private AlertDialog exitDialog;
	static String TAG = "Crazy Bomber";
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		JniBrige.getInstance().init(this);
	}

	@Override
	public boolean onKeyUp(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			exitDialog = new AlertDialog.Builder(this).setTitle("Crazy Bomber")
					.setMessage("Do you really want to quit the game?")
					.setPositiveButton("Continue", new OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							exitDialog.dismiss();
						}
					}).setNegativeButton("Quit", new OnClickListener() {

						@Override
						public void onClick(DialogInterface dialog, int which) {
							android.os.Process.killProcess(android.os.Process
									.myPid());
						}
					}).show();
		}
		return super.onKeyUp(keyCode, event);
	}

	@Override
	 protected void onActivityResult(int requestCode, int resultCode, Intent data) {
	        Log.d(TAG, "onActivityResult(" + requestCode + "," + resultCode + "," + data);
	        if (JniBrige.getInstance().mHelper == null) return;

	        if (!JniBrige.getInstance().mHelper.handleActivityResult(requestCode, resultCode, data)) {
	            super.onActivityResult(requestCode, resultCode, data);
	        }
	        else {
	            Log.d(TAG, "onActivityResult handled by IABUtil.");
	        }
	 }
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		JniBrige.getInstance().dispose();
	}
	
}
