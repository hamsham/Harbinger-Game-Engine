
// testing an observer

#include <iostream>

class observer {
    friend class dispatcher;
    
    private:
        bool eventHasOccurred = false;
        
        virtual void notify() { eventHasOccurred = true; }
        
    public:
        observer() {}
        observer( const observer& ) {}
        ~observer() {}
};

class dispatcher {
    private:
        typedef std::vector< observer* > observer_array_t;
        typedef std::vector< observer* >::size_type observer_size_t;
        
        bool state = false;
        observer_array_t observers;
        
    public:
        dispatcher();
        dispatcher( const dispatcher& );
        ~dispatcher() {}
        
        void setState( bool s ) {
            if ( s == state ) return;
            
            state = s;
            
            for ( observer_size_t i( 0 ); i < observers.size(); ++ i )
                observers[ i ]->notify();
        }
};
