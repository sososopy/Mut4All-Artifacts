//source file
#include "../include/Add_Decltype_This_Return_Type_381.h"

// ========================================================================================================
#define MUT381_OUTPUT 1

void MutatorFrontendAction_381::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        if (!FD->hasBody())
            return;
        SourceManager &SM = *Result.SourceManager;
        SourceLocation start = FD->getBeginLoc();
        SourceLocation end = FD->getEndLoc();
        if (start.isInvalid() || end.isInvalid())
            return;
        std::string originalCode = stringutils::rangetoStr(SM, CharSourceRange::getTokenRange(start, end).getAsRange());
        size_t openParenPos = originalCode.find('(');
        if (openParenPos == std::string::npos)
            return;
        std::string returnType = originalCode.substr(0, originalCode.find(' '));
        std::string rest = originalCode.substr(originalCode.find(' ') + 1);
        std::string newCode = "auto " + rest + " -> decltype(this)";
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(start, end).getAsRange(), newCode);
    }
}
  
void MutatorFrontendAction_381::MutatorASTConsumer_381::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(unless(cxxMethodDecl())).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}