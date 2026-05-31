//source file
#include "../include/modify_return_type_for_template_functions_598.h"

// ========================================================================================================
#define MUT598_OUTPUT 1

void MutatorFrontendAction_598::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("templateFunc")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getReturnType()->isVoidType() && FD->isTemplateInstantiation()) {
            auto paramType = FD->getParamDecl(0)->getType().getAsString();
            std::string newFuncDecl = "template<typename T>\nT " + FD->getNameAsString() + "(T &&s) { return T(); }";
            Rewrite.ReplaceText(FD->getSourceRange(), newFuncDecl);
        }
    }
}

void MutatorFrontendAction_598::MutatorASTConsumer_598::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl(isTemplateInstantiation(), returns(voidType()), hasParameter(0, hasType(qualType()))).bind("templateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}