//source file
#include "../include/insert_unreachable_code_block_284.h"

// ========================================================================================================
#define MUT284_OUTPUT 1

void MutatorFrontendAction_284::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasBody()) {
            auto body = FD->getBody();
            auto bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());

            std::string unreachableCode = "\n/*mut284*/if (false) {\n    int unreachable = 0;\n}\n";

            if (bodyText.rfind('}') != std::string::npos)
                bodyText.insert(bodyText.rfind('}'), unreachableCode);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), bodyText);
        }
    }
}
  
void MutatorFrontendAction_284::MutatorASTConsumer_284::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}