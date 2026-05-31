//source file
#include "../include/modify_explicit_template_argument_10.h"

// ========================================================================================================
#define MUT10_OUTPUT 1

void MutatorFrontendAction_10::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("TemplateFunctionCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CallExpr->getExprLoc()))
            return;

        if (CallExpr->getNumArgs() > 0) {
            const auto *TemplateArgs = CallExpr->getDirectCallee()->getTemplateSpecializationArgs();
            if (!TemplateArgs)
                return;
            
            std::string newType = "float";
            const auto *OldArg = TemplateArgs->get(0).getAsType().getTypePtrOrNull();
            if (!OldArg)
                return;

            auto OldTypeSourceRange = CallExpr->getArg(0)->getSourceRange();
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(OldTypeSourceRange), newType);
        }
    }
}

void MutatorFrontendAction_10::MutatorASTConsumer_10::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasAnyTemplateArgument(anything())))).bind("TemplateFunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}