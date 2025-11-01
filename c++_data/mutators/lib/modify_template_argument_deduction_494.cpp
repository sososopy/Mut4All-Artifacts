//source file
#include "../include/modify_template_argument_deduction_494.h"

// ========================================================================================================
#define MUT494_OUTPUT 1

void MutatorFrontendAction_494::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CT->getLocation()))
            return;

        for (auto *spec : CT->specializations()) {
            if (spec->isExplicitSpecialization())
                continue;

            for (auto *ctor : spec->getSpecializedTemplate()->getTemplatedDecl()->ctors()) {
                auto ctorSource = stringutils::rangetoStr(*(Result.SourceManager), ctor->getSourceRange());

                // Perform mutation by changing the constructor parameter type to a pointer type
                std::string mutatedCtor = ctorSource;
                size_t pos = mutatedCtor.find('(');
                if (pos != std::string::npos) {
                    mutatedCtor.insert(pos + 1, "T* ");
                }

                Rewrite.ReplaceText(CharSourceRange::getTokenRange(ctor->getSourceRange()), mutatedCtor);
            }
        }
    }
}

void MutatorFrontendAction_494::MutatorASTConsumer_494::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(has(cxxConstructorDecl())).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}