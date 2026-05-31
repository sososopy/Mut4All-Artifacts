//source file
#include "../include/replace_virtual_with_final_432.h"

// ========================================================================================================
#define MUT432_OUTPUT 1

void MutatorFrontendAction_432::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("VirtualMethods")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;

      if (VD->isVirtual() && !VD->isPure()) {
        //Get the source code text of target node
        auto methodDecl = stringutils::rangetoStr(*(Result.SourceManager),
                                                  VD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        size_t pos = methodDecl.find("virtual");
        if (pos != std::string::npos) {
            methodDecl.replace(pos, 7, "final");
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(VD->getSourceRange()), methodDecl);
      }
    }
}
  
void MutatorFrontendAction_432::MutatorASTConsumer_432::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(isVirtual(), unless(isPure())).bind("VirtualMethods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}