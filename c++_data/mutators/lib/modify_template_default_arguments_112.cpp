//source file
#include "../include/modify_template_default_arguments_112.h"

// ========================================================================================================
#define MUT112_OUTPUT 1

void MutatorFrontendAction_112::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        for (auto *Param : *TD->getTemplateParameters()) {
            if (auto *NTTP = llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(Param)) {
                if (NTTP->hasDefaultArgument()) {
                    auto *defaultArg = NTTP->getDefaultArgument();
                    std::string newDefaultArg = "decltype([](char ch) { return ch != '?' && ch != 'x'; })";
                    Rewrite.ReplaceText(defaultArg->getSourceRange(), newDefaultArg);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_112::MutatorASTConsumer_112::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl())).bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}