//source file
#include "../include/Mutator_Complex_Template_Argument_Deduction_In_Nested_Initialization_Contexts_494.h"

// ========================================================================================================
#define MUT494_OUTPUT 1

void MutatorFrontendAction_494::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("ConstructExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getLocation()))
        return;
      //Check that construction uses list initialization
      if (!CE->isListInitialization())
        return;
      //Get the source code text of target node
      auto Args = CE->getArgs();
      if (CE->getNumArgs() != 1)
        return;
      const Expr *ArgExpr = Args[0];
      //Check if argument is a template function call or dependent expression
      if (!isa<CallExpr>(ArgExpr) && !isa<CXXConstructExpr>(ArgExpr))
        return;
      //Ensure we have a template argument
      bool HasTemplateArg = false;
      if (auto *Call = dyn_cast<CallExpr>(ArgExpr)) {
        if (auto *Callee = Call->getCallee()->IgnoreImplicit()) {
          if (isa<DeclRefExpr>(Callee)) {
            auto *DRE = dyn_cast<DeclRefExpr>(Callee);
            if (auto *FD = dyn_cast<FunctionDecl>(DRE->getDecl())) {
              if (FD->getTemplateSpecializationInfo())
                HasTemplateArg = true;
            }
          }
        }
      } else if (auto *InnerConstruct = dyn_cast<CXXConstructExpr>(ArgExpr)) {
        auto *Ctor = InnerConstruct->getConstructor();
        if (Ctor->getTemplateSpecializationInfo())
          HasTemplateArg = true;
      }
      if (!HasTemplateArg)
        return;
      //Perform mutation on the source code text by applying string replacement
      SourceRange ArgRange = ArgExpr->getSourceRange();
      string ArgText = stringutils::rangetoStr(*(Result.SourceManager), ArgRange);
      string MutatedText = "(" + ArgText + ")";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ArgRange, MutatedText);
    }
}
  
void MutatorFrontendAction_494::MutatorASTConsumer_494::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = cxxConstructExpr(isListInitialization()).bind("ConstructExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}