//source file
#include "../include/Mutator_Binary_Operator_Overload_478.h"

// ========================================================================================================
#define MUT478_OUTPUT 1

void MutatorFrontendAction_478::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("BinaryOperatorOverload")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (MT->isOverloadedOperator() &&
          MT->getOverloadedOperator() == OverloadedOperatorKind::OO_Plus) {
        auto body = MT->getBody();
        llvm::outs() << "Mut478: " << body << '\n';
        auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                body->getSourceRange());
        llvm::outs() << "Mut478: " << body_str << '\n';
        //Perform mutation on the source code text by applying string replacement
        stringutils::strReplace(body_str, "+", "*");
        body_str = "/*mut478*/" + body_str;
        llvm::outs() << "Mut478: " << body_str << '\n';
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(body->getSourceRange(), body_str);
      }
    }
}
  
void MutatorFrontendAction_478::MutatorASTConsumer_478::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl().bind("BinaryOperatorOverload");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}