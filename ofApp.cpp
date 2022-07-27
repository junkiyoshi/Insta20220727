#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);

	this->mesh.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);

	ofColor color;
	for (int i = 0; i < 20; i++) {

		this->noise_seed_list.push_back(ofRandom(1000));
		this->noise_param_list.push_back(0);

		color.setHsb(ofMap(i, 0, 20, 0, 255), 255, 255);
		this->color_list.push_back(color);
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	this->mesh.clear();

	int span = 28;
	for (int i = 0; i < this->noise_seed_list.size(); i++) {

		int radius = (ofGetFrameNum() * 3 + i * 30) % 600 + 30;

		for (int deg = 0; deg < 360; deg += 3) {

			auto noise_location = glm::vec2(320 * cos(deg * DEG_TO_RAD), 320 * sin(deg * DEG_TO_RAD));
			auto noise_param = ofMap(ofNoise(this->noise_seed_list[i], noise_location.x * 0.005, noise_location.y * 0.005, this->noise_param_list[i]), 0, 1, 0.35, 1);

			this->mesh.addVertex(glm::vec3(radius * noise_param * cos(deg * DEG_TO_RAD), radius * noise_param * sin(deg * DEG_TO_RAD), 0));
			this->mesh.addColor(ofColor(this->color_list[i], 0));
		}

		this->noise_param_list[i] += 0.001;
	}

	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		for (int k = i + 1; k < this->mesh.getNumVertices(); k++) {

			auto distance = glm::distance(this->mesh.getVertex(i), this->mesh.getVertex(k));
			if (distance < span) {

				auto alpha = distance < span * 0.25 ? 255 : ofMap(distance, span * 0.25, span, 255, 0);

				if (this->mesh.getColor(i).a < alpha) {

					this->mesh.setColor(i, ofColor(this->mesh.getColor(i), alpha));
				}

				if (this->mesh.getColor(k).a < alpha) {

					this->mesh.setColor(k, ofColor(this->mesh.getColor(k), alpha));
				}

				this->mesh.addIndex(i);
				this->mesh.addIndex(k);
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);

	this->mesh.drawWireframe();

	for (int i = 0; i < this->mesh.getNumVertices(); i++) {

		ofSetColor(this->mesh.getColor(i));
		ofDrawCircle(this->mesh.getVertex(i), 1.85);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}