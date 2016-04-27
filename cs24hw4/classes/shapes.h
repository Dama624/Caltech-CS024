#ifndef SHAPES_H
#define SHAPES_H


/*============================================================================
 * General Functions
 */


/*!
 * This function performs static initialization of all classes.  It must be
 * called before any of the classes can be used.
 */  
void static_init();


/*============================================================================
 * Shape base class
 *
 * Represents a 3D shape with a volume and density.  The mass of the shape can
 * be computed by multiplying the volume and density.
 *
 * As one would expect, the getVolume() method in this class is ABSTRACT (in
 * C++ parlance, we call it "pure virtual"), because there is no way to define
 * what this function should do at this level in the abstraction hierarchy.
 * However, we know that all concrete shapes will have a volume, so the sub-
 * classes must implement getVolume() themselves.
 *
 * Roughly corresponds to the C++ class declaration:
 *
 *     class Shape {
 *         float density;
 *     public:
 *         Shape(float D) {
 *             setDensity(D);
 *         }
 *
 *         void setDensity(float D) {
 *             assert(D > 0);
 *             density = D;
 *         }
 *
 *         // This method is abstract (a.k.a. pure-virtual), since it cannot
 *         // be defined for this generic type.
 *         virtual float getVolume() = 0;
 *
 *         float getMass() {
 *             return density * getVolume();
 *         }
 *     };
 */

/* (declare these typedefs first, since the structs reference each other) */
typedef struct Shape_Class Shape_Class;
typedef struct Shape_Data Shape_Data;

/*! Shape class information, static members, and virtual function pointers. */
struct Shape_Class {
    /* This will be NULL for the Shape class since the method is abstract
     * (i.e. pure-virtual).  Subclasses will set this function-pointer to
     * non-NULL, since they provide an implementation of the method.
     */
    float (*getVolume)(Shape_Data *this);
};

/*! Shape object data-members, and reference to Shape class information. */
struct Shape_Data {
    Shape_Class *class;         /*!< Class information for the Shape class. */

    float density;              /*!< The density of this shape object. */
};

/*! Static initialization for the Shape class. */
void Shape_class_init(Shape_Class *class);

/*!
 * Object initialization (i.e. the constructor) for the Shape class.  This
 * function initializes the density of the shape, as well as the class info.
 */
void Shape_init(Shape_Data *this, Shape_Class *class, float D);

/*! Sets the density of this shape.  The argument must be nonnegative! */
void Shape_setDensity(Shape_Data *this, float D);

/*! Returns the mass of this shape, computed from the density and volume. */
float Shape_getMass(Shape_Data *this);

/*
 * THERE IS NO Shape_getVolume() FUNCTION, because Shape doesn't provide an
 * implementation!  In the class initialization, set the function-pointer to
 * NULL to signify this.
 */
 
/*
 * There is also no new_Shape() function, since Shape is abstract (missing
 * some of its implementation), and therefore is not instantiable.
 */


/*============================================================================
 * Box subclass
 *
 * A box is a 3D cuboid (aka "right prism"), with a length, width, and height.
 * The box's volume is simply length * width * height.
 *
 * Roughly corresponds to the C++ class declaration:
 *
 *     class Box : public Shape {
 *         float length, width, height;
 *
 *     public:
 *         Box(float L, float W, float H, float D) : Shape(D) {
 *             setSize(L, W, H);
 *         }
 *
 *         void setSize(float L, float W, float H) {
 *             assert(L > 0);
 *             assert(W > 0);
 *             assert(H > 0);
 *
 *             length = L;
 *             width  = W;
 *             height = H;
 *         }
 *
 *         virtual float getVolume() {
 *             return length * width * height;
 *         }
 *     };
 */

/* (declare these typedefs first, since the structs reference each other) */
typedef struct Box_Class Box_Class;
typedef struct Box_Data Box_Data;

/*! Box class information, static members, and virtual function pointers. */
struct Box_Class {
    float (*getVolume)(Box_Data *this);
};

/*! Box object data-members, and reference to Shape class information. */
struct Box_Data {
    Box_Class *class;           /*!< Class information for the Box class. */
    float density;              /*!< The density of this shape object. */

    float length;               /*!< The length of the box. */
    float width;                /*!< The width of the box. */
    float height;               /*!< The height of the box. */
};

/*! Static initialization for the Box class. */
void Box_class_init(Box_Class *class);

/*!
 * Object initialization (i.e. the constructor) for the Box class.  This
 * function first calls the Shape constructor to initialize the class info and
 * density, and then it initializes its data members with the specified values.
 */
void Box_init(Box_Data *this, Box_Class *class,
    float L, float W, float H, float D);

/*!
 * This function implements the operation corresponding to the C++ code
 * "new Box(L, W, H, D)", performing both heap-allocation and initialization. 
 */
Box_Data * new_Box(float L, float W, float H, float D);

/*!
 * Sets the dimensions of the box.  The arguments are asserted to be positive.
 */
void Box_setSize(Box_Data *this, float L, float W, float H);

/*!
 * Computes the volume of the box.  This function provides the implementation
 * of Shape::getVolume(), which is abstract (i.e. pure-virtual).
 */
float Box_getVolume(Box_Data *this);


/*============================================================================
 * Sphere subclass
 *
 * A sphere is a simple object, having only a radius.
 * The sphere's volume is 4/3 * PI * radius^3.
 *
 * Roughly corresponds to the C++ class declaration:
 *
 *     class Sphere : public Shape {
 *         float radius;
 *
 *     public:
 *         Sphere(float R, float D) : Shape(D) {
 *             setRadius(R);
 *         }
 *
 *         void setRadius(float R) {
 *             assert(R > 0);
 *             radius = R;
 *         }
 *
 *         virtual float getVolume() {
 *             // Fastest to compute R^3 by multiplying it out ourselves.
 *             return (4 / 3) * 3.14159 * radius * radius * radius;
 *         }
 *     };
 */

/* (declare these typedefs first, since the structs reference each other) */
typedef struct Sphere_Class Sphere_Class;
typedef struct Sphere_Data Sphere_Data;

/*! Sphere class information, static members, and virtual function pointers. */
struct Sphere_Class {
    float (*getVolume)(Sphere_Data *this);
};

/*! Sphere object data-members, and reference to Shape class information. */
struct Sphere_Data {
    Sphere_Class *class;        /*!< Class information for the Sphere class. */
    float density;              /*!< The density of this shape object. */

    float radius;               /*!< The radius of the sphere. */
};

/*! Static initialization for the Sphere class. */
void Sphere_class_init(Sphere_Class *class);

/*!
 * Object initialization (i.e. the constructor) for the Sphere class.  This
 * function first calls the Shape constructor to initialize the class info and
 * density, and then it initializes its data members with the specified values.
 */
void Sphere_init(Sphere_Data *this, Sphere_Class *class, float R, float D);

/*!
 * This function implements the operation corresponding to the C++ code
 * "new Sphere(R, D)", performing both heap-allocation and initialization. 
 */
Sphere_Data * new_Sphere(float R, float D);

/*! Sets the radius of the sphere.  The argument is asserted to be positive. */
void Sphere_setRadius(Sphere_Data *this, float R);

/*!
 * Computes the volume of the sphere.  This function provides the implementation
 * of Shape::getVolume(), which is abstract (i.e. pure-virtual).
 */
float Sphere_getVolume(Sphere_Data *this);


/*============================================================================
 * Cone subclass
 *
 * A cone has both a radius of its base, and a height.
 * The cone's volume is 1/3 * PI * base_radius * base_radius * height.
 *
 * Roughly corresponds to the C++ class declaration:
 *
 *     class Cone : public Shape {
 *         float base_radius;
 *         float height;
 *
 *     public:
 *         Cone(float BR, float H, float D) : Shape(D) {
 *             setBaseHeight(BR, H);
 *         }
 *
 *         void setBaseHeight(float BR, float H) {
 *             assert(BR > 0);
 *             assert(H > 0);
 *
 *             base_radius = BR;
 *             height = H;
 *         }
 *
 *         virtual float getVolume() {
 *             return (1 / 3) * 3.14159 * base_radius * base_radius * height;
 *         }
 *     };
 */

/* (declare these typedefs first, since the structs reference each other) */
typedef struct Cone_Class Cone_Class;
typedef struct Cone_Data Cone_Data;

/*! Cone class information, static members, and virtual function pointers. */
struct Cone_Class {
    float (*getVolume)(Cone_Data *this);
};

/*! Cone object data-members, and reference to Shape class information. */
struct Cone_Data {
    Cone_Class *class;          /*!< Class information for the Cone class. */
    float density;              /*!< The density of this shape object. */

    float base_radius;          /*!< The radius of the cone at its base. */
    float height;               /*!< The height of the cone. */
};

/*! Static initialization for the Cone class. */
void Cone_class_init(Cone_Class *class);

/*!
 * Object initialization (i.e. the constructor) for the Cone class.  This
 * function first calls the Shape constructor to initialize the class info and
 * density, and then it initializes its data members with the specified values.
 */
void Cone_init(Cone_Data *this, Cone_Class *class, float BR, float H, float D);

/*!
 * This function implements the operation corresponding to the C++ code
 * "new Cone(BR, H, D)", performing both heap-allocation and initialization. 
 */
Cone_Data * new_Cone(float BR, float H, float D);

/*!
 * Sets the dimensions of the cone.  The arguments are asserted to be positive.
 */
void Cone_setBaseHeight(Cone_Data *this, float BR, float H);

/*!
 * Computes the volume of the cone.  This function provides the implementation
 * of Shape::getVolume(), which is abstract (i.e. pure-virtual).
 */
float Cone_getVolume(Cone_Data *this);


#endif /* SHAPES_H */

