#ifndef MAINWINDOW_INTERFACE
#define MAINWINDOW_INTERFACE

struct MainWindowInterface {

    // set status text
    // signature <
    // InSetStatusText,
    // const std::string - the text to set
    // >
    struct InSetStatusText {};

    // set progressbar progress
    // signature <
    // InSetProgress,
    // int - progress, from 0 to 100
    // >
    struct InSetProgress {};

    // set if button is enabled
    // signature <
    // InSetButtonEnabled,
    // bool - state
    // >
    struct InSetButtonEnabled {};

    // attach callback
    // signature <
    // InAttachCallback,
    // std::unique_ptr< - callback handler
    //     templatious::VirtualMatchFunctor
    //     >
    // >
    struct InAttachCallback {};

    // show window
    // signature < InShow >
    struct InShow {};

    // query text for the label
    // signature <
    // QueryLabelText,
    // std::string - the text output
    // >
    struct QueryLabelText {};

    // emitted when button is pressed
    // signature <OutButtonClicked>
    struct OutButtonClicked {};
};

#endif // MAINWINDOW_INTERFACE

