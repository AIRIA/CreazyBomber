package com.giant.crazy.net;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.util.EntityUtils;

public class NetManager {
	
	public static String sendHttpRequest(String server){
		HttpClient client = new DefaultHttpClient();
		HttpGet request = new HttpGet(server);
		String result = null;
		try {
			HttpResponse response = client.execute(request);
			if(response.getStatusLine().getStatusCode()==200){
				HttpEntity httpEntity = response.getEntity();  
                result = EntityUtils.toString(httpEntity);
                result.replaceAll("\r", "");
			}else{
				request.abort();
			}
		} catch (ClientProtocolException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
		return result;
	}
	
	public static String sendHttpRequest(String serverUrl, String params) {
		HttpURLConnection urlCon = null;
		try {
			URL url = new URL(serverUrl);
			urlCon = (HttpURLConnection) url.openConnection();
			urlCon.setRequestMethod("GET");
			urlCon.setDoOutput(true);
			urlCon.setConnectTimeout(30000);
			urlCon.setReadTimeout(30000);
			OutputStream oStream = urlCon.getOutputStream();
			DataOutputStream dataOutStream = new DataOutputStream(oStream);
			dataOutStream.writeUTF(params);
			dataOutStream.flush();
			dataOutStream.close();

			if (urlCon.getResponseCode() == 200) {
				DataInputStream dataInStream = new DataInputStream(
						urlCon.getInputStream());
				String res = dataInStream.readUTF();
				System.out.print(res);
				return res;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}
}
