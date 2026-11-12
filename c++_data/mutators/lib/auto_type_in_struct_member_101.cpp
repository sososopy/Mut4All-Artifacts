//source file
#include "../include/auto_type_in_struct_member_101.h"

// ========================================================================================================
#define MUT101_OUTPUT 1

void MutatorFrontendAction_101::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("fieldDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasInClassInitializer()) {
            QualType fieldType = FD->getType();
            if (!fieldType->isDependentType() && !fieldType->getContainedAutoType()) {
                std::string initExpr = stringutils::rangetoStr(*(Result.SourceManager), FD->getInClassInitializer()->getSourceRange());
                std::string newDecl = "auto " + FD->getNameAsString() + " = " + initExpr + ";";
                Rewrite.ReplaceText(FD->getSourceRange(), newDecl);
            }
        }
    }
}

void MutatorFrontendAction_101::MutatorASTConsumer_101::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(hasInClassInitializer(anything())).bind("fieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}