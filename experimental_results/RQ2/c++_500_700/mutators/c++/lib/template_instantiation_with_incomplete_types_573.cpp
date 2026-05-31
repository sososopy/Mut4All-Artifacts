//source file
#include "../include/Template_Instantiation_With_Incomplete_Types_573.h"

// ========================================================================================================
#define MUT573_OUTPUT 1

void MutatorFrontendAction_573::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("varDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                VD->getLocation()))
            return;

        auto varType = VD->getType();
        if (varType->isIncompleteType()) {
            return;
        }

        std::string incompleteTypeName = "IncompleteType";
        std::string replacementText = "struct " + incompleteTypeName + "; " + incompleteTypeName + " obj;";
        Rewrite.ReplaceText(VD->getSourceRange(), replacementText);
    }
}

void MutatorFrontendAction_573::MutatorASTConsumer_573::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(hasType(recordType())).bind("varDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}