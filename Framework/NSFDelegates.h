// Copyright 2004-2014, North State Software, LLC.  All rights reserved.

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef NSF_DELEGATES_H
#define NSF_DELEGATES_H

#include "NSFDelegateContext.h"
#include "NSFVoidAction.h"
#include "NSFBooleanGuard.h"
#include <map>

namespace NorthStateFramework
{
    /// <summary>
    /// Represents a common base for delegate list.
    /// </summary>
    /// <remarks>
    /// This class is for use only by the North State Framework's internal logic.
    /// </remarks>
    class NSFDelegateListBase
    {
    public:

        /// <summary>
        /// Gets the action to take if a delegate throws an exception when called.
        /// </summary>
        /// <remarks>
        /// The exception action is set to NULL by default.
        /// </remarks>
        NSFVoidAction<NSFExceptionContext>* getExceptionAction() const { return exceptionAction; }

        /// <summary>
        /// Sets the action to take if a delegate throws an exception when called.
        /// </summary>
        /// <remarks>
        /// The exception action is set to NULL by default.
        /// </remarks>
        void setExceptionAction(const NSFVoidAction<NSFExceptionContext>* value);

        /// <summary>
        /// Sets the action to take if a delegate throws an exception when called.
        /// </summary>
        /// <remarks>
        /// The exception action is set to NULL by default.
        /// </remarks>
        void setExceptionAction(const NSFVoidAction<NSFExceptionContext>& value)
        {
            setExceptionAction(&value);
        }

    protected:

        NSFVoidAction<NSFExceptionContext>* exceptionAction;

        /// <summary>
        /// Creates a delegate list base class.
        /// </summary>
        NSFDelegateListBase()
            : exceptionAction(NULL)
        {}

        /// <summary>
        /// Destroys a delegate list base class.
        /// </summary>
        virtual ~NSFDelegateListBase();

        /// <summary>
        /// Gets the delegate mutex.
        /// </summary>
        /// <returns>The mutex.</returns>
        static NSFOSMutex* getDelegateListMutex() { static NSFOSMutex* delegateMutex = NSFOSMutex::create(); return delegateMutex; }
    };

    /// <summary>
    /// Represents a list of function delegates (multi-cast delegate).
    /// </summary>
    /// <remarks>
    /// This class encapsulates common behavior for concrete delegates list types.
    /// </remarks>
    template<class DelegateType>
    class NSFDelegateList : public NSFDelegateListBase
    {
    public:

        /// <summary>
        /// Checks if the list is empty.
        /// </summary>
        /// <returns>True if the list is empty, false otherwise.</returns>
        bool isEmpty();

        /// <summary>
        /// Adds a delegate to the list.
        /// </summary>
        /// <param name="nsfDelegate">The delegate to add.</param>
        /// <returns>The list of delegates.</returns>
        NSFDelegateList& operator+=(const DelegateType& nsfDelegate);

        /// <summary>
        /// Adds a delegate to the list.
        /// </summary>
        /// <param name="nsfDelegate">The delegate to add.</param>
        /// <returns>The list of delegates.</returns>
        NSFDelegateList& operator+=(const DelegateType* nsfDelegate);

        /// <summary>
        /// Removes a delegate from the list.
        /// </summary>
        /// <param name="nsfDelegate">The delegate to add.</param>
        /// <returns>The list of delegates.</returns>
        NSFDelegateList& operator-=(const DelegateType& nsfDelegate);

        /// <summary>
        /// Removes a delegate from the list.
        /// </summary>
        /// <param name="nsfDelegate">The delegate to add.</param>
        /// <returns>The list of delegates.</returns>
        NSFDelegateList& operator-=(const DelegateType* nsfDelegate);

    protected:

        std::list<DelegateType*> delegateList;

        /// <summary>
        /// Creates a delegate list.
        /// </summary>
        NSFDelegateList()
        {
        }

        /// <summary>
        /// Creates a delegate list.
        /// </summary>
        /// <param name="nsfDelegate">A delegate to add.</param>
        NSFDelegateList(const DelegateType& nsfDelegate)
        {
            *this += nsfDelegate;
        }

        /// <summary>
        /// Creates a delegate list.
        /// </summary>
        /// <param name="nsfDelegate">A delegate to add.</param>
        NSFDelegateList(const DelegateType* nsfDelegate)
        {
            *this += nsfDelegate;
        }

        /// <summary>
        /// Creates a copy of a delegate list.
        /// </summary>
        /// <param name="other">The delegate list to copy.</param>
        /// <remarks>
        /// Only the list of delegates is copied.
        /// The exception action is set to the default NULL value.
        /// </remarks>
        NSFDelegateList(const NSFDelegateList& other);

        /// <summary>
        /// Destroys a delegate list.
        /// </summary>
        virtual ~NSFDelegateList();
    };

    /// <summary>
    /// Represents a list of guards.
    /// </summary>
    /// <remarks>
    /// The primary purpose of this class is to provide exception handling around guard invocations.
    /// An exception thrown by any guard will be caught, allowing the other guards to execute.
    /// Notification of guard exceptions is also available.
    /// The execute() method returns true if all guards return true.
    /// If an exception is thrown by any guard, the execute() method returns false.
    /// </remarks>
    template<class ContextType>
    class NSFBoolGuards : public NSFDelegateList<NSFBoolGuard<ContextType> >
    {
        friend class NSFTransition;

    protected:

        /// <summary>
        /// Creates a list of guards.
        /// </summary>
        NSFBoolGuards()
            : NSFDelegateList<NSFBoolGuard<ContextType> >()
        {
        }

        /// <summary>
        /// Creates a list of guards.
        /// </summary>
        /// <param name="guard">A guard to add.</param>
        NSFBoolGuards(const NSFBoolGuard<ContextType>& guard)
            : NSFDelegateList<NSFBoolGuard<ContextType> >(guard)
        {
        }

        /// <summary>
        /// Creates a list of guards.
        /// </summary>
        /// <param name="guard">A guard to add.</param>
        NSFBoolGuards(const NSFBoolGuard<ContextType>* guard)
            : NSFDelegateList<NSFBoolGuard<ContextType> >(guard)
        {
        }

        /// <summary>
        /// Creates a list of guards.
        /// </summary>
        /// <param name="other">The list to copy.</param>
        NSFBoolGuards(const NSFBoolGuards& other)
            : NSFDelegateList<NSFBoolGuard<ContextType> >(other)
        {
        }

        /// <summary>
        /// Destroys a list of guards.
        /// </summary>
        virtual ~NSFBoolGuards();

        /// <summary>
        /// Executes the guards.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        /// <returns>
        /// True if all guards return true, false otherwise.
        /// If an exception is thrown by any guard, returns false.
        /// </returns>
        bool execute(ContextType context);
    };

    /// <summary>
    /// Represents a list of actions.
    /// </summary>
    /// <remarks>
    /// The primary purpose of this class is to provide exception handling around action invocations.
    /// An exception thrown by any action will be caught, allowing the other actions to execute.
    /// Notification of action exceptions is also available.
    /// </remarks>
    template<class ContextType>
    class NSFVoidActions : public NSFDelegateList<NSFVoidAction<ContextType> >
    {
        friend class NSFTransition;
        friend class NSFTimerThread;
        friend class NSFStateMachine;
        friend class NSFState;
        friend class NSFLocalTransition;
        friend class NSFExternalTransition;
        friend class NSFEventHandler;
        friend class NSFInternalTransition;
        friend class NSFScheduledAction;
        friend class NSFThread;
        friend class NSFExceptionHandler;
        friend class std::map<NSFId, NSFVoidActions<NSFEventContext> >;
        friend struct std::pair<Int64 const ,NorthStateFramework::NSFVoidActions<NorthStateFramework::NSFEventContext> >;

    protected:

        /// <summary>
        /// Creates a list of actions.
        /// </summary>
        NSFVoidActions()
            : NSFDelegateList<NSFVoidAction<ContextType> >()
        {
        }

        /// <summary>
        /// Creates a list of actions.
        /// </summary>
        /// <param name="action">An action to add.</param>
        NSFVoidActions(const NSFVoidAction<ContextType>& action)
            : NSFDelegateList<NSFVoidAction<ContextType> >(action)
        {
        }

        /// <summary>
        /// Creates a list of actions.
        /// </summary>
        /// <param name="action">An action to add.</param>
        NSFVoidActions(const NSFVoidAction<ContextType>* action)
            : NSFDelegateList<NSFVoidAction<ContextType> >(action)
        {
        }

        /// <summary>
        /// Creates a list of actions.
        /// </summary>
        /// <param name="other">The list to copy.</param>
        NSFVoidActions(const NSFVoidActions& other)
            : NSFDelegateList<NSFVoidAction<ContextType> >(other)
        {
        }

        /// <summary>
        /// Destroys an action.
        /// </summary>
        virtual ~NSFVoidActions();

        /// <summary>
        /// Calls all the actions in the list.
        /// </summary>
        /// <param name="context">Additional contextual information.</param>
        void execute(ContextType context);
    };


    // Template method definitions

    // NSFDelegateList

    template<class DelegateType>
    NSFDelegateList<DelegateType>::~NSFDelegateList()
    {
        LOCK(getDelegateListMutex())
        {
            while (!delegateList.empty())
            {
                delete delegateList.front();
                delegateList.pop_front();
            }
        }
        ENDLOCK;
    }

    template<class DelegateType>
    bool NSFDelegateList<DelegateType>::isEmpty()
    {
        LOCK(getDelegateListMutex())
        {
            return delegateList.empty();
        }
        ENDLOCK;
    }

    template<class DelegateType>
    NSFDelegateList<DelegateType>& NSFDelegateList<DelegateType>::operator+=(const DelegateType& nsfDelegate)
    {
        LOCK(getDelegateListMutex())
        {
            delegateList.push_back(nsfDelegate.copy());
            return *this;
        }
        ENDLOCK;
    }

    template<class DelegateType>
    NSFDelegateList<DelegateType>& NSFDelegateList<DelegateType>::operator+=(const DelegateType* nsfDelegate)
    {
        if (nsfDelegate != NULL)
        {
            *this += *nsfDelegate;
        }
        return *this;
    }

    template<class DelegateType>
    NSFDelegateList<DelegateType>& NSFDelegateList<DelegateType>::operator-=(const DelegateType& nsfDelegate)
    {
        LOCK(getDelegateListMutex())
        {
            typename std::list<DelegateType*>::iterator delegateIterator;
            for (delegateIterator = delegateList.begin(); delegateIterator != delegateList.end(); ++delegateIterator)
            {
                if ((*(*delegateIterator)) == nsfDelegate)
                {
                    delete (*delegateIterator);
                    delegateList.erase(delegateIterator);
                    break;
                }
            }

            return *this;
        }
        ENDLOCK;
    }

    template<class DelegateType>
    NSFDelegateList<DelegateType>& NSFDelegateList<DelegateType>::operator-=(const DelegateType* nsfDelegate)
    {
        if (nsfDelegate != NULL)
        {
            *this -= *nsfDelegate;
        }
        return *this;
    }

    template<class DelegateType>
    NSFDelegateList<DelegateType>::NSFDelegateList(const NSFDelegateList& other)
    {
        LOCK(getDelegateListMutex())
        {
            typename std::list<DelegateType*>::const_iterator delegateIterator;
            for (delegateIterator = other.delegateList.begin(); delegateIterator != other.delegateList.end(); ++delegateIterator)
            {
                delegateList.push_back((*delegateIterator)->copy());
            }
        }
        ENDLOCK;
    }

    // NSFBoolGuards

    template<class ContextType>
    NSFBoolGuards<ContextType>::~NSFBoolGuards()
    {
    }

    template<class ContextType>
    bool NSFBoolGuards<ContextType>::execute(ContextType context)
    {
        bool returnValue = true;

        NSFBoolGuards<ContextType> guardsCopy(*this);

        typename std::list<NSFBoolGuard<ContextType>*>::iterator guardIterator;
        for (guardIterator = guardsCopy.delegateList.begin(); guardIterator != guardsCopy.delegateList.end(); ++guardIterator)
        {
            try
            {
                returnValue &= (*(*guardIterator))(context);
            }
            catch(const std::exception& exception)
            {
                returnValue = false;
                try
                {
                    if (this->exceptionAction != NULL)
                    {
                        (*this->exceptionAction)(NSFExceptionContext(this, std::runtime_error(NSFString("Exception executing delegate in list: ") + exception.what())));
                    }
                }
                catch(...)
                {
                    // Exception handler had a problem, nothing to do
                }
            }
            catch(...)
            {
                returnValue = false;
                try
                {
                    if (this->exceptionAction != NULL)
                    {
                        (*this->exceptionAction)(NSFExceptionContext(this, std::runtime_error("Unknown exception executing delegate in list")));
                    }
                }
                catch(...)
                {
                    // Exception handler had a problem, nothing to do
                }
            }
        }
        return returnValue;
    }

    // NSFVoidActions

    template<class ContextType>
    NSFVoidActions<ContextType>::~NSFVoidActions()
    {
    }

    template<class ContextType>
    void NSFVoidActions<ContextType>::execute(ContextType context)
    {
        NSFVoidActions<ContextType> actionsCopy(*this);

        typename std::list<NSFVoidAction<ContextType>*>::iterator actionIterator;
        for (actionIterator = actionsCopy.delegateList.begin(); actionIterator != actionsCopy.delegateList.end(); ++actionIterator)
        {
            try
            {
                (*(*actionIterator))(context);
            }
            catch(const std::exception& exception)
            {
                try
                {
                    if (this->exceptionAction != NULL)
                    {
                        (*this->exceptionAction)(NSFExceptionContext(this, std::runtime_error(NSFString("Exception executing delegate in list: ") + exception.what())));
                    }
                }
                catch(...)
                {
                    // Exception handler had a problem, nothing to do
                }
            }
            catch(...)
            {
                try
                {
                    if (this->exceptionAction != NULL)
                    {
                        (*this->exceptionAction)(NSFExceptionContext(this, (std::runtime_error("Unknown exception executing delegate in list"))));
                    }
                }
                catch(...)
                {
                    // Exception handler had a problem, nothing to do
                }
            }
        }
    }
}

#endif // NSF_DELEGATES_H
