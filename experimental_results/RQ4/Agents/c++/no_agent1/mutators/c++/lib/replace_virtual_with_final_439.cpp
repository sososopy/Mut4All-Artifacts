//source file
#include "../include/replace_virtual_with_final_439.h"

// ========================================================================================================
#define MUT439_OUTPUT 1

void MutatorFrontendAction_439::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Method = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("VirtualMethod")) {
      //Filter nodes in header files
      if (!Method || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Method->getLocation()))
        return;

      // Ensure the method is virtual and not pure virtual
      if (Method->isVirtual() && !Method->isPure()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   Method->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t pos = declaration.find("virtual");
        if (pos != std::string::npos) {
            declaration.replace(pos, 7, "final");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Method->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_439::MutatorASTConsumer_439::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isVirtual(), unless(isPure())).bind("VirtualMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}