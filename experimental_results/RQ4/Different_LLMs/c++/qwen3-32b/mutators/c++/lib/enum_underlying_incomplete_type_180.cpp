//source file
#include "../include/Enum_Underlying_Incomplete_Type_180.h"

// ========================================================================================================
#define MUT180_OUTPUT 1

int MutatorFrontendAction_180::Callback::counter = 0;

void MutatorFrontendAction_180::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("enumDecl")) {
        if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(ED->getLocation()))
            return;
        if (!ED->getIntegerType().getTypePtr())
            return;
        std::string newTypeName = "IncompleteType_" + std::to_string(counter++);
        Rewrite.InsertTextBefore(ED->getLocation(), "struct " + newTypeName + ";\n");
        SourceRange typeRange = ED->getIntegerTypeSourceInfo()->getTypeLoc().getSourceRange();
        Rewrite.ReplaceText(typeRange, newTypeName);
    }
}
  
void MutatorFrontendAction_180::MutatorASTConsumer_180::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = enumDecl().bind("enumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}