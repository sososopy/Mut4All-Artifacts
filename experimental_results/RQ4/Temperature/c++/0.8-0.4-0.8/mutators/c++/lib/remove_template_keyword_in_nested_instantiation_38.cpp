//source file
#include "../include/remove_template_keyword_in_nested_instantiation_38.h"

// ========================================================================================================
#define MUT38_OUTPUT 1

void MutatorFrontendAction_38::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("templateCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CallExpr->getBeginLoc()))
            return;

        auto MemberExpr = llvm::dyn_cast<clang::MemberExpr>(CallExpr->getCallee());
        if (!MemberExpr || !MemberExpr->getNumTemplateArgs())
            return;

        auto SourceRange = MemberExpr->getSourceRange();
        auto Start = SourceRange.getBegin();
        auto End = SourceRange.getEnd();

        llvm::StringRef SourceTextRef = clang::Lexer::getSourceText(
            clang::CharSourceRange::getTokenRange(Start, End),
            *Result.SourceManager, clang::LangOptions());

        std::string SourceText = SourceTextRef.str();

        size_t templatePos = SourceText.find("template");
        if (templatePos != std::string::npos) {
            SourceText.erase(templatePos, 8);
            Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(Start, End), SourceText);
        }
    }
}
  
void MutatorFrontendAction_38::MutatorASTConsumer_38::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    
    MatchFinder matchFinder;
    auto matcher = callExpr(
        callee(memberExpr(hasDescendant(expr())).bind("templateCall"))
    ).bind("templateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}