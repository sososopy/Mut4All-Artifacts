//source file
#include "../include/introduce_invalid_partial_specialization_7.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;

      //Get the source code text of target node
      auto className = CT->getNameAsString();
      std::string invalidSpecialization = "\ntemplate<> class " + className + "<void { /* Invalid specialization */";
      SourceLocation insertLoc = CT->getEndLoc().getLocWithOffset(1);
      
      //Perform mutation on the source code text by applying string replacement
      Rewrite.InsertText(insertLoc, invalidSpecialization, true, true);
    }
}
  
void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = classTemplateDecl(hasName("Base")).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}