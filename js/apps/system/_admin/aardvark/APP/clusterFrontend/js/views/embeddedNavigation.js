/*jshint browser: true */
/*jshint unused: false */
/*global Backbone, templateEngine, $, window, arangoHelper*/
(function () {
  "use strict";
  window.EmbeddedNavigationView = Backbone.View.extend({

    el: '.clusterNavbar',
    el2: '.clusterNavbar ul',

    dummyData: {
      state: "okay",
      nodes: "7/7",
      data: "100mb",
      cpu: 80,
      ram: 55,
      uptime: "5 days",
      version: "V1"
    },

    events: {
    },

    initialize: function () {
    },

    template: templateEngine.createTemplate("embeddedNavigationView.ejs"),

    render: function () {

      $(this.el).html(this.template.render());

      //just for dev
      this.rerender();
    },

    rerender: function() {
      var self = this;

      $(self.el2).empty();

      _.each(this.dummyData, function(val, key) {

        var string = key;
        var info =
          '<a href="#health"><i title="You can find detailed health information here"' +
          'class="fa fa-info-circle"></i></a>';
        if (typeof val === 'string') {
          if (val === 'okay') {
            string += ': <span class="state-green">' + val + info +'</span>';
          }
          else if (val === 'warning') {
            string += ': <span class="state-yellow">' + val + info + '</span>';
          }
          else if (val === 'critical') {
            string += ': <span class="state-red">' + val + info +'</span>';
          }
          else {
            string += ": " + val;
          }
        }
        else if (typeof val === 'number') {
          if (val < 33) {
            string += ': <span class="state-green">' + val + ' %</span>';
          }
          else if (val > 33 && val < 66) {
            string += ': <span class="state-yellow">' + val + ' %</span>';
          }
          else {
            string += ': <span class="state-red">' + val + ' %</span>';
          }
        }

        jQuery('<li/>', {
          html: string
        }).appendTo($(self.el2));
      });
      $(this.el2).tooltip();
    }

  });
}());
