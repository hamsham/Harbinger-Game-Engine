
#ifndef __HGE_SYSTEM_H__
#define __HGE_SYSTEM_H__

class c_system {
    private:
    
    protected:
    
    public:
        c_system() {}
        virtual ~c_system() {}
        
        virtual bool    init        () = 0;
        virtual int     update      ( float timeDelta ) = 0;
        virtual void    terminate   () = 0;
        
        virtual void    notify      ( const c_message& ) = 0;
};

#endif /* __HGE_SYSTEM_H__ */

