//source file
#include "../include/modify_function_template_return_type_47.h"

// ========================================================================================================
#define MUT47_OUTPUT 1

void MutatorFrontendAction_47::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      // Check if the function template has an 'auto' or 'decltype(auto)' return type
      if (auto *FD = FTD->getTemplatedDecl()) {
          if (FD->getReturnType()->getContainedAutoType()) {
              //Get the source code text of target node
              auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                         FD->getSourceRange());
              //Perform mutation on the source code text by applying string replacement
              std::string newReturnType = "int";  // Choose a concrete return type
              std::string oldReturnType = "auto";
              size_t pos = declaration.find(oldReturnType);
              if (pos != std::string::npos) {
                  declaration.replace(pos, oldReturnType.length(), newReturnType);
              }
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
          }
      }
    }
}
  
void MutatorFrontendAction_47::MutatorASTConsumer_47::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}