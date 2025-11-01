//source file
#include "../include/Mutator_Modify_Consteval_Constructor_96.h"

// ========================================================================================================
#define MUT96_OUTPUT 1

void MutatorFrontendAction_96::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ConstevalConstructor")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(CD->getLocation()))
        return;

      if (CD->isConsteval()) {
        auto constructorRange = CD->getSourceRange();
        auto constructorText = stringutils::rangetoStr(*(Result.SourceManager), constructorRange);

        // Perform mutation on the source code text by applying string replacement
        size_t pos = constructorText.find("consteval");
        if (pos != std::string::npos) {
          constructorText.replace(pos, 9, "constexpr");
          pos = constructorText.find("compileTimeFunction");
          if (pos != std::string::npos) {
            constructorText.replace(pos, 18, "runtimeFunction");
          }

          // Replace the original AST node with the mutated one
          Rewrite.ReplaceText(CharSourceRange::getTokenRange(constructorRange), constructorText);
        }
      }
    }
}
  
void MutatorFrontendAction_96::MutatorASTConsumer_96::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(isConsteval()).bind("ConstevalConstructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}