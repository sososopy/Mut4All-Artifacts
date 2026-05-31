//source file
#include "../include/modify_template_partial_specialization_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTPSD = Result.Nodes.getNodeAs<clang::ClassTemplatePartialSpecializationDecl>("PartialSpecialization")) {
        if (!CTPSD || !Result.Context->getSourceManager().isWrittenInMainFile(CTPSD->getLocation()))
            return;

        if (!ExistingSpecialization) {
            ExistingSpecialization = CTPSD;
            return;
        }

        if (ExistingSpecialization->getSpecializedTemplate() == CTPSD->getSpecializedTemplate()) {
            auto specializationSource = stringutils::rangetoStr(*(Result.SourceManager), CTPSD->getSourceRange());
            std::string newSpecialization = "\n/*mut19*/template<> class " + ExistingSpecialization->getSpecializedTemplate()->getNameAsString() + "<short> {};";

            Rewrite.InsertTextAfterToken(CTPSD->getEndLoc(), newSpecialization);
        }
    }
}
  
void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplatePartialSpecializationDecl().bind("PartialSpecialization");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}