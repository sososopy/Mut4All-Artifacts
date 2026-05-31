//source file
#include "../include/Mutator_Insert_Global_Module_Fragment_Before_Module_Declaration_72.h"

// ========================================================================================================
#define MUT72_OUTPUT 1

void MutatorFrontendAction_72::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MD = Result.Nodes.getNodeAs<clang::NamedDecl>("ModuleDecl")) {
      //Filter nodes in header files
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      //Get the source code text of target node
      auto moduleDeclRange = MD->getSourceRange();
      auto moduleDeclText = Lexer::getSourceText(CharSourceRange::getTokenRange(moduleDeclRange), *Result.SourceManager, Result.Context->getLangOpts()).str();
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedText = "module;\n/*mut72*/" + moduleDeclText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(moduleDeclRange), mutatedText);
    }
}
  
void MutatorFrontendAction_72::MutatorASTConsumer_72::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = namedDecl(hasName("ModuleDecl")).bind("ModuleDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}