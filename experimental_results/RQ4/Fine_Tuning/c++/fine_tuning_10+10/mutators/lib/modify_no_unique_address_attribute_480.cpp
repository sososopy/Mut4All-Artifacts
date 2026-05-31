//source file
#include "../include/Modify_No_Unique_Address_Attribute_480.h"

// ========================================================================================================
#define MUT480_OUTPUT 1

void MutatorFrontendAction_480::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("fieldDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->hasAttr<clang::NoUniqueAddressAttr>()) {
            noUniqueAddressFields.push_back(FD);
        } else {
            otherFields.push_back(FD);
        }
    }

    if (!noUniqueAddressFields.empty() && !otherFields.empty()) {
        const FieldDecl *removeAttrField = noUniqueAddressFields.front();
        const FieldDecl *addAttrField = otherFields.front();

        // Remove [[no_unique_address]] attribute
        if (removeAttrField->hasAttr<clang::NoUniqueAddressAttr>()) {
            SourceLocation startLoc = removeAttrField->getBeginLoc();
            SourceLocation endLoc = startLoc.getLocWithOffset(strlen("[[no_unique_address]]"));
            Rewrite.RemoveText(SourceRange(startLoc, endLoc));
        }

        // Add [[no_unique_address]] attribute
        SourceLocation insertLoc = addAttrField->getBeginLoc();
        Rewrite.InsertText(insertLoc, "[[no_unique_address]] ", true, true);
    }
}

void MutatorFrontendAction_480::MutatorASTConsumer_480::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = fieldDecl(isExpansionInMainFile()).bind("fieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}