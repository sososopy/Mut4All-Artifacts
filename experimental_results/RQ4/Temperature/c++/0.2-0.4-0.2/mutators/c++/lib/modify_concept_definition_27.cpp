//source file
#include "../include/modify_concept_definition_27.h"

// ========================================================================================================
#define MUT27_OUTPUT 1

void MutatorFrontendAction_27::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CD = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
        if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
            return;

        auto constraintExpr = CD->getConstraintExpr();
        if (!constraintExpr)
            return;

        std::string modifiedConstraint = "42"; // Modify constraint to a non-boolean type

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(constraintExpr->getSourceRange()), modifiedConstraint);
    }
}

void MutatorFrontendAction_27::MutatorASTConsumer_27::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = conceptDecl().bind("ConceptDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}