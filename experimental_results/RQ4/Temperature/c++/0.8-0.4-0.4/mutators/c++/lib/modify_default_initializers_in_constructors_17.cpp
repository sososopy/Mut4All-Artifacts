//source file
#include "../include/modify_default_initializers_in_constructors_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
        if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(Ctor->getLocation()))
            return;

        for (auto *Param : Ctor->parameters()) {
            if (Param->hasDefaultArg()) {
                // Optionally remove the default argument or replace it with an incorrect expression
                std::string newArg = getrandom::getRandomIndex(2) ? "" : " /*mut17*/ = ";
                auto paramRange = Param->getSourceRange();
                auto defaultArgRange = Param->getDefaultArgRange();
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(defaultArgRange), newArg);
            }
        }
    }
}

void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(hasAnyParameter(hasDefaultArgument())).bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}