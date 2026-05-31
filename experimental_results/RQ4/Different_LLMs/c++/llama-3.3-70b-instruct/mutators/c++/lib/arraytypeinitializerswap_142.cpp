//source file
#include "../include/ArrayTypeInitializerSwap_142.h"
#include "clang/AST/Type.h"
#include "clang/AST/Expr.h"

// ========================================================================================================
#define MUT142_OUTPUT 1

void MutatorFrontendAction_142::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Get the source code text of target node
      varDecls.push_back(VD);
    }

    if (varDecls.size() >= 2) {
      // Perform mutation on the source code text by applying string replacement
      std::string init1 = stringutils::rangetoStr(*(Result.SourceManager), varDecls[0]->getInit()->getSourceRange());
      std::string init2 = stringutils::rangetoStr(*(Result.SourceManager), varDecls[1]->getInit()->getSourceRange());
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(varDecls[0]->getInit()->getSourceRange()), init2);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(varDecls[1]->getInit()->getSourceRange()), init1);
    }
}
  
void MutatorFrontendAction_142::MutatorASTConsumer_142::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(hasInitializer(initListExpr())).bind("VarDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}