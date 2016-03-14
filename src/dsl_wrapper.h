// DO NOT EDIT THIS FILE
// generated using scripts/generate.hy and code in src/dsl/

#pragma once
#define PINEAL(_)

namespace dsl{
	namespace primitives{
		PINEAL("cube")
		void cube(double r){
			ofDrawBox(r);
		}

		PINEAL("polygon")
		void polygon_n_r(int n, float r){
			ofPushMatrix();

			ofScale(r, r, r);
			ofRotateZ(90);

			ofSetCircleResolution(n);
			ofDrawCircle(0, 0, 1);

			ofPopMatrix();
		}

		PINEAL("polygon")
		void polygon_n(int n){
			polygon_n_r(n, 1);
		}
	}

	namespace transformations{
		PINEAL("scale")
		void scale_xyz(py::object f, double x, double y, double z){
			ofPushMatrix();
			ofScale(x, y, z);
			f();
			ofPopMatrix();
		}

		PINEAL("scale")
		void scale_xy(py::object f, double x, double y){
			scale_xyz(f, x, y, 1);
		}

		PINEAL("scale")
		void scale_r(py::object f, double r){
			scale_xyz(f, r, r, r);
		}

		PINEAL("translate")
		void translate_xyz(py::object f, double x, double y, double z){
			ofPushMatrix();
			ofTranslate(x, y, z);
			f();
			ofPopMatrix();
		}

		PINEAL("translate")
		void translate_xy(py::object f, double x, double y){
			translate_xyz(f, x, y, 0);
		}

		PINEAL("translate")
		void translate_x(py::object f, double x){
			translate_xyz(f, x, 0, 0);
		}

		PINEAL("rotate_x")
		void rotate_x(py::object f, double rad){
			ofPushMatrix();
			ofRotateX(180 * rad / PI);
			f();
			ofPopMatrix();
		}

		PINEAL("rotate_y")
		void rotate_y(py::object f, double rad){
			ofPushMatrix();
			ofRotateY(180 * rad / PI);
			f();
			ofPopMatrix();
		}

		PINEAL("rotate_z")
		void rotate_z(py::object f, double rad){
			ofPushMatrix();
			ofRotateZ(180 * rad / PI);
			f();
			ofPopMatrix();
		}

		typedef enum{
			X, Y, Z
		}Axis;

		void turn(py::object f, Axis axis, int n){
			double rot;

			ofPushMatrix();
			for(int i=0; i<n; i++){
				f();
				rot = 360.0 / n;
				if(axis == X){
					ofRotateX(rot);
				}else if(axis == Y){
					ofRotateY(rot);
				}else if(axis == Z){
					ofRotateZ(rot);
				}
			}
			ofPopMatrix();
		}

		PINEAL("turn_x")
		void turn_x(py::object f, int n){
			turn(f, X, n);
		}

		PINEAL("turn_y")
		void turn_y(py::object f, int n){
			turn(f, Y, n);
		}

		PINEAL("turn_z")
		void turn_z(py::object f, int n){
			turn(f, Z, n);
		}
	}

	namespace audio{
		bool beat_value = false;
		int beat_count = 0;
		float beat_time = 1.0;
		float last_beat = 0;

		bool onset_value = false;
		float last_onset = 0;

		ofSoundBuffer inBuf;


		void update(){
			beat_value = false;
			onset_value = false;
		}

		void set_inBuf(ofSoundBuffer in){
			inBuf = in;
		}

		void set_beat(){
			beat_value = true;
			beat_count += 1;

			float actual_time = (float)ofGetSystemTimeMicros() / 1000;
			beat_time = actual_time - last_beat;
			last_beat = actual_time;
		}

		void set_onset(){
			onset_value = true;

			float actual_time = (float)ofGetSystemTimeMicros() / 1000;
			last_onset = actual_time;
		}

		PINEAL("beat")
		bool beat_n_t(int n, float t){
			float actual_time = (float)ofGetSystemTimeMicros() / 1000;

			if(beat_count % n == 0 && actual_time - last_beat < beat_time * t){
				return true;
			}else{
				return false;
			}
		}

		PINEAL("beat")
		bool beat_n(int n){
			return beat_n_t(n, 1.0);
		}

		PINEAL("beat")
		bool beat(){
			return beat_n(1);
		}

		PINEAL("onset")
		bool onset_t(float t){
			float actual_time = (float)ofGetSystemTimeMicros() / 1000;

			if(actual_time - last_onset < beat_time * t){
				return true;
			}else{
				return false;
			}
		}

		PINEAL("onset")
		bool onset(){
			return onset_value;
		}

		PINEAL("rms")
		float rms(){
			return inBuf.getRMSAmplitude();
		}
	}

	namespace layers{
		unordered_map<string, shared_ptr<ofFbo>> layers_map;

		void new_layer(string name){
			if(layers_map.find(name) != layers_map.end()){
				return;
			}
			auto fbo = make_shared<ofFbo>();

			fbo->allocate(BUFFER_SIZE, BUFFER_SIZE, GL_RGBA);
			fbo->begin();
			ofClear(255,255,255, 0);
			fbo->end();
			layers_map[name] = fbo;
		}

		PINEAL("on_layer")
		void on_layer(py::object f, string name){
			if(layers_map.find(name) == layers_map.end()){
				new_layer(name);
			}
			ofEasyCam camera;
			camera.setDistance(1);
			camera.setNearClip(0.01);

			layers_map[name]->begin();
			camera.begin();
			f();
			camera.end();
			layers_map[name]->end();
		}

		PINEAL("draw_layer")
		void draw_layer(string name){
			if(layers_map.find(name) == layers_map.end()){
				new_layer(name);
			}
			layers_map[name]->getTexture().draw(-1, -1, 2, 2);
		}
	}

	namespace colors{
		void setup(){
			ofSetColor(255);
			ofFill();
			ofSetLineWidth(1);
		}

		PINEAL("background")
		void background(double r, double g, double b, double a){
			ofBackground(r * 255, g * 255, b * 255, a * 255);
		}

		PINEAL("color")
		void color(py::object f, double r, double g, double b, double a){
			static ofColor status_color;
			ofColor old_color = status_color;
			ofColor new_color = ofColor(r * 255, g * 255, b * 255, a * 255);

			status_color = new_color;
			ofSetColor(status_color);

			f();

			status_color = old_color;
			ofSetColor(status_color);
		}

		PINEAL("color")
		void color_rgb(py::object f, double r, double g, double b){
			color(f, r, g, b, 1);
		}

		PINEAL("color")
		void color_grey(py::object f, double c){
			color(f, c, c, c, 1);
		}

		PINEAL("color")
		void color_grey_alpha(py::object f, double c, double a){
			color(f, c, c, c, a);
		}

		void fill_status(py::object f, bool status){
			static bool status_fill = true;
			bool old_fill = status_fill;
			bool new_fill = status;

			status_fill = new_fill;
			if(status_fill){
				ofFill();
			}else{
				ofNoFill();
			}

			f();

			status_fill = old_fill;
			if(status_fill){
				ofFill();
			}else{
				ofNoFill();
			}
		}

		PINEAL("fill")
		void fill(py::object f){
			fill_status(f, true);
		}

		PINEAL("no_fill")
		void no_fill(py::object f){
			fill_status(f, false);
		}

		PINEAL("line_width")
		void line_width(py::object f, double new_width){
			static double status_line_width = 1;
			double old_width = status_line_width;

			status_line_width = new_width;
			ofSetLineWidth(status_line_width);

			f();

			status_line_width = old_width;
			ofSetLineWidth(status_line_width);
		}
	}

	BOOST_PYTHON_MODULE(core){
		py::def("cube", &primitives::cube);
		py::def("polygon", &primitives::polygon_n_r);
		py::def("polygon", &primitives::polygon_n);

		py::def("scale", &transformations::scale_xyz);
		py::def("scale", &transformations::scale_xy);
		py::def("scale", &transformations::scale_r);
		py::def("translate", &transformations::translate_xyz);
		py::def("translate", &transformations::translate_xy);
		py::def("translate", &transformations::translate_x);
		py::def("rotate_x", &transformations::rotate_x);
		py::def("rotate_y", &transformations::rotate_y);
		py::def("rotate_z", &transformations::rotate_z);
		py::def("turn_x", &transformations::turn_x);
		py::def("turn_y", &transformations::turn_y);
		py::def("turn_z", &transformations::turn_z);

		py::def("beat", &audio::beat_n_t);
		py::def("beat", &audio::beat_n);
		py::def("beat", &audio::beat);
		py::def("onset", &audio::onset_t);
		py::def("onset", &audio::onset);
		py::def("rms", &audio::rms);

		py::def("on_layer", &layers::on_layer);
		py::def("draw_layer", &layers::draw_layer);

		py::def("background", &colors::background);
		py::def("color", &colors::color);
		py::def("color", &colors::color_rgb);
		py::def("color", &colors::color_grey);
		py::def("color", &colors::color_grey_alpha);
		py::def("fill", &colors::fill);
		py::def("no_fill", &colors::no_fill);
		py::def("line_width", &colors::line_width);
	}
}