package cz.growmat.growmatweb;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.Calendar;
import java.util.Date;

import android.app.AlarmManager;
import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.PowerManager;

import android.support.v4.app.NotificationCompat;
import android.support.v4.app.TaskStackBuilder;
import android.util.Log;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import static android.content.Context.NOTIFICATION_SERVICE;


public class AlarmManagerBroadcastReceiver extends BroadcastReceiver {

    final public static String TAG = AlarmManagerBroadcastReceiver.class.getName();
    public static final String PREFS_NAME = "GrowmatWeb";

    String sAlarm = "";
    String sLastAlarm = "";
    String sCreated_at = "";

    String response;

    static public StringBuffer request(String urlString) {

        StringBuffer buffer = new StringBuffer("");
        try{
            URL url = new URL(urlString);
            HttpURLConnection connection = (HttpURLConnection)url.openConnection();
            connection.setRequestProperty("User-Agent", "");
            //connection.setRequestMethod("POST");
            connection.setRequestMethod("GET");
            connection.setDoInput(true);
            connection.connect();

            InputStream inputStream = connection.getInputStream();

            BufferedReader rd = new BufferedReader(new InputStreamReader(inputStream));
            String line = "";
            while ((line = rd.readLine()) != null) {
                buffer.append(line);
            }
        }
        catch (IOException e) {
            e.printStackTrace();
        }
        return buffer;
    }

	@Override
	public void onReceive(Context context, Intent intent) {

        SharedPreferences settings = context.getSharedPreferences(PREFS_NAME, 0);
        sLastAlarm = settings.getString("sLastAlarm", "");

        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                try  {
                    //response = request("https://api.thingspeak.com/channels/222602/fields/1/last").toString();
                    response = request("https://api.thingspeak.com/channels/337563/fields/1.json?api_key=ME6K2DRYYIEZX7VO&results=1").toString();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });
        thread.start();
        try {
            thread.join();
        } catch (Exception e) {
            e.printStackTrace();
        }

        try {
            JSONObject jObject = new JSONObject(response);
            JSONArray jFeedsArray = jObject.getJSONArray("feeds");
            JSONObject oneObject = jFeedsArray.getJSONObject(0);
            sAlarm = oneObject.getString("field1");
            sCreated_at = oneObject.getString("created_at");
        } catch (JSONException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }

        Log.d(TAG, "lastAlarm: " + sLastAlarm);
        Log.d(TAG, "alarm: " + sAlarm);
        Log.d(TAG, "created_at: " + sCreated_at);
/*
        //2017-09-28T07:19:43Z
        sCreated_at = sCreated_at.replace('T', ' ');
        sCreated_at = sCreated_at.replace('Z', ' ');
        SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss ");
        try {
            Date d = format.parse(sCreated_at);
            Log.d(TAG, d.toString());

            if(Calendar.getInstance().getTime().getTime() - d.getTime() > 300000) {
                Log.d(TAG, "OFFLINE");
                sAlarm = "-1";
            }

        } catch (Exception e) {
            e.printStackTrace();
        }*/
        if(!GrowmatUtils.isOnline(sCreated_at)) {
            sAlarm = "-1";
        }

        if(sAlarm.equals(sLastAlarm)) {
        }
        else {
            sLastAlarm = sAlarm;
            SharedPreferences.Editor editor = settings.edit();
            editor.putString("sLastAlarm", sLastAlarm);
            editor.commit();

            NotificationCompat.Builder mBuilder =
                    new NotificationCompat.Builder(context)
                            .setSmallIcon(R.drawable.ic_launcher)
                            .setContentTitle("GROWMAT WEB")
                            .setContentText(GrowmatUtils.getAlarmName(sAlarm));

            Intent resultIntent = new Intent(context, AlarmManagerActivity.class);
            TaskStackBuilder stackBuilder = TaskStackBuilder.create(context);
            stackBuilder.addParentStack(AlarmManagerActivity.class);
            stackBuilder.addNextIntent(resultIntent);
            PendingIntent resultPendingIntent = stackBuilder.getPendingIntent(0, PendingIntent.FLAG_UPDATE_CURRENT);
            mBuilder.setContentIntent(resultPendingIntent);

            Notification note = mBuilder.build();

            note.defaults |= Notification.DEFAULT_VIBRATE;
            note.defaults |= Notification.DEFAULT_SOUND;

            NotificationManager mNotificationManager = (NotificationManager) context.getSystemService(Context.NOTIFICATION_SERVICE);
            //mNotificationManager.notify(9011, mBuilder.build());
            mNotificationManager.notify(9011, note);
        }


	/*	 PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
         PowerManager.WakeLock wl = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, "YOUR TAG");
         //Acquire the lock
         wl.acquire();

         //You can do the processing here update the widget/remote views.
         Bundle extras = intent.getExtras();
         StringBuilder msgStr = new StringBuilder();
         
         if(extras != null && extras.getBoolean(ONE_TIME, Boolean.FALSE)){
        	 msgStr.append("One time Timer : ");
         }
         Format formatter = new SimpleDateFormat("hh:mm:ss a");
         msgStr.append(formatter.format(new Date()));

         Toast.makeText(context, msgStr, Toast.LENGTH_LONG).show();
         
         //Release the lock
         wl.release();*/
         
	}
	public void SetAlarm(Context context)
    {
        ComponentName receiver = new ComponentName(context, DeviceBootReceiver.class);
        context.getPackageManager().setComponentEnabledSetting(receiver,
                PackageManager.COMPONENT_ENABLED_STATE_ENABLED,
                PackageManager.DONT_KILL_APP);

        AlarmManager am=(AlarmManager)context.getSystemService(Context.ALARM_SERVICE);
        Intent intent = new Intent(context, AlarmManagerBroadcastReceiver.class);
        //intent.putExtra(ONE_TIME, Boolean.FALSE);
        PendingIntent pi = PendingIntent.getBroadcast(context, 0, intent, 0);
        //After after 30 seconds
        am.setInexactRepeating(AlarmManager.RTC_WAKEUP, System.currentTimeMillis(), 1000 * 5 , pi);

        SharedPreferences settings = context.getSharedPreferences(PREFS_NAME, 0);
        SharedPreferences.Editor editor = settings.edit();
        editor.putString("sLastAlarm", "");
        editor.commit();
    }

    public void CancelAlarm(Context context)

    {

        ComponentName receiver = new ComponentName(context, DeviceBootReceiver.class);
        context.getPackageManager().setComponentEnabledSetting(receiver,
                PackageManager.COMPONENT_ENABLED_STATE_DISABLED,
                PackageManager.DONT_KILL_APP);

        Intent intent = new Intent(context, AlarmManagerBroadcastReceiver.class);
        PendingIntent sender = PendingIntent.getBroadcast(context, 0, intent, 0);
        AlarmManager alarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
        alarmManager.cancel(sender);
    }

    public void setOnetimeTimer(Context context){
    	AlarmManager am=(AlarmManager)context.getSystemService(Context.ALARM_SERVICE);
        Intent intent = new Intent(context, AlarmManagerBroadcastReceiver.class);
        //intent.putExtra(ONE_TIME, Boolean.TRUE);
        PendingIntent pi = PendingIntent.getBroadcast(context, 0, intent, 0);
        am.set(AlarmManager.RTC_WAKEUP, System.currentTimeMillis(), pi);
    }
}
