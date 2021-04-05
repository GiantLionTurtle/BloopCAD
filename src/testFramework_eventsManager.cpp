
#include "testFramework_eventsManager.hpp"

#include <bloop.hpp>
#include <document.hpp>

#include <tools/partDesign/startSketch_tool.hpp>
#include <tools/sketchDesign/line_tool.hpp>
#include <tools/sketchDesign/coincidence_tool.hpp>
#include <tools/sketchDesign/horizontality_tool.hpp>
#include <tools/sketchDesign/perpendicularity_tool.hpp>

test_eventsManager::test_eventsManager():
	mTestStep(-1),
	mTestStepDone(0),
	mLastAction(std::chrono::steady_clock::now())
{

}

bool test_eventsManager::manage_key_press(GdkEventKey* event)
{
	if(event->keyval == GDK_KEY_o) {
		mTestStep++;
	}
	return true;
}

bool test_eventsManager::manage_tick(const Glib::RefPtr<Gdk::FrameClock>& frame_clock)
{
	do_test();
	return stimuli_eventsManager::manage_tick(frame_clock);
}

/*
	line
		sketchPoint(0.238092,  0.299035)
		sketchPoint(1.02165,  0.171687)
			coincidence_annot[(1.02165,  0.171687) ; (-1.09278e-06,  25)]
	line
		sketchPoint(1.02165,  0.171687)
			[(1.02165,  0.171687) ; (-1.09278e-06,  25)]
		sketchPoint(1.1587,  0.799526)
			[(1.1587,  0.799526) ; (-1.09278e-06,  25)]
	line
		sketchPoint(1.1587,  0.799526)
			[(1.1587,  0.799526) ; (-1.09278e-06,  25)]
		sketchPoint(0.362863,  1.27707)
			[(0.362863,  1.27707) ; (-1.09278e-06,  25)]
	line
		sketchPoint(0.362863,  1.27707)
			[(0.362863,  1.27707) ; (-1.09278e-06,  25)]
		sketchPoint(0.387337,  0.436465)

*/

void test_eventsManager::do_test()
{
	if(mTestStep >= mTestStepDone) {
		document_ptr doc = mBloopWindow->currentDocument();
		if(!doc) {
			LOG_ERROR("Invalid document");
		}
		switch(mTestStep) {
		case 0:
			mBloopWindow->partWorkspace()->startSketch()->init();
			mBloopWindow->partWorkspace()->startSketch()->start_sketch(doc->target()->XY(), mBloopWindow->partWorkspace()->state()->cam->state());
			break;
		case 1:
			mBloopWindow->sketchWorkspace()->line()->init();
			mFirst_pt = mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(0.238092,  0.299035));
			break;
		case 2:
			mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(1.02165,  0.171687));
			mLines[0] = mBloopWindow->sketchWorkspace()->line()->lastAdded();
			break;
		case 3:
			mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(1.1587,  0.799526));
			mLines[1] = mBloopWindow->sketchWorkspace()->line()->lastAdded();
			break;
		case 4:
			mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(0.362863,  1.27707));
			mLines[2] = mBloopWindow->sketchWorkspace()->line()->lastAdded();
			break;
		case 5:
			mLast_pt = mBloopWindow->sketchWorkspace()->line()->add_point(glm::vec2(0.387337,  0.436465));
			mLines[3] = mBloopWindow->sketchWorkspace()->line()->lastAdded();
			mBloopWindow->sketchWorkspace()->line()->finish();
			break;
		case 6:
			mBloopWindow->sketchWorkspace()->coincidence()->init();			
			mBloopWindow->sketchWorkspace()->coincidence()->add_entity(mFirst_pt);
			mBloopWindow->sketchWorkspace()->coincidence()->add_entity(mLast_pt);
			mBloopWindow->sketchWorkspace()->coincidence()->add_constraint();
			mBloopWindow->sketchWorkspace()->coincidence()->finish();
			break;
		case 7:
			mBloopWindow->sketchWorkspace()->horizontality()->init();
			mBloopWindow->sketchWorkspace()->horizontality()->add_entity(mLines[0]);
			mBloopWindow->sketchWorkspace()->horizontality()->add_constraint();
			mBloopWindow->sketchWorkspace()->horizontality()->finish();
			break;
		case 8:
			mBloopWindow->sketchWorkspace()->perpendicularity()->init();
			mBloopWindow->sketchWorkspace()->perpendicularity()->add_entity(mLines[0]);
			mBloopWindow->sketchWorkspace()->perpendicularity()->add_entity(mLines[1]);
			mBloopWindow->sketchWorkspace()->perpendicularity()->add_constraint();
			mBloopWindow->sketchWorkspace()->perpendicularity()->finish();
			break;
		case 9:
			mBloopWindow->sketchWorkspace()->perpendicularity()->init();
			mBloopWindow->sketchWorkspace()->perpendicularity()->add_entity(mLines[1]);
			mBloopWindow->sketchWorkspace()->perpendicularity()->add_entity(mLines[2]);
			mBloopWindow->sketchWorkspace()->perpendicularity()->add_constraint();
			mBloopWindow->sketchWorkspace()->perpendicularity()->finish();
			break;
		case 10:
			mBloopWindow->sketchWorkspace()->perpendicularity()->init();
			mBloopWindow->sketchWorkspace()->perpendicularity()->add_entity(mLines[2]);
			mBloopWindow->sketchWorkspace()->perpendicularity()->add_entity(mLines[3]);
			mBloopWindow->sketchWorkspace()->perpendicularity()->add_constraint();
			mBloopWindow->sketchWorkspace()->perpendicularity()->finish();
			break;
		default:
			break;
		}
		mTestStepDone++;
		// if(mTestStep > 0 && duration_cast<milliseconds>(steady_clock::now() - mLastAction).count() > 100) {
		// 	mLastAction = steady_clock::now();
		// 	std::cout<<"test! "<<i++<<"\n";
		// }
	}
}