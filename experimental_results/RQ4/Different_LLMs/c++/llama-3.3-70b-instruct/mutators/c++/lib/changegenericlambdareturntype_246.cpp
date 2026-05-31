//source file
#include "../include/ChangeGenericLambdaReturnType_246.h"

// ========================================================================================================
#define MUT246_OUTPUT 1

void MutatorFrontendAction_246::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getBeginLoc()))
        return;
      //Record the lambda expression
      lambdaExprs.push_back(LE);
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   TD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      for (const auto &LE : lambdaExprs) {
        if (LE->getCallOperator()->getReturnType()->isVoidType()) {
          declaration.replace(declaration.find("void"), 4, "int");
        } else if (LE->getCallOperator()->getReturnType()->isIntegerType()) {
          declaration.replace(declaration.find("int"), 3, "float");
        } else if (LE->getCallOperator()->getReturnType()->isFloatingType()) {
          declaration.replace(declaration.find("float"), 5, "double");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_246::MutatorASTConsumer_246::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher lambdaMatcher = lambdaExpr().bind("LambdaExpr");
    DeclarationMatcher typeDeclMatcher = decl().bind("TypeDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.addMatcher(typeDeclMatcher, &callback);
    matchFinder.matchAST(Context);
}