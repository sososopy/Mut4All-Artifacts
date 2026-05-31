//source file
#include "../include/Mutator_601.h"

// ========================================================================================================
#define MUT601_OUTPUT 1

void MutatorFrontendAction_601::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("templateInstance")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        const TemplateArgumentList &args = MT->getTemplateArgs();
        if (args.size() == 1) {
            std::string newInstantiation = "B<int, double>";
            Rewrite.ReplaceText(MT->getSourceRange(), newInstantiation);
        }
    }
}
  
void MutatorFrontendAction_601::MutatorASTConsumer_601::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl(hasName("B")).bind("templateInstance");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}