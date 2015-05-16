// All the domain logic is here.
// Notice, that while not knowing what
// Qt is or including single header of
// Qt we're able to fully manipulate
// the GUI as long as we specify the messages
// and GUI side implements them.

#include <templatious/FullPack.hpp>

#include "domain.h"
#include "mainwindow_interface.h"

TEMPLATIOUS_TRIPLET_STD;

bool isPrime(int number) {
    int count = 0;
    for (int i = 1; i <= number; ++i) {
        if (number % i == 0) {
            ++count;
        }
    }
    return count == 2;
}

void initDomain(std::shared_ptr< Messageable > mainWindow) {
    typedef MainWindowInterface Msg;

    // no shared_ptr loops
    Messageable* raw = mainWindow.get();
    typedef std::unique_ptr< templatious::VirtualMatchFunctor > Callback;
    Callback callbacks = SF::virtualMatchFunctorPtr(
        SF::virtualMatch< Msg::OutButtonClicked >(
            [=](Msg::OutButtonClicked) {
                // on callback we know that we're
                // in the same thread as GUI so we create stack
                // allocated virtual pack with SF::vpack
                auto disMsg = SF::vpack< Msg::InSetButtonEnabled, bool >(
                    Msg::InSetButtonEnabled(), false
                );

                // disable GO button
                raw->message(disMsg);

                // query current line edit
                // (it will be written to virtual pack
                auto outLabel = SF::vpack< Msg::QueryLabelText, std::string >(
                    Msg::QueryLabelText(), ""
                );
                raw->message(outLabel);

                // receive reference to a string
                // from virtual pack
                auto& str = outLabel.fGet<1>();

                // parse the line edit text
                int num = std::atoi(str.c_str());

                // prepare end message to enable button again,
                // this will be sent from another thread
                // in case parse succeeds
                auto enMsg = SF::vpackPtr< Msg::InSetButtonEnabled, bool >(
                    Msg::InSetButtonEnabled(), true
                );
                if (num > 0) {
                    // make a weak pointer because main window could
                    // have quitted when in our second thread,
                    // therefore avoid using freed memory.
                    std::weak_ptr< Messageable > wptr = mainWindow;

                    // number parsing succeeded,
                    // launch new thread for calculation
                    std::thread([=]() {
                        int primeCount = 0;
                        int i = 1;
                        while (primeCount < num) {
                            ++i;
                            if (isPrime(i)) {
                                ++primeCount;
                                if (primeCount % 100 == 0) {
                                    // string for the label
                                    std::string msgString = "Found ";
                                    msgString += std::to_string(primeCount);
                                    msgString += " primes.";

                                    // calculate progress
                                    float progress = (static_cast<float>(primeCount) / num) * 100;
                                    int progInt = static_cast<int>(progress);

                                    // lock weak pointer
                                    auto locked = wptr.lock();
                                    if (wptr.expired()) {
                                        // main window dead, return
                                        return;
                                    }

                                    // this message needs to be sent across threads
                                    // so we allocate virtual pack on the heap wrapped
                                    // inside shared_ptr with SF::vpackPtr.
                                    auto msg = SF::vpackPtr< Msg::InSetStatusText, const std::string >(
                                        Msg::InSetStatusText(),std::move(msgString)
                                    );
                                    auto progMsg = SF::vpackPtr< Msg::InSetProgress, const int >(
                                        Msg::InSetProgress(), progInt
                                    );

                                    // send messages across threads
                                    // send label update
                                    locked->message(msg);
                                    // send progress bar update
                                    locked->message(progMsg);
                                }
                            }
                        }

                        // calculation finished in it's own thread,
                        // prepare result strings
                        std::string final = std::to_string(num);
                        final += " prime is ";
                        final += std::to_string(i);
                        final += ".";

                        auto setLabelMsg = SF::vpackPtr< Msg::InSetStatusText, const std::string >(
                            Msg::InSetStatusText(), std::move(final)
                        );
                        auto setProgMsg = SF::vpackPtr< Msg::InSetProgress, const int >(
                            Msg::InSetProgress(), 100
                        );

                        // set progress bar to 100 on end
                        raw->message(setProgMsg);
                        // set final status label
                        raw->message(setLabelMsg);
                        // disable GO button
                        raw->message(enMsg);
                    }).detach();
                } else {
                    // parsing failed, set status
                    auto failMessage = SF::vpack< Msg::InSetStatusText, const std::string >(
                        Msg::InSetStatusText(), "Could not parse integer."
                    );
                    raw->message(failMessage);

                    // reenable GO button
                    raw->message(enMsg);
                }
            }
        )
    );

    // message to attach callback handler in Qt side
    auto setCallbacks = SF::vpack< Msg::InAttachCallback, Callback >(
        Msg::InAttachCallback(), std::move(callbacks)
    );

    // set current progress from default to 0
    auto setProgress = SF::vpack< Msg::InSetProgress, const int >(Msg::InSetProgress(),0);

    // show the window
    auto show = SF::vpack< Msg::InShow >(Msg::InShow());

    // this is called from the main single thread
    // so we send stack allocated messages here.
    mainWindow->message(setCallbacks);
    mainWindow->message(setProgress);
    mainWindow->message(show);
}
