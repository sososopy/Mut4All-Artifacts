//source file
#include "../include/remove_consteval_from_conversion_operator_84.h"

// ========================================================================================================
#define MUT84_OUTPUT 1

void MutatorFrontendAction_84::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ConvOp = Result.Nodes.getNodeAs<clang::CXXConversionDecl>("ConversionOp")) {
      //Filter nodes in header files
      if (!ConvOp || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ConvOp->getLocation()))
        return;

      if (ConvOp->isConsteval()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   ConvOp->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t pos = declaration.find("consteval");
        if (pos != std::string::npos) {
            declaration.erase(pos, std::string("consteval").length());
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ConvOp->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_84::MutatorASTConsumer_84::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConversionDecl(isConsteval()).bind("ConversionOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}