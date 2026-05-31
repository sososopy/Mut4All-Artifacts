//source file
#include "../include/modify_template_parameter_pack_usage_9.h"

// ========================================================================================================
#define MUT9_OUTPUT 1

void MutatorFrontendAction_9::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        auto templateArgs = TD->getTemplateArgs().asArray();
        if (templateArgs.size() > 0) {
            std::string newInstantiation = TD->getNameAsString() + "<>";

            auto locStart = TD->getBeginLoc();
            auto locEnd = TD->getEndLoc();
            auto range = CharSourceRange::getTokenRange(locStart, locEnd);
            Rewrite.ReplaceText(range, newInstantiation);
        }
    }
}

void MutatorFrontendAction_9::MutatorASTConsumer_9::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}