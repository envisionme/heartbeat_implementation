
Drupal.heartbeat = Drupal.heartbeat || {};

Drupal.behaviors.heartbeatCustom = function(context) {
  
}

Drupal.heartbeat.toggleComments = function(element) {
  var elements = $(element).parents('.heartbeat-message-block').find('.heartbeat-comments');
  //elements.toggle('slow');
  if (elements.is(":hidden")) {
    elements.fadeIn("slow");
  } else {
    elements.fadeOut("slow");
  }  
}

Drupal.heartbeat.toggleVideo = function(element) {
  $(element).hide('slow');
  $(element).parents('.beat-item').find('.heartbeat-video-wrapper').show('slow');
}

Drupal.heartbeat.toggleBigStream = function(element) {
  $('#heartbeat-bigstream-buttons li').each(function(){
    $(this).css('background-color', '#ededed');
  });
  $('li.' + element).css('background-color', '#cccccc');
  $('.block-heartbeat').hide('fast');
  $('#block-heartbeat-' + element).show('fast');
}

Drupal.heartbeat.tabbedStreams = null;

Drupal.heartbeat.toggleTabbedStreams = function(n) {
  
  for (i in Drupal.heartbeat.tabbedStreams) {
    Drupal.heartbeat.tabbedStreams[i].stream.hide();    
  }
  Drupal.heartbeat.tabbedStreams[n].stream.show('slow');
  
}

Drupal.heartbeat.createTabbedStreams = function() {
  
  if ($('.block-heartbeat', $('#right-sidebar')).length <= 1) {
    return false;
  }
  
  var tabs = new Array();
  $('.block-heartbeat').each(function(i) {
    var heartbeatBlock = $(this);
    tabs[i] = {
      title: $('h2', heartbeatBlock),
      stream: $('.heartbeat-stream ', heartbeatBlock)
    };
  });
  Drupal.heartbeat.tabbedStreams = tabs;
  
  var html = '<ul class="heartbeat-tabs">';  
  for (var n in tabs) {
    html += '<li><a href="#" class="heartbeat-tab" onclick="javascript:Drupal.heartbeat.toggleTabbedStreams(' + n + '); return false;">' + tabs[n].title.text() + '</a></li>';
    if (n == 0) {      
      tabs[n].stream.hide('fast');      
    }
    tabs[n].title.remove();
  }
  html += '</ul>'

  $('#right-sidebar').prepend(html);
  
}

function afterVideo(obj) {
  var newstate = obj.newstate;
  if(obj.newstate == "COMPLETED"){
    var element = $('#' + obj.id).parent();
    $(element).hide('slow');
    $(element).parents('.beat-item').find('.heartbeat-video-toggle').show('slow');
  }
}

function playerReady(thePlayer) {
  player = document.getElementById(thePlayer.id);
  player.addModelListener("STATE", "afterVideo");
}

$(document).ready(function() {

  $('.block-shouts .resizable-textarea').removeClass('resizable-textarea').addClass('resizable-testarea');
  
  $('.block-shouts textarea.resizable').bind('focus', function() {
    this.innerHTML = '';
    $('.block-shouts .resizable-testarea').removeClass('resizable-testarea').addClass('resizable-textarea');
    $(this).parents('.block-shouts').find('.shout-submit').show('fast');
    
  });
  
  Drupal.heartbeat.createTabbedStreams();
  
});
