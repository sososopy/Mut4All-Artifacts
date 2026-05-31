//source file
#include "../include/incomplete_struct_with_constexpr_5.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("constexprVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        if (!structDeclared) {
            incompleteStructName = "Y";
            std::string structDecl = "struct " + incompleteStructName + ";";
            Rewrite.InsertTextBefore(VD->getBeginLoc(), structDecl + "\n");
            structDeclared = true;
        }

        std::string newDecl = "constexpr struct " + incompleteStructName + " " + VD->getNameAsString() + " = {0};";
        Rewrite.ReplaceText(VD->getSourceRange(), newDecl);
    }
}

void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(isConstQualified())).bind("constexprVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}