//source file
#include "../include/rearrange_constructor_initializer_17.h"

// ========================================================================================================
#define MUT17_OUTPUT 1

void MutatorFrontendAction_17::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CtorDecl = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ctorDecl")) {
        if (!CtorDecl || !Result.Context->getSourceManager().isWrittenInMainFile(CtorDecl->getLocation()))
            return;

        if (CtorDecl->getNumCtorInitializers() < 2)
            return;

        std::vector<const clang::CXXCtorInitializer*> initializers;
        for (auto *init : CtorDecl->inits()) {
            initializers.push_back(init);
        }

        if (initializers.size() < 2)
            return;

        std::swap(initializers[0], initializers[1]);

        std::string newInitList;
        for (const auto *init : initializers) {
            if (!newInitList.empty()) {
                newInitList += ", ";
            }
            newInitList += clang::Lexer::getSourceText(
                clang::CharSourceRange::getTokenRange(init->getSourceRange()),
                *Result.SourceManager, Result.Context->getLangOpts()).str();
        }

        clang::SourceLocation startLoc = CtorDecl->getBeginLoc();
        clang::SourceLocation endLoc = CtorDecl->getEndLoc();

        Rewrite.ReplaceText(clang::SourceRange(startLoc, endLoc), newInitList);
    }
}

void MutatorFrontendAction_17::MutatorASTConsumer_17::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(isExplicit()).bind("ctorDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}