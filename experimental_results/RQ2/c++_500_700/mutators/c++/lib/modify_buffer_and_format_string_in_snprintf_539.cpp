//source file
#include "../include/modify_buffer_and_format_string_in_snprintf_539.h"

// ========================================================================================================
#define MUT539_OUTPUT 1

void MutatorFrontendAction_539::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Call = Result.Nodes.getNodeAs<clang::CallExpr>("snprintfCall")) {
        //Filter nodes in header files
        if (!Call || !Result.Context->getSourceManager().isWrittenInMainFile(
                         Call->getBeginLoc()))
            return;

        // Ensure the call is to snprintf
        if (auto *FD = Call->getDirectCallee()) {
            if (FD->getNameAsString() != "snprintf")
                return;
        }

        // Check if the number of arguments is at least 3 (buffer, size, format)
        if (Call->getNumArgs() < 3)
            return;

        // Get buffer size argument
        auto *SizeArg = Call->getArg(1);
        if (!SizeArg)
            return;

        // Get format string argument
        auto *FormatArg = Call->getArg(2);
        if (!FormatArg)
            return;

        // Perform mutation on the buffer size and format string
        std::string newSizeText = "5"; // Example: reduce size by 5
        std::string newFormatText = "\"%c %d\""; // Example: add an extra specifier

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(SizeArg->getSourceRange(), newSizeText);
        Rewrite.ReplaceText(FormatArg->getSourceRange(), newFormatText);
    }
}
  
void MutatorFrontendAction_539::MutatorASTConsumer_539::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(hasName("snprintf")))).bind("snprintfCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}