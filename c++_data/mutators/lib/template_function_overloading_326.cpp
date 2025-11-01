//source file
#include "../include/Template_Function_Overloading_326.h"

// ========================================================================================================
#define MUT326_OUTPUT 1

void MutatorFrontendAction_326::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      if (FTD->getTemplatedDecl()->hasBody()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   FTD->getSourceRange());

        //Perform mutation on the source code text by applying string replacement
        size_t pos = declaration.find("==");
        if (pos != std::string::npos) {
          declaration.replace(pos, 2, "== static_cast<int>(");
          pos = declaration.find(";", pos);
          if (pos != std::string::npos) {
            declaration.insert(pos, ")");
          }
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_326::MutatorASTConsumer_326::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(
        hasDescendant(binaryOperator(hasOperatorName("==")))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}