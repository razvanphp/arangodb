////////////////////////////////////////////////////////////////////////////////
/// @brief transaction manager
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2013 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Jan Steemann
/// @author Copyright 2011-2013, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_TRANSACTION_MANAGER_H
#define TRIAGENS_TRANSACTION_MANAGER_H 1

#include "Basics/Common.h"
#include "Basics/ReadWriteLock.h"
#include "Transaction/IdGenerator.h"
#include "Transaction/Transaction.h"

extern "C" {
  struct TRI_vocbase_s;
}

namespace triagens {
  namespace transaction {

// -----------------------------------------------------------------------------
// --SECTION--                                          class TransactionManager
// -----------------------------------------------------------------------------

    class Manager {

////////////////////////////////////////////////////////////////////////////////
/// @brief TransactionManager
////////////////////////////////////////////////////////////////////////////////

      private:
        Manager (Manager const&);
        Manager& operator= (Manager const&);
        Manager ();
        ~Manager ();

// -----------------------------------------------------------------------------
// --SECTION--                                                    public methods
// -----------------------------------------------------------------------------

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief get the transaction manager instance
////////////////////////////////////////////////////////////////////////////////

        static Manager* instance ();

////////////////////////////////////////////////////////////////////////////////
/// @brief initialise the transaction manager instance
////////////////////////////////////////////////////////////////////////////////

        void initialise ();

////////////////////////////////////////////////////////////////////////////////
/// @brief shutdown the transaction manager instance
////////////////////////////////////////////////////////////////////////////////

        void shutdown ();

////////////////////////////////////////////////////////////////////////////////
/// @brief create a transaction object
////////////////////////////////////////////////////////////////////////////////

        Transaction* createTransaction (struct TRI_vocbase_s*);

////////////////////////////////////////////////////////////////////////////////
/// @brief begin a transaction
////////////////////////////////////////////////////////////////////////////////

        int beginTransaction (Transaction*);

////////////////////////////////////////////////////////////////////////////////
/// @brief commit a transaction
////////////////////////////////////////////////////////////////////////////////

        int commitTransaction (Transaction*);

////////////////////////////////////////////////////////////////////////////////
/// @brief abort a transaction
////////////////////////////////////////////////////////////////////////////////

        int abortTransaction (Transaction*);

// -----------------------------------------------------------------------------
// --SECTION--                                                 private variables
// -----------------------------------------------------------------------------

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief transaction id generator
////////////////////////////////////////////////////////////////////////////////

        IdGenerator _generator;

////////////////////////////////////////////////////////////////////////////////
/// @brief R/W lock for transactions tables
////////////////////////////////////////////////////////////////////////////////

        basics::ReadWriteLock _lock;

////////////////////////////////////////////////////////////////////////////////
/// @brief all running or aborted transactions
////////////////////////////////////////////////////////////////////////////////

        map<Transaction::IdType, Transaction::StateType> _transactions;

    };

  }
}

#endif

// Local Variables:
// mode: outline-minor
// outline-regexp: "/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}"
// End:
