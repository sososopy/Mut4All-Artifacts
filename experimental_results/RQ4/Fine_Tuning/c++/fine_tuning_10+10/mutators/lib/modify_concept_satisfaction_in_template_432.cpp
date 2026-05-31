//source file
#include "../include/modify_concept_satisfaction_in_template_432.h"

// ========================================================================================================
#define MUT432_OUTPUT 1

void MutatorFrontendAction_432::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TemplateClass")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        for (auto *Ctor : CT->ctors()) {
            if (Ctor->getBody()) {
                auto ctorSource = stringutils::rangetoStr(*(Result.SourceManager), Ctor->getSourceRange());
                std::string staticAssert = "\nstatic_assert(sizeof(T) > 4, \"T must be larger than 4 bytes\");\n";
                size_t insertPos = ctorSource.find("{") + 1;
                ctorSource.insert(insertPos, staticAssert);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(Ctor->getSourceRange()), ctorSource);
            }
        }
    }
}
  
void MutatorFrontendAction_432::MutatorASTConsumer_432::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isTemplateInstantiation()).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}