/*****************************************************************
 *
 * This file is part of the GMAPPING project
 *
 * GMAPPING Copyright (c) 2004 Giorgio Grisetti, 
 * Cyrill Stachniss, and Wolfram Burgard
 *
 * This software is licensed under the "Creative Commons 
 * License (Attribution-NonCommercial-ShareAlike 2.0)" 
 * and is copyrighted by Giorgio Grisetti, Cyrill Stachniss, 
 * and Wolfram Burgard.
 * 
 * Further information on this license can be found at:
 * http://creativecommons.org/licenses/by-nc-sa/2.0/
 * 
 * GMAPPING is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied 
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  
 *
 *****************************************************************/


#ifndef QPARTICLEVIEWER_H
#define QPARTICLEVIEWER_H

#include <qpainter.h>
#include <qpixmap.h>
#include <qwidget.h>
#include <qwmatrix.h>
#include <qtextstream.h>
#include <vector>
#include <assert.h>
#include <sstream>
#include <iostream>
#include <qimage.h>
#include <qbuffer.h>

#include <utils/point.h>
#include "gsp_thread.h"

namespace GMapping {

class QParticleViewer :  public QWidget{
	Q_OBJECT
	public:
		struct StartParameters{
			//motionmodel
			double srr, srt, str, stt;
			//map
			double xmin, ymin, xmax, ymax, delta;
			OrientedPoint initialPose;
			//likelihood
			double lsigma, lgain;
			unsigned int lskip;
			//update
			double linearUpdate, angularUpdate;
			//filter
			unsigned int particles;
			double resampleThreshold;
			//mode
			bool drawFromObservation;
			//output
			const char * outFileName;
		};
		
		struct MatchingParameters{
			//ranges
			double maxrange, urange;
			//score
			double ssigma, sreg, scrit;
			unsigned int ksize;
			//search
			double lstep, astep;
			unsigned int iterations;
		};

		void refreshParameters(); //reads the parameters from the thread
		inline void setGSP( GridSlamProcessorThread* thread){gfs_thread=thread;}
		
		
		typedef std::vector<OrientedPoint> OrientedPointVector;
		QParticleViewer( QWidget * parent = 0, const char * name = 0, WFlags f = 0, GridSlamProcessorThread* thread=0 );
		virtual ~QParticleViewer();
		virtual void timerEvent(QTimerEvent * te);
		virtual void resizeEvent(QResizeEvent *);


		void drawFromEvents(GridSlamProcessorThread::EventDeque events);
		inline ScanMatcherMap* getBestMap() {return bestMap;}
		inline OrientedPoint getBestParticlePose() {return bestParticlePose;}
		inline QPixmap* getPixmap() {return m_pixmap;}
		inline Point pic2map(const IntPoint& p)
			{return viewCenter+Point(p.x/mapscale, -p.y/mapscale); }
		inline IntPoint map2pic(const Point& p)
			{return IntPoint((int)((p.x-viewCenter.x)*mapscale),(int)((viewCenter.y-p.y)*mapscale)); }

		
		void drawFrontiers(vector<vector<IntPoint> >& frontiers, QColor color); //Matan
		void drawRobot(OrientedPoint robotLocation, QColor color);
		void drawFromFile();
		void drawFromMemory();
		void drawMap(const ScanMatcherMap& map);
		void start(int period);
		QTextIStream* tis;
		void saveToFile(char* filename);
		void saveToBuffer(char** buffer, unsigned long& size);
		
		MatchingParameters matchingParameters;
		StartParameters startParameters;
		
		int writeToFile;
	public slots:
		void setMatchingParameters(const MatchingParameters& mp);
		void setStartParameters(const StartParameters& mp);
		void start();
		void stop();
		void loadFile(const char *);
	signals:
		void neffChanged(double);
		void poseEntropyChanged(double, double, double);
		void trajectoryEntropyChanged(double, double, double);
		void mapsEntropyChanged(double);
		void mapsIGainChanged(double);
		
	protected:
		ifstream inputStream;
		ofstream outputStream;
		
			
	protected:
		int timer;
		virtual void paintEvent ( QPaintEvent *paintevent );
		void drawParticleMove(const OrientedPointVector& start, const OrientedPointVector& end); 
		QPixmap* m_pixmap;
		
		//thread interaction
		GridSlamProcessorThread* gfs_thread;
		GridSlamProcessorThread::EventDeque history;
		
		//mouse movement
		virtual void mousePressEvent(QMouseEvent*);
		virtual void mouseReleaseEvent(QMouseEvent*);
		virtual void mouseMoveEvent(QMouseEvent*);
		QPoint draggingPos;
		bool dragging;
		
		//particle plotting
		virtual void keyPressEvent ( QKeyEvent* e );
		
		//map painting
		double mapscale;
		Point viewCenter;
		OrientedPoint bestParticlePose;
		ScanMatcherMap * bestMap;
		
		// view mode
		bool showPaths;
		bool showBestPath;
		
		// file plotting
		QParticleViewer::OrientedPointVector m_oldPose, m_newPose;
		unsigned int m_particleSize;
		bool m_refresh;
		int count;
};

};

#endif

