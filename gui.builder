<?xml version="1.0" encoding="UTF-8"?>
<interface>
  <requires lib="gtk+" version="2.24"/>
  <!-- interface-naming-policy project-wide -->
  <object class="GtkWindow" id="window1">
    <property name="can_focus">False</property>
    <signal name="destroy" handler="gtk_main_quit" swapped="no"/>
    <signal name="delete-event" handler="cb_delete_event" swapped="no"/>
    <child>
      <object class="GtkTable" id="table1">
        <property name="visible">True</property>
        <property name="can_focus">False</property>
        <property name="n_rows">3</property>
        <property name="n_columns">3</property>
        <child>
          <object class="GtkTable" id="table2">
            <property name="visible">True</property>
            <property name="can_focus">False</property>
            <property name="n_rows">13</property>
            <property name="n_columns">3</property>
            <child>
              <object class="GtkLabel" id="servoData_title_servono">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">&lt;b&gt;servo #&lt;/b&gt;</property>
                <property name="use_markup">True</property>
                <property name="justify">center</property>
              </object>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_servo0">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">&lt;span foreground="red"&gt;0&lt;/span&gt;</property>
                <property name="use_markup">True</property>
              </object>
              <packing>
                <property name="top_attach">1</property>
                <property name="bottom_attach">2</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_pw">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">&lt;b&gt;pulsewidth&lt;/b&gt;</property>
                <property name="use_markup">True</property>
                <property name="justify">center</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_angle">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">&lt;b&gt;Angle&lt;/b&gt;</property>
                <property name="use_markup">True</property>
                <property name="justify">center</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_servo1">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">1</property>
              </object>
              <packing>
                <property name="top_attach">2</property>
                <property name="bottom_attach">3</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_servo2">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">2</property>
              </object>
              <packing>
                <property name="top_attach">3</property>
                <property name="bottom_attach">4</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_servo3">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">3</property>
              </object>
              <packing>
                <property name="top_attach">4</property>
                <property name="bottom_attach">5</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_servo4">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">4</property>
              </object>
              <packing>
                <property name="top_attach">5</property>
                <property name="bottom_attach">6</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_servo5">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">5</property>
              </object>
              <packing>
                <property name="top_attach">6</property>
                <property name="bottom_attach">7</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_servo6">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">6</property>
              </object>
              <packing>
                <property name="top_attach">7</property>
                <property name="bottom_attach">8</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_servo7">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">7</property>
              </object>
              <packing>
                <property name="top_attach">8</property>
                <property name="bottom_attach">9</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_servo8">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">8</property>
              </object>
              <packing>
                <property name="top_attach">9</property>
                <property name="bottom_attach">10</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_servo9">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">9</property>
              </object>
              <packing>
                <property name="top_attach">10</property>
                <property name="bottom_attach">11</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_servo10">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">10</property>
              </object>
              <packing>
                <property name="top_attach">11</property>
                <property name="bottom_attach">12</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_title_servo11">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">11</property>
              </object>
              <packing>
                <property name="top_attach">12</property>
                <property name="bottom_attach">13</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_pw0">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
                <property name="top_attach">1</property>
                <property name="bottom_attach">2</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_pw1">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
                <property name="top_attach">2</property>
                <property name="bottom_attach">3</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_pw2">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
                <property name="top_attach">3</property>
                <property name="bottom_attach">4</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_pw3">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
                <property name="top_attach">4</property>
                <property name="bottom_attach">5</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_pw4">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
                <property name="top_attach">5</property>
                <property name="bottom_attach">6</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_pw5">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
                <property name="top_attach">6</property>
                <property name="bottom_attach">7</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_pw6">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
                <property name="top_attach">7</property>
                <property name="bottom_attach">8</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_pw7">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
                <property name="top_attach">8</property>
                <property name="bottom_attach">9</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_pw8">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
                <property name="top_attach">9</property>
                <property name="bottom_attach">10</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_pw9">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
                <property name="top_attach">10</property>
                <property name="bottom_attach">11</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_pw10">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
                <property name="top_attach">11</property>
                <property name="bottom_attach">12</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_pw11">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">1</property>
                <property name="right_attach">2</property>
                <property name="top_attach">12</property>
                <property name="bottom_attach">13</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_angle0">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
                <property name="top_attach">1</property>
                <property name="bottom_attach">2</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_angle1">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
                <property name="top_attach">2</property>
                <property name="bottom_attach">3</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_angle2">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
                <property name="top_attach">3</property>
                <property name="bottom_attach">4</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_angle3">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
                <property name="top_attach">4</property>
                <property name="bottom_attach">5</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_angle4">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
                <property name="top_attach">5</property>
                <property name="bottom_attach">6</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_angle5">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
                <property name="top_attach">6</property>
                <property name="bottom_attach">7</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_angle6">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
                <property name="top_attach">7</property>
                <property name="bottom_attach">8</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_angle7">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
                <property name="top_attach">8</property>
                <property name="bottom_attach">9</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_angle8">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
                <property name="top_attach">9</property>
                <property name="bottom_attach">10</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_angle9">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
                <property name="top_attach">10</property>
                <property name="bottom_attach">11</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_angle10">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
                <property name="top_attach">11</property>
                <property name="bottom_attach">12</property>
              </packing>
            </child>
            <child>
              <object class="GtkLabel" id="servoData_angle11">
                <property name="visible">True</property>
                <property name="can_focus">False</property>
                <property name="label" translatable="yes">label</property>
              </object>
              <packing>
                <property name="left_attach">2</property>
                <property name="right_attach">3</property>
                <property name="top_attach">12</property>
                <property name="bottom_attach">13</property>
              </packing>
            </child>
          </object>
        </child>
        <child>
          <placeholder/>
        </child>
        <child>
          <placeholder/>
        </child>
        <child>
          <placeholder/>
        </child>
        <child>
          <placeholder/>
        </child>
        <child>
          <placeholder/>
        </child>
        <child>
          <placeholder/>
        </child>
        <child>
          <placeholder/>
        </child>
        <child>
          <placeholder/>
        </child>
      </object>
    </child>
  </object>
</interface>
