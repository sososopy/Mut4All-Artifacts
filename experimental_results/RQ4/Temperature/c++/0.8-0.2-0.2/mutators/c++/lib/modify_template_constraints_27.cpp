//source file
#include "../include/Modify_Template_Constraints_27.h"

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (auto *CTSD = dyn_cast<clang::ClassTemplateSpecializationDecl>(TD)) {
            auto *TC = CTSD->getSpecializedTemplate();
            if (TC) {
                std::string originalName = TC->getNameAsString();
                std::string newName = originalName + "1"; // Assuming 'originalName1' is not declared
                std::string sourceText = stringutils::rangetoStr(*(Result.SourceManager), CTSD->getSourceRange());
                std::string mutatedText = std::regex_replace(sourceText, std::regex(originalName), newName);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(CTSD->getSourceRange()), mutatedText);
            }
        }
    }
}

void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateSpecializationDecl().bind("TemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}