(function() {
  loadOptions();
  submitHandler();
})();

function submitHandler() {
  var $submitButton = $('#submitButton');

  $submitButton.on('click', function() {
    console.log('Submit');

    var return_to = getQueryParam('return_to', 'pebblejs://close#');
    document.location = return_to + encodeURIComponent(JSON.stringify(getAndStoreConfigData()));
  });
}

function loadOptions() {
  var $backgroundColorPicker = $('#backgroundColorPicker');
  var $hourColorPicker = $('#hourColorPicker');
  var $hourinColorPicker = $('#hourinColorPicker');
  var $minuteColorPicker = $('#minuteColorPicker');
  var $textColorPicker = $('#textColorPicker');

  if (localStorage.backgroundColor) {
    $backgroundColorPicker[0].value = localStorage.backgroundColor;
    $hourColorPicker[0].value = localStorage.hourColor;
    $hourinColorPicker[0].value = localStorage.hourinColor;
    $minuteColorPicker[0].value = localStorage.minuteColor;
    $textColorPicker[0].value = localStorage.textColor;
  }
}

function getAndStoreConfigData() {
  var $backgroundColorPicker = $('#backgroundColorPicker');
  var $hourColorPicker = $('#hourColorPicker');
  var $hourinColorPicker = $('#hourinColorPicker');
  var $minuteColorPicker = $('#minuteColorPicker');
  var $textColorPicker = $('#textColorPicker');

  var options = {
    backgroundColor: $backgroundColorPicker.val(),
    hourColor: $hourColorPicker.val(),
    hourinColor: $hourinColorPicker.val(),
    minuteColor: $minuteColorPicker.val(),
    textColor: $textColorPicker.val()
  };

  localStorage.backgroundColor = options.backgroundColor;
  localStorage.hourColor = options.hourColor;
  localStorage.hourinColor = options.hourinColor;
  localStorage.minuteColor = options.minuteColor;
  localStorage.textColor = options.textColor;

  console.log('Got options: ' + JSON.stringify(options));
  return options;
}

function getQueryParam(variable, defaultValue) {
  var query = location.search.substring(1);
  var vars = query.split('&');
  for (var i = 0; i < vars.length; i++) {
    var pair = vars[i].split('=');
    if (pair[0] === variable) {
      return decodeURIComponent(pair[1]);
    }
  }
  return defaultValue || false;
}
