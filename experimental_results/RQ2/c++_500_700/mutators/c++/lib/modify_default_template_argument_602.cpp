//source file
#include "../include/modify_default_template_argument_602.h"

// ========================================================================================================
#define MUT602_OUTPUT 1

void MutatorFrontendAction_602::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->getTemplateParameters()->size() > 0) {
            for (auto *param : *TD->getTemplateParameters()) {
                if (auto *typeParam = llvm::dyn_cast<clang::TemplateTypeParmDecl>(param)) {
                    if (typeParam->hasDefaultArgument()) {
                        QualType defaultArg = typeParam->getDefaultArgument();
                        if (defaultArg.getAsString() == "int") {
                            SourceLocation startLoc = typeParam->getDefaultArgumentLoc();
                            SourceLocation endLoc = startLoc.getLocWithOffset(defaultArg.getAsString().length() - 1);
                            Rewrite.ReplaceText(SourceRange(startLoc, endLoc), "double");
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_602::MutatorASTConsumer_602::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("TemplateFunc");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}