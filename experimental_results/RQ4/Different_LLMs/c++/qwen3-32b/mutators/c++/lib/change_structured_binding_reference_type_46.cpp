//source file
#include "../include/Change_Structured_Binding_Reference_Type_46.h"

// ========================================================================================================
#define MUT46_OUTPUT 1

void MutatorFrontendAction_46::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DD = Result.Nodes.getNodeAs<clang::DecompositionDecl>("decomp")) {
        if (!DD || !Result.Context->getSourceManager().isWrittenInMainFile(DD->getLocation()))
            return;

        clang::TypeLoc typeLoc = DD->getTypeSourceInfo()->getTypeLoc();
        clang::QualType qualType = typeLoc.getType();
        if (!qualType->isReferenceType())
            return;

        SourceRange typeRange = typeLoc.getSourceRange();
        SourceManager &SM = *Result.SourceManager;

        std::string originalType = stringutils::rangetoStr(SM, typeRange);
        if (originalType.find("&") == std::string::npos)
            return;

        int choice = getrandom::getRandomIndex(2);
        std::string newType;
        if (choice == 0) {
            // Remove reference
            newType = originalType.substr(0, originalType.size() - 1);
        } else {
            // Add rvalue reference
            newType = originalType + "&";
        }

        Rewrite.ReplaceText(typeRange, newType);
    }
}

void MutatorFrontendAction_46::MutatorASTConsumer_46::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = decompositionDecl(hasType(references(qualType(anything())))).bind("decomp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}