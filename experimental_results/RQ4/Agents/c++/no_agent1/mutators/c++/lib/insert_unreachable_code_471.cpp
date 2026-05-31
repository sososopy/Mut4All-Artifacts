//source file
#include "../include/insert_unreachable_code_471.h"

// ========================================================================================================
#define MUT471_OUTPUT 1

void MutatorFrontendAction_471::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(FS->getLocation()))
        return;

      if (FS->hasBody()) {
        auto body = FS->getBody();
        auto bodyStr = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
        
        // Insert unreachable code at the end of the function body
        std::string unreachableCode = "\n/*mut471*/ while(true) {}";
        if (bodyStr.rfind('}') != std::string::npos) {
          bodyStr.insert(bodyStr.rfind('}'), unreachableCode);
        }
        
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), bodyStr);
      }
    }
}

void MutatorFrontendAction_471::MutatorASTConsumer_471::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isDefinition()).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}