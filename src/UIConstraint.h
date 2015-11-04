#ifndef UIConstraint_h
#define UIConstraint_h

#include "UIObject.h"

class UIConstraint {
public:
    enum ConstraintStyle {
        LEFT, RIGHT, TOP, BOTTOM, INSIDE_LEFT, INSIDE_RIGHT, INSIDE_TOP, INSIDE_BOTTOM, VALUE,
        PERCENTAGE_OF_SIZE_X, PERCENTAGE_OF_SIZE_Y, PARALLEL_TO_TOP, PARALLEL_TO_LEFT,
        SIZE_X_MINUS_ARGUMENT, SIZE_Y_MINUS_ARGUMENT
    };

    UIConstraint() {
        
    }
    
    UIConstraint(float arg) {
        argument = arg;
    }

    UIConstraint(ConstraintStyle style, UIObject *ref, float arg) {
        if ((style == LEFT) || (style == TOP) || (style == INSIDE_BOTTOM) || (style == INSIDE_RIGHT)) {
            ofLog() << "ERROR IN CONSTRAINT: insufficient references";
            constraintStyle = VALUE;
            argument = arg;
        }
        constraintStyle = style;
        reference = ref;
        argument = arg;
    }

    UIConstraint(ConstraintStyle style, UIObject *ref, UIObject *thisobj, float arg) {
        constraintStyle = style;
        reference = ref;
        thisobject = thisobj;
        argument = arg;
    }
    
    
    // Problems with solving:
    // 1) The reference can only be at the same level. You can't reference, say, parent object.
    // 2) Placing object to the left of something needs object size x for position.
    //    It might be a big problem if object's size x depends on something else.
    float solve() {
        switch (constraintStyle) {
            case VALUE:
                return argument;
                break;
            case LEFT:
                return reference->position.x - argument - thisobject->size.x;
                break;
            case RIGHT:
                return reference->position.x + reference->size.x + argument;
                break;
            case TOP:
                return reference->position.y - argument - thisobject->size.y;
                break;
            case INSIDE_LEFT:
                return argument;
            case INSIDE_TOP:
                return argument;
            case INSIDE_RIGHT:
                return reference->size.x - thisobject->size.x - argument;
            case INSIDE_BOTTOM:
                return reference->size.y - thisobject->size.y - argument;
            case BOTTOM:
                return reference->position.y + reference->size.y + argument;
                break;
            case PERCENTAGE_OF_SIZE_X:
                return reference->size.x * argument;
                break;
            case PERCENTAGE_OF_SIZE_Y:
                return reference->size.y * argument;
                break;
            case PARALLEL_TO_TOP:
                return reference->position.y + argument;
                break;
            case PARALLEL_TO_LEFT:
                return reference->position.x + argument;
                break;
            case SIZE_X_MINUS_ARGUMENT:
                return reference->size.x - argument;
                break;
            case SIZE_Y_MINUS_ARGUMENT:
                return reference->size.y - argument;
                break;

            default:
                break;
        }
    }
    
    float argument = 0;
    UIObject* reference = NULL;
    UIObject* thisobject = NULL;
    ConstraintStyle constraintStyle = VALUE;
    
    
};



class UIConstraint2D {
public:
    UIConstraint2D() {
        
    }
    
    UIConstraint2D(UIConstraint X, UIConstraint Y) {
        x = X;
        y = Y;
    }
    
    UIConstraint x, y;
    
    ofPoint solve() {
        return ofPoint(x.solve(), y.solve());
    }
};

class ConstraintSolver {
public:
    ConstraintSolver(){}
    
    /// static stuff
    
    static std::map<UIObject*, UIConstraint2D> positionConstraints, sizeConstraints;
    
    static void solveConstraints() {
        // if it will not compile on windows, replace with smthn like:
        //typedef std::map<std::string, std::map<std::string, std::string>>::iterator it_type;
        for (auto i = positionConstraints.begin(); i != positionConstraints.end(); i++) {
            i->first->position = positionConstraints[i->first].solve();
        }
        for (auto i = sizeConstraints.begin(); i != sizeConstraints.end(); i++) {
            i->first->size = sizeConstraints[i->first].solve();
        }
    }
    
    static void addPositionConstraint(UIObject *obj, UIConstraint2D cc) {
        positionConstraints[obj] = cc;
    }
    
    static void addSizeConstraint(UIObject *obj, UIConstraint2D cc) {
        sizeConstraints[obj] = cc;
    }
};


#endif /* UIConstraint_h */
