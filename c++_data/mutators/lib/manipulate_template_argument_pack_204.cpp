//source file
#include "../include/Manipulate_Template_Argument_Pack_204.h"

// ========================================================================================================
#define MUT204_OUTPUT 1

void MutatorFrontendAction_204::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->getSpecializationKind() != TSK_ExplicitSpecialization)
            return;

        if (TD->getTemplateArgs().size() > 0) {
            std::string replacement = "Container<>::get<0>();";
            SourceLocation startLoc = TD->getBeginLoc();
            SourceLocation endLoc = TD->getEndLoc();
            Rewrite.ReplaceText(SourceRange(startLoc, endLoc), replacement);
        }
    }
}

void MutatorFrontendAction_204::MutatorASTConsumer_204::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}