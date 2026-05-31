//source file
#include "../include/instantiate_unused_template_function_39.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;
      
      // Ensure the template function is not instantiated elsewhere
      if (FTD->isUsed(false)) return;

      unusedTemplates.push_back(FTD);
    }

    for (auto *FTD : unusedTemplates) {
      //Get the source code text of target node
      std::string instantiation = "template void " + FTD->getNameAsString() + "<int>();\n";
      instantiation += "template void " + FTD->getNameAsString() + "<double>();\n";
      
      //Perform mutation on the source code text by applying string replacement
      SourceLocation insertLoc = FTD->getEndLoc().getLocWithOffset(1);
      Rewrite.InsertText(insertLoc, instantiation, true, true);
    }
}
  
void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}