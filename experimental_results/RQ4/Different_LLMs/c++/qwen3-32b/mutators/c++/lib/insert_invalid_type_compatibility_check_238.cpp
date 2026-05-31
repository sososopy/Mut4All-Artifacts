//source file
#include "../include/Insert_Invalid_Type_Compatibility_Check_238.h"

// ========================================================================================================
#define MUT238_OUTPUT 1

void MutatorFrontendAction_238::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("record")) {
        if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(RD->getLocation()))
            return;
        if (!RD->isCompleteDefinition() || RD->isUnion() || RD->isLambda())
            return;
        SourceLocation endLoc = RD->getEndLoc();
        if (endLoc.isInvalid())
            return;
        std::string assertion = "_Static_assert(!(__builtin_types_compatible_p(typeof(*unknown), int)));\n";
        Rewrite.InsertTextBefore(endLoc, assertion);
    }
}

void MutatorFrontendAction_238::MutatorASTConsumer_238::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("record");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}