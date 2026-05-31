//source file
#include "../include/incorrect_template_sfinae_usage_48.h"

// ========================================================================================================
#define MUT48_OUTPUT 1

void MutatorFrontendAction_48::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Func = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
        if (!Func || !Result.Context->getSourceManager().isWrittenInMainFile(Func->getLocation()))
            return;

        for (auto *Spec : Func->specializations()) {
            if (Spec->isThisDeclarationADefinition()) {
                auto ReturnType = Spec->getReturnType().getAsString();
                std::string newReturnType = ReturnType;
                size_t pos = newReturnType.find("::value");
                if (pos != std::string::npos) {
                    newReturnType.replace(pos, 7, "::nonExistentMember");
                    Rewrite.ReplaceText(Spec->getReturnTypeSourceRange(), newReturnType);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_48::MutatorASTConsumer_48::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(callExpr())).bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}