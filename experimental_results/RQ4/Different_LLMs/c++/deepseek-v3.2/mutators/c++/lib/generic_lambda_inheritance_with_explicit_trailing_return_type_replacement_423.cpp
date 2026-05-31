//source file
#include "../include/Mutator_Generic_Lambda_Inheritance_With_Explicit_Trailing_Return_Type_Replacement_423.h"

// ========================================================================================================
#define MUT423_OUTPUT 1

void MutatorFrontendAction_423::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("TrailingReturn")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto trailingReturnType = MT->getReturnType();
      if (trailingReturnType.isNull()) return;
      
      auto decltypeExpr = dyn_cast<DecltypeType>(trailingReturnType.getTypePtr());
      if (!decltypeExpr) return;
      
      auto expr = decltypeExpr->getUnderlyingExpr();
      if (!expr) return;
      
      auto commaExpr = dyn_cast<BinaryOperator>(expr);
      if (!commaExpr || commaExpr->getOpcode() != BO_Comma) return;
      
      auto lhs = commaExpr->getLHS();
      auto rhs = commaExpr->getRHS();
      
      auto callExpr = dyn_cast<CXXMemberCallExpr>(lhs);
      if (!callExpr) return;
      
      auto memberExpr = dyn_cast<MemberExpr>(callExpr->getCallee());
      if (!memberExpr) return;
      
      auto declRef = dyn_cast<DeclRefExpr>(memberExpr->getBase());
      if (!declRef) return;
      
      auto templateParamDecl = dyn_cast<TemplateTypeParmType>(declRef->getDecl()->getType().getTypePtr());
      if (!templateParamDecl) return;
      
      auto lambdaExpr = dyn_cast<LambdaExpr>(callExpr->getImplicitObjectArgument());
      if (!lambdaExpr) return;
      
      //Perform mutation on the source code text by applying string replacement
      string originalText = stringutils::rangetoStr(*(Result.SourceManager), MT->getReturnTypeSourceRange());
      
      string mutatedText;
      int choice = getrandom::getRandomIndex(2);
      if (choice == 0) {
        mutatedText = "-> decltype(static_cast<Func*>(this)->operator()(args...), void())";
      } else {
        mutatedText = "-> decltype(this->Func::operator()(args...), void())";
      }
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getReturnTypeSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_423::MutatorASTConsumer_423::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(
        hasTrailingReturn(),
        hasTemplateArgument(0, refersToType(templateTypeParmType())),
        hasParameter(0, hasType(autoType()))
    ).bind("TrailingReturn");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}