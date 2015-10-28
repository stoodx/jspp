function verifyFile(){var t=null;if(t=readASCII(4),t!=FILE_SIGNATURE)return!1;var r=0;return r=readInt32(),1!=r?!1:!0}function readNextCommand(){var t=ioUtils.readCommandSize();if(void 0===t)return void 0;var r=ioUtils.readCommandId();commandsFactory.clearData();var e=commandsFactory.getCommand(r),i=e(t-1);return i.setCommandId(r),i}function readCommands(){for(var t=[],r=0;MAX_ITERATIONS>r;r++){var e=readNextCommand();if(void 0===e)break;e.data.relevant&&t.push(e)}return t}function process_snss_file(t){if(verifyFile()){var r=readCommands(),e=eventsAnalyzer.parse(r,t),i=JSON.constructor();return i.header=t.header,i.header.a_ver=ANALYZER_VERSION,i.events=e,saveJson(t),JSON.stringify(i)}}function main(){var t=null;t=loadJson(),t&&t.params&&t.params.lastEventTime>MAX_MILLISECONDS&&(t.params.lastEventTime=0,saveJson(t));var r=process_snss_file(t);return logger.info("Done"),r}var ANALYZER_VERSION=2,FILE_SIGNATURE="SNSS",MAX_ITERATIONS=1e4,MAX_MILLISECONDS=22089888e5,logger=function(){var t=function(t,r){c_log||(c_log=function(){}),c_log(r,t+"\n")},r=function(r){t(r,"debug")},e=function(r){t(r,"info")},i=function(r){t(r,"warn")},n=function(r){t(r,"error")};return{debug:r,info:e,warn:i,error:n}}(),commandsFactory=function(){function t(){var t=SessionCommandData({tab_id:r.tabId,index:r.index,url:r.url,title:r.title,web_history_item:r.webHistoryItem,timestamp:timeUtils.getTimestamp(r.itemSequenceNumber),transition_type:r.transitionType,has_post_data:r.hasPostData,referrer_url:r.referrerUrl,referrer_policy:r.referrerPolicy,request_url:r.requestUrl,is_overriding_user_agent:r.isOverridingUserAgent});return t.setRelevant(!0),t}var r={},e=function(t){ioUtils.resetPosition(t);var r=ioUtils.readInt(),e=ioUtils.readInt(),i=SessionCommandData({window_id:r,tab_id:e});return i},i=function(t){ioUtils.resetPosition(t);var r=ioUtils.readInt(),e=ioUtils.readInt(),i=SessionCommandData({index:e,tab_id:r});return i.setRelevant(!0),i},n=function(t){ioUtils.resetPosition(t);var r=ioUtils.readInt(),e=ioUtils.readInt(),i=SessionCommandData({tab_id:r,index:e});return i.setRelevant(!0),i},a=function(){for(var t=function(){var t=ioUtils.readInt();return{byteCount:t,data:ioUtils.readBytes(t)}},r=function(){return{file_path:ioUtils.readString16(),file_start:ioUtils.readLong(),file_length:ioUtils.readLong(),file_modification_time:ioUtils.readDouble()}},e=function(){return{blob_uuid:ioUtils.readString16()}},i=function(){return{filesystem_url:ioUtils.readString16(),file_start:ioUtils.readLong(),file_length:ioUtils.readLong(),file_modification_time:ioUtils.readDouble()}},n={0:t,1:r,2:e,3:i},a=ioUtils.readInt(),o=[],s=0;a>s;s++){var l=ioUtils.readInt(),u=n[l]();o.push({type:l,data:u})}return o},o=function(){ioUtils.readString16(),ioUtils.readString16(),ioUtils.readInt(),ioUtils.readInt(),ioUtils.readString16(),ioUtils.readStringVector(),ioUtils.readDouble();r.itemSequenceNumber||(r.itemSequenceNumber=ioUtils.readLong());var t=(ioUtils.readLong(),ioUtils.readLong(),ioUtils.readInt(),ioUtils.readDouble(),ioUtils.readDouble(),ioUtils.readBool()),e="";t===!0&&(e=ioUtils.readString16());for(var i=(a(),ioUtils.readString16(),ioUtils.readInt()),n=0;i>n;n++)o()},s=function(){var t=ioUtils.readInt(),r=ioUtils.getCurrentPosition();try{if(t>4){var e=ioUtils.readInt();if(-1==e){var i=ioUtils.readString8();return WebHistoryItem(i,void 0,void 0)}var n=ioUtils.readStringVector();return o(),WebHistoryItem(e,n,void 0)}return WebHistoryItem(void 0,void 0,void 0)}finally{var a=t-(ioUtils.getCurrentPosition()-r);ioUtils.skipBytes(a)}},l=function(e){ioUtils.resetPosition(e);try{ioUtils.skipBytes(4),r.tabId=ioUtils.readInt(),r.index=ioUtils.readInt(),r.url=ioUtils.readString8(),r.title=ioUtils.readStringWide16(),ioUtils.skipBytes(4),r.webHistoryItem=s(),r.transitionType=ioUtils.readInt(),r.hasPostData=ioUtils.readInt()>0,r.referrerUrl="",r.referrerPolicy=0,r.requestUrl="",r.isOverridingUserAgent=!1,r.referrerUrl=ioUtils.readString8(),r.referrerPolicy=ioUtils.readInt(),r.requestUrl=ioUtils.readString8(),r.isOverridingUserAgent=ioUtils.readInt()>0}catch(i){}finally{var n=e-ioUtils.getCurrentPosition();ioUtils.skipBytes(n)}return t()},u=function(t){ioUtils.resetPosition(t);var r=SessionCommandData({});return r.setRelevant(!0),ioUtils.skipBytes(t-ioUtils.getCurrentPosition()),r},d={0:e,1:u,2:i,5:u,6:l,7:n,8:n,9:u,10:u,11:u,12:u,13:u,14:u,15:u,16:u,17:u,18:u,19:u,20:u},f=function(t){return d[t]},c=function(){r={}};return{getCommand:f,clearData:c}}(),timeUtils=function(){var t=function(t){for(var r=t;r>MAX_MILLISECONDS;)r/=1e3;return r.toFixed(0)},r=function(t){return!t||isNaN(t)?1:parseInt(timeUtils.toMillis(t))};return{toMillis:t,getTimestamp:r}}(),StringUtils=function(){var t=function(t,r){var e=0,i=1,n=0;"Ã¾Ã¿"==t.slice(0,2)?(r=!0,e=2):"Ã¿Ã¾"==t.slice(0,2)&&(r=!1,e=2),r&&(i=0,n=1);for(var a="";e<t.length;e+=2){var o=t[e+i],s=t[e+n],l=(o<<8)+s;if(216>o||o>=224)a+=String.fromCharCode(l);else{e+=2;var u=t[e+i],d=t[e+n],f=(u<<8)+d;a+=String.fromCharCode(l,f)}}return a},r=function(t,r){if(r.length>t.length)return!1;for(var e=0;e<r.length;e++)if(t.charAt(e)!=r.charAt(e))return!1;return!0};return{toUtf16:t,startsWith:r}}(),ioUtils=function(){function t(t){if(r+t>e)throw"tried to read more than available, inc="+t+", commandSize: "+e;r+=t}var r=0,e=0,i=0,n=function(){return r},a=function(t){i+=r,r=0,e=t},o=function(){return i+r},s=function(){return t(4),readInt32()},l=function(){return t(2),readInt16()},u=function(r){t(r);var e=null;return e=readASCII(r)},d=function(){t(8);var r=null;return r=readInt64()},f=function(t){for(var r=[],e=0;t>e;e++){var i=null;i=readByte(),r.push(i)}return r},c=function(t){var r=0;return r=t%4===0?0:4-t%4},m=function(r){var e=s();if(0>=e)return"";var i=r?2*e:e,n=c(i);t(i);var a=f(i),o=null;return o=StringUtils.toUtf16(a,!1),y(n),o},v=function(){return m(!0)},U=function(){return m(!1)},g=function(){var r=s();if(0>=r)return"";var e=c(r);t(r);var i=null;return i=readASCII(r),y(e),i},I=function(){for(var t=s(),r=[],e=0;t>e;e++){var i=U();r.push(i)}return r},S=function(){var t=s();return 0!==t},_=function(){var t=null;return t=readInt16()},p=function(){var t=null;return t=readByte()},y=function(r){1>r||(t(r),skipBytes(r))},b=function(){s();y(8);var t=0;return t};return{readInt:s,readIntShort:l,readString16:U,readStringWide16:v,readString8:g,readStringVector:I,readBool:S,readCommandId:p,readCommandSize:_,skipBytes:y,readBytes:u,readDouble:b,readLong:d,getCurrentPosition:n,resetPosition:a,getGlobalPosition:o}}(),FrameState=function(t,r,e,i,n,a,o,s,l,u,d,f,c,m,v,U,g){var I=t,S=r,_=e,p=i,y=n,b=a,h=o,C=s,A=l,N=u,D=d,P=f,E=c,L=m,O=v,T=U,q=g;return{url:I,target:S,scrollOffsetY:_,scrollOffsetX:p,ref:y,documentState:b,pageScaleFactor:h,itemSequenceNumber:C,documentSequenceNumber:A,frameSequenceNumber:N,referrerPolicy:D,pinchViewPortScrollOffsetX:P,pinchViewPortScrollOffsetY:E,stateObject:L,bodyElementsArray:O,contentType:T,subItems:q}},SessionCommandData=function(t){var r=t;r.relevant=!1;var e=function(t){r.id=t},i=function(t){r.relevant=t};return{data:r,setCommandId:e,setRelevant:i}},WebHistoryItem=function(t,r,e){var i=t,n=r,a=e;return{version:i,referencedFiles:n,frameState:a}},eventsAnalyzer=function(){var t=function(t,e){return r(t,e)},r=function(t,r){for(var i=[],n={},o=parseInt(r.params.timeDifference),s=0,l=parseInt(r.params.lastEventTime),u=0;u<t.length;u++){var d=t[u];if(7==d.data.id)s=d.data.tab_id;else if(6==d.data.id){var f=d.data.url;if(n[d.data.tab_id]=f,e(f)){n[d.data.tab_id]="";continue}var c=d.data.timestamp,m=c+o;l=m;var v=i.length>0?i[i.length-1].q:"",U=d.data.referrer_url,g={time:m,q:f,prev:v,hreferer:U};i.push(g)}}var I=a(i,r.params.lastEventTime);return r.params.lastEventTime=l,I},e=function(t){return StringUtils.startsWith(t,"chrome://")},i=function(t,r){return t.time>r},n=function(t){return 1!=t.time},a=function(t,r){function e(t){var r=t.q+t.time;return-1==o.indexOf(r)?(o.push(r),!0):!1}for(var a=[],o=[],s=0;s<t.length;s++){var l=t[s];n(l)&&i(l,r)&&e(l)&&a.push(l)}return a};return{parse:t}}();