//source file
#include "../include/Mutator_Auto_Type_In_Struct_Member_10101.h"

// ========================================================================================================
#define MUT101_OUTPUT 1

void MutatorFrontendAction_10101::Callback::run(const MatchFinder::MatchResult &Result) {
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

void MutatorFrontendAction_10101::MutatorASTConsumer_10101::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(hasInClassInitializer(anything())).bind("fieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}