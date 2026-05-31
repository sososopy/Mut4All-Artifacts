//source file
#include "../include/Insert_Invalid_Semicolon_In_Record_165.h"

// ========================================================================================================
#define MUT165_OUTPUT 1

void MutatorFrontendAction_165::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *recordDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        if (!recordDecl || !Result.Context->getSourceManager().isWrittenInMainFile(recordDecl->getLocation()))
            return;
        if (!recordDecl->isCompleteDefinition())
            return;
        if (!recordDecl->isStruct() && !recordDecl->isUnion() && !recordDecl->isClass())
            return;
        auto fields = recordDecl->fields();
        if (std::distance(fields.begin(), fields.end()) < 2)
            return;
        auto firstField = *fields.begin();
        Rewrite.InsertText(firstField->getEndLoc(), ";", true, true);
    }
}

void MutatorFrontendAction_165::MutatorASTConsumer_165::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("record");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}