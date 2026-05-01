#pragma once

namespace Bones
{
    enum BonesEnum : int {
        origin = 0,
        pelvis = 1,
        spine_0 = 2,
        spine_1 = 3,
        spine_2 = 4,
        neck = 6,
        head = 7,
        shoulder_L = 9,
        elbow_L = 10,
        hand_L = 11,
        shoulder_R = 13,
        elbow_R = 14,
        hand_R = 15,
        hip_L = 17,
        knee_L = 18,
        foot_heel_L = 19,
        hip_R = 20,
        knee_R = 21,
        foot_heel_R = 22,
        chest = 23,
    };

    struct BoneConnection
    {
        int bone1;
        int bone2;

        BoneConnection(int b1, int b2) : bone1(b1), bone2(b2) {}
    };

    static const BoneConnection BoneConnections[] = {
    // Spine
    { pelvis, spine_1 },
    { spine_1, spine_2 },
    { spine_2, chest },
    { chest, neck },
    { neck,  head },

    // Left arm
    { neck,           shoulder_L },
    { shoulder_L,    elbow_L },
    { elbow_L,    hand_L },

    // Right arm
    { neck,           shoulder_R },
    { shoulder_R,    elbow_R },
    { elbow_R,    hand_R },

    // Left leg
    { pelvis,         hip_L },
    { hip_L,    knee_L },
    { knee_L,    foot_heel_L },

    // Right leg
    { pelvis,         hip_R },
    { hip_R,    knee_R },
    { knee_R,    foot_heel_R },
    };
}