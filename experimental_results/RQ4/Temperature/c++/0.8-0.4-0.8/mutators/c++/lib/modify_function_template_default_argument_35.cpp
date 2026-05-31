//source file
#include "../include/modify_function_template_default_argument_35.h"

// ========================================================================================================
#define MUT35_OUTPUT 1

void MutatorFrontendAction_35::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        if (FT->getTemplateParameters()->size() > 0) {
            auto *TP = FT->getTemplateParameters()->getParam(0);
            if (auto *NTTP = llvm::dyn_cast<clang::NonTypeTemplateParmDecl>(TP)) {
                if (NTTP->hasDefaultArgument()) {
                    auto defaultArg = NTTP->getDefaultArgument();
                    std::string replacement = "std::string";
                    Rewrite.ReplaceText(defaultArg->getSourceRange(), replacement);
                }
            }
        }
    }
}

void MutatorFrontendAction_35::MutatorASTConsumer_35::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(has(templateTypeParmDecl().bind("FunctionTemplate"))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}