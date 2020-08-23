#include "trigger.h"
#include <stdio.h>

changeViewUpdater updater_ChangeView;
changePlaneUpdater updater_ChangePlane;
// ...Updater updater_...;

rect::rect(glm::vec3 ctr, glm::vec3 ld, float *size_rot) {

    glm::vec3 givenPointPosition = glm::vec3(0.0f, 0.0f, 0.0f);  // If center position is provided, leftDown will be calculated, and center-position is supposed tobe at Zero before trans/rot, vice versa.
    glm::vec3 orgTargetPosition = glm::vec3(0.0f, 0.0f, 0.0f);   // Position of the vertex tobe calculated before trans/rot
    glm::vec3 *ptrTarget = NULL;                                 // points to the vertex tobe calculated (center or leftBofftom)

    // BUG in this design: if center coor is 000, then the rect could not be inited with center coord,
    // it could only be inited with leftbottom coord
    // vice versa
    if ( ctr != glm::vec3(0.0f, 0.0f, 0.0f) && size_rot ) {

        // center position is provided, and is supposed tobe at 0,0,0 position 
        // before translation/rotation in this case
        center = givenPointPosition = ctr;

        // leftDown vertex's position shall be calculated according to center and rotZ in this case
        orgTargetPosition = glm::vec3(-(size_rot[0] / 2), -(size_rot[1] / 2), ctr.z);
        ptrTarget = &leftDown;
    }
    else if ( ld != glm::vec3(0.0f, 0.0f, 0.0f) && size_rot ) {

        // leftDown position is provided, and is supposed tobe at 0,0,0 position 
        // before translation/rotation in this case
        leftDown = givenPointPosition = ld;

        // center vertex's position shall be calculated according to leftDown and rotZ in this case
        orgTargetPosition = glm::vec3( (size_rot[0] / 2), (size_rot[1] / 2), ld.z);
        ptrTarget = &center;
    }
    else {
        // Both center and leftDown vertex poisition not provided,
        // unable to init. Or, size is NULL pointer.
        return;
    }

    // Init width, height, and rotation Angle.
    width = size_rot[0]; height = size_rot[1]; 
    rotZ = size_rot[2];

    // Init the position for the 'leftDown' or 'center' vertex, according to param input
    glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(givenPointPosition.x, givenPointPosition.y, 0.0f));
    glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), size_rot[2], glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 modelMat = transMat * rotMat;
    *ptrTarget = glm::vec3(modelMat * glm::vec4(orgTargetPosition, 1.0));

    // Init the directions for the 4 normals
    glm::vec3 normalBeforeRot[nIdx::max] = {
        glm::vec3( 0.0f,  1.0f, center.z),  // north, 'ld.z' could also be used
        glm::vec3( 1.0f,  0.0f, center.z),  // east
        glm::vec3( 0.0f, -1.0f, center.z),  // south
        glm::vec3(-1.0f,  0.0f, center.z)   // west
    };
    for (unsigned char i = 0; i < nIdx::max; i++) {
        normal[i] = glm::vec3( rotMat * glm::vec4(normalBeforeRot[i], 0.0f) );
    }

    // Init the collisionInfo
    collisionInfo = 0;
}

cube::cube(glm::vec3 rectCenter, glm::vec3 rectLeftDown, float *size_rot, float cubeHeight) {
    // : bottomRect(rectCenter, rectLeftDown, size_rot) {
    bottomRect = rect(rectCenter, rectLeftDown, size_rot);
    height = cubeHeight;

    glm::vec3 vtBeforeTrans[NUM_VERTICES_FOR_CUBE] = {
        glm::vec3(          0,           0, 0),               // vt0 
        glm::vec3(          0,           0, cubeHeight),  // vt1 

        glm::vec3(size_rot[0],           0, 0),               // 
        glm::vec3(size_rot[0],           0, cubeHeight),  // 

        glm::vec3(size_rot[0], size_rot[1], 0),               // 
        glm::vec3(size_rot[0], size_rot[1], cubeHeight),  // 

        glm::vec3(          0, size_rot[1], 0),               // 
        glm::vec3(          0, size_rot[1], cubeHeight)   // 
    };

    glm::mat4 transMat = glm::translate(glm::mat4(1.0f), bottomRect.leftDown);
    glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), size_rot[2], glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 modelMat = transMat * rotMat;
    
    for (unsigned char i = 0; i < NUM_VERTICES_FOR_CUBE; i++) {
        vertices[i] = glm::vec3(modelMat * glm::vec4(vtBeforeTrans[i], 1.0f));
    }
}

void triggerPad::init(triggerUpdater* p) {
    updater = p;
}

unsigned char triggerPad::isTriggered() { 
    return 0; 
}

void triggerPad::update(float delta) {
    if (updater) {
        updater->update(this, delta);
    }
}



// Updaters for different triggers
void changeViewUpdater::update(triggerPad *trigger, float delta) {
    // printf("  update for ChangeView is called.\n\n");
}

void changePlaneUpdater::update(triggerPad *trigger, float delta) {
    // printf("  update for ChangePlane is called.\n\n");
}

// void ...Updater::update(triggerPad *trigger, float delta) { 
//     ... 
// }



// rect::rect(glm::vec3 ctr, glm::vec3 ld, float *size, float r) {
//     if ( ctr != glm::vec3(0.0f, 0.0f, 0.0f) && size ) {
//         width = size[0]; height = size[1];
//         rotZ = r;

//         // ctr position is supposed tobe at 0,0,0 position before translation/rotation
//         glm::vec3 ld_org = glm::vec3(-(size[0] / 2), -(size[1] / 2), ctr.z);

//         glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(ctr.x, ctr.y, 0.0f ));
//         glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), r, glm::vec3(0.0f, 0.0f, 1.0f));
//         glm::mat4 modelMat = transMat * rotMat;

//         leftDown = glm::vec3(modelMat * glm::vec4(ld_org, 1.0));
//     }
//     else if ( ld != glm::vec3(0.0f, 0.0f, 0.0f) && size ) {
//         width = size[0]; height = size[1];
//         rotZ = r;

//         // ld vertex is supposed tobe at 0,0,0 position before translation/rotation
//         glm::vec3 ctr_org = glm::vec3( (size[0] / 2), (size[1] / 2), ld.z);

//         glm::mat4 transMat = glm::translate(glm::mat4(1.0f), glm::vec3(ld.x, ld.y, 0.0f));
//         glm::mat4   rotMat = glm::rotate(glm::mat4(1.0f), r, glm::vec3(0.0f, 0.0f, 1.0f));
//         glm::mat4 modelMat = transMat * rotMat;

//         center = glm::vec3(modelMat * glm::vec4(ctr_org, 1.0));
//     }
//     else {
//         // Both center and leftDown vertex poisition not provided,
//         // unable to init. Or, size is NULL pointer.
//     }
// }
