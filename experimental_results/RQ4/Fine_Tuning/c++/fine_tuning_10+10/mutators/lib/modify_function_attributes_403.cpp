//source file
#include "../include/modify_function_attributes_403.h"

// ========================================================================================================
#define MUT403_OUTPUT 1

void MutatorFrontendAction_403::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithAttributes")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      //Get the source code text of target node
      auto attrList = FD->getAttrs();
      for (auto *attr : attrList) {
        if (auto *annotateAttr = dyn_cast<AnnotateAttr>(attr)) {
          std::string annotation = annotateAttr->getAnnotation().str();
          if (annotation.find("note") != std::string::npos) {
            //Perform mutation on the source code text by applying string replacement
            std::string mutatedAnnotation = "/*mut403*/" + annotation + " modified";
            Rewrite.ReplaceText(annotateAttr->getLocation(), annotation.size(), mutatedAnnotation);
          }
        }
      }
    }
}
  
void MutatorFrontendAction_403::MutatorASTConsumer_403::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAttr(attr::Annotate)).bind("FunctionWithAttributes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}