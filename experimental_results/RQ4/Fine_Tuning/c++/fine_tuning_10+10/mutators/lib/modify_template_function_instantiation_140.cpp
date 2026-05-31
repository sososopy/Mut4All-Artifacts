//source file
#include "../include/modify_template_function_instantiation_140.h"

// ========================================================================================================
#define MUT140_OUTPUT 1

void MutatorFrontendAction_140::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("templateCall")) {
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CallExpr->getBeginLoc()))
            return;

        const TemplateArgumentList *TemplateArgs = CallExpr->getDirectCallee()->getTemplateSpecializationArgs();
        if (!TemplateArgs)
            return;

        std::string modifiedCall = Lexer::getSourceText(CharSourceRange::getTokenRange(CallExpr->getSourceRange()), 
                                                        *Result.SourceManager, 
                                                        Result.Context->getLangOpts()).str();

        size_t angleBracketPos = modifiedCall.find_last_of('>');
        if (angleBracketPos != std::string::npos) {
            modifiedCall.insert(angleBracketPos, ", int");
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(CallExpr->getSourceRange()), modifiedCall);
        }
    }
}

void MutatorFrontendAction_140::MutatorASTConsumer_140::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = callExpr(callee(functionDecl(hasAnyTemplateArgument(anything())).bind("templateCall")));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}