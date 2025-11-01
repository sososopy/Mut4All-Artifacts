//source file
#include "../include/Misuse_Template_Params_In_Function_Call_205.h"

// ========================================================================================================
#define MUT205_OUTPUT 1

void MutatorFrontendAction_205::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        if (FD->isConstexpr()) {
            auto sourceRange = FD->getSourceRange();
            auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

            size_t pos = sourceText.find("<");
            if (pos != std::string::npos) {
                sourceText.insert(pos + 1, "int, ");
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), sourceText);
            }
        }
    }
}
  
void MutatorFrontendAction_205::MutatorASTConsumer_205::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(isConstexpr()).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}