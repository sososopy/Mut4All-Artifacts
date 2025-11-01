//source file
#include "../include/modify_constrained_constructor_template_127.h"

// ========================================================================================================
#define MUT127_OUTPUT 1

void MutatorFrontendAction_127::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;

      if (CD->getDescribedFunctionTemplate()) {
        // Attempt to get constraints via template parameter list
        if (auto *TPL = CD->getDescribedFunctionTemplate()->getTemplateParameters()) {
          for (auto *Param : *TPL) {
            if (auto *NTTP = llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(Param)) {
              auto constraintExpr = NTTP->getSourceRange();
              auto constraintText = stringutils::rangetoStr(*(Result.SourceManager), constraintExpr);
              
              //Perform mutation on the source code text by applying string replacement
              std::string newConstraint = constraintText + " && sizeof...(args) > 1";
              
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(constraintExpr), newConstraint);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_127::MutatorASTConsumer_127::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(isTemplateInstantiation()).bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}