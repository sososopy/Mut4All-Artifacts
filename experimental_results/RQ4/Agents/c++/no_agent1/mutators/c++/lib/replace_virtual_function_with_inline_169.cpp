//source file
#include "../include/replace_virtual_function_with_inline_169.h"

// ========================================================================================================
#define MUT169_OUTPUT 1

void MutatorFrontendAction_169::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("VirtualMethods")) {
        //Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       MT->getLocation()))
            return;

        if (MT->isVirtual() && MT->hasBody()) {
            //Get the source code text of target node
            auto methodSource = stringutils::rangetoStr(*(Result.SourceManager),
                                                        MT->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            std::string inlineMethodSource = methodSource;
            size_t pos = inlineMethodSource.find("virtual");
            if (pos != std::string::npos) {
                inlineMethodSource.replace(pos, 7, "inline");
            }
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), inlineMethodSource);
        }
    }
}

void MutatorFrontendAction_169::MutatorASTConsumer_169::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isVirtual()).bind("VirtualMethods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}