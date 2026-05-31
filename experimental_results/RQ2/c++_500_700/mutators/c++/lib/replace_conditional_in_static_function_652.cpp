//source file
#include "../include/replace_conditional_in_static_function_652.h"

// ========================================================================================================
#define MUT652_OUTPUT 1

void MutatorFrontendAction_652::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CondExpr = Result.Nodes.getNodeAs<clang::ConditionalOperator>("condExpr")) {
        if (!CondExpr || !Result.Context->getSourceManager().isWrittenInMainFile(CondExpr->getBeginLoc()))
            return;

        const auto *Func = Result.Nodes.getNodeAs<clang::FunctionDecl>("staticFunc");
        if (!Func || !Func->isStatic())
            return;

        const auto *ClassTemplate = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("classTemplate");
        if (!ClassTemplate)
            return;

        std::string templateType = ClassTemplate->getNameAsString();
        std::string newExpr = "class LocalType; " + templateType + "<LocalType>{}";

        std::string mutatedExpr = "/*mut652*/" + newExpr + " : " + newExpr;
        Rewrite.ReplaceText(CondExpr->getSourceRange(), mutatedExpr);
    }
}
  
void MutatorFrontendAction_652::MutatorASTConsumer_652::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = conditionalOperator(
                       hasAncestor(functionDecl(isStaticStorageClass()).bind("staticFunc")),
                       hasAncestor(classTemplateDecl().bind("classTemplate"))
                   ).bind("condExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}