//source file
#include "../include/modify_consteval_constructor_69.h"

// ========================================================================================================
#define MUT69_OUTPUT 1

void MutatorFrontendAction_69::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constevalCtor")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      //Perform mutation on the source code text by applying string replacement
      if (MT->isConsteval()) {
        SourceLocation startLoc = MT->getBeginLoc();
        SourceLocation endLoc = MT->getEndLoc();
        std::string originalText = Lexer::getSourceText(CharSourceRange::getTokenRange(startLoc, endLoc), 
                                                        *Result.SourceManager, 
                                                        Result.Context->getLangOpts()).str();
        std::string mutatedText = originalText;
        size_t pos = mutatedText.find("consteval");
        if (pos != std::string::npos) {
          mutatedText.replace(pos, 9, "constexpr");
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(startLoc, endLoc), mutatedText);
        }
      }
    }
}
  
void MutatorFrontendAction_69::MutatorASTConsumer_69::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(isConsteval()).bind("constevalCtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}