//source file
#include "../include/template_specialization_with_invalid_base_152.h"

// ========================================================================================================
#define MUT152_OUTPUT 1

void MutatorFrontendAction_152::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("DerivedStruct")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;

        for (auto &Base : RD->bases()) {
            if (Base.getType()->getAs<clang::TemplateSpecializationType>()) {
                std::string mutatedBase = "UndefinedType";
                auto baseRange = Base.getSourceRange();
                Rewrite.ReplaceText(baseRange, mutatedBase);
            }
        }
    }
}

void MutatorFrontendAction_152::MutatorASTConsumer_152::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyBase(hasType(templateSpecializationType()))).bind("DerivedStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}