/****************************************************************************************

Copyright (C) 2015 Autodesk, Inc.
All rights reserved.

Use of this software is subject to the terms of the Autodesk license agreement
provided at the time of installation or download, or which otherwise accompanies
this software in either electronic or hard copy form.

****************************************************************************************/

/////////////////////////////////////////////////////////////////////////
//
// This example illustrates how to display the content of a FBX or a OBJ file
// in a graphical window. This program is based on the OpenGL Utility Toolkit 
// (GLUT). Start the program on the command line by providing a FBX or a 
// OBJ file name. A menu is provided to select the current camera and the current 
// animation stack.
//
// Among other things, the example illustrates how to:
// 1)  Use a custom memory allocator
// 2)  Import a scene from a .FBX, .DAE or .OBJ file;
// 3)  Convert the nurbs and patch attribute types of a scene into mesh 
//     node attributes; And trianglate all meshes.
// 4)  Get the list of all the cameras in the scene;
// 5)  Find the current camera;
// 6)  Get the relevant settings of a camera depending on it's projection
//     type and aperture mode;
// 7)  Compute the local and global positions of a node;
// 8)  Compute the orientation of a camera;
// 9)  Compute the orientation of a light;
// 10)  Compute the shape deformation of mesh vertices;
// 11) Compute the link deformation of mesh vertices.
// 12) Display the point cache simulation of a mesh.
// 13) Get the list of all pose in the scene;
// 14) Show the scene using at a specific pose.
//
/////////////////////////////////////////////////////////////////////////

#include "SceneContext.h"
#include "GL/glut.h"

void ExitFunction();
void CreateMenus();
void CameraSelectionCallback(int pItem);
void CameraZoomModeCallback(int pItem);
void AnimStackSelectionCallback(int pItem);
void MenuSelectionCallback(int pItem);
void PoseSelectionCallback(int pItem);
void ShadingModeSelectionCallback(int pItem);
void TimerCallback(int);
void DisplayCallback();
void ReshapeCallback(int pWidth, int pHeight);
void KeyboardCallback(unsigned char pKey, int, int);
void MouseCallback(int button, int state, int x, int y);
void MotionCallback(int x, int y);

SceneContext * gSceneContext;

// Menu item ids.
#define PRODUCER_PERSPECTIVE_ITEM   100
#define PRODUCER_TOP_ITEM           101
#define PRODUCER_BOTTOM_ITEM        102
#define PRODUCER_FRONT_ITEM         103
#define PRODUCER_BACK_ITEM          104
#define PRODUCER_RIGHT_ITEM         105
#define PRODUCER_LEFT_ITEM          106
#define CAMERA_SWITCHER_ITEM        107
#define PLAY_ANIMATION              200


const int MENU_SHADING_MODE_WIREFRAME = 300;
const int MENU_SHADING_MODE_SHADED = 301;
const char * MENU_STRING_SHADING_MODE_WIREFRAME = "Wireframe";
const char * MENU_STRING_SHADING_MODE_SHADED = "Shaded";

const int MENU_ZOOM_FOCAL_LENGTH =          401;
const int MENU_ZOOM_POSITION     =          402;

const int MENU_EXIT = 400;

const int DEFAULT_WINDOW_WIDTH = 720;
const int DEFAULT_WINDOW_HEIGHT = 486;

#define TEST 1
#if (TEST==0)
class MyMemoryAllocator
{
public:
	static void* MyMalloc(size_t pSize)
    {
        char *p = (char*)malloc(pSize + FBXSDK_MEMORY_ALIGNMENT);
        if (p == nullptr) {
            return p;
        }
		memset(p, '#', FBXSDK_MEMORY_ALIGNMENT);
        return p + FBXSDK_MEMORY_ALIGNMENT;
    }

	static void* MyCalloc(size_t pCount, size_t pSize)
    {
        char *p = (char*)calloc(pCount, pSize + FBXSDK_MEMORY_ALIGNMENT);
        if (p == nullptr) {
            return p;
        }
		memset(p, '#', FBXSDK_MEMORY_ALIGNMENT);
        return p + FBXSDK_MEMORY_ALIGNMENT;
    }

	static void* MyRealloc(void* pData, size_t pSize)
    {
        if (pData)
        {
            FBX_ASSERT(*((char*)pData-1)=='#');
            if (*((char*)pData-1)=='#')
            {
                char *p = (char*)realloc((char*)pData - FBXSDK_MEMORY_ALIGNMENT, pSize + FBXSDK_MEMORY_ALIGNMENT);
                if (p == nullptr) {
                    return p;
                }
				memset(p, '#', FBXSDK_MEMORY_ALIGNMENT);
                return p + FBXSDK_MEMORY_ALIGNMENT;
            }
            else
            {   // Mismatch
                char *p = (char*)realloc((char*)pData, pSize + FBXSDK_MEMORY_ALIGNMENT);
                if (p == nullptr) {
                    return p;
                }
				memset(p, '#', FBXSDK_MEMORY_ALIGNMENT);
                return p + FBXSDK_MEMORY_ALIGNMENT;
            }
        }
        else
        {
            char *p = (char*)realloc(NULL, pSize + FBXSDK_MEMORY_ALIGNMENT);
            if (p == nullptr) {
                return p;
            }
			memset(p, '#', FBXSDK_MEMORY_ALIGNMENT);
            return p + FBXSDK_MEMORY_ALIGNMENT;
        }
    }

	static void MyFree(void* pData)
    {
        if (pData==NULL)
            return;
        FBX_ASSERT(*((char*)pData-1)=='#');
        if (*((char*)pData-1)=='#')
        {
            free((char*)pData - FBXSDK_MEMORY_ALIGNMENT);
        }
        else
        {   // Mismatch
            free(pData);
        }
    }
};

#elif 1

int total = 0;
//#define iprintf printf
#define iprintf (void)

// 출력이 정상적으로 되지 않는 이유 : 멀티스레드 방식
// 해결책 : 파일 로그로 남기는 방식(*.log) + 파일 읽고 쓸때 데드락 주의(큐)

void* imalloc(size_t size)
{
    iprintf("total = %d\n", ++total);
    return malloc(sizeof(char) * size);
}

void* icalloc(size_t size, size_t count)
{
    iprintf("total = %d\n", ++total);
    return calloc(size, count);
}

void* irealloc(void* mem, size_t size)
{
    iprintf("total = %d\n", total);
    return realloc(mem, size);
}

void ifree(void* m)
{
    if (m == NULL)
        return;

    iprintf("total = %d\n", --total);
    free(m);
}

#else

#include "iArray.h"

struct MemInfo
{
    void* m;
    size_t s;
};

iArray* array;
static int total = 0;

//#define iprintf printf
#define iprintf (void)

void* imalloc(size_t size)
{
    void* m = malloc(sizeof(char) * size);

    total += size;
    iprintf("imalloc::total = %d\n", total);

    if (array == NULL)
        array = new iArray();
    MemInfo* mi = new MemInfo;
    mi->m = m;
    mi->s = size;
    array->add(mi);

    return m;
}

void* icalloc(size_t size, size_t count)
{
    void* m = calloc(size, count);

    total += size * count;
    iprintf("icalloc::total = %d\n", total);

    if (array == NULL)
        array = new iArray();
    MemInfo* mi = new MemInfo;
    mi->m = m;
    mi->s = size * count;
    array->add(mi);

    return m;
}
void* irealloc(void* mem, size_t size)
{
#if 1
    for (int i = 0; i < array->count; i++)
    {
        MemInfo* mi = (MemInfo*)array->at(i);
        if (mi->m == mem)
        {
            total -= mi->s;
            iprintf("irealloc::total = %d\n", total);
            array->remove(i);
            break;
        }
    }

    void* m = realloc(mem, size);

    total += size;
    iprintf("irealloc::total = %d\n", total);

    MemInfo* mi = new MemInfo;
    mi->m = m;
    mi->s = size;
    array->add(mi);

    return m;
#else
    if (mem)
    {
        free(mem);
        for (int i = 0; i < array->count; i++)
        {
            MemInfo* mi = (MemInfo*)array->at(i);
            if (mi->m == mem)
            {
                total -= mi->s;
                iprintf("irealloc::total = %d\n", total);
                array->remove(i);
                break;
            }
        }
    }
    void* m = malloc(sizeof(char) * size);

    total += size;
    iprintf("irealloc::total = %d\n", total);

    MemInfo* mi = new MemInfo;
    mi->m = m;
    mi->s = size;
    array->add(mi);
#endif
    return m;
}
void ifree(void* m)
{
    if (m == NULL)
        return;
    free(m);
    if (array == NULL)
        array = new iArray();
    for (int i = 0; i < array->count; i++)
    {
        MemInfo* mi = (MemInfo*)array->at(i);
        if (mi->m == m)
        {
            total -= mi->s;
            iprintf("ifree::total = %d\n", total);
            array->remove(i);
            break;
        }
    }
}
#endif

static bool gAutoQuit = false;



int main(int argc, char** argv)
{
    // Set exit function to destroy objects created by the FBX SDK.
    atexit(ExitFunction);

	// Use a custom memory allocator
#if (TEST==0)
    FbxSetMallocHandler(MyMemoryAllocator::MyMalloc);
    FbxSetReallocHandler(MyMemoryAllocator::MyRealloc);
    FbxSetFreeHandler(MyMemoryAllocator::MyFree);
    FbxSetCallocHandler(MyMemoryAllocator::MyCalloc);
#else
    FbxSetMallocHandler(imalloc);
    FbxSetReallocHandler(irealloc);
    FbxSetFreeHandler(ifree);
    FbxSetCallocHandler(icalloc);
#endif
	// glut initialisation
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT); 
    glutInitWindowPosition(100, 100);
    glutCreateWindow("ViewScene");

    // Initialize OpenGL.
    const bool lSupportVBO = InitializeOpenGL();

	// set glut callbacks 
    glutDisplayFunc(DisplayCallback); 
    glutReshapeFunc(ReshapeCallback);
    glutKeyboardFunc(KeyboardCallback);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MotionCallback);

	FbxString lFilePath("");
	for( int i = 1, c = argc; i < c; ++i )
	{
		if( FbxString(argv[i]) == "-test" ) gAutoQuit = true;
		else if( lFilePath.IsEmpty() ) lFilePath = argv[i];
	}

	try {
		gSceneContext = new SceneContext(!lFilePath.IsEmpty() ? lFilePath.Buffer() : NULL, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, lSupportVBO);
	}
	catch (std::bad_alloc) {
		FBXSDK_printf("Unable to satisfy request for memory.\n");
		exit(1);
	}

	glutMainLoop();

    return 0;
}


// Function to destroy objects created by the FBX SDK.
void ExitFunction()
{
    delete gSceneContext;
}

// Create the menus to select the current camera and the current animation stack.
void CreateMenus()
{
    // Create the submenu to select the current camera.
    int lCameraMenu = glutCreateMenu(CameraSelectionCallback);

    // Add the producer cameras.
    glutAddMenuEntry(FBXSDK_CAMERA_PERSPECTIVE, PRODUCER_PERSPECTIVE_ITEM);
    glutAddMenuEntry(FBXSDK_CAMERA_TOP, PRODUCER_TOP_ITEM);
    glutAddMenuEntry(FBXSDK_CAMERA_BOTTOM, PRODUCER_BOTTOM_ITEM);
    glutAddMenuEntry(FBXSDK_CAMERA_FRONT, PRODUCER_FRONT_ITEM);
    glutAddMenuEntry(FBXSDK_CAMERA_BACK, PRODUCER_BACK_ITEM);
    glutAddMenuEntry(FBXSDK_CAMERA_RIGHT, PRODUCER_RIGHT_ITEM);
    glutAddMenuEntry(FBXSDK_CAMERA_LEFT, PRODUCER_LEFT_ITEM);

    // Add the camera switcher if there is at least one camera in the scene.
    const FbxArray<FbxNode *> & lCameraArray = gSceneContext->GetCameraArray();
    if (lCameraArray.GetCount() > 0)
    {
        glutAddMenuEntry(FBXSDK_CAMERA_SWITCHER, CAMERA_SWITCHER_ITEM);
    }

    // Add the cameras contained in the scene.
    for (int lCameraIndex = 0; lCameraIndex < lCameraArray.GetCount(); ++lCameraIndex)
    {
        glutAddMenuEntry(lCameraArray[lCameraIndex]->GetName(), lCameraIndex);
    }   

    // Create the submenu to select the current animation stack.
    int lAnimStackMenu = glutCreateMenu(AnimStackSelectionCallback);
    int lCurrentAnimStackIndex = 0;

    // Add the animation stack names.
    const FbxArray<FbxString *> & lAnimStackNameArray = gSceneContext->GetAnimStackNameArray();
    for (int lPoseIndex = 0; lPoseIndex < lAnimStackNameArray.GetCount(); ++lPoseIndex)
    {
        glutAddMenuEntry(lAnimStackNameArray[lPoseIndex]->Buffer(), lPoseIndex);

        // Track the current animation stack index.
        if (lAnimStackNameArray[lPoseIndex]->Compare(gSceneContext->GetScene()->ActiveAnimStackName.Get()) == 0)
        {
            lCurrentAnimStackIndex = lPoseIndex;
        }
    }

    // Call the animation stack selection callback immediately to 
    // initialize the start, stop and current time.
    AnimStackSelectionCallback(lCurrentAnimStackIndex);

    const int lShadingModeMenu = glutCreateMenu(ShadingModeSelectionCallback);
    glutAddMenuEntry(MENU_STRING_SHADING_MODE_WIREFRAME, MENU_SHADING_MODE_WIREFRAME);
    glutAddMenuEntry(MENU_STRING_SHADING_MODE_SHADED, MENU_SHADING_MODE_SHADED);

    int lBindPoseCount = 0;
    int lRestPoseCount = 0;
    // Create a submenu for bind poses
    int lBindPoseMenu = glutCreateMenu(PoseSelectionCallback);

    // Add the list of bind poses
    const FbxArray<FbxPose *> & lPoseArray = gSceneContext->GetPoseArray();
    for (int lPoseIndex = 0; lPoseIndex < lPoseArray.GetCount(); ++lPoseIndex)
    {
        if (lPoseArray[lPoseIndex]->IsBindPose())
        {
            glutAddMenuEntry(lPoseArray[lPoseIndex]->GetName(), lPoseIndex);
            lBindPoseCount++;
        }
    }

    // Create a submenu for rest poses
    int lRestPoseMenu = glutCreateMenu(PoseSelectionCallback);

    // Add the list of bind poses
    for (int lPoseIndex = 0; lPoseIndex < lPoseArray.GetCount(); ++lPoseIndex)
    {
        if (lPoseArray[lPoseIndex]->IsRestPose())
        {
            glutAddMenuEntry(lPoseArray[lPoseIndex]->GetName(), lPoseIndex);
            lRestPoseCount++;
        }
    }

    // Create the submenu to go to a specific pose
    int lPoseMenu = 0;
    if (lBindPoseCount>0 || lRestPoseCount>0)
    {
        lPoseMenu = glutCreateMenu(PoseSelectionCallback);
        if (lBindPoseCount>0)
            glutAddSubMenu("Bind Pose", lBindPoseMenu);
        if (lRestPoseCount>0)
            glutAddSubMenu("Rest Pose", lRestPoseMenu);
    }

    // Create the submenu to zoom mode
    int lZoomMenu = glutCreateMenu( CameraZoomModeCallback);
    glutAddMenuEntry( "Zooming lens", MENU_ZOOM_FOCAL_LENGTH);
    glutAddMenuEntry( "Zooming Position", MENU_ZOOM_POSITION);

    // Build the main menu.
    glutCreateMenu(MenuSelectionCallback);
    glutAddSubMenu("Select Camera", lCameraMenu); 
    glutAddSubMenu("Select Animation Stack", lAnimStackMenu);
    glutAddSubMenu("Select Shading Mode", lShadingModeMenu);
    if (lBindPoseCount>0 || lRestPoseCount>0)
        glutAddSubMenu("Go to Pose", lPoseMenu);
    glutAddSubMenu( "Zoom Mode", lZoomMenu);
    glutAddMenuEntry("Play", PLAY_ANIMATION);
    glutAddMenuEntry("Exit", MENU_EXIT); 
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Select the current camera.
void CameraSelectionCallback(int pItem)
{
    const FbxArray<FbxNode*> & lCameraArray = gSceneContext->GetCameraArray();
    if (pItem == PRODUCER_PERSPECTIVE_ITEM)
    {
        gSceneContext->SetCurrentCamera(FBXSDK_CAMERA_PERSPECTIVE);
    }
    else if (pItem == PRODUCER_TOP_ITEM)
    {
        gSceneContext->SetCurrentCamera(FBXSDK_CAMERA_TOP);
    }
    else if (pItem == PRODUCER_BOTTOM_ITEM)
    {
        gSceneContext->SetCurrentCamera(FBXSDK_CAMERA_BOTTOM);
    }
    else if (pItem == PRODUCER_FRONT_ITEM)
    {
        gSceneContext->SetCurrentCamera(FBXSDK_CAMERA_FRONT);
    }
    else if (pItem == PRODUCER_BACK_ITEM)
    {
        gSceneContext->SetCurrentCamera(FBXSDK_CAMERA_BACK);
    }
    else if (pItem == PRODUCER_RIGHT_ITEM)
    {
        gSceneContext->SetCurrentCamera(FBXSDK_CAMERA_RIGHT);
    }
    else if (pItem == PRODUCER_LEFT_ITEM)
    {
        gSceneContext->SetCurrentCamera(FBXSDK_CAMERA_LEFT);
    }
    else if (pItem == CAMERA_SWITCHER_ITEM)
    {
        gSceneContext->SetCurrentCamera(FBXSDK_CAMERA_SWITCHER);
    }
    else if (pItem >= 0 && pItem < lCameraArray.GetCount())
    {
        gSceneContext->SetCurrentCamera(lCameraArray[pItem]->GetName());
    }
}

void CameraZoomModeCallback(int pItem)
{
    if( pItem == MENU_ZOOM_FOCAL_LENGTH)
        gSceneContext->SetZoomMode( SceneContext::ZOOM_FOCAL_LENGTH);
    else
        gSceneContext->SetZoomMode(SceneContext::ZOOM_POSITION);
}

// Select the current animation stack and set the start, stop and current time.
void AnimStackSelectionCallback( int pItem )
{
    gSceneContext->SetCurrentAnimStack(pItem);
}

void PoseSelectionCallback(int pItem)
{
    gSceneContext->SetCurrentPoseIndex(pItem);
}

void ShadingModeSelectionCallback(int pItem)
{
    if (pItem == MENU_SHADING_MODE_WIREFRAME)
    {
        gSceneContext->SetShadingMode(SHADING_MODE_WIREFRAME);
    }
    else if (pItem == MENU_SHADING_MODE_SHADED)
    {
        gSceneContext->SetShadingMode(SHADING_MODE_SHADED);
    }
}

// Exit the application from the main menu.
void MenuSelectionCallback(int pItem)
{
    if (pItem == PLAY_ANIMATION)
    {
        gSceneContext->SetCurrentPoseIndex(-1);
    }
    else if (pItem == MENU_EXIT)
    {
        exit(0);
    }
}


// Trigger the display of the current frame.
void TimerCallback(int)
{
    // Ask to display the current frame only if necessary.
    if (gSceneContext->GetStatus() == SceneContext::MUST_BE_REFRESHED)
    {
        glutPostRedisplay();
    }

    gSceneContext->OnTimerClick();

    // Call the timer to display the next frame.
    glutTimerFunc((unsigned int)gSceneContext->GetFrameTime().GetMilliSeconds(), TimerCallback, 0);
}


// Refresh the application window.
void DisplayCallback()
{
    gSceneContext->OnDisplay();

    glutSwapBuffers();

    // Import the scene if it's ready to load.
    if (gSceneContext->GetStatus() == SceneContext::MUST_BE_LOADED)
    {
        // This function is only called in the first display callback
        // to make sure that the application window is opened and a 
        // status message is displayed before.
        try {
            if (!gSceneContext->LoadFile())
                exit(1);
        }
        catch (const std::bad_alloc) {
            FBXSDK_printf("Unable to satisfy request for memory.\n");
            exit(1);
        }

        CreateMenus();

        // Call the timer to display the first frame.
        glutTimerFunc((unsigned int)gSceneContext->GetFrameTime().GetMilliSeconds(), TimerCallback, 0);
    }

	if( gAutoQuit ) exit(0);
}


// Resize the application window.
void ReshapeCallback(int pWidth, int pHeight)
{
    gSceneContext->OnReshape(pWidth, pHeight);
}

// Exit the application from the keyboard.
void KeyboardCallback(unsigned char pKey, int /*pX*/, int /*pY*/)
{
    // Exit on ESC key.
    if (pKey == 27)
    {
        exit(0);
    }

    gSceneContext->OnKeyboard(pKey);
}

void MouseCallback(int button, int state, int x, int y)
{
    gSceneContext->OnMouse(button, state, x, y);
}


void MotionCallback(int x, int y)
{
    gSceneContext->OnMouseMotion(x, y);
}




