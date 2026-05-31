//source file
#include "../include/modify_template_instantiation_559.h"

// ========================================================================================================
#define MUT559_OUTPUT 1

void MutatorFrontendAction_559::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("templateCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CallExpr->getBeginLoc()))
            return;

        const auto *FuncDecl = CallExpr->getDirectCallee();
        if (!FuncDecl || !FuncDecl->getDescribedFunctionTemplate())
            return;

        const auto *TemplateDecl = FuncDecl->getDescribedFunctionTemplate();
        if (templateTypeMap.find(TemplateDecl) == templateTypeMap.end())
            return;

        clang::QualType newType = templateTypeMap[TemplateDecl];
        std::string newTypeName = newType.getAsString();

        auto callSourceRange = CallExpr->getSourceRange();
        std::string callText = stringutils::rangetoStr(*(Result.SourceManager), callSourceRange);

        size_t angleBracketPos = callText.find('<');
        if (angleBracketPos != std::string::npos) {
            size_t endBracketPos = callText.find('>', angleBracketPos);
            if (endBracketPos != std::string::npos) {
                callText.replace(angleBracketPos + 1, endBracketPos - angleBracketPos - 1, newTypeName);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(callSourceRange), callText);
            }
        }
    }
}

void MutatorFrontendAction_559::MutatorASTConsumer_559::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = functionTemplateDecl().bind("templateDecl");
    StatementMatcher callMatcher = callExpr(callee(functionDecl(hasDescendant(templateTypeParmType())))).bind("templateCall");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}