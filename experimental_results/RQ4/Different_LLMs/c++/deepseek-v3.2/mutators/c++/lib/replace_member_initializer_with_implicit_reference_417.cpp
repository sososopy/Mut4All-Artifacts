//source file
#include "../include/Mutator_Replace_Member_Initializer_With_Implicit_Reference_417.h"

// ========================================================================================================
#define MUT417_OUTPUT 1

void MutatorFrontendAction_417::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Check if the field is inside a template
      const DeclContext *DC = FD->getDeclContext();
      bool isInTemplate = false;
      while (DC) {
        if (DC->isDependentContext() || 
            llvm::isa<ClassTemplateDecl>(DC) || 
            llvm::isa<FunctionTemplateDecl>(DC)) {
          isInTemplate = true;
          break;
        }
        DC = DC->getParent();
      }
      if (!isInTemplate) return;
      //Check if the field has an initializer
      if (!FD->hasInClassInitializer()) return;
      const Expr *InitExpr = FD->getInClassInitializer();
      if (!InitExpr) return;
      //Get the source code text of target node
      std::string MemberName = FD->getNameAsString();
      std::string InitText = stringutils::rangetoStr(*(Result.SourceManager), InitExpr->getSourceRange());
      //Check if initializer already is a self-reference (just the member name)
      if (InitText == MemberName) return;
      //Perform mutation on the source code text by applying string replacement
      std::string NewInitText = MemberName;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(InitExpr->getSourceRange()), NewInitText);
    }
}
  
void MutatorFrontendAction_417::MutatorASTConsumer_417::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasInClassInitializer(anything())).bind("FieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}