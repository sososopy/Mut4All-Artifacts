//source file
#include "../include/Modify_Template_Parameter_Indexing_485.h"

// ========================================================================================================
#define MUT485_OUTPUT 1

void MutatorFrontendAction_485::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("TemplateFunction")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->isTemplateInstantiation()) {
            const FunctionTemplateDecl *FTD = FD->getPrimaryTemplate();
            if (FTD) {
                unsigned packSize = FTD->getTemplateParameters()->size();
                paramPackSize[FD] = packSize;

                if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
                    if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CallExpr->getBeginLoc()))
                        return;

                    if (paramPackSize.find(FD) != paramPackSize.end()) {
                        unsigned newIndex = paramPackSize[FD] + 1;
                        std::string newCall = "getElement<" + std::to_string(newIndex) + ">(";
                        auto callSourceRange = CharSourceRange::getTokenRange(CallExpr->getSourceRange());
                        Rewrite.ReplaceText(callSourceRange, newCall);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_485::MutatorASTConsumer_485::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(isTemplateInstantiation(), hasName("getElement")).bind("TemplateFunction");
    auto callMatcher = callExpr(callee(functionDecl(hasName("getElement")))).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}