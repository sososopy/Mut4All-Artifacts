//source file
#include "../include/replace_function_with_template_199.h"

// ========================================================================================================
#define MUT199_OUTPUT 1

void MutatorFrontendAction_199::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody()) {
        //Get the source code text of target node
        auto funcDeclaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                       FD->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string templatePrefix = "template<typename T> ";
        std::string newFuncDeclaration = templatePrefix + funcDeclaration;
        newFuncDeclaration.insert(newFuncDeclaration.find("{") + 1, "\n/*mut199*/T dummyVar; // Template variable\n");
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), newFuncDeclaration);
      }
    }
}
  
void MutatorFrontendAction_199::MutatorASTConsumer_199::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}